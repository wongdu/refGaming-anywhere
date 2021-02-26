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

#ifndef BASE__DESKTOP__FRAME_DIB_H
#define BASE__DESKTOP__FRAME_DIB_H

#include "base/desktop/frame.h"
#include "base/win/scoped_gdi_object.h"

#include <memory>

namespace base {

class SharedMemory;
class SharedMemoryFactory;

class FrameDib : public Frame
{
public:
    ~FrameDib() = default;

    static std::unique_ptr<FrameDib> create(const Size& size,
                                            SharedMemoryFactory* shared_memory_factory,
                                            HDC hdc);

    HBITMAP bitmap() { return bitmap_; }

private:
    FrameDib(const Size& size,
             int stride,
             uint8_t* data,
             std::unique_ptr<SharedMemory> shared_memory,
             HBITMAP bitmap);

    win::ScopedHBITMAP bitmap_;
    std::unique_ptr<SharedMemory> owned_shared_memory_;

    DISALLOW_COPY_AND_ASSIGN(FrameDib);
};

} // namespace base

#endif // BASE__DESKTOP__FRAME_DIB_H
