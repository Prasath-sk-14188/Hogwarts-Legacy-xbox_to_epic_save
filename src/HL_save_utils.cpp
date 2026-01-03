#include "HL_save_utils.h"
#include <iostream>
#include <fstream>
#include <regex>
#include <cstdlib>

void error(const std::string &msg)
{
    std::cerr << "[FATAL] " << msg << std::endl;
    std::exit(EXIT_FAILURE);
}

bool fileExists(const fs::path &p)
{
    return fs::exists(p) && fs::is_regular_file(p);
}

bool dirExists(const fs::path &p)
{
    return fs::exists(p) && fs::is_directory(p);
}

std::string readFileSafe(const fs::path &file)
{
    if (!fileExists(file))
        error("File does not exist: " + file.string());

    auto size = fs::file_size(file);
    if (size == 0 || size > MAX_FILE_SIZE)
        error("Invalid file size: " + file.string());

    std::ifstream in(file, std::ios::binary);
    if (!in)
        error("Failed to open file: " + file.string());

    std::string data(size, '\0');
    in.read(data.data(), size);

    if (!in)
        error("Failed to read file completely: " + file.string());

    return data;
}

std::vector<std::string> extractHLTags(const std::string &data)
{
    std::regex pattern(R"(HL-\d{2}-\d{2})");
    std::sregex_iterator it(data.begin(), data.end(), pattern);
    std::sregex_iterator end;

    std::vector<std::string> results;
    for (; it != end; ++it)
        results.push_back(it->str());

    return results;
}

fs::path isValidEpicUserIdFolder(const fs::path &parent)
{
    if (!dirExists(parent))
        error("Directory does not exist: " + parent.string());

    fs::path found;
    int count = 0;

    for (const auto &entry : fs::directory_iterator(parent))
    {
        if (entry.is_directory())
        {
            found = entry.path();
            count++;
        }
    }

    if (count == 0)
        error("No subdirectory found in: " + parent.string());

    if (count > 1)
        error("Multiple subdirectories found in: " + parent.string());

    return found;
}

bool isValidXboxUserIdFolder(const fs::path &p)
{
    if (!fs::exists(p) || !fs::is_directory(p))
        return false;

    fs::path indexFile = p / "containers.index";
    return fs::exists(indexFile) && fs::is_regular_file(indexFile);
}