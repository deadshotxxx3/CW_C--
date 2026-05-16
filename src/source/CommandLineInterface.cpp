#include "CommandLineInterface.hpp"
#include "Error.hpp"
#include "ProcessingFlag.hpp"
#include <cctype>
#include <cstring>
#include <iostream>
#include <sstream>
#include <vector>

const int MAX_VALUE_COMPONENTS = 255;
const char DELIMETER = '.';

struct option options[] = {{"input", required_argument, 0, 'i'},
                           {"output", required_argument, 0, 'o'},

                           {"color_replace", no_argument, 0, 'c'},
                           {"old_color", required_argument, 0, 'O'},
                           {"new_color", required_argument, 0, 'N'},

                           {"copy", no_argument, 0, 'b'},
                           {"left_up", required_argument, 0, 'l'},
                           {"right_down", required_argument, 0, 'r'},
                           {"dest_left_up", required_argument, 0, 'd'},

                           {"mirror", no_argument, 0, 'm'},
                           {"axis", required_argument, 0, 'a'},

                           {"split", no_argument, 0, 's'},
                           {"number_x", required_argument, 0, 'x'},
                           {"number_y", required_argument, 0, 'y'},
                           {"thickness", required_argument, 0, 't'},
                           {"color", required_argument, 0, 'C'},

                           {"info", no_argument, 0, 'I'},
                           {"help", no_argument, 0, 'h'},
                           {0, 0, 0, 0}};

bool check_range_component(int values)
{
    if (values < 0 || values > MAX_VALUE_COMPONENTS)
        return false;
    return true;
}

bool check_valid_value(std::string values)
{
    for (char i : values) {
        if (!isdigit(i) && i != '-')
            return false;
    }
    return true;
}

std::pair<Coordinate, error_marker_t> getCoordinate(const std::string &str)
{
    std::stringstream sub_string(str);
    std::string token;
    std::vector<int> values;

    while (std::getline(sub_string, token, DELIMETER)) {
        if (!check_valid_value(token)) {
            std::cerr << "Invalid coordinate format: non-numeric character\n";
            return {Coordinate(), error_marker_t::ERR_INCORRECTARG};
        }
        values.push_back(std::stoi(token));
    }

    if (values.size() != 2) {
        std::cerr << "Invalid coordinate format: expected x.y\n";
        return {Coordinate(), error_marker_t::ERR_INCORRECTARG};
    }

    return {Coordinate(values[0], values[1]), error_marker_t::ERR_OK};
}

std::pair<Pixel, error_marker_t> getPixel(const std::string &str)
{
    std::stringstream sub_string(str);
    std::string token;
    std::vector<int> values;

    while (std::getline(sub_string, token, DELIMETER)) {
        if (!check_valid_value(token)) {
            std::cerr << "Invalid color format: non-numeric character\n";
            return {Pixel(), error_marker_t::ERR_INCORRECTARG};
        }
        values.push_back(std::stoi(token));
    }

    if (values.size() != 3) {
        std::cerr << "Invalid color format: expected r.g.b\n";
        return {Pixel(), error_marker_t::ERR_INCORRECTARG};
    }

    int r = values[0], g = values[1], b = values[2];

    if (!check_range_component(r) || !check_range_component(g) || !check_range_component(b)) {
        std::cerr << "Color component out of range (0-255)\n";
        return {Pixel(), error_marker_t::ERR_INCORRECTARG};
    }
    return {Pixel(r, g, b), error_marker_t::ERR_OK};
}

error_marker_t setInputFileIfNeeded(struct argument &arguments, int argc, char *argv[])
{
    if (arguments.inputName.empty() && optind < argc) {
        arguments.inputName = argv[optind];
        optind++;
    }

    if (arguments.inputName.empty()) {
        std::cerr << "Error: No input file specified\n";
        return error_marker_t::ERR_INCORRECTARG;
    }
    return error_marker_t::ERR_OK;
}

struct OpInfo {
    flags f;
    const char *name;
};

