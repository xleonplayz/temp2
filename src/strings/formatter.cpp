#include "strings/formatter.hpp"
#include <iomanip>
#include <cmath>
#include <algorithm>

namespace temp2::strings {

// =============================================================================
// NumberFormatter
// =============================================================================

std::string NumberFormatter::format_int(int value) {
    return std::to_string(value);
}

std::string NumberFormatter::format_with_commas(long long value) {
    bool negative = value < 0;
    if (negative) value = -value;

    std::string str = std::to_string(value);
    std::string result;

    int count = 0;
    for (int i = str.size() - 1; i >= 0; --i) {
        if (count > 0 && count % 3 == 0) {
            result = ',' + result;
        }
        result = str[i] + result;
        ++count;
    }

    return negative ? '-' + result : result;
}

std::string NumberFormatter::format_binary(int value, int min_digits) {
    if (value == 0) {
        return std::string(std::max(1, min_digits), '0');
    }

    std::string result;
    unsigned int v = static_cast<unsigned int>(value);
    while (v > 0) {
        result = char('0' + (v & 1)) + result;
        v >>= 1;
    }

    while (static_cast<int>(result.size()) < min_digits) {
        result = '0' + result;
    }
    return result;
}

std::string NumberFormatter::format_hex(int value, bool uppercase, int min_digits) {
    std::ostringstream oss;
    oss << std::hex;
    if (uppercase) oss << std::uppercase;
    oss << std::setfill('0') << std::setw(min_digits) << value;
    return oss.str();
}

std::string NumberFormatter::format_octal(int value, int min_digits) {
    std::ostringstream oss;
    oss << std::oct << std::setfill('0') << std::setw(min_digits) << value;
    return oss.str();
}

std::string NumberFormatter::format_fixed(double value, int decimals) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(decimals) << value;
    return oss.str();
}

std::string NumberFormatter::format_scientific(double value, int decimals) {
    std::ostringstream oss;
    oss << std::scientific << std::setprecision(decimals) << value;
    return oss.str();
}

std::string NumberFormatter::format_percent(double value, int decimals) {
    return format_fixed(value * 100.0, decimals) + "%";
}

std::string NumberFormatter::format_currency(double value, const std::string& symbol) {
    std::ostringstream oss;
    oss << symbol << std::fixed << std::setprecision(2) << value;
    return oss.str();
}

std::string NumberFormatter::format_bytes(long long bytes, bool use_binary) {
    const char* units_binary[] = {"B", "KiB", "MiB", "GiB", "TiB", "PiB"};
    const char* units_decimal[] = {"B", "KB", "MB", "GB", "TB", "PB"};
    const char** units = use_binary ? units_binary : units_decimal;
    double divisor = use_binary ? 1024.0 : 1000.0;

    double value = static_cast<double>(bytes);
    int unit_index = 0;

    while (std::abs(value) >= divisor && unit_index < 5) {
        value /= divisor;
        ++unit_index;
    }

    std::ostringstream oss;
    if (unit_index == 0) {
        oss << bytes << " " << units[unit_index];
    } else {
        oss << std::fixed << std::setprecision(2) << value << " " << units[unit_index];
    }
    return oss.str();
}

std::string NumberFormatter::format_bits(long long bits) {
    const char* units[] = {"b", "Kb", "Mb", "Gb", "Tb"};
    double value = static_cast<double>(bits);
    int unit_index = 0;

    while (std::abs(value) >= 1000.0 && unit_index < 4) {
        value /= 1000.0;
        ++unit_index;
    }

    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << value << " " << units[unit_index];
    return oss.str();
}

