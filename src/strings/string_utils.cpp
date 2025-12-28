#include "strings/string_utils.hpp"
#include <algorithm>
#include <cctype>
#include <sstream>

namespace temp2::strings {

// =============================================================================
// StringUtils
// =============================================================================

std::string StringUtils::to_upper(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(),
                   [](unsigned char c) { return std::toupper(c); });
    return result;
}

std::string StringUtils::to_lower(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return result;
}

std::string StringUtils::capitalize(const std::string& str) {
    if (str.empty()) return str;
    std::string result = to_lower(str);
    result[0] = std::toupper(static_cast<unsigned char>(result[0]));
    return result;
}

std::string StringUtils::title_case(const std::string& str) {
    std::string result = str;
    bool new_word = true;

    for (char& c : result) {
        if (std::isspace(static_cast<unsigned char>(c))) {
            new_word = true;
        } else if (new_word) {
            c = std::toupper(static_cast<unsigned char>(c));
            new_word = false;
        } else {
            c = std::tolower(static_cast<unsigned char>(c));
        }
    }
    return result;
}

std::string StringUtils::swap_case(const std::string& str) {
    std::string result = str;
    for (char& c : result) {
        if (std::isupper(static_cast<unsigned char>(c))) {
            c = std::tolower(static_cast<unsigned char>(c));
        } else if (std::islower(static_cast<unsigned char>(c))) {
            c = std::toupper(static_cast<unsigned char>(c));
        }
    }
    return result;
}

std::string StringUtils::trim(const std::string& str) {
    return trim_right(trim_left(str));
}

std::string StringUtils::trim_left(const std::string& str) {
    size_t start = 0;
    while (start < str.size() && std::isspace(static_cast<unsigned char>(str[start]))) {
        ++start;
    }
    return str.substr(start);
}

std::string StringUtils::trim_right(const std::string& str) {
    if (str.empty()) return str;
    size_t end = str.size();
    while (end > 0 && std::isspace(static_cast<unsigned char>(str[end - 1]))) {
        --end;
    }
    return str.substr(0, end);
}

std::string StringUtils::trim_chars(const std::string& str, const std::string& chars) {
    size_t start = str.find_first_not_of(chars);
    if (start == std::string::npos) return "";
    size_t end = str.find_last_not_of(chars);
    return str.substr(start, end - start + 1);
}

std::string StringUtils::pad_left(const std::string& str, size_t width, char pad) {
    if (str.size() >= width) return str;
    return std::string(width - str.size(), pad) + str;
}

std::string StringUtils::pad_right(const std::string& str, size_t width, char pad) {
    if (str.size() >= width) return str;
    return str + std::string(width - str.size(), pad);
}

std::string StringUtils::center(const std::string& str, size_t width, char pad) {
    if (str.size() >= width) return str;
    size_t total_padding = width - str.size();
    size_t left_padding = total_padding / 2;
    size_t right_padding = total_padding - left_padding;
    return std::string(left_padding, pad) + str + std::string(right_padding, pad);
}

bool StringUtils::starts_with(const std::string& str, const std::string& prefix) {
    if (prefix.size() > str.size()) return false;
    return str.compare(0, prefix.size(), prefix) == 0;
}

