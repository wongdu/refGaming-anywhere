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

#ifndef BASE__PEER__AUTHENTICATOR_H
#define BASE__PEER__AUTHENTICATOR_H

#include "base/waitable_timer.h"
#include "base/version.h"
#include "base/net/network_channel.h"
#include "proto/key_exchange.pb.h"

namespace base {

class Location;

class Authenticator : public NetworkChannel::Listener
{
public:
    explicit Authenticator(std::shared_ptr<TaskRunner> task_runner);
    virtual ~Authenticator() = default;

    enum class State
    {
        STOPPED, // The authenticator has not been started yet.
        PENDING, // The authenticator is waiting for completion.
        FAILED,  // The authenticator failed.
        SUCCESS  // The authenticator completed successfully.
    };

    enum class ErrorCode
    {
        SUCCESS,
        UNKNOWN_ERROR,
        NETWORK_ERROR,
        PROTOCOL_ERROR,
        ACCESS_DENIED,
        SESSION_DENIED
    };

    using Callback = std::function<void(ErrorCode error_code)>;

    void start(std::unique_ptr<NetworkChannel> channel, Callback callback);

    [[nodiscard]] proto::Identify identify() const { return identify_; }
    [[nodiscard]] proto::Encryption encryption() const { return encryption_; }
    [[nodiscard]] const Version& peerVersion() const { return peer_version_; }
    [[nodiscard]] const std::string& peerOsName() const { return peer_os_name_; }
    [[nodiscard]] const std::string& peerComputerName() const { return peer_computer_name_; }
    [[nodiscard]] uint32_t sessionType() const { return session_type_; }
    [[nodiscard]] const std::string& userName() const { return user_name_; }

    // Returns the current state.
    [[nodiscard]] State state() const { return state_; }

    // Releases network channel.
    [[nodiscard]] std::unique_ptr<NetworkChannel> takeChannel();

    static const char* stateToString(State state);
    static const char* errorToString(Authenticator::ErrorCode error_code);

protected:
    [[nodiscard]] virtual bool onStarted() = 0;
    virtual void onReceived(const ByteArray& buffer) = 0;
    virtual void onWritten() = 0;

    void sendMessage(const google::protobuf::MessageLite& message);
    void finish(const Location& location, ErrorCode error_code);
    void setPeerVersion(const proto::Version& version);
    void setPeerOsName(const std::string& name);
    void setPeerComputerName(const std::string& name);

    // base::NetworkChannel::Listener implementation.
    void onConnected() final;
    void onDisconnected(NetworkChannel::ErrorCode error_code) final;
    void onMessageReceived(const ByteArray& buffer) final;
    void onMessageWritten(size_t pending) final;

    [[nodiscard]] bool onSessionKeyChanged();

    proto::Encryption encryption_ = proto::ENCRYPTION_UNKNOWN;
    proto::Identify identify_ = proto::IDENTIFY_SRP;
    ByteArray session_key_;
    ByteArray encrypt_iv_;
    ByteArray decrypt_iv_;

    uint32_t session_type_ = 0; // Selected session type.
    std::string user_name_;

private:
    WaitableTimer timer_;
    std::unique_ptr<NetworkChannel> channel_;
    Callback callback_;
    State state_ = State::STOPPED;
    Version peer_version_; // Remote peer version.
    std::string peer_os_name_;
    std::string peer_computer_name_;
};

} // namespace base

#endif // BASE__PEER__AUTHENTICATOR_H
