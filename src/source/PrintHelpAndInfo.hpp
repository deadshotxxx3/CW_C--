#pragma once

/**
 * @file PrintHelpAndInfo.hpp
 * @brief Declares utility functions for printing program documentation and metadata.
 * @details Provides the public interface for outputting CLI usage instructions,
 *          flag descriptions, and format specifications. Called when the user
 *          requests assistance via --help or -h.
 */

/**
 * @brief Outputs the complete command-line usage guide to standard output.
 * @details Prints a formatted help message listing all supported operations,
 *          required/optional flags, coordinate (x.y) and color (r.g.b) formats,
 *          and usage examples. Output is directed to std::cout and formatted
 *          for terminal readability. No state modification or return value.
 */
void print_help();