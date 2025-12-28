#ifndef TEMP2_STRINGS_FORMATTER_HPP
#define TEMP2_STRINGS_FORMATTER_HPP

#include <string>
#include <vector>
#include <map>
#include <sstream>

namespace temp2::strings {

/**
 * @brief Number formatting utilities
 */
class NumberFormatter {
public:
    // Integer formatting
    static std::string format_int(int value);
    static std::string format_with_commas(long long value);
    static std::string format_binary(int value, int min_digits = 0);
    static std::string format_hex(int value, bool uppercase = false, int min_digits = 0);
    static std::string format_octal(int value, int min_digits = 0);

    // Floating point formatting
    static std::string format_fixed(double value, int decimals = 2);
    static std::string format_scientific(double value, int decimals = 2);
    static std::string format_percent(double value, int decimals = 1);
    static std::string format_currency(double value, const std::string& symbol = "$");

    // Byte sizes
    static std::string format_bytes(long long bytes, bool use_binary = true);
    static std::string format_bits(long long bits);

    // Time durations
    static std::string format_duration_ms(long long milliseconds);
    static std::string format_duration_seconds(double seconds);
};

/**
 * @brief Date/time formatting
 */
class DateFormatter {
public:
    struct Date {
        int year;
        int month;
        int day;
    };

    struct Time {
        int hour;
        int minute;
        int second;
        int millisecond = 0;
    };

    struct DateTime {
        Date date;
        Time time;
    };

    // Date formatting
    static std::string format_date(const Date& date, const std::string& format = "YYYY-MM-DD");
    static std::string format_date_iso(const Date& date);
    static std::string format_date_us(const Date& date);
    static std::string format_date_eu(const Date& date);

    // Time formatting
    static std::string format_time(const Time& time, bool include_ms = false);
    static std::string format_time_12h(const Time& time);
    static std::string format_time_24h(const Time& time);

    // DateTime formatting
    static std::string format_datetime(const DateTime& dt);
    static std::string format_datetime_iso(const DateTime& dt);

    // Parsing
    static Date parse_date(const std::string& str, const std::string& format = "YYYY-MM-DD");
    static Time parse_time(const std::string& str);
};

/**
 * @brief Simple template-based string formatting
 */
class TemplateFormatter {
public:
    TemplateFormatter();
    explicit TemplateFormatter(const std::string& template_str);

    void set_template(const std::string& template_str);
    void set(const std::string& key, const std::string& value);
    void set(const std::string& key, int value);
    void set(const std::string& key, double value);
    void clear_values();

    std::string format() const;

    // Static helpers
    static std::string format(const std::string& template_str,
                               const std::map<std::string, std::string>& values);

private:
    std::string template_;
    std::map<std::string, std::string> values_;
};

/**
 * @brief Table formatter for aligned text output
 */
class TableFormatter {
public:
    enum class Alignment { Left, Center, Right };

    TableFormatter();

    void add_column(const std::string& header, Alignment align = Alignment::Left);
    void add_row(const std::vector<std::string>& values);
    void set_column_width(size_t column, size_t width);
    void set_separator(char horizontal = '-', char vertical = '|', char corner = '+');

    std::string format() const;
    void clear_rows();
    void clear();

    size_t row_count() const;
    size_t column_count() const;

private:
    struct Column {
        std::string header;
        Alignment alignment;
        size_t width;
    };

    std::vector<Column> columns_;
    std::vector<std::vector<std::string>> rows_;
    char h_sep_ = '-';
    char v_sep_ = '|';
    char corner_ = '+';

    void calculate_widths();
    std::string format_cell(const std::string& content, size_t width, Alignment align) const;
    std::string format_separator() const;
};

/**
 * @brief JSON-like formatting for nested structures
 */
class PrettyPrinter {
public:
    PrettyPrinter();

    void set_indent(int spaces);
    void set_array_inline_threshold(size_t threshold);

    std::string format_map(const std::map<std::string, std::string>& data);
    std::string format_list(const std::vector<std::string>& data);
    std::string format_nested_map(const std::map<std::string, std::map<std::string, std::string>>& data);

private:
    int indent_spaces_ = 2;
    size_t inline_threshold_ = 3;
};

}  // namespace temp2::strings

#endif  // TEMP2_STRINGS_FORMATTER_HPP