std::string NumberFormatter::format_duration_ms(long long milliseconds) {
    if (milliseconds < 1000) {
        return std::to_string(milliseconds) + "ms";
    }

    long long total_seconds = milliseconds / 1000;
    long long ms = milliseconds % 1000;
    long long seconds = total_seconds % 60;
    long long minutes = (total_seconds / 60) % 60;
    long long hours = total_seconds / 3600;

    std::ostringstream oss;
    if (hours > 0) {
        oss << hours << "h " << minutes << "m " << seconds << "s";
    } else if (minutes > 0) {
        oss << minutes << "m " << seconds << "s";
    } else {
        oss << seconds << "." << std::setfill('0') << std::setw(3) << ms << "s";
    }
    return oss.str();
}

std::string NumberFormatter::format_duration_seconds(double seconds) {
    return format_duration_ms(static_cast<long long>(seconds * 1000));
}

// =============================================================================
// DateFormatter
// =============================================================================

std::string DateFormatter::format_date(const Date& date, const std::string& format) {
    std::string result = format;

    // Replace year patterns
    std::ostringstream year_full, year_short;
    year_full << std::setfill('0') << std::setw(4) << date.year;
    year_short << std::setfill('0') << std::setw(2) << (date.year % 100);

    size_t pos;
    while ((pos = result.find("YYYY")) != std::string::npos) {
        result.replace(pos, 4, year_full.str());
    }
    while ((pos = result.find("YY")) != std::string::npos) {
        result.replace(pos, 2, year_short.str());
    }

    // Replace month patterns
    std::ostringstream month;
    month << std::setfill('0') << std::setw(2) << date.month;
    while ((pos = result.find("MM")) != std::string::npos) {
        result.replace(pos, 2, month.str());
    }

    // Replace day patterns
    std::ostringstream day;
    day << std::setfill('0') << std::setw(2) << date.day;
    while ((pos = result.find("DD")) != std::string::npos) {
        result.replace(pos, 2, day.str());
    }

    return result;
}

std::string DateFormatter::format_date_iso(const Date& date) {
    return format_date(date, "YYYY-MM-DD");
}

std::string DateFormatter::format_date_us(const Date& date) {
    return format_date(date, "MM/DD/YYYY");
}

std::string DateFormatter::format_date_eu(const Date& date) {
    return format_date(date, "DD.MM.YYYY");
}

std::string DateFormatter::format_time(const Time& time, bool include_ms) {
    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(2) << time.hour << ":"
        << std::setfill('0') << std::setw(2) << time.minute << ":"
        << std::setfill('0') << std::setw(2) << time.second;
    if (include_ms) {
        oss << "." << std::setfill('0') << std::setw(3) << time.millisecond;
    }
    return oss.str();
}

std::string DateFormatter::format_time_12h(const Time& time) {
    int hour = time.hour % 12;
    if (hour == 0) hour = 12;
    const char* ampm = time.hour < 12 ? "AM" : "PM";

    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(2) << hour << ":"
        << std::setfill('0') << std::setw(2) << time.minute << ":"
        << std::setfill('0') << std::setw(2) << time.second << " " << ampm;
    return oss.str();
}

std::string DateFormatter::format_time_24h(const Time& time) {
    return format_time(time, false);
}

std::string DateFormatter::format_datetime(const DateTime& dt) {
    return format_date_iso(dt.date) + " " + format_time(dt.time, false);
}

std::string DateFormatter::format_datetime_iso(const DateTime& dt) {
    return format_date_iso(dt.date) + "T" + format_time(dt.time, true) + "Z";
}

DateFormatter::Date DateFormatter::parse_date(const std::string& str, const std::string& format) {
    Date date = {0, 0, 0};

    if (format == "YYYY-MM-DD" && str.size() >= 10) {
        date.year = std::stoi(str.substr(0, 4));
        date.month = std::stoi(str.substr(5, 2));
        date.day = std::stoi(str.substr(8, 2));
    } else if (format == "MM/DD/YYYY" && str.size() >= 10) {
        date.month = std::stoi(str.substr(0, 2));
        date.day = std::stoi(str.substr(3, 2));
        date.year = std::stoi(str.substr(6, 4));
    } else if (format == "DD.MM.YYYY" && str.size() >= 10) {
        date.day = std::stoi(str.substr(0, 2));
        date.month = std::stoi(str.substr(3, 2));
        date.year = std::stoi(str.substr(6, 4));
    }

    return date;
}

