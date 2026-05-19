#include "CommandLineInterface.hpp"
#include "ProcessingFlag.hpp"
#include <cctype>
#include <cstring>
#include <iostream>
#include <sstream>
#include <vector>

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

/**
 * @internal
 * @brief Maps a short option character to its corresponding operation enum and display name.
 * @details Used by the CLI parser to resolve primary operation flags. Returns a structured
 *          pair containing the flags enum value and a human-readable string identifier.
 *          If the character does not correspond to a known operation, returns NO_FLAG and nullptr.
 *
 * @param[in] c The short option character to resolve (e.g., 'm', 'c', 'h', 'I').
 * @return OpInfo A structure containing the resolved flags enum and operation name.
 *         Defaults to {flags::NO_FLAG, nullptr} for unrecognized characters.
 */
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

/**
 * @internal
 * @brief Validates operation flags for duplicates or mutual exclusivity conflicts.
 * @details Resolves the short option character to a known operation using get_op_info().
 *          If no operation has been selected yet, it assigns the new operation and updates the
 *          active operation name. If an operation is already active, it detects duplicates
 *          (same flag twice) or conflicts (two different operations) and reports an error.
 *          Non-operation flags are safely ignored and return success.
 *
 * @param[in]     c             The short option character to validate (e.g., 'm', 'c', 'h').
 * @param[in,out] arguments     Reference to the parsed arguments structure. Reads the current
 *                              operation state and updates it if this is the first valid operation.
 * @param[in,out] active_op_name Reference to a pointer holding the human-readable name of the
 *                              currently active operation. Read for conflict messages, updated
 *                              upon successful assignment.
 * @return error_marker_t       ERR_OK if the operation is valid/new or not an operation flag;
 *                              ERR_INCORRECTARG if a duplicate or conflicting operation is detected.
 */
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

/**
 * @internal
 * @brief Identifies and assigns the primary operation category based on a parsed option character.
 * @details Maps various short options (including operation flags and their dependent parameters)
 *          to a unified primary operation identifier. For example, '-l', '-r', and '-d' all map
 *          to the 'copy' ('b') category, while '-O' and '-N' map to 'color_replace' ('c').
 *          Typically called only on the first encountered relevant option to lock the operation context.
 *
 * @param[in]     check_c      The option character to evaluate.
 * @param[in,out] primary_op   Reference to a char variable tracking the primary operation category.
 *                             Assigned the category character on a successful match.
 * @param[in,out] active_name  Reference to a pointer holding the operation's display name.
 *                             Assigned the corresponding string literal on a successful match.
 */
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

/**
 * @internal
 * @brief Checks if a command-line option belongs to the current primary operation group.
 * @details Validates option grouping to prevent mixing flags from different operations.
 *          Global flags ('i', 'o') are always accepted. If no primary operation is set yet
 *          (primary == 0), all options are temporarily allowed. Otherwise, strictly enforces
 *          the allowed subset for mirror, copy, split, color_replace, help, and info.
 *
 * @param[in] primary The character representing the currently active primary operation category.
 * @param[in] c       The option character to validate against the primary group.
 * @return true       If the option is permitted for the current context or is a global flag.
 * @return false      If the option conflicts with the active operation group.
 */
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

/**
 * @internal
 * @brief Formats and outputs an error message for ignored or superfluous arguments.
 * @details Concatenates a list of unexpected arguments into a human-readable string
 *          separated by " and ", then prints a standardized warning to standard error.
 *          Used when flags that do not accept arguments receive them, or when extra
 *          positional arguments are provided after valid parsing completes.
 *
 * @param[in] list    Vector of argument strings that were deemed invalid or ignored.
 * @param[in] op_name Pointer to the name of the active operation flag. If nullptr,
 *                    defaults to "used flags" in the output message.
 */
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

/**
 * @internal
 * @brief Processes remaining positional arguments after getopt_long parsing.
 * @details Validates that positional arguments are not provided alongside operations
 *          that do not accept them (help, info). If an input file was not explicitly
 *          specified via the -i flag, attempts to consume the next positional argument
 *          as the input path. Rejects any additional trailing arguments with a standardized
 *          error message to standard error.
 *
 * @param[in,out] optind         Reference to the global getopt index. Advanced when
 *                               consuming a positional input file.
 * @param[in]     argc           Total number of command-line arguments.
 * @param[in]     argv           Array of command-line argument strings.
 * @param[in,out] arguments      Reference to the parsed arguments structure. Updated
 *                               with inputName if a fallback positional argument is used.
 * @param[in]     active_op_name Pointer to the name of the currently active operation
 *                               for contextual error reporting.
 * @return error_marker_t        ERR_OK if arguments are handled correctly,
 *                               ERR_INCORRECTARG if invalid or extra arguments are detected.
 */
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

