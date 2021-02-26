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

#include "base/desktop/shared_memory_frame.h"

#include "base/ipc/shared_memory.h"
#include "base/ipc/shared_memory_factory.h"

namespace base {

SharedMemoryFrame::SharedMemoryFrame(const Size& size,
                                     SharedMemoryBase* shared_memory)
    : Frame(size, size.width() * kBytesPerPixel,
            reinterpret_cast<uint8_t*>(shared_memory->data()), shared_memory)
{
    // Nothing
}

SharedMemoryFrame::~SharedMemoryFrame()
{
    delete shared_memory_;
}

// static
std::unique_ptr<Frame> SharedMemoryFrame::create(
    const Size& size, SharedMemoryFactory* shared_memory_factory)
{
    const size_t buffer_size = calcMemorySize(size, kBytesPerPixel);

    std::unique_ptr<SharedMemory> shared_memory = shared_memory_factory->create(buffer_size);
    if (!shared_memory)
        return nullptr;

    return std::unique_ptr<Frame>(new SharedMemoryFrame(size, shared_memory.release()));
}

// static
std::unique_ptr<Frame> SharedMemoryFrame::open(
    const Size& size, int id, SharedMemoryFactory* shared_memory_factory)
{
    std::unique_ptr<SharedMemory> shared_memory = shared_memory_factory->open(id);
    if (!shared_memory)
        return nullptr;

    return std::unique_ptr<Frame>(new SharedMemoryFrame(size, shared_memory.release()));
}

// static
std::unique_ptr<Frame> SharedMemoryFrame::attach(
    const Size& size, std::unique_ptr<SharedMemoryBase> shared_memory)
{
    return std::unique_ptr<Frame>(new SharedMemoryFrame(size, shared_memory.release()));
}

} // namespace base
