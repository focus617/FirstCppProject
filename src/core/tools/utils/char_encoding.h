#pragma once

#include <ios>
#include <sstream>
#include <string>

#include "core/visibility_control.hpp"

namespace xuzy::internal {

// Converts a Unicode code point to a narrow string in UTF-8 encoding.
// code_point parameter is of type UInt32 because wchar_t may not be
// wide enough to contain a code point.
// If the code_point is not a valid Unicode code point
// (i.e. outside of Unicode range U+0 to U+10FFFF) it will be converted
// to "(Invalid Unicode 0xXXXXXXXX)".
XUZY_API std::string CodePointToUtf8(uint32_t code_point);

// Converts a wide string to a narrow string in UTF-8 encoding.
// The wide string is assumed to have the following encoding:
//   UTF-16 if sizeof(wchar_t) == 2 (on Windows, Cygwin)
//   UTF-32 if sizeof(wchar_t) == 4 (on Linux)
// Parameter str points to a null-terminated wide string.
// Parameter num_chars may additionally limit the number
// of wchar_t characters processed. -1 is used when the entire string
// should be processed.
// If the string contains code points that are not valid Unicode code points
// (i.e. outside of Unicode range U+0 to U+10FFFF) they will be output
// as '(Invalid Unicode 0xXXXXXXXX)'.
// If the string is in UTF16 encoding and contains invalid UTF-16 surrogate
// pairs, values in those pairs will be encoded as individual Unicode
// characters from Basic Normal Plane.
XUZY_API std::string WideStringToUtf8(const wchar_t* str, int num_chars);

// Gets the content of the stringstream's buffer as an std::string.  Each '\0'
// character in the buffer is replaced with "\\0".
XUZY_API std::string StringStreamToString(::std::stringstream* stream);

// Converts a wide C string to an std::string using the UTF-8 encoding.
// NULL will be converted to "(null)".
std::string ShowWideCString(const wchar_t* wide_c_str);

}  // namespace xuzy::internal