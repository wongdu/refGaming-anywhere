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

#ifndef COMMON__FILE_DEPACKETIZER_H
#define COMMON__FILE_DEPACKETIZER_H

#include "base/macros_magic.h"
#include "proto/file_transfer.pb.h"

#include <filesystem>
#include <fstream>
#include <memory>

namespace common {

class FileDepacketizer
{
public:
    ~FileDepacketizer();

    static std::unique_ptr<FileDepacketizer> create(const std::filesystem::path& file_path,
                                                    bool overwrite);

    // Reads the packet and writes its contents to a file.
    bool writeNextPacket(const proto::FilePacket& packet);

private:
    FileDepacketizer(const std::filesystem::path& file_path, std::ofstream&& file_stream);

    std::filesystem::path file_path_;
    std::ofstream file_stream_;

    uint64_t file_size_ = 0;
    uint64_t left_size_ = 0;

    DISALLOW_COPY_AND_ASSIGN(FileDepacketizer);
};

} // namespace common

#endif // COMMON__FILE_DEPACKETIZER_H
