#include "storage/Serializer.h"

#include <cstdint>
#include <stdexcept>

void Serializer::writeAccount(
    std::ofstream &out,
    const Account &account)
{
    std::int32_t id = account.getId();

    std::int32_t nameLength =
        static_cast<std::int32_t>(
            account.getName().size());

    std::int32_t type =
        static_cast<std::int32_t>(
            account.getType());

    std::int64_t balance =
        account.getBalance().getPaise();

    out.write(
        reinterpret_cast<const char *>(&id),
        sizeof(id));

    out.write(
        reinterpret_cast<const char *>(&nameLength),
        sizeof(nameLength));

    out.write(
        account.getName().data(),
        nameLength);

    out.write(
        reinterpret_cast<const char *>(&type),
        sizeof(type));

    out.write(
        reinterpret_cast<const char *>(&balance),
        sizeof(balance));

    if (!out)
    {
        throw std::runtime_error(
            "Failed to write account");
    }
}

Account Serializer::readAccount(
    std::ifstream &in)
{
    std::int32_t id;
    std::int32_t nameLength;
    std::int32_t type;
    std::int64_t balance;

    in.read(
        reinterpret_cast<char *>(&id),
        sizeof(id));

    in.read(
        reinterpret_cast<char *>(&nameLength),
        sizeof(nameLength));

    if (!in)
    {
        throw std::runtime_error(
            "Failed to read account header");
    }

    if (nameLength < 0 || nameLength > 10000)
    {
        throw std::runtime_error(
            "Invalid account name length");
    }

    std::string name(nameLength, '\0');

    in.read(
        name.data(),
        nameLength);

    in.read(
        reinterpret_cast<char *>(&type),
        sizeof(type));

    in.read(
        reinterpret_cast<char *>(&balance),
        sizeof(balance));

    if (!in)
    {
        throw std::runtime_error(
            "Failed to read account data");
    }

    Account account(
        id,
        name,
        static_cast<AccountType>(type));

    account.setBalanceForStorage(
        Money(balance));

    return account;
}

void Serializer::writeTransaction(
    std::ofstream& out,
    const Transaction& transaction)
    {
    std::int32_t id = transaction.getId();

    std::int32_t descriptionLength =
        static_cast<std::int32_t>(
            transaction.getDescription().size()
        );

    std::int32_t entryCount =
        static_cast<std::int32_t>(
            transaction.getEntries().size()
        );

    // Write transaction ID
    out.write(
        reinterpret_cast<const char*>(&id),
        sizeof(id)
    );

    // Write description length
    out.write(
        reinterpret_cast<const char*>(&descriptionLength),
        sizeof(descriptionLength)
    );

    // Write description
    out.write(
        transaction.getDescription().data(),
        descriptionLength
    );

    // Write number of entries
    out.write(
        reinterpret_cast<const char*>(&entryCount),
        sizeof(entryCount)
    );

    // Write every entry
    for (const auto& entry :
         transaction.getEntries()) {

        std::int32_t accountId = entry.accountId;

        std::int64_t debit =
            entry.debit.getPaise();

        std::int64_t credit =
            entry.credit.getPaise();

        out.write(
            reinterpret_cast<const char*>(&accountId),
            sizeof(accountId)
        );

        out.write(
            reinterpret_cast<const char*>(&debit),
            sizeof(debit)
        );

        out.write(
            reinterpret_cast<const char*>(&credit),
            sizeof(credit)
        );
    }

    if (!out) {
        throw std::runtime_error(
            "Failed to write transaction"
        );
    }
}

Transaction Serializer::readTransaction(
    std::ifstream& in) 
    {
    std::int32_t id;
    std::int32_t descriptionLength;
    std::int32_t entryCount;

    // Read transaction ID
    in.read(
        reinterpret_cast<char*>(&id),
        sizeof(id)
    );

    // Read description length
    in.read(
        reinterpret_cast<char*>(&descriptionLength),
        sizeof(descriptionLength)
    );

    if (!in) {
        throw std::runtime_error(
            "Failed to read transaction header"
        );
    }

    if (descriptionLength < 0 ||
        descriptionLength > 100000) {

        throw std::runtime_error(
            "Invalid transaction description length"
        );
    }

    std::string description(
        descriptionLength,
        '\0'
    );

    in.read(
        description.data(),
        descriptionLength
    );

    // Read number of entries
    in.read(
        reinterpret_cast<char*>(&entryCount),
        sizeof(entryCount)
    );

    if (!in) {
        throw std::runtime_error(
            "Failed to read transaction metadata"
        );
    }

    if (entryCount < 0 ||
        entryCount > 100000) {

        throw std::runtime_error(
            "Invalid transaction entry count"
        );
    }

    Transaction transaction(
        id,
        description
    );

    for (std::int32_t i = 0;
         i < entryCount;
         ++i) {

        std::int32_t accountId;
        std::int64_t debit;
        std::int64_t credit;

        in.read(
            reinterpret_cast<char*>(&accountId),
            sizeof(accountId)
        );

        in.read(
            reinterpret_cast<char*>(&debit),
            sizeof(debit)
        );

        in.read(
            reinterpret_cast<char*>(&credit),
            sizeof(credit)
        );

        if (!in) {
            throw std::runtime_error(
                "Failed to read transaction entry"
            );
        }

        transaction.addEntry(
            accountId,
            Money(debit),
            Money(credit)
        );
    }

    return transaction;
}