bool StringUtils::ends_with(const std::string& str, const std::string& suffix) {
    if (suffix.size() > str.size()) return false;
    return str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

bool StringUtils::contains(const std::string& str, const std::string& substr) {
    return str.find(substr) != std::string::npos;
}

size_t StringUtils::count_occurrences(const std::string& str, const std::string& substr) {
    if (substr.empty()) return 0;
    size_t count = 0;
    size_t pos = 0;
    while ((pos = str.find(substr, pos)) != std::string::npos) {
        ++count;
        pos += substr.size();
    }
    return count;
}

std::string StringUtils::replace(const std::string& str, const std::string& from,
                                  const std::string& to) {
    if (from.empty()) return str;
    size_t pos = str.find(from);
    if (pos == std::string::npos) return str;
    return str.substr(0, pos) + to + str.substr(pos + from.size());
}

std::string StringUtils::replace_all(const std::string& str, const std::string& from,
                                      const std::string& to) {
    if (from.empty()) return str;
    std::string result = str;
    size_t pos = 0;
    while ((pos = result.find(from, pos)) != std::string::npos) {
        result.replace(pos, from.size(), to);
        pos += to.size();
    }
    return result;
}

std::string StringUtils::remove(const std::string& str, const std::string& substr) {
    return replace_all(str, substr, "");
}

std::vector<std::string> StringUtils::split(const std::string& str, char delimiter) {
    std::vector<std::string> result;
    std::istringstream stream(str);
    std::string token;
    while (std::getline(stream, token, delimiter)) {
        result.push_back(token);
    }
    return result;
}

std::vector<std::string> StringUtils::split(const std::string& str, const std::string& delimiter) {
    std::vector<std::string> result;
    if (delimiter.empty()) {
        result.push_back(str);
        return result;
    }

    size_t start = 0;
    size_t end;
    while ((end = str.find(delimiter, start)) != std::string::npos) {
        result.push_back(str.substr(start, end - start));
        start = end + delimiter.size();
    }
    result.push_back(str.substr(start));
    return result;
}

std::vector<std::string> StringUtils::split_lines(const std::string& str) {
    std::vector<std::string> result;
    std::istringstream stream(str);
    std::string line;
    while (std::getline(stream, line)) {
        // Remove trailing \r if present
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        result.push_back(line);
    }
    return result;
}

std::vector<std::string> StringUtils::split_words(const std::string& str) {
    std::vector<std::string> result;
    std::istringstream stream(str);
    std::string word;
    while (stream >> word) {
        result.push_back(word);
    }
    return result;
}

std::string StringUtils::join(const std::vector<std::string>& parts, const std::string& separator) {
    if (parts.empty()) return "";

    std::string result = parts[0];
    for (size_t i = 1; i < parts.size(); ++i) {
        result += separator + parts[i];
    }
    return result;
}

bool StringUtils::is_alpha(const std::string& str) {
    if (str.empty()) return false;
    return std::all_of(str.begin(), str.end(),
                       [](unsigned char c) { return std::isalpha(c); });
}

bool StringUtils::is_numeric(const std::string& str) {
    if (str.empty()) return false;
    size_t start = 0;
    if (str[0] == '-' || str[0] == '+') start = 1;
    if (start >= str.size()) return false;

    bool has_dot = false;
    for (size_t i = start; i < str.size(); ++i) {
        if (str[i] == '.') {
            if (has_dot) return false;
            has_dot = true;
        } else if (!std::isdigit(static_cast<unsigned char>(str[i]))) {
            return false;
        }
    }
    return true;
}

bool StringUtils::is_alphanumeric(const std::string& str) {
    if (str.empty()) return false;
    return std::all_of(str.begin(), str.end(),
                       [](unsigned char c) { return std::isalnum(c); });
}

bool StringUtils::is_whitespace(const std::string& str) {
    if (str.empty()) return false;
    return std::all_of(str.begin(), str.end(),
                       [](unsigned char c) { return std::isspace(c); });
}

bool StringUtils::is_empty_or_whitespace(const std::string& str) {
    return str.empty() || is_whitespace(str);
}

std::string StringUtils::substring(const std::string& str, size_t start, size_t length) {
    if (start >= str.size()) return "";
    return str.substr(start, length);
}

std::string StringUtils::left(const std::string& str, size_t count) {
    return str.substr(0, count);
}

std::string StringUtils::right(const std::string& str, size_t count) {
    if (count >= str.size()) return str;
    return str.substr(str.size() - count);
}

std::string StringUtils::reverse(const std::string& str) {
    std::string result = str;
    std::reverse(result.begin(), result.end());
    return result;
}

std::string StringUtils::repeat(const std::string& str, size_t count) {
    std::string result;
    result.reserve(str.size() * count);
    for (size_t i = 0; i < count; ++i) {
        result += str;
    }
    return result;
}

// =============================================================================
// CharUtils
// =============================================================================

bool CharUtils::is_upper(char c) {
    return std::isupper(static_cast<unsigned char>(c));
}

bool CharUtils::is_lower(char c) {
    return std::islower(static_cast<unsigned char>(c));
}

bool CharUtils::is_alpha(char c) {
    return std::isalpha(static_cast<unsigned char>(c));
}

bool CharUtils::is_digit(char c) {
    return std::isdigit(static_cast<unsigned char>(c));
}

bool CharUtils::is_alphanumeric(char c) {
    return std::isalnum(static_cast<unsigned char>(c));
}

bool CharUtils::is_whitespace(char c) {
    return std::isspace(static_cast<unsigned char>(c));
}

bool CharUtils::is_punctuation(char c) {
    return std::ispunct(static_cast<unsigned char>(c));
}

bool CharUtils::is_printable(char c) {
    return std::isprint(static_cast<unsigned char>(c));
}

char CharUtils::to_upper(char c) {
    return static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
}

char CharUtils::to_lower(char c) {
    return static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
}

int CharUtils::digit_value(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return 10 + c - 'a';
    if (c >= 'A' && c <= 'F') return 10 + c - 'A';
    return -1;
}

char CharUtils::from_digit(int value) {
    if (value >= 0 && value <= 9) return '0' + value;
    if (value >= 10 && value <= 15) return 'a' + value - 10;
    return '?';
}

// =============================================================================
// StringBuilder
// =============================================================================

StringBuilder::StringBuilder() {}

StringBuilder::StringBuilder(size_t initial_capacity) {
    buffer_.reserve(initial_capacity);
}

StringBuilder& StringBuilder::append(const std::string& str) {
    buffer_ += str;
    return *this;
}

StringBuilder& StringBuilder::append(char c) {
    buffer_ += c;
    return *this;
}

StringBuilder& StringBuilder::append(int value) {
    buffer_ += std::to_string(value);
    return *this;
}

StringBuilder& StringBuilder::append(double value) {
    buffer_ += std::to_string(value);
    return *this;
}

StringBuilder& StringBuilder::append_line(const std::string& str) {
    buffer_ += str + "\n";
    return *this;
}

StringBuilder& StringBuilder::insert(size_t pos, const std::string& str) {
    if (pos > buffer_.size()) pos = buffer_.size();
    buffer_.insert(pos, str);
    return *this;
}

StringBuilder& StringBuilder::remove(size_t pos, size_t length) {
    if (pos < buffer_.size()) {
        buffer_.erase(pos, length);
    }
    return *this;
}

StringBuilder& StringBuilder::clear() {
    buffer_.clear();
    return *this;
}

std::string StringBuilder::to_string() const {
    return buffer_;
}

size_t StringBuilder::length() const {
    return buffer_.size();
}

size_t StringBuilder::capacity() const {
    return buffer_.capacity();
}

bool StringBuilder::empty() const {
    return buffer_.empty();
}

char StringBuilder::at(size_t index) const {
    return buffer_.at(index);
}

void StringBuilder::set_at(size_t index, char c) {
    buffer_.at(index) = c;
}

}  // namespace temp2::strings
