//
// Aspia Project
// Copyright (C) 2020 Dmitry Chapyshev <dmitry@aspia.ru>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <https://www.gnu.org/licenses/>.
//

#ifndef BASE__STRINGS__STRING_PRINTF_H
#define BASE__STRINGS__STRING_PRINTF_H

#include "build/build_config.h"

#include <string>

namespace base {

// Return a C++ string given vprintf-like input.
std::string stringPrintfV(const char* format, va_list args);

// Return a C++ string given printf-like input.
std::string stringPrintf(const char* format, ...);

// Store result into a supplied string and return it.
const std::string& sStringPrintf(std::string* dst, const char* format, ...);

#if defined(OS_WIN)
std::wstring stringPrintfV(const wchar_t* format, va_list args);
std::wstring stringPrintf(const wchar_t* format, ...);
const std::wstring& sStringPrintf(std::wstring* dst, const wchar_t* format, ...);
#endif // defined(OS_WIN)

} // namespace base

#endif // BASE__STRINGS__STRING_PRINTF_H
