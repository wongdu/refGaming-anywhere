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

#ifndef BASE__DESKTOP__SHARED_MEMORY_FRAME_H
#define BASE__DESKTOP__SHARED_MEMORY_FRAME_H

#include "base/desktop/frame.h"

#include <memory>

namespace base {

class SharedMemoryFactory;

class SharedMemoryFrame : public Frame
{
public:
    ~SharedMemoryFrame();

    static std::unique_ptr<Frame> create(
        const Size& size, SharedMemoryFactory* shared_memory_factory);

    static std::unique_ptr<Frame> open(
        const Size& size, int id, SharedMemoryFactory* shared_memory_factory);

    static std::unique_ptr<Frame> attach(
        const Size& size, std::unique_ptr<SharedMemoryBase> shared_memory);

private:
    SharedMemoryFrame(const Size& size, SharedMemoryBase* shared_memory);

    DISALLOW_COPY_AND_ASSIGN(SharedMemoryFrame);
};

} // namespace base

#endif // BASE__DESKTOP__SHARED_MEMORY_FRAME_H