/**
 * @internal
 * @brief Validates that all mandatory parameters for the selected operation have been provided.
 * @details Inspects the boolean tracking flags (has_*) within the argument structure to ensure
 *          every required CLI option was explicitly passed by the user. Skips validation for
 *          global flags (help, info) or when no operation is selected. If any required parameter
 *          is missing, formats a comma-separated list of the absent flags, prints an error to
 *          standard error, and returns a specific error code.
 *
 * @param[in] args Reference to the fully parsed argument structure containing operation state and tracking
 * flags.
 * @return error_marker_t ERR_OK if all required arguments are present or validation is not applicable,
 *         ERR_FEWARGS if one or more mandatory parameters are missing.
 */
static error_marker_t validate_required_args(const argument &args)
{
    if (args.flag == flags::NO_FLAG || args.flag == flags::FLAG_HELP || args.flag == flags::FLAG_INFO) {
        return error_marker_t::ERR_OK;
    }

    std::vector<std::string> missing;
    switch (args.flag) {
        case flags::FLAG_COPY:
            if (!args.has_left_up)
                missing.push_back("-l");
            if (!args.has_right_down)
                missing.push_back("-r");
            if (!args.has_dest_left_up)
                missing.push_back("-d");
            break;
        case flags::FLAG_MIRROR:
            if (!args.has_axis)
                missing.push_back("-a");
            if (!args.has_left_up)
                missing.push_back("-l");
            if (!args.has_right_down)
                missing.push_back("-r");
            break;
        case flags::FLAG_COLOR_REPLACE:
            if (!args.has_old_color)
                missing.push_back("-O");
            if (!args.has_new_color)
                missing.push_back("-N");
            break;
        case flags::FLAG_SPLIT:
            if (!args.has_number_x)
                missing.push_back("-x");
            if (!args.has_number_y)
                missing.push_back("-y");
            if (!args.has_thickness)
                missing.push_back("-t");
            if (!args.has_color_split)
                missing.push_back("-C");
            break;
        default:
            break;
    }

    if (!missing.empty()) {
        std::cerr << "Error: Missing required arguments for operation: ";
        for (size_t i = 0; i < missing.size(); ++i) {
            std::cerr << missing[i] << (i < missing.size() - 1 ? ", " : "");
        }
        std::cerr << "\n";
        return error_marker_t::ERR_FEWARGS;
    }
    return error_marker_t::ERR_OK;
}

/**
 * @internal
 * @brief Validates the geometric alignment of region-defining coordinates.
 * @details Ensures that for operations requiring a rectangular area (copy, mirror),
 *          the bottom-right coordinate is not positioned to the left or above the top-left coordinate.
 *          Skips validation entirely for operations that do not use region boundaries.
 *
 * @param[in] args Parsed argument structure containing the active operation flag and coordinate pairs.
 * @return error_marker_t ERR_OK if coordinates are valid or validation is not applicable,
 *         ERR_INCORRECTARG if the region is geometrically invalid (right_down < left_up).
 */
static error_marker_t validate_region_coords(const argument &args)
{
    if (args.flag != flags::FLAG_COPY && args.flag != flags::FLAG_MIRROR) {
        return error_marker_t::ERR_OK;
    }
    if (args.right_down.x < args.left_up.x || args.right_down.y < args.left_up.y) {
        std::cerr << "Error: Bottom-right coordinate (-r) must be to the "
                     "right and below top-left coordinate (-l).\n";
        return error_marker_t::ERR_INCORRECTARG;
    }
    return error_marker_t::ERR_OK;
}

/**
 * @internal
 * @brief Resolves the input file path from remaining positional arguments.
 * @details Checks if the input file was explicitly specified via the `-i` flag.
 *          If not, it treats the next remaining command-line argument (left after flag parsing)
 *          as the input file path. Guarantees that an input path is always present before processing.
 *
 * @param[in,out] arguments Reference to the argument structure. Updates `inputName` if a fallback path is
 * detected.
 * @param[in]     argc      Total number of command-line arguments passed to the program.
 * @param[in]     argv      Array of command-line argument strings.
 * @return error_marker_t ERR_OK if `inputName` is successfully resolved,
 *         ERR_INCORRECTARG if no input file path is provided by either method.
 */
static error_marker_t setInputFileIfNeeded(struct argument &arguments, int argc, char *argv[])
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

