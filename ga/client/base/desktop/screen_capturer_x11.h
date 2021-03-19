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

#ifndef BASE__DESKTOP__SCREEN_CAPTURER_X11_H
#define BASE__DESKTOP__SCREEN_CAPTURER_X11_H

#include "base/desktop/screen_capturer.h"

namespace base {

class ScreenCapturerX11 : public ScreenCapturer
{
public:
    ScreenCapturerX11();
    ~ScreenCapturerX11();

    // ScreenCapturer implementation.
    int screenCount() override;
    bool screenList(ScreenList* screens) override;
    bool selectScreen(ScreenId screen_id) override;
    const Frame* captureFrame(Error* error) override;

protected:
    // ScreenCapturer implementation.
    void reset() override;

private:
    DISALLOW_COPY_AND_ASSIGN(ScreenCapturerX11);
};

} // namespace base

#endif // BASE__DESKTOP__SCREEN_CAPTURER_X11_H