static OpInfo get_op_info(char c)
{
    switch (c) {
        case 'h':
            return {flags::FLAG_HELP, "help"};
        case 'c':
            return {flags::FLAG_COLOR_REPLACE, "color_replace"};
        case 'b':
            return {flags::FLAG_COPY, "copy"};
        case 'm':
            return {flags::FLAG_MIRROR, "mirror"};
        case 's':
            return {flags::FLAG_SPLIT, "split"};
        case 'I':
            return {flags::FLAG_INFO, "info"};
        default:
            return {flags::NO_FLAG, nullptr};
    }
}

error_marker_t check_operation(char c, struct argument &arguments, const char *&active_op_name)
{
    OpInfo info = get_op_info(c);
    if (info.f == flags::NO_FLAG)
        return error_marker_t::ERR_OK;

    if (arguments.flag == flags::NO_FLAG) {
        arguments.flag = info.f;
        active_op_name = info.name;
        return error_marker_t::ERR_OK;
    }

    if (arguments.flag == info.f) {
        std::cerr << "Error: Duplicate operation '" << info.name << "'\n";
    } else {
        std::cerr << "Error: Conflicting operations '" << active_op_name << "' and '" << info.name << "'\n";
    }
    return error_marker_t::ERR_INCORRECTARG;
}

static void set_primary_op(char check_c, char &primary_op, const char *&active_name)
{
    if (check_c == 'h') {
        primary_op = 'h';
        active_name = "help";
    } else if (check_c == 'I') {
        primary_op = 'I';
        active_name = "info";
    } else if (check_c == 'c') {
        primary_op = 'c';
        active_name = "color_replace";
    } else if (check_c == 'b') {
        primary_op = 'b';
        active_name = "copy";
    } else if (check_c == 'm') {
        primary_op = 'm';
        active_name = "mirror";
    } else if (check_c == 's') {
        primary_op = 's';
        active_name = "split";
    } else if (check_c == 'a') {
        primary_op = 'm';
        active_name = "mirror";
    } else if (check_c == 'd') {
        primary_op = 'b';
        active_name = "copy";
    } else if (check_c == 'O' || check_c == 'N') {
        primary_op = 'c';
        active_name = "color_replace";
    } else if (check_c == 'x' || check_c == 'y' || check_c == 't' || check_c == 'C') {
        primary_op = 's';
        active_name = "split";
    }
}

static bool belongs_to(char primary, char c)
{
    if (c == 'i' || c == 'o')
        return true;
    if (primary == 0)
        return true;
    switch (primary) {
        case 'h':
        case 'I':
            return c == primary;
        case 'c':
            return c == 'c' || c == 'O' || c == 'N';
        case 'b':
            return c == 'b' || c == 'l' || c == 'r' || c == 'd';
        case 'm':
            return c == 'm' || c == 'a' || c == 'l' || c == 'r';
        case 's':
            return c == 's' || c == 'x' || c == 'y' || c == 't' || c == 'C' || c == 'l' || c == 'r';
        default:
            return false;
    }
}

static void print_ignored(const std::vector<std::string> &list, const char *op_name)
{
    std::string formatted;
    for (size_t i = 0; i < list.size(); ++i) {
        formatted += "\"" + list[i] + "\"";
        if (i < list.size() - 1) {
            formatted += " and ";
        }
    }
    const char *name = op_name ? op_name : "used flags";
    std::cerr << "Arguments " << formatted << " are ignored because flag '--" << name
              << "' does not accept arguments.\n";
}

static error_marker_t handle_positional(int &optind, int argc, char *argv[], struct argument &arguments,
                                        const char *active_op_name)
{
    bool is_help_info = (arguments.flag == flags::FLAG_HELP || arguments.flag == flags::FLAG_INFO);
    if (is_help_info && optind < argc) {
        std::string extra;
        for (int i = optind; i < argc; ++i) {
            if (!extra.empty())
                extra += " ";
            extra += argv[i];
        }
        const char *flag = active_op_name ? active_op_name : "used flags";
        std::cerr << "Arguments '" << extra << "' are ignored because flag '--" << flag
                  << "' does not accept arguments.\n";
        return error_marker_t::ERR_INCORRECTARG;
    }

    if (arguments.inputName.empty() && optind < argc) {
        arguments.inputName = argv[optind];
        optind++;
    }

    if (optind < argc) {
        std::string extra;
        for (int i = optind; i < argc; ++i) {
            if (!extra.empty())
                extra += " ";
            extra += argv[i];
        }
        const char *flag = active_op_name ? active_op_name : "used flags";
        std::cerr << "Arguments '" << extra << "' are ignored because flag '--" << flag
                  << "' does not accept arguments.\n";
        return error_marker_t::ERR_INCORRECTARG;
    }
    return error_marker_t::ERR_OK;
}

