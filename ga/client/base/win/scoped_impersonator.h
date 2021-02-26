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

#ifndef BASE__WIN__SCOPED_IMPERSONATOR_H
#define BASE__WIN__SCOPED_IMPERSONATOR_H

#include "base/macros_magic.h"
#include "base/threading/thread_checker.h"

#include <Windows.h>

namespace base::win {

class ScopedImpersonator
{
public:
    ScopedImpersonator();
    ~ScopedImpersonator();

    bool loggedOnUser(HANDLE user_token);
    bool anonymous();
    bool namedPipeClient(HANDLE named_pipe);

    void revertToSelf();

    bool isImpersonated() const { return impersonated_; }

private:
    bool impersonated_ = false;
    THREAD_CHECKER(thread_checker_);

    DISALLOW_COPY_AND_ASSIGN(ScopedImpersonator);
};

} // namespace base::win

#endif // BASE__WIN__SCOPED_IMPERSONATOR_H
