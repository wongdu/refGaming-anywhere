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

#ifndef ASPIA_VERSION_H
#define ASPIA_VERSION_H

#define ASPIA_VERSION_MAJOR 2
#define ASPIA_VERSION_MINOR 1
#define ASPIA_VERSION_PATCH 0

#define ASPIA_VERSION_POSTFIX "dev"

#define STR_HELPER_INTERNAL(x) #x
#define STR_INTERNAL(x) STR_HELPER_INTERNAL(x)

#if defined(ASPIA_VERSION_POSTFIX)
#define ASPIA_VERSION_STRING                 \
    STR_INTERNAL(ASPIA_VERSION_MAJOR) "."    \
    STR_INTERNAL(ASPIA_VERSION_MINOR) "."    \
    STR_INTERNAL(ASPIA_VERSION_PATCH) " "    \
    ASPIA_VERSION_POSTFIX
#else
#define ASPIA_VERSION_STRING                 \
    STR_INTERNAL(ASPIA_VERSION_MAJOR) "."    \
    STR_INTERNAL(ASPIA_VERSION_MINOR) "."    \
    STR_INTERNAL(ASPIA_VERSION_PATCH)
#endif // defined(ASPIA_VERSION_POSTFIX)

#define ASPIA_VERSION                        \
    ASPIA_VERSION_MAJOR,ASPIA_VERSION_MINOR, \
    ASPIA_VERSION_PATCH

#endif // ASPIA_VERSION_H