error_marker_t CLI(int argc, char *argv[], struct argument &arguments)
{
    opterr = 0;
    int options_index = 0;
    int c;
    arguments.flag = flags::NO_FLAG;
    const char *active_op_name = nullptr;
    char primary_op = 0;
    std::vector<std::string> ignored_list;
    error_marker_t res = error_marker_t::ERR_OK;
    const char *arg = "";

    using ProcessFn = error_marker_t (*)(char, const char *, argument &);
    auto handle_op = [&](char trigger, ProcessFn proc_fn) {
        if (c == trigger) {
            res = check_operation(c, arguments, active_op_name);
        }
        if (res == error_marker_t::ERR_OK) {
            res = proc_fn(c, arg, arguments);
        }
    };

    while ((c = getopt_long(argc, argv, "i:o:O:N:l:r:d:a:x:y:t:C:cbmsIh", options, &options_index)) != -1) {
        arg = optarg ? optarg : "";
        char check_c = (c == '?') ? static_cast<char>(optopt) : static_cast<char>(c);

        if (primary_op == 0) {
            set_primary_op(check_c, primary_op, active_op_name);
        }

        if (!belongs_to(primary_op, check_c)) {
            std::string entry = "-" + std::string(1, check_c);
            if (optarg && optarg[0] != '-') {
                entry += " " + std::string(optarg);
            }
            ignored_list.push_back(entry);
            if (optarg && optarg[0] == '-') {
                ignored_list.push_back(optarg);
            }
            continue;
        }

        switch (c) {
            case 'i':
                if (!arg) {
                    std::cerr << "Error: input requires an argument\n";
                    return error_marker_t::ERR_INCORRECTARG;
                }
                arguments.inputName = arg;
                break;
            case 'o':
                if (!arg) {
                    std::cerr << "Error: output requires an argument\n";
                    return error_marker_t::ERR_INCORRECTARG;
                }
                arguments.outputName = arg;
                break;
            case 'h':
                res = check_operation(c, arguments, active_op_name);
                break;
            case 'c':
            case 'O':
            case 'N':
                handle_op('c', processing_color_flags);
                break;
            case 'b':
            case 'l':
            case 'r':
            case 'd':
                handle_op('b', processing_copy_flags);
                break;
            case 'm':
            case 'a':
                handle_op('m', processing_mirror_flag);
                break;
            case 's':
            case 'x':
            case 'y':
            case 't':
            case 'C':
                handle_op('s', processing_split_flag);
                break;
            case 'I':
                res = check_operation(c, arguments, active_op_name);
                break;
            default:
                if (optopt) {
                    std::cerr << "Error: Unknown option: -" << static_cast<char>(optopt) << '\n';
                } else {
                    std::cerr << "Error: Unknown option: " << argv[optind - 1] << '\n';
                }
                return error_marker_t::ERR_INCORRECTARG;
        }

        if (res != error_marker_t::ERR_OK) {
            return res;
        }
    }

    if (!ignored_list.empty()) {
        print_ignored(ignored_list, active_op_name);
        return error_marker_t::ERR_INCORRECTARG;
    }

    error_marker_t pos_res = handle_positional(optind, argc, argv, arguments, active_op_name);
    if (pos_res != error_marker_t::ERR_OK) {
        return pos_res;
    }

    if (arguments.flag == flags::FLAG_HELP || arguments.flag == flags::FLAG_INFO) {
        return error_marker_t::ERR_OK;
    }

    if (arguments.flag == flags::NO_FLAG) {
        std::cerr << "Error: No operation specified. Available: -m "
                  << "(mirror), -c (color_replace), -b (copy), -s " << "(split), -I (info).\n";
        return error_marker_t::ERR_INCORRECTARG;
    }

    return setInputFileIfNeeded(arguments, argc, argv);
}