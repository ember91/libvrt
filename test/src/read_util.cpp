#include "read_util.h"

#include <any>
#include <map>
#include <sstream>
#include <string>

#include <gtest/gtest.h>

void check_remaining(const std::map<std::string, std::any>& values) {
    if (!values.empty()) {
        std::stringstream ss;
        if (values.size() == 1) {
            ss << "Unknown map key '" << values.begin()->first << "'";
        } else if (values.size() > 1) {
            ss << "Unknown map keys:\n";
            for (const auto& kv : values) {
                ss << kv.first << "\n";
            }
        }
        FAIL() << ss.str();
    }
}
