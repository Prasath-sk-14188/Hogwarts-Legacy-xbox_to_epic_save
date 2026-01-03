#pragma once
#include <string>
#include <filesystem>
#include <vector>

namespace fs = std::filesystem;

constexpr std::uintmax_t MAX_FILE_SIZE = 50 * 1024 * 1024; // 50 MB

void error(const std::string &msg);

bool fileExists(const fs::path &p);
bool dirExists(const fs::path &p);
std::string readFileSafe(const fs::path &file);
std::vector<std::string> extractHLTags(const std::string &data);
fs::path isValidEpicUserIdFolder(const fs::path &parent);
bool isValidXboxUserIdFolder(const fs::path &p);