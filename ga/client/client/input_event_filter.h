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

#ifndef CLIENT__INPUT_EVENT_FILTER_H
#define CLIENT__INPUT_EVENT_FILTER_H

#include "proto/common.pb.h"
#include "proto/desktop.pb.h"

#include <optional>

namespace client {

class InputEventFilter
{
public:
    InputEventFilter();
    ~InputEventFilter();

    void setSessionType(proto::SessionType session_type);
    void setClipboardEnabled(bool enable);

    std::optional<proto::MouseEvent> mouseEvent(const proto::MouseEvent& event);
    std::optional<proto::KeyEvent> keyEvent(const proto::KeyEvent& event);

    std::optional<proto::ClipboardEvent> readClipboardEvent(const proto::ClipboardEvent& event);
    std::optional<proto::ClipboardEvent> sendClipboardEvent(const proto::ClipboardEvent& event);

    int sendMouseCount() const { return send_mouse_count_; }
    int dropMouseCount() const { return drop_mouse_count_; }
    int sendKeyCount() const { return send_key_count_; }
    int readClipboardCount() const { return read_clipboard_count_; }
    int sendClipboardCount() const { return send_clipboard_count_; }

private:
    proto::SessionType session_type_ = proto::SESSION_TYPE_UNKNOWN;
    bool clipboard_enabled_ = false;

    int32_t last_pos_x_ = 0;
    int32_t last_pos_y_ = 0;
    uint32_t last_mask_ = 0;

    int send_mouse_count_ = 0;
    int drop_mouse_count_ = 0;
    int send_key_count_ = 0;
    int read_clipboard_count_ = 0;
    int send_clipboard_count_ = 0;
};

} // namespace client

#endif // CLIENT__INPUT_EVENT_FILTER_H
