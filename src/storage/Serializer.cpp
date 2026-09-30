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