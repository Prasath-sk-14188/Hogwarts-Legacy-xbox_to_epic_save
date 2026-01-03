
#include <iostream>
#include <filesystem>
#include <string>
#include "HL_save_utils.h"

namespace fs = std::filesystem;

int main()
{
    try
    {
        std::cout << "Enter user directory (example: C:\\Users\\JohnDoe): ";
        std::string userDirInput;
        std::getline(std::cin, userDirInput);

        if (userDirInput.empty())
            error("User directory input is empty.");

        fs::path userDir = fs::path(userDirInput);
        if (!dirExists(userDir))
            error("User directory does not exist.");

        // Construct Game Pass WGS root
        fs::path gamePassWgsDir = userDir / "AppData" / "Local" / "Packages" / "WarnerBros.Interactive.PHX_ktmk1xygcecda" / "SystemAppData" / "wgs";
        if (!dirExists(gamePassWgsDir))
            error("Game Pass WGS root not found.");

        std::vector<fs::path> gamePassWgsSubFolders;

        for (const auto &entry : fs::directory_iterator(gamePassWgsDir))
        {
            if (isValidXboxUserIdFolder(entry.path()))
                gamePassWgsSubFolders.push_back(entry.path());
        }

        if (gamePassWgsSubFolders.empty())
            error("No Game Pass userId folder found (container.index missing).");

        if (gamePassWgsSubFolders.size() > 1)
        {
            std::cerr << "[ERROR] Multiple Game Pass userId folders detected:\n";
            for (const auto &p : gamePassWgsSubFolders)
                std::cerr << "  - " << p << "\n";

            error("Ambiguous Game Pass userId folders. Cannot continue safely.");
        }

        fs::path gamePassUserDir = gamePassWgsSubFolders.front();
        std::cout << "[INFO] Game Pass userId folder identified: " << gamePassUserDir.filename() << "\n";

        fs::path tempDir = fs::current_path() / "ConvertedSaves";
        fs::create_directories(tempDir);

        std::cout << "[INFO] Scanning Game Pass save files...\n";
        size_t convertedCount = 0;
        for (const auto &saveFolder : fs::directory_iterator(gamePassUserDir))
        {
            if (!saveFolder.is_directory())
                continue;

            for (const auto &file : fs::directory_iterator(saveFolder))
            {
                if (!file.is_regular_file())
                    continue;

                std::string data = readFileSafe(file.path());
                std::vector<std::string> tags = extractHLTags(data);
                if (tags.empty())
                    continue;

                if (tags.size() > 1)
                {
                    std::cout << "[SKIP] Multiple HL tags found: " << file.path() << "\n";
                    continue;
                }
                fs::path outFile = tempDir / (tags[0] + ".sav");
                fs::copy_file(file.path(), outFile, fs::copy_options::overwrite_existing);
                std::cout << "[OK] " << outFile.filename() << "\n";
                convertedCount++;
            }
        }
        if (convertedCount == 0)
            error("No valid save files were converted.");
        else
            std::cout << "[INFO] Converted " << convertedCount << " save files in total.\n";

        std::string userConfirm;
        std::cout << "Do you want to proceed with copying to Epic Games Hogwarts Legacy saves Directory? (y/n): ";
        std::getline(std::cin, userConfirm);

        if (userConfirm != "y" && userConfirm != "Y")
            return 0;

        // Construct Epic save root
        fs::path epicRoot = userDir / "AppData" / "Local" / "HogwartsLegacy" / "Saved" / "SaveGames";
        if (!dirExists(epicRoot))
            error("Epic Games SaveGames directory not found.");
        fs::path epicUserDir = isValidEpicUserIdFolder(epicRoot);
        std::cout << "[INFO] Epic Games userId folder identified: " << epicUserDir.filename() << "\n";

        std::cout << "Proceed with deleting existing saves?? (y/n): ";
        std::getline(std::cin, userConfirm);

        if (userConfirm != "y" && userConfirm != "Y")
            return 0;

        std::cout << "[INFO] Cleaning Existing Hogwarts legacy Epic save directory...\n";
        for (const auto &file : fs::directory_iterator(epicUserDir))
        {
            if (!file.is_regular_file())
                continue;
            std::string name = file.path().filename().string();
            if (((name.size() >= 3 && name.compare(0, 3, "HL-") == 0) && file.path().extension() == ".sav") || name == "SaveGameList.sav")
            {
                fs::remove(file.path());
                std::cout << "[DEL] " << name << "\n";
            }
        }

        std::cout << "[INFO] Copying converted saves...\n";
        for (const auto &file : fs::directory_iterator(tempDir))
        {
            fs::copy_file(file.path(), epicUserDir / file.path().filename(), fs::copy_options::overwrite_existing);
            std::cout << "[ADD] " << file.path().filename() << "\n";
        }

        std::cout << "\n[SUCCESS] Save migration completed.\n";
    }
    catch (const std::exception &e) {
        error(std::string("Unhandled exception: ") + e.what());
    }
}