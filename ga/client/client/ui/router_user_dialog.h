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

#ifndef CLIENT__UI__ROUTER_USER_DIALOG_H
#define CLIENT__UI__ROUTER_USER_DIALOG_H

#include "base/macros_magic.h"
#include "base/peer/user.h"
#include "proto/router_common.pb.h"
#include "ui_router_user_dialog.h"

namespace client {

class RouterUserDialog : public QDialog
{
    Q_OBJECT

public:
    RouterUserDialog(const base::User& user,
                     const std::vector<std::u16string>& users,
                     QWidget* parent);
    ~RouterUserDialog();

    const base::User& user() const;

protected:
    // QDialog implementation.
    bool eventFilter(QObject* object, QEvent* event) override;

private:
    void onButtonBoxClicked(QAbstractButton* button);
    void setAccountChanged(bool changed);
    static QString sessionTypeToString(proto::RouterSession session_type);

    Ui::RouterUserDialog ui;

    base::User user_;
    std::vector<std::u16string> users_;
    bool account_changed_ = true;

    DISALLOW_COPY_AND_ASSIGN(RouterUserDialog);
};

} // namespace client

#endif // CLIENT__ROUTER_USER_DIALOG_H
