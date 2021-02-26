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

#include "base/net/ip_util.h"

#include "base/strings/unicode.h"
#include "build/build_config.h"

//#include <ip2string.h>

#if defined(OS_WIN)
#include <ws2tcpip.h>
#elif defined(OS_POSIX)
#include <arpa/inet.h>
#else
#error Platform support not implemented
#endif // defined(OS_*)

namespace base {

bool isValidIpV4Address(std::u16string_view address)
{
    //struct sockaddr_in sa;
    //inet_addr();
    //return inet_pton(AF_INET, base::local8BitFromUtf16(address).c_str(), &(sa.sin_addr)) != 0;
    /*in_addr ipV4Addr;
    return RtlIpv4StringToAddress(base::local8BitFromUtf16(address).c_str(), FALSE, NULL, &ipV4Addr) != 0;*/
   
	std::string IP = base::local8BitFromUtf16(address);
	int begin = 0, end = 0, cnt = 0;
	while (end < IP.size()) {
		int leadingZeroCount = 0, num = 0;
		while (end < IP.size() && isdigit(IP[end])) {
			num = num * 10 + IP[end] - '0';
			if (IP[end] == '0' && num == 0) leadingZeroCount++;
			if ((leadingZeroCount > 0 && num != 0)
				|| leadingZeroCount > 1
				|| end - begin + 1 > 3
				|| num > 255) return false;
			++end;
		}
		if (end == begin) return false;
		++cnt;
		if (cnt <= 3) {
			if (end >= IP.size() || IP[end] != '.') return false;
			begin = ++end;
		}
		else {
			if (end != IP.size()) return false;
		}
	}
	return cnt == 4;
}

bool isValidIpV6Address(std::u16string_view address)
{
    //struct sockaddr_in6 sa;
   // return inet_pton(AF_INET6, base::local8BitFromUtf16(address).c_str(), &(sa.sin6_addr)) != 0;
    //in6_addr ipV6Addr;
    ////return RtlIpv6StringToAddressA(base::local8BitFromUtf16(address).c_str(),"\0",&ipV6Addr) != STATUS_SUCCESS;
    //return RtlIpv6StringToAddress(base::local8BitFromUtf16(address).c_str(), NULL,&ipV6Addr) != 0;
	std::string IP = base::local8BitFromUtf16(address);
	int cnt = 0, begin = 0, end = 0;
	while (end < IP.size()) {
		while (end < IP.size() && isalnum(IP[end])) {
			if ((IP[end] > 'f' && IP[end] <= 'z')
				|| (IP[end] > 'F' && IP[end] <= 'Z')
				|| end - begin + 1 > 4) return false;
			++end;
		}
		if (begin == end) return false;
		++cnt;
		if (cnt <= 7) {
			if (end >= IP.size() || IP[end] != ':') return false;
			begin = ++end;
		}
		else {
			if (end != IP.size()) return false;
		}
	}
	return cnt == 8;
}

} // namespace base
