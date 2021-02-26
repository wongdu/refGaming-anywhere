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

#ifndef CLIENT__STATUS_WINDOW_H
#define CLIENT__STATUS_WINDOW_H

#include "base/peer/client_authenticator.h"
#include "client/router_controller.h"

namespace client {

class StatusWindow
{
public:
    virtual ~StatusWindow() = default;

    virtual void onStarted(const std::u16string& address_or_id) = 0;
    virtual void onStopped() = 0;
    virtual void onConnected() = 0;
    virtual void onDisconnected(base::NetworkChannel::ErrorCode error_code) = 0;
    virtual void onAccessDenied(base::ClientAuthenticator::ErrorCode error_code) = 0;
    virtual void onRouterError(const RouterController::Error& error) = 0;
};

} // namespace client

#endif // CLIENT__STATUS_WINDOW_H
