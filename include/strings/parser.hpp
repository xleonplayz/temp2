#ifndef TEMP2_STRINGS_PARSER_HPP
#define TEMP2_STRINGS_PARSER_HPP

#include <string>
#include <vector>
#include <optional>
#include <map>

namespace temp2::strings {

/**
 * @brief Parse numbers from strings
 */
class NumberParser {
public:
    // Integer parsing
    static std::optional<int> parse_int(const std::string& str);
    static std::optional<long long> parse_long(const std::string& str);
    static std::optional<int> parse_binary(const std::string& str);
    static std::optional<int> parse_hex(const std::string& str);
    static std::optional<int> parse_octal(const std::string& str);

    // Floating point parsing
    static std::optional<double> parse_double(const std::string& str);
    static std::optional<float> parse_float(const std::string& str);

    // Boolean parsing
    static std::optional<bool> parse_bool(const std::string& str);

    // With defaults
    static int parse_int_or(const std::string& str, int default_value);
    static double parse_double_or(const std::string& str, double default_value);
    static bool parse_bool_or(const std::string& str, bool default_value);
};

/**
 * @brief URL/URI parsing
 */
class UrlParser {
public:
    struct Url {
        std::string scheme;
        std::string host;
        int port = 0;
        std::string path;
        std::string query;
        std::string fragment;
        std::string user;
        std::string password;
    };

    static std::optional<Url> parse(const std::string& url_string);
    static std::string to_string(const Url& url);

    // Query string handling
    static std::map<std::string, std::string> parse_query_string(const std::string& query);
    static std::string build_query_string(const std::map<std::string, std::string>& params);

    // Encoding
    static std::string url_encode(const std::string& str);
    static std::string url_decode(const std::string& str);
};

/**
 * @brief Key-value pair parsing
 */
class KeyValueParser {
public:
    KeyValueParser();
    explicit KeyValueParser(char separator, char delimiter = '\n');

    std::map<std::string, std::string> parse(const std::string& input);
    std::string build(const std::map<std::string, std::string>& data);

    void set_separator(char sep);
    void set_delimiter(char delim);
    void set_trim_whitespace(bool trim);
    void set_allow_empty_values(bool allow);

private:
    char separator_ = '=';
    char delimiter_ = '\n';
    bool trim_whitespace_ = true;
    bool allow_empty_ = true;
};

/**
 * @brief CSV parsing
 */
class CsvParser {
public:
    CsvParser();
    explicit CsvParser(char delimiter, char quote = '"');

    std::vector<std::vector<std::string>> parse(const std::string& input);
    std::vector<std::string> parse_line(const std::string& line);
    std::string build(const std::vector<std::vector<std::string>>& data);
    std::string build_line(const std::vector<std::string>& row);

    void set_delimiter(char delim);
    void set_quote(char quote);
    void set_has_header(bool has_header);

    std::vector<std::string> get_headers() const;

private:
    char delimiter_ = ',';
    char quote_ = '"';
    bool has_header_ = false;
    std::vector<std::string> headers_;

    std::string escape_field(const std::string& field) const;
};

/**
 * @brief Path parsing utilities
 */
class PathParser {
public:
    struct Path {
        std::string directory;
        std::string filename;
        std::string stem;
        std::string extension;
    };

    static Path parse(const std::string& path_string);
    static std::string join(const std::string& base, const std::string& path);
    static std::string normalize(const std::string& path);
    static std::string get_parent(const std::string& path);
    static std::string get_filename(const std::string& path);
    static std::string get_extension(const std::string& path);
    static std::string change_extension(const std::string& path, const std::string& new_ext);

    static bool is_absolute(const std::string& path);
    static bool is_relative(const std::string& path);

    static std::vector<std::string> split_path(const std::string& path);
};

/**
 * @brief Command line argument parsing
 */
class ArgParser {
public:
    ArgParser();

    void add_flag(const std::string& name, const std::string& short_name = "");
    void add_option(const std::string& name, const std::string& short_name = "",
                    const std::string& default_value = "");
    void add_positional(const std::string& name);

    bool parse(int argc, const char* argv[]);
    bool parse(const std::vector<std::string>& args);

    bool has_flag(const std::string& name) const;
    std::string get_option(const std::string& name) const;
    std::optional<std::string> get_option_optional(const std::string& name) const;
    std::vector<std::string> get_positional() const;

    std::string get_error() const;
    std::string get_usage() const;

private:
    struct Flag {
        std::string name;
        std::string short_name;
        bool value = false;
    };

    struct Option {
        std::string name;
        std::string short_name;
        std::string default_value;
        std::optional<std::string> value;
    };

    std::vector<Flag> flags_;
    std::vector<Option> options_;
    std::vector<std::string> positional_names_;
    std::vector<std::string> positional_values_;
    std::string error_;
    std::string program_name_;
};

}  // namespace temp2::strings

#endif  // TEMP2_STRINGS_PARSER_HPP
