#ifndef TEMP2_STRINGS_STRING_UTILS_HPP
#define TEMP2_STRINGS_STRING_UTILS_HPP

#include <string>
#include <vector>

namespace temp2::strings {

/**
 * @brief String manipulation utilities
 */
class StringUtils {
public:
    // Case conversion
    static std::string to_upper(const std::string& str);
    static std::string to_lower(const std::string& str);
    static std::string capitalize(const std::string& str);
    static std::string title_case(const std::string& str);
    static std::string swap_case(const std::string& str);

    // Trimming
    static std::string trim(const std::string& str);
    static std::string trim_left(const std::string& str);
    static std::string trim_right(const std::string& str);
    static std::string trim_chars(const std::string& str, const std::string& chars);

    // Padding
    static std::string pad_left(const std::string& str, size_t width, char pad = ' ');
    static std::string pad_right(const std::string& str, size_t width, char pad = ' ');
    static std::string center(const std::string& str, size_t width, char pad = ' ');

    // Search and replace
    static bool starts_with(const std::string& str, const std::string& prefix);
    static bool ends_with(const std::string& str, const std::string& suffix);
    static bool contains(const std::string& str, const std::string& substr);
    static size_t count_occurrences(const std::string& str, const std::string& substr);
    static std::string replace(const std::string& str, const std::string& from,
                                const std::string& to);
    static std::string replace_all(const std::string& str, const std::string& from,
                                    const std::string& to);
    static std::string remove(const std::string& str, const std::string& substr);

    // Splitting and joining
    static std::vector<std::string> split(const std::string& str, char delimiter);
    static std::vector<std::string> split(const std::string& str, const std::string& delimiter);
    static std::vector<std::string> split_lines(const std::string& str);
    static std::vector<std::string> split_words(const std::string& str);
    static std::string join(const std::vector<std::string>& parts, const std::string& separator);

    // Character checks
    static bool is_alpha(const std::string& str);
    static bool is_numeric(const std::string& str);
    static bool is_alphanumeric(const std::string& str);
    static bool is_whitespace(const std::string& str);
    static bool is_empty_or_whitespace(const std::string& str);

    // Substring
    static std::string substring(const std::string& str, size_t start, size_t length = std::string::npos);
    static std::string left(const std::string& str, size_t count);
    static std::string right(const std::string& str, size_t count);

    // Reversal
    static std::string reverse(const std::string& str);

    // Repetition
    static std::string repeat(const std::string& str, size_t count);
};

/**
 * @brief Character utilities
 */
class CharUtils {
public:
    static bool is_upper(char c);
    static bool is_lower(char c);
    static bool is_alpha(char c);
    static bool is_digit(char c);
    static bool is_alphanumeric(char c);
    static bool is_whitespace(char c);
    static bool is_punctuation(char c);
    static bool is_printable(char c);

    static char to_upper(char c);
    static char to_lower(char c);

    static int digit_value(char c);
    static char from_digit(int value);
};

/**
 * @brief String builder for efficient concatenation
 */
class StringBuilder {
public:
    StringBuilder();
    explicit StringBuilder(size_t initial_capacity);

    StringBuilder& append(const std::string& str);
    StringBuilder& append(char c);
    StringBuilder& append(int value);
    StringBuilder& append(double value);
    StringBuilder& append_line(const std::string& str = "");

    StringBuilder& insert(size_t pos, const std::string& str);
    StringBuilder& remove(size_t pos, size_t length);
    StringBuilder& clear();

    std::string to_string() const;
    size_t length() const;
    size_t capacity() const;
    bool empty() const;

    char at(size_t index) const;
    void set_at(size_t index, char c);

private:
    std::string buffer_;
};

}  // namespace temp2::strings

#endif  // TEMP2_STRINGS_STRING_UTILS_HPP
