#ifndef _H_OPERATIONLOGGER
#define _H_OPERATIONLOGGER

#include <string>
#include <vector>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

class OperationLogger {
public:
    struct LogEntry {
        std::string timestamp;
        std::string request;
        std::string response;
    };

    OperationLogger(const std::string& logFilePath = "chat_ai_operations.log");

    void logRequestResponse(const std::string& request, const std::string& response);

    std::vector<LogEntry> getLogEntries() const;

    void clearLogs();

    void reloadLogs();

private:
    std::string logFilePath_;
    std::vector<LogEntry> logEntries_;

    std::string getCurrentTimestamp();

    void saveToFile(const LogEntry& entry);

    // 新增LoadFromFile接口
    bool LoadFromFile(const std::string& filePath);
};

#endif // _H_OPERATIONLOGGER