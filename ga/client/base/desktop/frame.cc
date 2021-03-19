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

#include "base/desktop/frame.h"

#include "base/logging.h"

#include <cstring>

#include <libyuv/convert_argb.h>

namespace base {

Frame::Frame(const Size& size,
             int stride,
             uint8_t* data,
             SharedMemoryBase* shared_memory)
    : data_(data),
      shared_memory_(shared_memory),
      size_(size),
      stride_(stride)
{
    DCHECK(size_.width() >= 0);
    DCHECK(size_.height() >= 0);
}

bool Frame::contains(int x, int y) const
{
    return (x >= 0 && x <= size_.width() && y >= 0 && y <= size_.height());
}

void Frame::copyPixelsFrom(const uint8_t* src_buffer, int src_stride, const Rect& dest_rect)
{
    libyuv::ARGBCopy(src_buffer, src_stride,
                     frameDataAtPos(dest_rect.topLeft()), stride(),
                     dest_rect.width(), dest_rect.height());
}

void Frame::copyPixelsFrom(const Frame& src_frame, const Point& src_pos, const Rect& dest_rect)
{
    copyPixelsFrom(src_frame.frameDataAtPos(src_pos), src_frame.stride(), dest_rect);
}

uint8_t* Frame::frameDataAtPos(const Point& pos) const
{
    return frameDataAtPos(pos.x(), pos.y());
}

uint8_t* Frame::frameDataAtPos(int x, int y) const
{
    return frameData() + stride() * y + kBytesPerPixel * x;
}

void Frame::copyFrameInfoFrom(const Frame& other)
{
    updated_region_ = other.updated_region_;
    top_left_ = other.top_left_;
    dpi_ = other.dpi_;
    capturer_type_ = other.capturer_type_;
}

// static
size_t Frame::calcMemorySize(const Size& size, int bytes_per_pixel)
{
    return ((size.width() + 128 * 2) * (size.height() + 128 * 2)) * bytes_per_pixel;
}

} // namespace base