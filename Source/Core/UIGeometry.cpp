#include "EunoiaUI/Core/UIGeometry.h"
#include <sstream>

namespace EunoiaUI {

UIColor UIColor::FromHex(const std::string& hex) {
    std::string clean = hex;
    if (!clean.empty() && clean[0] == '#') {
        clean = clean.substr(1);
    }

    if (clean.length() == 6) {
        unsigned int val = 0;
        std::stringstream ss;
        ss << std::hex << clean;
        ss >> val;
        int r = (val >> 16) & 0xFF;
        int g = (val >> 8) & 0xFF;
        int b = val & 0xFF;
        return UIColor(r, g, b, 255);
    } else if (clean.length() == 8) {
        unsigned int val = 0;
        std::stringstream ss;
        ss << std::hex << clean;
        ss >> val;
        int r = (val >> 24) & 0xFF;
        int g = (val >> 16) & 0xFF;
        int b = (val >> 8) & 0xFF;
        int a = val & 0xFF;
        return UIColor(r, g, b, a);
    }
    return UIColor::White();
}

} // namespace EunoiaUI
