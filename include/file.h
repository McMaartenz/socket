#ifndef H__FILE
#define H__FILE

#include <string>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <vector>

/** Read a file into a std::string
 * @param path Path to file
 */
std::string read_to_string(std::filesystem::path path);

/** Read a file into a std::vector
 * @param path Path to file
 */
std::vector<char> read_to_vector(std::filesystem::path path);

#endif

