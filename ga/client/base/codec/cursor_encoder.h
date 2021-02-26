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

#ifndef BASE__CODEC__CURSOR_ENCODER_H
#define BASE__CODEC__CURSOR_ENCODER_H

#include "base/macros_magic.h"
#include "base/codec/scoped_zstd_stream.h"

#include <vector>

namespace proto {
class CursorShape;
} // namespace proto

namespace base {

class MouseCursor;

class CursorEncoder
{
public:
    CursorEncoder();
    ~CursorEncoder();

    bool encode(const MouseCursor& mouse_cursor, proto::CursorShape* cursor_shape);

private:
    bool compressCursor(const MouseCursor& mouse_cursor, proto::CursorShape* cursor_shape) const;

    ScopedZstdCStream stream_;
    std::vector<uint32_t> cache_;

    DISALLOW_COPY_AND_ASSIGN(CursorEncoder);
};

} // namespace base

#endif // BASE__CODEC__CURSOR_ENCODER_H
