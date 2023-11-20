#ifndef H__FILE
#define H__FILE

#include <string>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <vector>

std::string read_to_string(std::filesystem::path path);
std::vector<char> read_to_vector(std::filesystem::path path);

#endif

