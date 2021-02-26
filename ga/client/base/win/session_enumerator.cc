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

#include "base/win/session_enumerator.h"

#include "base/logging.h"
#include "base/strings/unicode.h"

namespace base::win {

SessionEnumerator::SessionEnumerator()
{
    DWORD level = 1;

    if (!WTSEnumerateSessionsExW(WTS_CURRENT_SERVER_HANDLE, &level, 0, info_.recieve(), &count_))
    {
        PLOG(LS_WARNING) << "WTSEnumerateSessionsExW failed";
        return;
    }
}

SessionEnumerator::~SessionEnumerator() = default;

void SessionEnumerator::advance()
{
    ++current_;
}

bool SessionEnumerator::isAtEnd() const
{
    return current_ >= count_;
}

WTS_CONNECTSTATE_CLASS SessionEnumerator::state() const
{
    return info_[current_]->State;
}

SessionId SessionEnumerator::sessionId() const
{
    return info_[current_]->SessionId;
}

std::string SessionEnumerator::sessionName() const
{
    if (!info_[current_]->pSessionName)
        return std::string();

    return base::utf8FromWide(info_[current_]->pSessionName);
}

std::string SessionEnumerator::hostName() const
{
    if (!info_[current_]->pHostName)
        return std::string();

    return base::utf8FromWide(info_[current_]->pHostName);
}

std::string SessionEnumerator::userName() const
{
    if (!info_[current_]->pUserName)
        return std::string();

    return base::utf8FromWide(info_[current_]->pUserName);
}

std::string SessionEnumerator::domainName() const
{
    if (!info_[current_]->pDomainName)
        return std::string();

    return base::utf8FromWide(info_[current_]->pDomainName);
}

std::string SessionEnumerator::farmName() const
{
    if (!info_[current_]->pFarmName)
        return std::string();

    return base::utf8FromWide(info_[current_]->pFarmName);
}

} // namespace base::win
