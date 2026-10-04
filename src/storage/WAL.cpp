#include "storage/WAL.h"
#include "storage/Serializer.h"

#include <fstream>
#include <cstdint>
#include <stdexcept>

WAL::WAL(const std::string& filePath)
    : filePath(filePath)
{
}

void WAL::append(const Transaction& transaction) const
{
    std::ofstream file(
        filePath,
        std::ios::binary |
        std::ios::app
    );

    if (!file)
    {
        throw std::runtime_error(
            "Failed to open WAL file"
        );
    }

    std::ofstream temp(
        filePath + ".tmp",
        std::ios::binary |
        std::ios::trunc
    );

    if (!temp)
    {
        throw std::runtime_error(
            "Failed to create temporary WAL file"
        );
    }

    Serializer::writeTransaction(
        temp,
        transaction
    );

    temp.flush();

    if (!temp)
    {
        throw std::runtime_error(
            "Failed to serialize transaction"
        );
    }

    temp.close();

    std::ifstream transactionData(
        filePath + ".tmp",
        std::ios::binary
    );

    if (!transactionData)
    {
        throw std::runtime_error(
            "Failed to read temporary WAL record"
        );
    }

    transactionData.seekg(0, std::ios::end);

    std::streamsize size =
        transactionData.tellg();

    transactionData.seekg(0);

    std::vector<char> buffer(
        static_cast<std::size_t>(size)
    );

    transactionData.read(
        buffer.data(),
        size
    );

    transactionData.close();

    std::int32_t recordSize =
        static_cast<std::int32_t>(size);

    file.write(
        reinterpret_cast<const char*>(&recordSize),
        sizeof(recordSize)
    );

    file.write(
        buffer.data(),
        size
    );

    file.flush();

    if (!file)
    {
        throw std::runtime_error(
            "Failed to write transaction to WAL"
        );
    }

    file.close();

    std::remove(
        (filePath + ".tmp").c_str()
    );
}

std::vector<Transaction> WAL::read() const
{
    std::vector<Transaction> transactions;

    std::ifstream file(
        filePath,
        std::ios::binary
    );

    if (!file)
    {
        return transactions;
    }

    while (true)
    {
        std::int32_t recordSize;

        file.read(
            reinterpret_cast<char*>(&recordSize),
            sizeof(recordSize)
        );

        if (file.eof())
        {
            break;
        }

        if (!file)
        {
            break;
        }

        if (recordSize <= 0)
        {
            break;
        }

        std::streampos recordStart =
            file.tellg();

        file.seekg(
            0,
            std::ios::end
        );

        std::streampos fileEnd =
            file.tellg();

        file.seekg(recordStart);

        std::streamoff remaining =
            fileEnd - recordStart;

        if (remaining < recordSize)
        {
            break;
        }

        std::vector<char> buffer(
            static_cast<std::size_t>(recordSize)
        );

        file.read(
            buffer.data(),
            recordSize
        );

        if (!file)
        {
            break;
        }

        std::string tempPath =
            filePath + ".read.tmp";

        std::ofstream temp(
            tempPath,
            std::ios::binary |
            std::ios::trunc
        );

        if (!temp)
        {
            throw std::runtime_error(
                "Failed to create temporary WAL read file"
            );
        }

        temp.write(
            buffer.data(),
            recordSize
        );

        temp.close();

        std::ifstream recordFile(
            tempPath,
            std::ios::binary
        );

        transactions.push_back(
            Serializer::readTransaction(recordFile)
        );

        recordFile.close();

        std::remove(tempPath.c_str());
    }

    return transactions;
}

void WAL::clear() const
{
    std::ofstream file(
        filePath,
        std::ios::binary |
        std::ios::trunc
    );

    if (!file)
    {
        throw std::runtime_error(
            "Failed to clear WAL file"
        );
    }
}