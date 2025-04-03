#include "OperationLogger.h"

OperationLogger::OperationLogger(const std::string& logFilePath) : logFilePath_(logFilePath) {
	LoadFromFile(logFilePath);
}

void OperationLogger::logRequestResponse(const std::string& request, const std::string& response) {
    LogEntry entry;
    entry.timestamp = getCurrentTimestamp();
    entry.request = request;
    entry.response = response;
    logEntries_.push_back(entry);
    saveToFile(entry);
}

std::vector<OperationLogger::LogEntry> OperationLogger::getLogEntries() const {
    return logEntries_;
}

void OperationLogger::clearLogs() {
    logEntries_.clear();
    std::ofstream ofs(logFilePath_, std::ofstream::trunc);
    ofs.close();
}

std::string OperationLogger::getCurrentTimestamp() {
    auto now = std::time(nullptr);
    auto tm = *std::localtime(&now);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

void OperationLogger::saveToFile(const LogEntry& entry) {
    std::ofstream ofs(logFilePath_, std::ios_base::app);
    if (ofs.is_open()) {
        ofs << "[" << entry.timestamp << "]\n"
            << "Request: " << entry.request << "\n"
            << "Response: " << entry.response << "\n\n";
        ofs.close();
    }
}


bool OperationLogger::LoadFromFile(const std::string& filePath) {
    std::ifstream ifs(filePath);
    if (!ifs.is_open()) {
        return false;
    }

    std::string line;
    LogEntry entry;
    while (std::getline(ifs, line)) {
        if (line.empty()) continue;
        if (line[0] == '[') {
            if (!entry.timestamp.empty()) {
                logEntries_.push_back(entry);
                entry = LogEntry();
            }
            entry.timestamp = line.substr(1, line.length() - 2);
        } else if (line.find("Request: ") == 0) {
            entry.request = line.substr(9);
        } else if (line.find("Response: ") == 0) {
            entry.response = line.substr(10);
        }
    }
    if (!entry.timestamp.empty()) {
        logEntries_.push_back(entry);
    }

    ifs.close();
    return true;

}

void OperationLogger::reloadLogs() {
    logEntries_.clear();
    LoadFromFile(logFilePath_);    
}