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

//#include  <limits>
//#include <netioapi.h>
#include "base/net/adapter_enumerator.h"

#include "base/logging.h"
#include "base/strings/string_printf.h"
#include "base/strings/unicode.h"


#if defined(OS_WIN)
#include <ws2tcpip.h>
#include <iphlpapi.h>
//#include <Ws2def.h>
#endif // defined(OS_WIN)

namespace base {

namespace {

#ifdef SPRINTF_CHAR
# define SPRINTF(x) strlen(sprintf/**/x)
#else
# define SPRINTF(x) ((size_t)sprintf x)
#endif

	static const char* inet_ntop4_(const u_char* src, char* dst, socklen_t size);
	static const char* inet_ntop6_(const u_char* src, char* dst, socklen_t size);

	const char*
		inet_ntop_(int af, const void* src, char* dst, socklen_t size)
	{
		switch (af) {
		case AF_INET:
			return (inet_ntop4_((const u_char*)src, dst, size));
		case AF_INET6:
			return (inet_ntop6_((const u_char*)src, dst, size));
		default:
			return (NULL);
		}
		/* NOTREACHED */
	}

	static const char* inet_ntop4_(const u_char* src, char* dst, socklen_t size)
	{
		static const char fmt[] = "%u.%u.%u.%u";
		char tmp[sizeof "255.255.255.255"];
		if (SPRINTF((tmp, fmt, src[0], src[1], src[2], src[3])) >= size) {
			return (NULL);
		}
		return strcpy(dst, tmp);
	}

