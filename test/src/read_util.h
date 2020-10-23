#ifndef TEST_SRC_READ_UTIL_H_
#define TEST_SRC_READ_UTIL_H_

#include <any>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <typeinfo>

/**
 * Test helper function. Get value as type from map if any, otherwise return default value. If not possible to cast to
 * type, throw an exception.
 *
 * \tparam T       Type of value.
 * \param values   Lists non-default values as string->value. May remove an object if there's a hit.
 * \param name     Field name.
 * \param def      Default value.
 *
 * \return Value from map if there was a hit, or default if no hit.
 *
 * \throw std::runtime_error If key was present with missing value, or if the value could not be cast to type T.
 *
 * \note It is unfortunately not possible to use googletest macros inside templated code.
 */
template <typename T>
T get_val(std::map<std::string, std::any>* values, const std::string& name, T def) {
    T val2 = def;

    auto it = values->find(name);
    if (it != values->end()) {
        if (!it->second.has_value()) {
            std::stringstream ss;
            ss << "Field '" << name << "' with assumed type '" << typeid(T).name() << "' is missing its value'";
            throw std::runtime_error(ss.str());
        }
        try {
            val2 = std::any_cast<T>(it->second);
        } catch (const std::bad_cast&) {
            std::stringstream     ss;
            const std::type_info& info = it->second.type();
            ss << "Failed to cast field '" << name << "' from type '" << typeid(info).name() << "' to '"
               << typeid(T).name() << "'";
            throw std::runtime_error(ss.str());
        }

        /* Remove so it is possible to test for unused keys later */
        values->erase(it);
    }

    return val2;
}

/**
 * Check for remaining keys in map, and fail if so.
 *
 * \param values Map to check for remaining elements.
 */
void check_remaining(const std::map<std::string, std::any>& values);

#endif
