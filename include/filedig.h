#ifndef FILEDIG_H
#define FILEDIG_H

#include <string>

class FileDig {
public:
    std::string getFileType(const std::string& filePath);
    std::string getFileContent(const std::string& filePath);
};

#endif // FILEDIG_H