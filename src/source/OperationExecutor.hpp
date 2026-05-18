#pragma once

/**
 * @file OperationExecutor.hpp
 * @brief Dispatcher module that routes parsed CLI arguments to corresponding image processing operations.
 * @details Acts as a bridge between the command-line interface and the BmpImage manipulation methods.
 *          Selects and executes the appropriate operation based on the parsed flags, or triggers
 *          help/info output when requested. Ensures strict one-operation-per-run policy.
 */

#include "BmpImage.hpp"
#include "CommandLineInterface.hpp"

/**
 * @brief Executes the selected image operation based on parsed command-line arguments.
 * @details Routes execution to the appropriate BmpImage method (mirror, copy, color_replace, split)
 *          or outputs help/info metadata. The function acts as a central switchboard, ensuring
 *          that only the operation specified by the user is performed. Modifies the image in-place
 *          or prints metadata depending on the active flag.
 *
 * @param[in,out] bmp       Reference to the BmpImage instance to be processed/modified.
 * @param[in]     arguments Structure containing all parsed CLI options, operation parameters, and state
 * flags.
 */
void operationExecutor(BmpImage &bmp, struct argument arguments);