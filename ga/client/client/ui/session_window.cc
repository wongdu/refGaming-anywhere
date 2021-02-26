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

#include "client/ui/session_window.h"

#include "base/logging.h"
#include "client/client.h"
#include "client/client_proxy.h"
#include "client/status_window_proxy.h"
#include "client/ui/authorization_dialog.h"
#include "common/ui/status_dialog.h"
#include "qt_base/application.h"

namespace client {

SessionWindow::SessionWindow(QWidget* parent)
    : QWidget(parent),
      status_window_proxy_(
          std::make_shared<StatusWindowProxy>(qt_base::Application::uiTaskRunner(), this))
{
    // Nothing
}

SessionWindow::~SessionWindow()
{
    status_window_proxy_->dettach();
}

bool SessionWindow::connectToHost(Config config)
{
    if (client_proxy_)
    {
        DLOG(LS_ERROR) << "Attempt to start an already running client";
        return false;
    }

    // Set the window title.
    setClientTitle(config);

    if (config.username.empty() || config.password.empty())
    {
        AuthorizationDialog auth_dialog(this);

        auth_dialog.setOneTimePasswordEnabled(config.router_config.has_value());
        auth_dialog.setUserName(QString::fromStdU16String(config.username));
        auth_dialog.setPassword(QString::fromStdU16String(config.password));

        if (auth_dialog.exec() == AuthorizationDialog::Rejected)
            return false;

        config.username = auth_dialog.userName().toStdU16String();
        config.password = auth_dialog.password().toStdU16String();
    }

    // When connecting with a one-time password, the username must be in the following format:
    // #host_id.
    if (config.username.empty())
        config.username = u"#" + config.address_or_id;

    // Create a client instance.
    std::unique_ptr<Client> client = createClient();

    // Set the window that will receive notifications.
    client->setStatusWindow(status_window_proxy_);

    client_proxy_ = std::make_unique<ClientProxy>(
        qt_base::Application::ioTaskRunner(), std::move(client), config);

    client_proxy_->start();
    return true;
}

Config SessionWindow::config() const
{
    return client_proxy_->config();
}

void SessionWindow::closeEvent(QCloseEvent* /* event */)
{
    if (client_proxy_)
    {
        client_proxy_->stop();
        client_proxy_.reset();
    }
}

void SessionWindow::onStarted(const std::u16string& address_or_id)
{
    // Create a dialog to display the connection status.
    status_dialog_ = new common::StatusDialog(this);

    // After closing the status dialog, close the session window.
    connect(status_dialog_, &common::StatusDialog::finished, this, &SessionWindow::close);

    status_dialog_->setWindowFlag(Qt::WindowStaysOnTopHint);
    status_dialog_->addMessageAndActivate(tr("Attempt to connect to %1.").arg(address_or_id));
}

void SessionWindow::onStopped()
{
    status_dialog_->close();
}

void SessionWindow::onConnected()
{
    status_dialog_->addMessageAndActivate(tr("Connection established."));
    status_dialog_->hide();
}

void SessionWindow::onDisconnected(base::NetworkChannel::ErrorCode error_code)
{
    onErrorOccurred(netErrorToString(error_code));
}

void SessionWindow::onAccessDenied(base::ClientAuthenticator::ErrorCode error_code)
{
    onErrorOccurred(authErrorToString(error_code));
}

void SessionWindow::onRouterError(const RouterController::Error& error)
{
    switch (error.type)
    {
        case RouterController::ErrorType::NETWORK:
        {
            onErrorOccurred(tr("Network error when connecting to the router: %1")
                            .arg(netErrorToString(error.code.network)));
        }
        break;

        case RouterController::ErrorType::AUTHENTICATION:
        {
            onErrorOccurred(tr("Authentication error when connecting to the router: %1")
                            .arg(authErrorToString(error.code.authentication)));
        }
        break;

        case RouterController::ErrorType::ROUTER:
        {
            onErrorOccurred(routerErrorToString(error.code.router));
        }
        break;

        default:
            NOTREACHED();
            break;
    }
}

void SessionWindow::setClientTitle(const Config& config)
{
    QString session_name;

    switch (config.session_type)
    {
        case proto::SESSION_TYPE_DESKTOP_MANAGE:
            session_name = tr("Desktop Manage");
            break;

        case proto::SESSION_TYPE_DESKTOP_VIEW:
            session_name = tr("Desktop View");
            break;

        case proto::SESSION_TYPE_FILE_TRANSFER:
            session_name = tr("File Transfer");
            break;

        default:
            NOTREACHED();
            break;
    }

    QString computer_name = QString::fromStdU16String(config.computer_name);
    if (computer_name.isEmpty())
    {
        if (config.router_config.has_value())
            computer_name = QString::fromStdU16String(config.address_or_id);
        else
            computer_name = QString("%1:%2").arg(config.address_or_id).arg(config.port);
    }

    setWindowTitle(QString("%1 - %2").arg(computer_name).arg(session_name));
}

void SessionWindow::onErrorOccurred(const QString& message)
{
    hide();

    for (const auto& object : children())
    {
        QWidget* widget = dynamic_cast<QWidget*>(object);
        if (widget)
            widget->hide();
    }

    status_dialog_->addMessageAndActivate(message);
}

// static
QString SessionWindow::netErrorToString(base::NetworkChannel::ErrorCode error_code)
{
    const char* message;

    switch (error_code)
    {
        case base::NetworkChannel::ErrorCode::INVALID_PROTOCOL:
            message = QT_TR_NOOP("Violation of the communication protocol.");
            break;

        case base::NetworkChannel::ErrorCode::ACCESS_DENIED:
            message = QT_TR_NOOP("Cryptography error (message encryption or decryption failed).");
            break;

        case base::NetworkChannel::ErrorCode::NETWORK_ERROR:
            message = QT_TR_NOOP("An error occurred with the network (e.g., the network cable was accidentally plugged out).");
            break;

        case base::NetworkChannel::ErrorCode::CONNECTION_REFUSED:
            message = QT_TR_NOOP("Connection was refused by the peer (or timed out).");
            break;

        case base::NetworkChannel::ErrorCode::REMOTE_HOST_CLOSED:
            message = QT_TR_NOOP("Remote host closed the connection.");
            break;

        case base::NetworkChannel::ErrorCode::SPECIFIED_HOST_NOT_FOUND:
            message = QT_TR_NOOP("Host address was not found.");
            break;

        case base::NetworkChannel::ErrorCode::SOCKET_TIMEOUT:
            message = QT_TR_NOOP("Socket operation timed out.");
            break;

        case base::NetworkChannel::ErrorCode::ADDRESS_IN_USE:
            message = QT_TR_NOOP("Address specified is already in use and was set to be exclusive.");
            break;

        case base::NetworkChannel::ErrorCode::ADDRESS_NOT_AVAILABLE:
            message = QT_TR_NOOP("Address specified does not belong to the host.");
            break;

        default:
        {
            if (error_code != base::NetworkChannel::ErrorCode::UNKNOWN)
            {
                LOG(LS_WARNING) << "Unknown error code: " << static_cast<int>(error_code);
            }

            message = QT_TR_NOOP("An unknown error occurred.");
        }
        break;
    }

    return tr(message);
}

// static
QString SessionWindow::authErrorToString(base::ClientAuthenticator::ErrorCode error_code)
{
    const char* message;

    switch (error_code)
    {
        case base::ClientAuthenticator::ErrorCode::SUCCESS:
            message = QT_TR_NOOP("Authentication successfully completed.");
            break;

        case base::ClientAuthenticator::ErrorCode::NETWORK_ERROR:
            message = QT_TR_NOOP("Network authentication error.");
            break;

        case base::ClientAuthenticator::ErrorCode::PROTOCOL_ERROR:
            message = QT_TR_NOOP("Violation of the data exchange protocol.");
            break;

        case base::ClientAuthenticator::ErrorCode::ACCESS_DENIED:
            message = QT_TR_NOOP("Wrong user name or password.");
            break;

        case base::ClientAuthenticator::ErrorCode::SESSION_DENIED:
            message = QT_TR_NOOP("Specified session type is not allowed for the user.");
            break;

        default:
            message = QT_TR_NOOP("An unknown error occurred.");
            break;
    }

    return tr(message);
}

// static
QString SessionWindow::routerErrorToString(RouterController::ErrorCode error_code)
{
    const char* message;

    switch (error_code)
    {
        case RouterController::ErrorCode::PEER_NOT_FOUND:
            message = QT_TR_NOOP("No host with the specified ID was found.");
            break;

        case RouterController::ErrorCode::KEY_POOL_EMPTY:
            message = QT_TR_NOOP("There are no relays available or the key pool is empty.");
            break;

        case RouterController::ErrorCode::RELAY_ERROR:
            message = QT_TR_NOOP("Failed to connect to the relay server.");
            break;

        case RouterController::ErrorCode::ACCESS_DENIED:
            message = QT_TR_NOOP("Access is denied.");
            break;

        default:
            message = QT_TR_NOOP("Unknown error.");
            break;
    }

    return tr(message);
}

} // namespace client
