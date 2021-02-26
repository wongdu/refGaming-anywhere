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

#ifndef BASE__SETTINGS__JSON_SETTINGS_H
#define BASE__SETTINGS__JSON_SETTINGS_H

#include "base/macros_magic.h"
#include "base/settings/settings.h"

#include <filesystem>

namespace base {

class JsonSettings : public Settings
{
public:
    enum class Scope { USER, SYSTEM };
    enum class Encrypted { YES, NO };

    explicit JsonSettings(std::string_view file_name, Encrypted encrypted = Encrypted::NO);
    JsonSettings(Scope scope,
                 std::string_view application_name,
                 std::string_view file_name,
                 Encrypted encrypted = Encrypted::NO);
    ~JsonSettings();

    bool isWritable() const;
    void sync();
    void flush();

    const std::filesystem::path& filePath() const { return path_; }

    static std::filesystem::path filePath(std::string_view file_name);
    static std::filesystem::path filePath(Scope scope,
                                          std::string_view application_name,
                                          std::string_view file_name);

    static bool readFile(const std::filesystem::path& file,
                         Map& map,
                         Encrypted encrypted = Encrypted::NO);
    static bool writeFile(const std::filesystem::path& file,
                          const Map& map,
                          Encrypted encrypted = Encrypted::NO);

private:
    const Encrypted encrypted_;
    std::filesystem::path path_;

    DISALLOW_COPY_AND_ASSIGN(JsonSettings);
};

} // namespace base

#endif // BASE__SETTINGS__JSON_SETTINGS_H
