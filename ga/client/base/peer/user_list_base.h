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

#ifndef BASE__PEER__USER_LIST_BASE_H
#define BASE__PEER__USER_LIST_BASE_H

#include "base/peer/user.h"

namespace base {

class UserListBase
{
public:
    virtual ~UserListBase() = default;

    virtual void add(const User& user) = 0;
    virtual User find(std::u16string_view username) const = 0;
    virtual const ByteArray& seedKey() const = 0;
    virtual void setSeedKey(const ByteArray& seed_key) = 0;
    virtual std::vector<User> list() const = 0;
};

} // namespace base

#endif // BASE__PEER__USER_LIST_BASE_H
