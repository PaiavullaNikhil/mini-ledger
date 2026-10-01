#pragma once

#include <string>

class AuditLog {
private:
    std::string filePath;

public:
    explicit AuditLog(
        const std::string& filePath
    );

    void log(
        const std::string& event
    ) const;
};