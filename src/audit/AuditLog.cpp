#include "audit/AuditLog.h"

#include <chrono>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <stdexcept>

AuditLog::AuditLog(
    const std::string &filePath)
    : filePath(filePath)
{
}

void AuditLog::log(
    const std::string &event) const
{

    std::ofstream out(
        filePath,
        std::ios::app);

    if (!out)
    {
        throw std::runtime_error(
            "Unable to open audit log");
    }

    auto now =
        std::chrono::system_clock::now();

    std::time_t time =
        std::chrono::system_clock::to_time_t(now);

    std::tm localTime{};

    #ifdef _WIN32
        localtime_s(
            &localTime,
            &time);
    #else
        localtime_r(
            &time,
            &localTime);
    #endif

        out
            << std::put_time(
                &localTime,
                "%Y-%m-%d %H:%M:%S")
            << " | "
            << event
            << '\n';

        if (!out)
        {
            throw std::runtime_error(
                "Failed to write audit log");
        }
}