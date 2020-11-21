#include "Gumball.hpp"

namespace gbLib {
    string getExtOfFilePath(string path) {
        return std::filesystem::path(path).extension().string();
    }
    string getNameOfFilePath(string path) {
        string fName = std::filesystem::path(path).filename().string();
        return fName.substr(0, fName.find_last_of("."));
    }
};