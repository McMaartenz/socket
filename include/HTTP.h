#ifndef H__HTTP
#define H__HTTP

#include <filesystem>
#include <sstream>
#include <fstream>
#include <string>

namespace HTTP {
        std::string create_response(int status_code, std::string const& page);
        std::string create_response(int status_code, std::filesystem::path const& path);
};

#endif

