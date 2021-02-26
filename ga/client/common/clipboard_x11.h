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

#ifndef COMMON__CLIPBOARD_X11_H
#define COMMON__CLIPBOARD_X11_H

#include "base/macros_magic.h"
#include "common/clipboard.h"

struct _XDisplay;

namespace base {
class FileDescriptorWatcher;
class XServerClipboard;
} // namespace base

namespace common {

class ClipboardX11 : public Clipboard
{
public:
    ClipboardX11();
    ~ClipboardX11();

protected:
    // Clipboard implementation.
    void init() override;
    void setData(const std::string& data) override;

private:
    void pumpXEvents();

    // Underlying X11 clipboard implementation.
    std::unique_ptr<base::XServerClipboard> x_server_clipboard_;

    // Watcher used to handle X11 events from |display_|.
    std::unique_ptr<base::FileDescriptorWatcher> x_connection_watcher_;

    // Connection to the X server, used by |x_server_clipboard_|. This is created and owned by
    // this class.
    _XDisplay* display_ = nullptr;

    DISALLOW_COPY_AND_ASSIGN(ClipboardX11);
};

} // namespace common

#endif // COMMON__CLIPBOARD_X11_H