/**
 * @internal
 * @brief Validates and finalizes parsed CLI arguments after the getopt loop.
 * @details Checks for ignored arguments, processes positional parameters,
 *          verifies operation selection, required flags, coordinate geometry,
 *          and input/output file constraints.
 *
 * @param[in,out] optind         Reference to getopt index (modified during positional handling).
 * @param[in]     argc           Total number of command-line arguments.
 * @param[in]     argv           Array of command-line argument strings.
 * @param[in,out] arguments      Parsed arguments structure (updated during validation).
 * @param[in]     ignored_list   List of arguments that conflicted with the active operation.
 * @param[in]     active_op_name Name of the currently active operation for error context.
 * @return error_marker_t        ERR_OK on success, or specific error code on validation failure.
 */
static error_marker_t finalize_cli_parsing(int &optind, int argc, char *argv[], struct argument &arguments,
                                           const std::vector<std::string> &ignored_list,
                                           const char *active_op_name)
{
    if (!ignored_list.empty()) {
        print_ignored(ignored_list, active_op_name);
        return error_marker_t::ERR_EXTRARGS;
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

    error_marker_t val_res = validate_required_args(arguments);
    if (val_res != error_marker_t::ERR_OK) {
        return val_res;
    }

    error_marker_t coord_res = validate_region_coords(arguments);
    if (coord_res != error_marker_t::ERR_OK) {
        return coord_res;
    }

    if (!arguments.inputName.empty() && !arguments.outputName.empty() &&
        arguments.inputName == arguments.outputName) {
        std::cerr << "Error: Input and output files must not be the same.\n";
        return error_marker_t::ERR_INCORRECTARG;
    }

    return setInputFileIfNeeded(arguments, argc, argv);
}

error_marker_t CLI(int argc, char *argv[], struct argument &arguments)
{
    opterr = 0;
    int long_opt_index = 0;
    int current_opt;
    const char *active_op_name = nullptr;
    char main_op_flag = 0;
    std::vector<std::string> ignored_args;
    error_marker_t parse_status = error_marker_t::ERR_OK;
    const char *opt_argument = "";

    using FlagHandler = error_marker_t (*)(char, const char *, argument &);
    auto process_flag_if_active = [&](char trigger, FlagHandler handler) {
        if (current_opt == trigger) {
            parse_status = check_operation(current_opt, arguments, active_op_name);
        }
        if (parse_status == error_marker_t::ERR_OK) {
            parse_status = handler(current_opt, opt_argument, arguments);
        }
    };

    while ((current_opt =
                getopt_long(argc, argv, "i:o:O:N:l:r:d:a:x:y:t:C:cbmsIh", options, &long_opt_index)) != -1) {
        opt_argument = optarg ? optarg : "";
        char validation_char =
            (current_opt == '?') ? static_cast<char>(optopt) : static_cast<char>(current_opt);

        if (main_op_flag == 0) {
            set_primary_op(validation_char, main_op_flag, active_op_name);
        }

        if (!belongs_to(main_op_flag, validation_char)) {
            std::string entry = "-" + std::string(1, validation_char);
            if (optarg && optarg[0] != '-') {
                entry += " " + std::string(optarg);
            }
            ignored_args.push_back(entry);
            if (optarg && optarg[0] == '-') {
                ignored_args.push_back(optarg);
            }
            continue;
        }

        switch (current_opt) {
            case 'i':
                if (!opt_argument) {
                    std::cerr << "Error: input requires an argument\n";
                    return error_marker_t::ERR_INCORRECTARG;
                }
                arguments.inputName = opt_argument;
                break;
            case 'o':
                if (!opt_argument) {
                    std::cerr << "Error: output requires an argument\n";
                    return error_marker_t::ERR_INCORRECTARG;
                }
                arguments.outputName = opt_argument;
                break;
            case 'h':
                parse_status = check_operation(current_opt, arguments, active_op_name);
                break;
            case 'c':
            case 'O':
            case 'N':
                process_flag_if_active('c', processing_color_flags);
                break;
            case 'b':
            case 'l':
            case 'r':
            case 'd':
                process_flag_if_active('b', processing_copy_flags);
                break;
            case 'm':
            case 'a':
                process_flag_if_active('m', processing_mirror_flag);
                break;
            case 's':
            case 'x':
            case 'y':
            case 't':
            case 'C':
                process_flag_if_active('s', processing_split_flag);
                break;
            case 'I':
                parse_status = check_operation(current_opt, arguments, active_op_name);
                break;
            default:
                if (optopt) {
                    std::cerr << "Error: Unknown option: -" << static_cast<char>(optopt) << '\n';
                } else {
                    std::cerr << "Error: Unknown option: " << argv[optind - 1] << '\n';
                }
                return error_marker_t::ERR_INCORRECTARG;
        }

        if (parse_status != error_marker_t::ERR_OK) {
            return parse_status;
        }
    }

    return finalize_cli_parsing(optind, argc, argv, arguments, ignored_args, active_op_name);
}