#pragma once

/**
 * @file CommandLineInterface.hpp
 * @brief Definitions for CLI argument parsing, flag enumeration, and command-line interface entry point.
 * @details Contains the `argument` structure for storing parsed options, the `flags` enumeration
 *          for operation selection, and the main `CLI` function signature. Used across all modules
 *          to share parsed configuration and validation state.
 */

#include "BmpImage.hpp"
#include <getopt.h>

/**
 * @enum flags
 * @brief Enumeration of available command-line operations and parser states.
 * @details Ensures mutual exclusivity of image processing commands. The parser
 *          switches to the corresponding enum value when a primary operation flag is encountered.
 */
enum class flags {
    NO_FLAG,            /**< No operation selected (initial state) */
    FLAG_HELP,          /**< --help / -h requested */
    FLAG_INFO,          /**< --info / -I requested */
    FLAG_MIRROR,        /**< --mirror / -m requested */
    FLAG_COPY,          /**< --copy / -b requested */
    FLAG_COLOR_REPLACE, /**< --color_replace / -c requested */
    FLAG_SPLIT,         /**< --split / -s requested */
};

/**
 * @struct argument
 * @brief Container for all parsed command-line arguments and validation states.
 * @details Holds file paths, operation parameters, coordinate/color data, and
 *          boolean tracking flags (`has_*`) to distinguish user-provided values
 *          from default-initialized ones. Populated entirely by CLI().
 */
struct argument {
    flags flag = flags::NO_FLAG;        /**< Currently selected primary operation */
    std::string inputName;              /**< Path to the source BMP file */
    std::string outputName = "out.bmp"; /**< Path to the destination BMP file (defaults to out.bmp) */
    Pixel old_color;                    /**< Target color for replacement operation */
    Pixel new_color;                    /**< Replacement color for color_replace operation */
    Coordinate left_up;                 /**< Top-left boundary of the processing region */
    Coordinate right_down;              /**< Bottom-right boundary of the processing region */
    Coordinate dest_left_up;            /**< Destination anchor point for copy operation */
    std::string axis;                   /**< Mirror axis identifier: "x" or "y" */
    int number_x;                       /**< Number of vertical grid divisions (split operation) */
    int number_y;                       /**< Number of horizontal grid divisions (split operation) */
    int thickness;                      /**< Line thickness in pixels (split/draw operations) */
    Pixel color_split;                  /**< Color used for grid lines or drawn elements */

    bool has_old_color = false;    /**< Tracks explicit -O flag usage */
    bool has_new_color = false;    /**< Tracks explicit -N flag usage */
    bool has_left_up = false;      /**< Tracks explicit -l flag usage */
    bool has_right_down = false;   /**< Tracks explicit -r flag usage */
    bool has_dest_left_up = false; /**< Tracks explicit -d flag usage */
    bool has_axis = false;         /**< Tracks explicit -a flag usage */
    bool has_number_x = false;     /**< Tracks explicit -x flag usage */
    bool has_number_y = false;     /**< Tracks explicit -y flag usage */
    bool has_thickness = false;    /**< Tracks explicit -t flag usage */
    bool has_color_split = false;  /**< Tracks explicit -C flag usage */
};

/**
 * @struct OpInfo
 * @brief Internal mapping between short option characters and operation metadata.
 * @details Used by check_operation() to resolve flags, assign the active operation,
 *          and detect duplicate or conflicting commands.
 */
struct OpInfo {
    flags f;          /**< Corresponding enum value from flags */
    const char *name; /**< Human-readable operation identifier */
};

/**
 * @brief Parses command-line arguments and populates the argument structure.
 * @details Processes short and long options using getopt_long, validates operation
 *          exclusivity, checks for missing required parameters, handles ignored/extra
 *          arguments, enforces coordinate ordering rules, and sets up input/output
 *          file paths. Stops execution and returns an error code on any validation failure.
 *
 * @param[in]  argc      Number of command-line arguments passed to main().
 * @param[in]  argv      Array of command-line argument strings.
 * @param[out] arguments Reference to the structure that will be filled with parsed data.
 * @return error_marker_t ERR_OK on successful parsing and validation,
 *         or a specific error code (ERR_INCORRECTARG, ERR_FEWARGS, ERR_EXTRARGS, etc.) on failure.
 */
error_marker_t CLI(int argc, char *argv[], struct argument &arguments);