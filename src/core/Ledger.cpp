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
    return accounts.find(accountId) != accounts.end();
}

Account *Ledger::getAccount(int accountId)
{

    auto it = accounts.find(accountId);

    if (it == accounts.end())
    {
        return nullptr;
    }

    return &it->second;
}

const Account *Ledger::getAccount(
    int accountId) const
{
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
    if (!transaction.isBalanced())
    {
        return false;
    }

    for (const auto &entry :
         transaction.getEntries())
    {
        if (!hasAccount(entry.accountId))
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
            getAccount(entry.accountId);

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
    return transactions;
}

void Ledger::recover()
{
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
            if (!hasAccount(entry.accountId))
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
                getAccount(entry.accountId);

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
    transactions = loadedTransactions;
}