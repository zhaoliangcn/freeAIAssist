#include "filedig.h"
#include <windows.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <io.h> // for _close() and _get_osfhandle() functions
//#include <fcntl.h> // for _O_NOINHERIT flag
#include <cstdio> // for fileno() and fflush() functions if needed
#include <cerrno> // for error checking if needed
#include <fstream>
#include <sstream>
#include <cctype>
#include <regex>
//#include "../thirdparty/include/doctotext_c_api.h"
#include "../thirdparty/include/StringEncode.hpp"
#include "uitl.hpp"


std::string FileDig::getFileType(const std::string& filePath) {
    size_t dotPos = filePath.find_last_of('.');
    if(dotPos == std::string::npos) return "unknown";
    return filePath.substr(dotPos+1);
}

std::string FileDig::getFileContent(const std::string& filePath) {

    std::string filecontent;
    std::string filetype = getFileType(filePath);
    std::regex pattern("(doc|docx|xls|xlsx|ppt|pptx)");
    if(std::regex_match(filetype, pattern))
    {
        int verbose = 0;
        try
        {
            // DocToTextExtractorParams* params = doctotext_create_extractor_params();
            // DocToTextException* exception = NULL;
            // doctotext_extractor_params_set_verbose_logging(params, verbose);
            // doctotext_extractor_params_set_log_file(params, "");
            // DocToTextExtractedData* text_data = doctotext_process_file(STRINGENCODE::U2A(filePath).c_str(), params, &exception);
            // doctotext_free_extractor_params(params);
            // if (text_data)
            // {
            //     //printf("%s\n", doctotext_extracted_data_get_text(text_data));
            //     filecontent = doctotext_extracted_data_get_text(text_data);
            //     doctotext_free_extracted_data(text_data);
            // }
            std::string currentPath = GetCurrentModulePath();
            char cmd[8192];
            sprintf(cmd, "\"\"%s\\doctotext\\doctotext.exe\"  \"%s\"\"", currentPath.c_str(), STRINGENCODE::U2A(filePath).c_str());
            FILE* fp = _popen(cmd, "r");
            if(fp)
            {
                char buffer[4096];
                while(fgets(buffer, sizeof(buffer), fp))
                {
                    filecontent += buffer;
                }
                _pclose(fp);
            }
        }
		catch (...)
		{
			return filecontent;
		}


    }
    else
    {
        std::ifstream file(filePath);
        if(!file.is_open()) return "";

        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }
    return filecontent;
}
