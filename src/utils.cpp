#include "utils.hpp"
#include <fstream>
#include <random>
#include <sstream>

std::time_t Utils::getCurrentTime() {
    return std::time(nullptr);
}

int Utils::calculateDaysDifference(std::time_t start, std::time_t end) {
    return static_cast<int>(difftime(end, start) / (60 * 60 * 24));
}

std::string Utils::generateUniqueId() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(0, 35);
    
    std::string id;
    const char chars[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    
    for(int i = 0; i < 8; ++i) {
        id += chars[dis(gen)];
    }
    return id;
}

void Utils::saveToFile(const std::string& filename, const std::string& content) {
    std::ofstream file(filename);
    if (file.is_open()) {
        file << content;
        file.close();
    }
}

std::string Utils::readFromFile(const std::string& filename) {
    std::ifstream file(filename);
    std::stringstream buffer;
    if (file.is_open()) {
        buffer << file.rdbuf();
        file.close();
    }
    return buffer.str();
}
