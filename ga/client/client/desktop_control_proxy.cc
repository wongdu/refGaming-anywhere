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

#include "client/desktop_control_proxy.h"

#include "base/logging.h"
#include "base/task_runner.h"
#include "client/desktop_control.h"

namespace client {

DesktopControlProxy::DesktopControlProxy(std::shared_ptr<base::TaskRunner> io_task_runner,
                                         DesktopControl* desktop_control)
    : io_task_runner_(std::move(io_task_runner)),
      desktop_control_(desktop_control)
{
    DCHECK(io_task_runner_);
    DCHECK(desktop_control_);
}

DesktopControlProxy::~DesktopControlProxy()
{
    DCHECK(!desktop_control_);
}

void DesktopControlProxy::dettach()
{
    DCHECK(io_task_runner_->belongsToCurrentThread());
    desktop_control_ = nullptr;
}

void DesktopControlProxy::setDesktopConfig(const proto::DesktopConfig& desktop_config)
{
    if (!io_task_runner_->belongsToCurrentThread())
    {
        io_task_runner_->postTask(
            std::bind(&DesktopControlProxy::setDesktopConfig, shared_from_this(), desktop_config));
        return;
    }

    if (desktop_control_)
        desktop_control_->setDesktopConfig(desktop_config);
}

void DesktopControlProxy::setCurrentScreen(const proto::Screen& screen)
{
    if (!io_task_runner_->belongsToCurrentThread())
    {
        io_task_runner_->postTask(
            std::bind(&DesktopControlProxy::setCurrentScreen, shared_from_this(), screen));
        return;
    }

    if (desktop_control_)
        desktop_control_->setCurrentScreen(screen);
}

void DesktopControlProxy::setPreferredSize(int width, int height)
{
    if (!io_task_runner_->belongsToCurrentThread())
    {
        io_task_runner_->postTask(
            std::bind(&DesktopControlProxy::setPreferredSize, shared_from_this(), width, height));
        return;
    }

    if (desktop_control_)
        desktop_control_->setPreferredSize(width, height);
}

void DesktopControlProxy::onKeyEvent(const proto::KeyEvent& event)
{
    if (!io_task_runner_->belongsToCurrentThread())
    {
        io_task_runner_->postTask(
            std::bind(&DesktopControlProxy::onKeyEvent, shared_from_this(), event));
        return;
    }

    if (desktop_control_)
        desktop_control_->onKeyEvent(event);
}

void DesktopControlProxy::onMouseEvent(const proto::MouseEvent& event)
{
    if (!io_task_runner_->belongsToCurrentThread())
    {
        io_task_runner_->postTask(
            std::bind(&DesktopControlProxy::onMouseEvent, shared_from_this(), event));
        return;
    }

    if (desktop_control_)
        desktop_control_->onMouseEvent(event);
}

void DesktopControlProxy::onPowerControl(proto::PowerControl::Action action)
{
    if (!io_task_runner_->belongsToCurrentThread())
    {
        io_task_runner_->postTask(
            std::bind(&DesktopControlProxy::onPowerControl, shared_from_this(), action));
        return;
    }

    if (desktop_control_)
        desktop_control_->onPowerControl(action);
}

void DesktopControlProxy::onRemoteUpdate()
{
    if (!io_task_runner_->belongsToCurrentThread())
    {
        io_task_runner_->postTask(
            std::bind(&DesktopControlProxy::onRemoteUpdate, shared_from_this()));
        return;
    }

    if (desktop_control_)
        desktop_control_->onRemoteUpdate();
}

void DesktopControlProxy::onSystemInfoRequest()
{
    if (!io_task_runner_->belongsToCurrentThread())
    {
        io_task_runner_->postTask(
            std::bind(&DesktopControlProxy::onSystemInfoRequest, shared_from_this()));
        return;
    }

    if (desktop_control_)
        desktop_control_->onSystemInfoRequest();
}

void DesktopControlProxy::onMetricsRequest()
{
    if (!io_task_runner_->belongsToCurrentThread())
    {
        io_task_runner_->postTask(
            std::bind(&DesktopControlProxy::onMetricsRequest, shared_from_this()));
        return;
    }

    if (desktop_control_)
        desktop_control_->onMetricsRequest();
}

} // namespace client
