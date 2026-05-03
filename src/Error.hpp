#pragma once

enum class error_marker_t{
    ERR_OK = 0,
    ERR_NOTBMP = 40,
    ERR_READING = 41,
    ERR_WRITING = 42,
    ERR_INCORRECTARG = 43,
    ERR_EXTRARGS = 44,
    ERR_FEWARGS = 45,
    ERR_FILENAME = 46
}; 