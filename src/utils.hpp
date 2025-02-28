#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <ctime>

class Utils {
public:
    static std::time_t getCurrentTime();
    static int calculateDaysDifference(std::time_t start, std::time_t end);
    static std::string generateUniqueId();
    static void saveToFile(const std::string& filename, const std::string& content);
    static std::string readFromFile(const std::string& filename);
};

#endif