DateFormatter::Time DateFormatter::parse_time(const std::string& str) {
    Time time = {0, 0, 0, 0};

    if (str.size() >= 8) {
        time.hour = std::stoi(str.substr(0, 2));
        time.minute = std::stoi(str.substr(3, 2));
        time.second = std::stoi(str.substr(6, 2));
        if (str.size() >= 12 && str[8] == '.') {
            time.millisecond = std::stoi(str.substr(9, 3));
        }
    }

    return time;
}

// =============================================================================
// TemplateFormatter
// =============================================================================

TemplateFormatter::TemplateFormatter() {}

TemplateFormatter::TemplateFormatter(const std::string& template_str)
    : template_(template_str) {}

void TemplateFormatter::set_template(const std::string& template_str) {
    template_ = template_str;
}

void TemplateFormatter::set(const std::string& key, const std::string& value) {
    values_[key] = value;
}

void TemplateFormatter::set(const std::string& key, int value) {
    values_[key] = std::to_string(value);
}

void TemplateFormatter::set(const std::string& key, double value) {
    values_[key] = std::to_string(value);
}

void TemplateFormatter::clear_values() {
    values_.clear();
}

std::string TemplateFormatter::format() const {
    std::string result = template_;

    for (const auto& [key, value] : values_) {
        std::string placeholder = "{{" + key + "}}";
        size_t pos = 0;
        while ((pos = result.find(placeholder, pos)) != std::string::npos) {
            result.replace(pos, placeholder.size(), value);
            pos += value.size();
        }
    }

    return result;
}

std::string TemplateFormatter::format(const std::string& template_str,
                                       const std::map<std::string, std::string>& values) {
    TemplateFormatter formatter(template_str);
    for (const auto& [key, value] : values) {
        formatter.set(key, value);
    }
    return formatter.format();
}

// =============================================================================
// TableFormatter
// =============================================================================

TableFormatter::TableFormatter() {}

void TableFormatter::add_column(const std::string& header, Alignment align) {
    columns_.push_back({header, align, header.size()});
}

void TableFormatter::add_row(const std::vector<std::string>& values) {
    rows_.push_back(values);
}

void TableFormatter::set_column_width(size_t column, size_t width) {
    if (column < columns_.size()) {
        columns_[column].width = width;
    }
}

void TableFormatter::set_separator(char horizontal, char vertical, char corner) {
    h_sep_ = horizontal;
    v_sep_ = vertical;
    corner_ = corner;
}

void TableFormatter::calculate_widths() {
    for (size_t col = 0; col < columns_.size(); ++col) {
        size_t max_width = columns_[col].header.size();
        for (const auto& row : rows_) {
            if (col < row.size() && row[col].size() > max_width) {
                max_width = row[col].size();
            }
        }
        columns_[col].width = max_width;
    }
}

std::string TableFormatter::format_cell(const std::string& content, size_t width,
                                         Alignment align) const {
    if (content.size() >= width) {
        return content.substr(0, width);
    }

    size_t padding = width - content.size();
    switch (align) {
        case Alignment::Left:
            return content + std::string(padding, ' ');
        case Alignment::Right:
            return std::string(padding, ' ') + content;
        case Alignment::Center: {
            size_t left = padding / 2;
            size_t right = padding - left;
            return std::string(left, ' ') + content + std::string(right, ' ');
        }
    }
    return content;
}

std::string TableFormatter::format_separator() const {
    std::ostringstream oss;
    oss << corner_;
    for (size_t col = 0; col < columns_.size(); ++col) {
        oss << std::string(columns_[col].width + 2, h_sep_) << corner_;
    }
    return oss.str();
}

