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

#ifndef COMMON__CLIPBOARD_MAC_H
#define COMMON__CLIPBOARD_MAC_H

#include "base/macros_magic.h"
#include "base/waitable_timer.h"
#include "common/clipboard.h"

namespace common {

class ClipboardMac : public Clipboard
{
public:
    ClipboardMac();
    ~ClipboardMac();

protected:
    // Clipboard implementation.
    void init() override;
    void setData(const std::string& data) override;

private:
    void startTimer();
    void checkForChanges();

    base::WaitableTimer timer_;
    int current_change_count_ = 0;

    DISALLOW_COPY_AND_ASSIGN(ClipboardMac);
};

} // namespace common

#endif // COMMON__CLIPBOARD_MAC_H
