#include "core/Ledger.h"
#include <string>
#include <iostream>

Ledger::Ledger(
    AuditLog *auditLog,
    WAL *wal)
    : auditLog(auditLog),
      wal(wal)
{
}

void Ledger::addAccount(
    const Account &account)
{   
    std::lock_guard<std::mutex> lock(mutex);

    accounts.emplace(
        account.getId(),
        account);

    if (auditLog != nullptr)
    {
        auditLog->log(
            "ACCOUNT_CREATED | ID=" +
            std::to_string(account.getId()) +
            " | Name=" +
            account.getName());
    }
}

bool Ledger::hasAccount(int accountId) const
{
    std::lock_guard<std::mutex> lock(mutex);

    return hasAccountUnlocked(accountId);
}

Account *Ledger::getAccount(int accountId)
{
    std::lock_guard<std::mutex> lock(mutex);

    return getAccountUnlocked(accountId);
}

const Account *Ledger::getAccount(
    int accountId) const
{
    std::lock_guard<std::mutex> lock(mutex);

    auto it = accounts.find(accountId);

    if (it == accounts.end())
    {
        return nullptr;
    }

    return &it->second;
}

bool Ledger::postTransaction(
    const Transaction &transaction)
{
    std::lock_guard<std::mutex> lock(mutex);

    if (!transaction.isBalanced())
    {
        return false;
    }

    for (const auto &entry :
         transaction.getEntries())
    {
        if (!hasAccountUnlocked(entry.accountId))
        {
            return false;
        }
    }

    if (wal != nullptr)
    {
        wal->append(transaction);
    }

    for (const auto &entry :
         transaction.getEntries())
    {
        Account *account =
            getAccountUnlocked(entry.accountId);

        if (!entry.debit.isZero())
        {
            account->debit(entry.debit);
        }

        if (!entry.credit.isZero())
        {
            account->credit(entry.credit);
        }
    }

    transactions.push_back(transaction);

    if (auditLog != nullptr)
    {
        auditLog->log(
            "TRANSACTION_POSTED | ID=" +
            std::to_string(transaction.getId()) +
            " | " +
            transaction.getDescription());
    }

    return true;
}

void Ledger::printBalances() const
{
    std::lock_guard<std::mutex> lock(mutex);

    for (const auto &[id, account] : accounts)
    {

        std::cout
            << id
            << " | "
            << account.getName()
            << " | "
            << account.getBalance().getPaise()
            << " paise\n";
    }
}

std::vector<Account> Ledger::getAccounts() const
{
    std::lock_guard<std::mutex> lock(mutex);

    std::vector<Account> result;

    result.reserve(accounts.size());

    for (const auto &[id, account] : accounts)
    {
        result.push_back(account);
    }

    return result;
}

std::vector<Transaction> Ledger::getTransactions() const
{
    std::lock_guard<std::mutex> lock(mutex);

    return transactions;
}

void Ledger::recover()
{
    std::lock_guard<std::mutex> lock(mutex);

    if (wal == nullptr)
    {
        return;
    }

    std::vector<Transaction> pending =
        wal->read();

    for (const auto &transaction :
         pending)
    {
        bool alreadyApplied = false;

        for (const auto &existing :
             transactions)
        {
            if (existing.getId() ==
                transaction.getId())
            {
                alreadyApplied = true;
                break;
            }
        }

        if (alreadyApplied)
        {
            continue;
        }

        bool valid = true;

        for (const auto &entry :
             transaction.getEntries())
        {
            if (!hasAccountUnlocked(entry.accountId))
            {
                valid = false;
                break;
            }
        }

        if (!valid)
        {
            continue;
        }

        for (const auto &entry :
             transaction.getEntries())
        {
            Account *account =
                getAccountUnlocked(entry.accountId);

            if (!entry.debit.isZero())
            {
                account->debit(entry.debit);
            }

            if (!entry.credit.isZero())
            {
                account->credit(entry.credit);
            }
        }

        transactions.push_back(transaction);
    }
}

void Ledger::loadAccounts(
    const std::vector<Account> &loadedAccounts)
{
    std::lock_guard<std::mutex> lock(mutex);

    for (const auto &account :
         loadedAccounts)
    {
        accounts.insert_or_assign(
            account.getId(),
            account);
    }
}

void Ledger::loadTransactions(
    const std::vector<Transaction> &
        loadedTransactions)
{   
    std::lock_guard<std::mutex> lock(mutex);
    transactions = loadedTransactions;
}

bool Ledger::hasAccountUnlocked(
    int accountId) const
{
    return accounts.find(accountId) != accounts.end();
}

Account *Ledger::getAccountUnlocked(
    int accountId)
{
    auto it = accounts.find(accountId);

    if (it == accounts.end())
    {
        return nullptr;
    }

    return &it->second;
}