    static const char*
        inet_ntop6_(const u_char* src, char* dst, socklen_t size)
    {
        /*
         * Note that int32_t and int16_t need only be "at least" large enough
         * to contain a value of the specified size.  On some systems, like
         * Crays, there is no such thing as an integer variable with 16 bits.
         * Keep this in mind if you think this function should have been coded
         * to use pointer overlays.  All the world's not a VAX.
         */
        char tmp[sizeof "ffff:ffff:ffff:ffff:ffff:ffff:255.255.255.255"], * tp;
        struct { int base, len; } best, cur;
        constexpr  int  NS_IN6ADDRSZ = 16;
        constexpr   int NS_INT16SZ = 2;
        u_int words[NS_IN6ADDRSZ / NS_INT16SZ];
        int i;
    
        memset(words, '\0', sizeof words);
        for (i = 0; i < NS_IN6ADDRSZ; i += 2)
            words[i / 2] = (src[i] << 8) | src[i + 1];
        best.base = -1;
        cur.base = -1;
        best.len = 0;
        cur.len = 0;
        for (i = 0; i < (NS_IN6ADDRSZ / NS_INT16SZ); i++) {
            if (words[i] == 0) {
                if (cur.base == -1)
                    cur.base = i, cur.len = 1;
                else
                    cur.len++;
            }
            else {
                if (cur.base != -1) {
                    if (best.base == -1 || cur.len > best.len)
                        best = cur;
                    cur.base = -1;
                }
            }
        }
        if (cur.base != -1) {
            if (best.base == -1 || cur.len > best.len)
                best = cur;
        }
        if (best.base != -1 && best.len < 2)
            best.base = -1;
        /*
         * Format the result.
         */
        tp = tmp;
        for (i = 0; i < (NS_IN6ADDRSZ / NS_INT16SZ); i++) {
            /* Are we inside the best run of 0x00's? */
            if (best.base != -1 && i >= best.base &&
                i < (best.base + best.len)) {
                if (i == best.base)
                    *tp++ = ':';
                continue;
            }
            /* Are we following an initial run of 0x00s or any real hex? */
            if (i != 0)
                *tp++ = ':';
            /* Is this address an encapsulated IPv4? */
            if (i == 6 && best.base == 0 &&
                (best.len == 6 || (best.len == 5 && words[5] == 0xffff))) {
                if (!inet_ntop4_(src + 12, tp, sizeof tmp - (tp - tmp)))
                    return (NULL);
                tp += strlen(tp);
                break;
            }
            tp += SPRINTF((tp, "%x", words[i]));
        }
        /* Was it a trailing run of 0x00's? */
        if (best.base != -1 && (best.base + best.len) ==
            (NS_IN6ADDRSZ / NS_INT16SZ))
            *tp++ = ':';
        *tp++ = '\0';
        /*
         * Check for overflow, copy, and we're done.
         */
        if ((socklen_t)(tp - tmp) > size) {
            return (NULL);
        }
        return strcpy(dst, tmp);
    }

#if defined(OS_WIN)
std::string addressToString(const SOCKET_ADDRESS& address)
{
    if (!address.lpSockaddr || address.iSockaddrLength <= 0)
        return std::string();

    char buffer[64] = { 0 };

    switch (address.lpSockaddr->sa_family)
    {
        case AF_INET:
        {
            sockaddr_in* addr = reinterpret_cast<sockaddr_in*>(address.lpSockaddr);

            if (!inet_ntop_(AF_INET, &addr->sin_addr, buffer, std::size(buffer)))
                return std::string();
        }
        break;

        case AF_INET6:
        {
            sockaddr_in6* addr = reinterpret_cast<sockaddr_in6*>(address.lpSockaddr);

            if (!inet_ntop_(AF_INET6, &addr->sin6_addr, buffer, std::size(buffer)))
                return std::string();
        }
        break;

        default:
            return std::string();
    }

    return buffer;
}
#endif // defined(OS_WIN)

} // namespace

//
// AdapterEnumerator
//

AdapterEnumerator::AdapterEnumerator()
{
#if defined(OS_WIN)
    const ULONG flags = GAA_FLAG_INCLUDE_GATEWAYS | GAA_FLAG_SKIP_ANYCAST |
        GAA_FLAG_SKIP_MULTICAST;

    ULONG buffer_size = sizeof(IP_ADAPTER_ADDRESSES);

    adapters_buffer_ = std::make_unique<uint8_t[]>(buffer_size);
    adapter_ = reinterpret_cast<PIP_ADAPTER_ADDRESSES>(adapters_buffer_.get());

    ULONG error_code = GetAdaptersAddresses(AF_INET, flags, nullptr, adapter_, &buffer_size);
    if (error_code != ERROR_SUCCESS)
    {
        if (error_code == ERROR_BUFFER_OVERFLOW)
        {
            adapters_buffer_ = std::make_unique<uint8_t[]>(buffer_size);
            adapter_ = reinterpret_cast<PIP_ADAPTER_ADDRESSES>(adapters_buffer_.get());

            error_code = GetAdaptersAddresses(AF_INET, flags, nullptr, adapter_, &buffer_size);
            if (error_code != ERROR_SUCCESS)
            {
                adapters_buffer_.reset();
                adapter_ = nullptr;
                return;
            }
        }
        else
        {
            adapters_buffer_.reset();
            adapter_ = nullptr;
            return;
        }
    }
#else
    NOTIMPLEMENTED();
#endif
}

AdapterEnumerator::~AdapterEnumerator() = default;

bool AdapterEnumerator::isAtEnd() const
{
#if defined(OS_WIN)
    return adapter_ == nullptr;
#else
    NOTIMPLEMENTED();
    return true;
#endif
}

void AdapterEnumerator::advance()
{
#if defined(OS_WIN)
    adapter_ = adapter_->Next;
#else
    NOTIMPLEMENTED();
#endif
}

std::string AdapterEnumerator::adapterName() const
{
#if defined(OS_WIN)
    if (!adapter_->Description)
        return std::string();

    return base::utf8FromWide(adapter_->Description);
#else
    NOTIMPLEMENTED();
    return std::string();
#endif
}

std::string AdapterEnumerator::connectionName() const
{
#if defined(OS_WIN)
    if (!adapter_->FriendlyName)
        return std::string();

    return base::utf8FromWide(adapter_->FriendlyName);
#else
    NOTIMPLEMENTED();
    return std::string();
#endif
}

std::string AdapterEnumerator::interfaceType() const
{
#if defined(OS_WIN)
    switch (adapter_->IfType)
    {
        case IF_TYPE_OTHER:
            return "Other";

        case IF_TYPE_ETHERNET_CSMACD:
            return "Ethernet";

        case IF_TYPE_ISO88025_TOKENRING:
            return "Token Ring";

        case IF_TYPE_PPP:
            return "PPP";

        case IF_TYPE_SOFTWARE_LOOPBACK:
            return "Software Lookback";

        case IF_TYPE_ATM:
            return "ATM";

        case IF_TYPE_IEEE80211:
            return "IEEE 802.11 Wireless";

        case IF_TYPE_TUNNEL:
            return "Tunnel type encapsulation";

        case IF_TYPE_IEEE1394:
            return "IEEE 1394 Firewire";

        default:
            return std::string();
    }
#else
    NOTIMPLEMENTED();
    return std::string();
#endif
}

uint32_t AdapterEnumerator::mtu() const
{
#if defined(OS_WIN)
    return adapter_->Mtu;
#else
    NOTIMPLEMENTED();
    return 0;
#endif
}

uint64_t AdapterEnumerator::speed() const
{
#if defined(OS_WIN)
#ifdef max
#undef max
#endif
    if (adapter_->TransmitLinkSpeed == std::numeric_limits<ULONG64>::max())
        return 0;

    return adapter_->TransmitLinkSpeed;
#else
    NOTIMPLEMENTED();
    return 0;
#endif
}

std::string AdapterEnumerator::macAddress() const
{
#if defined(OS_WIN)
    if (!adapter_->PhysicalAddressLength)
        return std::string();

    return base::stringPrintf("%.2X-%.2X-%.2X-%.2X-%.2X-%.2X",
                              adapter_->PhysicalAddress[0],
                              adapter_->PhysicalAddress[1],
                              adapter_->PhysicalAddress[2],
                              adapter_->PhysicalAddress[3],
                              adapter_->PhysicalAddress[4],
                              adapter_->PhysicalAddress[5],
                              adapter_->PhysicalAddress[6]);
#else
    NOTIMPLEMENTED();
    return std::string();
#endif
}

bool AdapterEnumerator::isDhcp4Enabled() const
{
#if defined(OS_WIN)
    return !!adapter_->Dhcpv4Enabled;
#else
    NOTIMPLEMENTED();
    return false;
#endif
}

std::string AdapterEnumerator::dhcp4Server() const
{
#if defined(OS_WIN)
    return addressToString(adapter_->Dhcpv4Server);
#else
    NOTIMPLEMENTED();
    return std::string();
#endif
}

//
// IpAddressEnumerator
//

AdapterEnumerator::IpAddressEnumerator::IpAddressEnumerator(const AdapterEnumerator& adapter)
#if defined(OS_WIN)
    : address_(adapter.adapter_->FirstUnicastAddress)
#endif // defined(OS_WIN)
{
    // Nothing
}

bool AdapterEnumerator::IpAddressEnumerator::isAtEnd() const
{
#if defined(OS_WIN)
    return address_ == nullptr;
#else
    NOTIMPLEMENTED();
    return true;
#endif
}

void AdapterEnumerator::IpAddressEnumerator::advance()
{
#if defined(OS_WIN)
    address_ = address_->Next;
#else
    NOTIMPLEMENTED();
#endif
}

std::string AdapterEnumerator::IpAddressEnumerator::address() const
{
#if defined(OS_WIN)
    return addressToString(address_->Address);
#else
    NOTIMPLEMENTED();
    return std::string();
#endif
}

std::string AdapterEnumerator::IpAddressEnumerator::mask() const
{
#if defined(OS_WIN)
    in_addr addr;

    if (ConvertLengthToIpv4Mask(address_->OnLinkPrefixLength, &addr.s_addr) != NO_ERROR)
        return std::string();

    char buffer[64] = { 0 };

    if (!inet_ntop_(AF_INET, &addr, buffer, std::size(buffer)))
        return std::string();

    return buffer;
#else
    NOTIMPLEMENTED();
    return std::string();
#endif
}

//
// GatewayEnumerator
//

AdapterEnumerator::GatewayEnumerator::GatewayEnumerator(const AdapterEnumerator& adapter)
#if defined(OS_WIN)
    : address_(adapter.adapter_->FirstGatewayAddress)
#endif
{
    // Nothing
}

bool AdapterEnumerator::GatewayEnumerator::isAtEnd() const
{
#if defined(OS_WIN)
    return address_ == nullptr;
#else
    NOTIMPLEMENTED();
    return true;
#endif
}

void AdapterEnumerator::GatewayEnumerator::advance()
{
#if defined(OS_WIN)
    address_ = address_->Next;
#else
    NOTIMPLEMENTED();
#endif
}

std::string AdapterEnumerator::GatewayEnumerator::address() const
{
#if defined(OS_WIN)
    return addressToString(address_->Address);
#else
    NOTIMPLEMENTED();
    return std::string();
#endif
}

//
// DnsEnumerator
//

AdapterEnumerator::DnsEnumerator::DnsEnumerator(const AdapterEnumerator& adapter)
#if defined(OS_WIN)
    : address_(adapter.adapter_->FirstDnsServerAddress)
#endif
{
    // Nothing
}

bool AdapterEnumerator::DnsEnumerator::isAtEnd() const
{
#if defined(OS_WIN)
    return address_ == nullptr;
#else
    NOTIMPLEMENTED();
    return true;
#endif
}

void AdapterEnumerator::DnsEnumerator::advance()
{
#if defined(OS_WIN)
    address_ = address_->Next;
#else
    NOTIMPLEMENTED();
#endif
}

std::string AdapterEnumerator::DnsEnumerator::address() const
{
#if defined(OS_WIN)
    return addressToString(address_->Address);
#else
    NOTIMPLEMENTED();
    return std::string();
#endif
}

} // namespace base
