//
// Aspia Project
// Copyright (C) 2019 Dmitry Chapyshev <dmitry@aspia.ru>
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

#include "host/client_session.h"

#include "base/guid.h"
#include "base/logging.h"
#include "host/client_session_desktop.h"
#include "host/client_session_file_transfer.h"
#include "net/network_channel_proxy.h"

namespace host {

ClientSession::ClientSession(
    proto::SessionType session_type, std::unique_ptr<net::Channel> channel)
    : session_type_(session_type),
      channel_(std::move(channel))
{
    DCHECK(channel_);

    channel_proxy_ = channel_->channelProxy();
    id_ = base::Guid::create().toStdString();
}

// static
std::unique_ptr<ClientSession> ClientSession::create(
    proto::SessionType session_type, std::unique_ptr<net::Channel> channel)
{
    if (!channel)
        return nullptr;

    switch (session_type)
    {
        case proto::SESSION_TYPE_DESKTOP_MANAGE:
        case proto::SESSION_TYPE_DESKTOP_VIEW:
            return std::unique_ptr<ClientSessionDesktop>(
                new ClientSessionDesktop(session_type, std::move(channel)));

        case proto::SESSION_TYPE_FILE_TRANSFER:
            return std::unique_ptr<ClientSessionFileTransfer>(
                new ClientSessionFileTransfer(std::move(channel)));

        default:
            return nullptr;
    }
}

void ClientSession::start()
{

}

void ClientSession::setVersion(const base::Version& version)
{
    version_ = version;
}

void ClientSession::setUserName(std::u16string_view username)
{
    username_ = username;
}

std::u16string ClientSession::peerAddress() const
{
    return channel_proxy_->peerAddress();
}

void ClientSession::sendMessage(base::ByteArray&& buffer)
{
    channel_proxy_->send(std::move(buffer));
}

void ClientSession::onNetworkConnected()
{
    NOTREACHED();
}

void ClientSession::onNetworkDisconnected()
{

}

void ClientSession::onNetworkError(net::ErrorCode error_code)
{

}

} // namespace host
