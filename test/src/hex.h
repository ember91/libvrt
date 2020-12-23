#ifndef TEST_SRC_HEX_H_
#define TEST_SRC_HEX_H_

#include <cstdint>
#include <iomanip>
#include <limits>
#include <ostream>

/**
 * Helps with output formatting when using google test framework.
 * For example:
 * \code{.cpp}
 * ASSERT_EQ(Hex(b), Hex(0x00ABAB45))
 * \endcode
 * will output in hexadecimal format in case the assertion fails.
 */
class Hex {
   public:
    explicit Hex(uint64_t v) : val_(v) {}

   private:
    friend std::ostream& operator<<(std::ostream&, const Hex&);
    friend bool          operator==(const Hex&, const Hex&);

    /** Value to present as hexadecimal */
    uint64_t val_;
};

/**
 * Hexadecimal equality operator.
 *
 * \param lhs Left hand side object.
 * \param rhs Right hand side object.
 *
 * \return True if values are equal.
 */
inline bool operator==(const Hex& lhs, const Hex& rhs) {
    return lhs.val_ == rhs.val_;
}

/**
 * Hexadecimal output operator.
 *
 * \param os  Output stream.
 * \param hex Hexadecimal formatter.
 *
 * \return Output stream.
 */
inline std::ostream& operator<<(std::ostream& os, const Hex& hex) {
    /* Set width depending on if it's 32 or 64 bit. Two characters per byte. */
    int w = hex.val_ > std::numeric_limits<uint32_t>::max() ? 16 : 8;

    return os << "0x" << std::setfill('0') << std::setw(w) << std::hex << std::uppercase << hex.val_ << std::nouppercase
              << std::dec << std::setw(0) << std::setfill(' ');
}

#endif
