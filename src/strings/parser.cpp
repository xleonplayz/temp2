#include "strings/parser.hpp"
#include "strings/string_utils.hpp"
#include <sstream>
#include <cctype>
#include <algorithm>

namespace temp2::strings {

// =============================================================================
// NumberParser
// =============================================================================

std::optional<int> NumberParser::parse_int(const std::string& str) {
    std::string trimmed = StringUtils::trim(str);
    if (trimmed.empty()) return std::nullopt;

    try {
        size_t pos;
        int result = std::stoi(trimmed, &pos);
        if (pos == trimmed.size()) {
            return result;
        }
    } catch (...) {}
    return std::nullopt;
}

std::optional<long long> NumberParser::parse_long(const std::string& str) {
    std::string trimmed = StringUtils::trim(str);
    if (trimmed.empty()) return std::nullopt;

    try {
        size_t pos;
        long long result = std::stoll(trimmed, &pos);
        if (pos == trimmed.size()) {
            return result;
        }
    } catch (...) {}
    return std::nullopt;
}

std::optional<int> NumberParser::parse_binary(const std::string& str) {
    std::string trimmed = StringUtils::trim(str);
    if (trimmed.empty()) return std::nullopt;

    // Remove 0b prefix if present
    if (trimmed.size() > 2 && trimmed[0] == '0' && (trimmed[1] == 'b' || trimmed[1] == 'B')) {
        trimmed = trimmed.substr(2);
    }

    int result = 0;
    for (char c : trimmed) {
        if (c != '0' && c != '1') return std::nullopt;
        result = (result << 1) | (c - '0');
    }
    return result;
}

std::optional<int> NumberParser::parse_hex(const std::string& str) {
    std::string trimmed = StringUtils::trim(str);
    if (trimmed.empty()) return std::nullopt;

    // Remove 0x prefix if present
    if (trimmed.size() > 2 && trimmed[0] == '0' && (trimmed[1] == 'x' || trimmed[1] == 'X')) {
        trimmed = trimmed.substr(2);
    }

    try {
        size_t pos;
        int result = std::stoi(trimmed, &pos, 16);
        if (pos == trimmed.size()) {
            return result;
        }
    } catch (...) {}
    return std::nullopt;
}

std::optional<int> NumberParser::parse_octal(const std::string& str) {
    std::string trimmed = StringUtils::trim(str);
    if (trimmed.empty()) return std::nullopt;

    // Remove leading 0 if present
    if (trimmed.size() > 1 && trimmed[0] == '0' && trimmed[1] != 'x' && trimmed[1] != 'X') {
        trimmed = trimmed.substr(1);
    }

    try {
        size_t pos;
        int result = std::stoi(trimmed, &pos, 8);
        if (pos == trimmed.size()) {
            return result;
        }
    } catch (...) {}
    return std::nullopt;
}

std::optional<double> NumberParser::parse_double(const std::string& str) {
    std::string trimmed = StringUtils::trim(str);
    if (trimmed.empty()) return std::nullopt;

    try {
        size_t pos;
        double result = std::stod(trimmed, &pos);
        if (pos == trimmed.size()) {
            return result;
        }
    } catch (...) {}
    return std::nullopt;
}

std::optional<float> NumberParser::parse_float(const std::string& str) {
    auto result = parse_double(str);
    if (result) {
        return static_cast<float>(*result);
    }
    return std::nullopt;
}

std::optional<bool> NumberParser::parse_bool(const std::string& str) {
    std::string lower = StringUtils::to_lower(StringUtils::trim(str));

    if (lower == "true" || lower == "yes" || lower == "1" || lower == "on") {
        return true;
    }
    if (lower == "false" || lower == "no" || lower == "0" || lower == "off") {
        return false;
    }
    return std::nullopt;
}

int NumberParser::parse_int_or(const std::string& str, int default_value) {
    return parse_int(str).value_or(default_value);
}

double NumberParser::parse_double_or(const std::string& str, double default_value) {
    return parse_double(str).value_or(default_value);
}

bool NumberParser::parse_bool_or(const std::string& str, bool default_value) {
    return parse_bool(str).value_or(default_value);
}

// =============================================================================
// UrlParser
// =============================================================================

std::optional<UrlParser::Url> UrlParser::parse(const std::string& url_string) {
    Url url;

    size_t pos = 0;

    // Parse scheme
    size_t scheme_end = url_string.find("://", pos);
    if (scheme_end == std::string::npos) {
        return std::nullopt;
    }
    url.scheme = url_string.substr(pos, scheme_end);
    pos = scheme_end + 3;

    // Parse user:password@
    size_t at_pos = url_string.find('@', pos);
    size_t slash_pos = url_string.find('/', pos);
    if (at_pos != std::string::npos && (slash_pos == std::string::npos || at_pos < slash_pos)) {
        std::string userinfo = url_string.substr(pos, at_pos - pos);
        size_t colon = userinfo.find(':');
        if (colon != std::string::npos) {
            url.user = userinfo.substr(0, colon);
            url.password = userinfo.substr(colon + 1);
        } else {
            url.user = userinfo;
        }
        pos = at_pos + 1;
    }

    // Parse host:port
    size_t host_end = url_string.find_first_of("/?#", pos);
    if (host_end == std::string::npos) {
        host_end = url_string.size();
    }

    std::string host_port = url_string.substr(pos, host_end - pos);
    size_t colon = host_port.rfind(':');
    if (colon != std::string::npos && host_port.find(']') < colon) {
        // Handle IPv6 [host]:port
        url.host = host_port.substr(0, colon);
        auto port = NumberParser::parse_int(host_port.substr(colon + 1));
        if (port) url.port = *port;
    } else if (colon != std::string::npos && host_port.find('[') == std::string::npos) {
        url.host = host_port.substr(0, colon);
        auto port = NumberParser::parse_int(host_port.substr(colon + 1));
        if (port) url.port = *port;
    } else {
        url.host = host_port;
    }
    pos = host_end;

    if (pos >= url_string.size()) {
        url.path = "/";
        return url;
    }

    // Parse path
    size_t query_start = url_string.find('?', pos);
    size_t fragment_start = url_string.find('#', pos);
    size_t path_end = std::min(query_start, fragment_start);
    if (path_end == std::string::npos) {
        path_end = url_string.size();
    }
    url.path = url_string.substr(pos, path_end - pos);
    if (url.path.empty()) url.path = "/";

    // Parse query
    if (query_start != std::string::npos) {
        size_t query_end = fragment_start != std::string::npos ? fragment_start : url_string.size();
        url.query = url_string.substr(query_start + 1, query_end - query_start - 1);
    }

    // Parse fragment
    if (fragment_start != std::string::npos) {
        url.fragment = url_string.substr(fragment_start + 1);
    }

    return url;
}

std::string UrlParser::to_string(const Url& url) {
    std::ostringstream oss;

    oss << url.scheme << "://";

    if (!url.user.empty()) {
        oss << url.user;
        if (!url.password.empty()) {
            oss << ":" << url.password;
        }
        oss << "@";
    }

    oss << url.host;
    if (url.port > 0) {
        oss << ":" << url.port;
    }

    oss << url.path;

    if (!url.query.empty()) {
        oss << "?" << url.query;
    }

    if (!url.fragment.empty()) {
        oss << "#" << url.fragment;
    }

    return oss.str();
}

std::map<std::string, std::string> UrlParser::parse_query_string(const std::string& query) {
    std::map<std::string, std::string> result;

    auto pairs = StringUtils::split(query, '&');
    for (const auto& pair : pairs) {
        size_t eq = pair.find('=');
        if (eq != std::string::npos) {
            std::string key = url_decode(pair.substr(0, eq));
            std::string value = url_decode(pair.substr(eq + 1));
            result[key] = value;
        } else if (!pair.empty()) {
            result[url_decode(pair)] = "";
        }
    }

    return result;
}

std::string UrlParser::build_query_string(const std::map<std::string, std::string>& params) {
    std::ostringstream oss;
    bool first = true;

    for (const auto& [key, value] : params) {
        if (!first) oss << "&";
        oss << url_encode(key) << "=" << url_encode(value);
        first = false;
    }

    return oss.str();
}

std::string UrlParser::url_encode(const std::string& str) {
    std::ostringstream oss;

    for (unsigned char c : str) {
        if (std::isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
            oss << c;
        } else if (c == ' ') {
            oss << '+';
        } else {
            oss << '%' << std::hex << std::uppercase
                << ((c >> 4) & 0xF) << (c & 0xF);
        }
    }

    return oss.str();
}

std::string UrlParser::url_decode(const std::string& str) {
    std::ostringstream oss;

    for (size_t i = 0; i < str.size(); ++i) {
        if (str[i] == '%' && i + 2 < str.size()) {
            auto val = NumberParser::parse_hex(str.substr(i + 1, 2));
            if (val) {
                oss << static_cast<char>(*val);
                i += 2;
                continue;
            }
        } else if (str[i] == '+') {
            oss << ' ';
            continue;
        }
        oss << str[i];
    }

    return oss.str();
}

// =============================================================================
// KeyValueParser
// =============================================================================

KeyValueParser::KeyValueParser() {}

KeyValueParser::KeyValueParser(char separator, char delimiter)
    : separator_(separator), delimiter_(delimiter) {}

std::map<std::string, std::string> KeyValueParser::parse(const std::string& input) {
    std::map<std::string, std::string> result;

    auto lines = StringUtils::split(input, delimiter_);
    for (const auto& line : lines) {
        size_t sep_pos = line.find(separator_);
        if (sep_pos != std::string::npos) {
            std::string key = line.substr(0, sep_pos);
            std::string value = line.substr(sep_pos + 1);

            if (trim_whitespace_) {
                key = StringUtils::trim(key);
                value = StringUtils::trim(value);
            }

            if (!key.empty() && (allow_empty_ || !value.empty())) {
                result[key] = value;
            }
        }
    }

    return result;
}

std::string KeyValueParser::build(const std::map<std::string, std::string>& data) {
    std::ostringstream oss;
    bool first = true;

    for (const auto& [key, value] : data) {
        if (!first) oss << delimiter_;
        oss << key << separator_ << value;
        first = false;
    }

    return oss.str();
}

void KeyValueParser::set_separator(char sep) {
    separator_ = sep;
}

void KeyValueParser::set_delimiter(char delim) {
    delimiter_ = delim;
}

void KeyValueParser::set_trim_whitespace(bool trim) {
    trim_whitespace_ = trim;
}

void KeyValueParser::set_allow_empty_values(bool allow) {
    allow_empty_ = allow;
}

// =============================================================================
// CsvParser
// =============================================================================

CsvParser::CsvParser() {}

CsvParser::CsvParser(char delimiter, char quote)
    : delimiter_(delimiter), quote_(quote) {}

std::vector<std::string> CsvParser::parse_line(const std::string& line) {
    std::vector<std::string> result;
    std::string field;
    bool in_quotes = false;

    for (size_t i = 0; i < line.size(); ++i) {
        char c = line[i];

        if (in_quotes) {
            if (c == quote_) {
                if (i + 1 < line.size() && line[i + 1] == quote_) {
                    field += quote_;
                    ++i;
                } else {
                    in_quotes = false;
                }
            } else {
                field += c;
            }
        } else {
            if (c == quote_) {
                in_quotes = true;
            } else if (c == delimiter_) {
                result.push_back(field);
                field.clear();
            } else {
                field += c;
            }
        }
    }

    result.push_back(field);
    return result;
}

std::vector<std::vector<std::string>> CsvParser::parse(const std::string& input) {
    std::vector<std::vector<std::string>> result;

    auto lines = StringUtils::split_lines(input);
    bool first = true;

    for (const auto& line : lines) {
        if (line.empty()) continue;

        auto row = parse_line(line);

        if (first && has_header_) {
            headers_ = row;
            first = false;
            continue;
        }

        result.push_back(row);
        first = false;
    }

    return result;
}

std::string CsvParser::escape_field(const std::string& field) const {
    bool needs_quotes = field.find(delimiter_) != std::string::npos ||
                        field.find(quote_) != std::string::npos ||
                        field.find('\n') != std::string::npos;

    if (!needs_quotes) return field;

    std::ostringstream oss;
    oss << quote_;
    for (char c : field) {
        if (c == quote_) {
            oss << quote_ << quote_;
        } else {
            oss << c;
        }
    }
    oss << quote_;
    return oss.str();
}

std::string CsvParser::build_line(const std::vector<std::string>& row) {
    std::ostringstream oss;
    for (size_t i = 0; i < row.size(); ++i) {
        if (i > 0) oss << delimiter_;
        oss << escape_field(row[i]);
    }
    return oss.str();
}

std::string CsvParser::build(const std::vector<std::vector<std::string>>& data) {
    std::ostringstream oss;

    if (has_header_ && !headers_.empty()) {
        oss << build_line(headers_) << "\n";
    }

    for (const auto& row : data) {
        oss << build_line(row) << "\n";
    }

    return oss.str();
}

void CsvParser::set_delimiter(char delim) {
    delimiter_ = delim;
}

void CsvParser::set_quote(char quote) {
    quote_ = quote;
}

void CsvParser::set_has_header(bool has_header) {
    has_header_ = has_header;
}

std::vector<std::string> CsvParser::get_headers() const {
    return headers_;
}

// =============================================================================
// PathParser
// =============================================================================

PathParser::Path PathParser::parse(const std::string& path_string) {
    Path result;

    size_t last_sep = path_string.find_last_of("/\\");
    if (last_sep != std::string::npos) {
        result.directory = path_string.substr(0, last_sep);
        result.filename = path_string.substr(last_sep + 1);
    } else {
        result.filename = path_string;
    }

    size_t dot = result.filename.rfind('.');
    if (dot != std::string::npos && dot > 0) {
        result.stem = result.filename.substr(0, dot);
        result.extension = result.filename.substr(dot);
    } else {
        result.stem = result.filename;
    }

    return result;
}

std::string PathParser::join(const std::string& base, const std::string& path) {
    if (base.empty()) return path;
    if (path.empty()) return base;
    if (is_absolute(path)) return path;

    char sep = '/';
    if (base.back() == '/' || base.back() == '\\') {
        return base + path;
    }
    return base + sep + path;
}

std::string PathParser::normalize(const std::string& path) {
    auto parts = split_path(path);
    std::vector<std::string> normalized;

    for (const auto& part : parts) {
        if (part == ".") {
            continue;
        } else if (part == "..") {
            if (!normalized.empty() && normalized.back() != "..") {
                normalized.pop_back();
            } else {
                normalized.push_back(part);
            }
        } else if (!part.empty()) {
            normalized.push_back(part);
        }
    }

    std::string result;
    if (!path.empty() && (path[0] == '/' || path[0] == '\\')) {
        result = "/";
    }

    for (size_t i = 0; i < normalized.size(); ++i) {
        if (i > 0) result += "/";
        result += normalized[i];
    }

    return result.empty() ? "." : result;
}

std::string PathParser::get_parent(const std::string& path) {
    size_t last_sep = path.find_last_of("/\\");
    if (last_sep == std::string::npos) return ".";
    if (last_sep == 0) return "/";
    return path.substr(0, last_sep);
}

std::string PathParser::get_filename(const std::string& path) {
    return parse(path).filename;
}

std::string PathParser::get_extension(const std::string& path) {
    return parse(path).extension;
}

std::string PathParser::change_extension(const std::string& path, const std::string& new_ext) {
    Path parsed = parse(path);
    std::string new_filename = parsed.stem + new_ext;

    if (parsed.directory.empty()) {
        return new_filename;
    }
    return join(parsed.directory, new_filename);
}

bool PathParser::is_absolute(const std::string& path) {
    if (path.empty()) return false;
    if (path[0] == '/') return true;
    // Windows drive letter
    if (path.size() >= 2 && path[1] == ':' && std::isalpha(path[0])) {
        return true;
    }
    return false;
}

bool PathParser::is_relative(const std::string& path) {
    return !is_absolute(path);
}

std::vector<std::string> PathParser::split_path(const std::string& path) {
    std::vector<std::string> result;
    std::string current;

    for (char c : path) {
        if (c == '/' || c == '\\') {
            if (!current.empty()) {
                result.push_back(current);
                current.clear();
            }
        } else {
            current += c;
        }
    }

    if (!current.empty()) {
        result.push_back(current);
    }

    return result;
}

// =============================================================================
// ArgParser
// =============================================================================

ArgParser::ArgParser() {}

void ArgParser::add_flag(const std::string& name, const std::string& short_name) {
    flags_.push_back({name, short_name, false});
}

void ArgParser::add_option(const std::string& name, const std::string& short_name,
                           const std::string& default_value) {
    options_.push_back({name, short_name, default_value, std::nullopt});
}

void ArgParser::add_positional(const std::string& name) {
    positional_names_.push_back(name);
}

bool ArgParser::parse(int argc, const char* argv[]) {
    std::vector<std::string> args;
    for (int i = 0; i < argc; ++i) {
        args.push_back(argv[i]);
    }
    return parse(args);
}

bool ArgParser::parse(const std::vector<std::string>& args) {
    if (!args.empty()) {
        program_name_ = args[0];
    }

    for (size_t i = 1; i < args.size(); ++i) {
        const std::string& arg = args[i];

        // Check for long flags/options
        if (arg.size() > 2 && arg[0] == '-' && arg[1] == '-') {
            std::string name = arg.substr(2);

            // Check if it's a flag
            bool found = false;
            for (auto& flag : flags_) {
                if (flag.name == name) {
                    flag.value = true;
                    found = true;
                    break;
                }
            }
            if (found) continue;

            // Check if it's an option
            for (auto& opt : options_) {
                if (opt.name == name) {
                    if (i + 1 >= args.size()) {
                        error_ = "Missing value for option --" + name;
                        return false;
                    }
                    opt.value = args[++i];
                    found = true;
                    break;
                }
            }
            if (found) continue;

            error_ = "Unknown option: " + arg;
            return false;
        }

        // Check for short flags/options
        if (arg.size() > 1 && arg[0] == '-') {
            std::string name = arg.substr(1);

            bool found = false;
            for (auto& flag : flags_) {
                if (flag.short_name == name) {
                    flag.value = true;
                    found = true;
                    break;
                }
            }
            if (found) continue;

            for (auto& opt : options_) {
                if (opt.short_name == name) {
                    if (i + 1 >= args.size()) {
                        error_ = "Missing value for option -" + name;
                        return false;
                    }
                    opt.value = args[++i];
                    found = true;
                    break;
                }
            }
            if (found) continue;

            error_ = "Unknown option: " + arg;
            return false;
        }

        // Positional argument
        positional_values_.push_back(arg);
    }

    return true;
}

bool ArgParser::has_flag(const std::string& name) const {
    for (const auto& flag : flags_) {
        if (flag.name == name || flag.short_name == name) {
            return flag.value;
        }
    }
    return false;
}

std::string ArgParser::get_option(const std::string& name) const {
    for (const auto& opt : options_) {
        if (opt.name == name || opt.short_name == name) {
            return opt.value.value_or(opt.default_value);
        }
    }
    return "";
}

std::optional<std::string> ArgParser::get_option_optional(const std::string& name) const {
    for (const auto& opt : options_) {
        if (opt.name == name || opt.short_name == name) {
            return opt.value;
        }
    }
    return std::nullopt;
}

std::vector<std::string> ArgParser::get_positional() const {
    return positional_values_;
}

std::string ArgParser::get_error() const {
    return error_;
}

std::string ArgParser::get_usage() const {
    std::ostringstream oss;
    oss << "Usage: " << program_name_;

    for (const auto& flag : flags_) {
        oss << " [--" << flag.name;
        if (!flag.short_name.empty()) {
            oss << "|-" << flag.short_name;
        }
        oss << "]";
    }

    for (const auto& opt : options_) {
        oss << " [--" << opt.name;
        if (!opt.short_name.empty()) {
            oss << "|-" << opt.short_name;
        }
        oss << " <value>]";
    }

    for (const auto& pos : positional_names_) {
        oss << " <" << pos << ">";
    }

    return oss.str();
}

}  // namespace temp2::strings