std::string TableFormatter::format() const {
    if (columns_.empty()) return "";

    // Make non-const copy to modify widths
    TableFormatter copy = *this;
    copy.calculate_widths();

    std::ostringstream oss;

    // Top separator
    oss << copy.format_separator() << "\n";

    // Header row
    oss << v_sep_;
    for (size_t col = 0; col < copy.columns_.size(); ++col) {
        oss << " " << copy.format_cell(copy.columns_[col].header,
                                        copy.columns_[col].width,
                                        copy.columns_[col].alignment) << " " << v_sep_;
    }
    oss << "\n";

    // Header separator
    oss << copy.format_separator() << "\n";

    // Data rows
    for (const auto& row : copy.rows_) {
        oss << v_sep_;
        for (size_t col = 0; col < copy.columns_.size(); ++col) {
            std::string value = col < row.size() ? row[col] : "";
            oss << " " << copy.format_cell(value, copy.columns_[col].width,
                                            copy.columns_[col].alignment) << " " << v_sep_;
        }
        oss << "\n";
    }

    // Bottom separator
    oss << copy.format_separator();

    return oss.str();
}

void TableFormatter::clear_rows() {
    rows_.clear();
}

void TableFormatter::clear() {
    columns_.clear();
    rows_.clear();
}

size_t TableFormatter::row_count() const {
    return rows_.size();
}

size_t TableFormatter::column_count() const {
    return columns_.size();
}

// =============================================================================
// PrettyPrinter
// =============================================================================

PrettyPrinter::PrettyPrinter() {}

void PrettyPrinter::set_indent(int spaces) {
    indent_spaces_ = spaces;
}

void PrettyPrinter::set_array_inline_threshold(size_t threshold) {
    inline_threshold_ = threshold;
}

std::string PrettyPrinter::format_map(const std::map<std::string, std::string>& data) {
    std::ostringstream oss;
    std::string indent(indent_spaces_, ' ');

    oss << "{\n";
    size_t i = 0;
    for (const auto& [key, value] : data) {
        oss << indent << "\"" << key << "\": \"" << value << "\"";
        if (++i < data.size()) oss << ",";
        oss << "\n";
    }
    oss << "}";
    return oss.str();
}

std::string PrettyPrinter::format_list(const std::vector<std::string>& data) {
    if (data.size() <= inline_threshold_) {
        std::ostringstream oss;
        oss << "[";
        for (size_t i = 0; i < data.size(); ++i) {
            oss << "\"" << data[i] << "\"";
            if (i < data.size() - 1) oss << ", ";
        }
        oss << "]";
        return oss.str();
    }

    std::ostringstream oss;
    std::string indent(indent_spaces_, ' ');

    oss << "[\n";
    for (size_t i = 0; i < data.size(); ++i) {
        oss << indent << "\"" << data[i] << "\"";
        if (i < data.size() - 1) oss << ",";
        oss << "\n";
    }
    oss << "]";
    return oss.str();
}

std::string PrettyPrinter::format_nested_map(
    const std::map<std::string, std::map<std::string, std::string>>& data) {

    std::ostringstream oss;
    std::string indent1(indent_spaces_, ' ');
    std::string indent2(indent_spaces_ * 2, ' ');

    oss << "{\n";
    size_t i = 0;
    for (const auto& [key, inner] : data) {
        oss << indent1 << "\"" << key << "\": {\n";
        size_t j = 0;
        for (const auto& [inner_key, value] : inner) {
            oss << indent2 << "\"" << inner_key << "\": \"" << value << "\"";
            if (++j < inner.size()) oss << ",";
            oss << "\n";
        }
        oss << indent1 << "}";
        if (++i < data.size()) oss << ",";
        oss << "\n";
    }
    oss << "}";
    return oss.str();
}

}  // namespace temp2::strings
