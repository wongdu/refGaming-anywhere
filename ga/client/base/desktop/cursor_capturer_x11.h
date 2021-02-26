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

#ifndef BASE__DESKTOP__CURSOR_CAPTURER_X11_H
#define BASE__DESKTOP__CURSOR_CAPTURER_X11_H

#include "base/macros_magic.h"
#include "desktop/cursor_capturer.h"

namespace base {

class CursorCapturerX11 : public CursorCapturer
{
public:
    CursorCapturerX11();
    ~CursorCapturerX11();

    const MouseCursor* captureCursor() override;
    void reset() override;

private:
    DISALLOW_COPY_AND_ASSIGN(CursorCapturerX11);
};

} // namespace base

#endif // BASE__DESKTOP__CURSOR_CAPTURER_X11_H
