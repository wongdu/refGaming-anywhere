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

#ifndef CLIENT__UI__CLIENT_WINDOW_H
#define CLIENT__UI__CLIENT_WINDOW_H

#include "base/macros_magic.h"
#include "client/client_config.h"
#include "proto/desktop.pb.h"
#include "ui_client_window.h"

#include <QMainWindow>

namespace client {

class ClientWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ClientWindow(QWidget* parent = nullptr);
    ~ClientWindow();

protected:
    // QMainWindow implementation.
    void closeEvent(QCloseEvent* event) override;

private slots:
    void onLanguageChanged(QAction* action);
    void onSettings();
    void onHelp();
    void onAbout();
    void sessionTypeChanged(int item_index);
    void sessionConfigButtonPressed();
    void connectToHost();

private:
    void createLanguageMenu(const QString& current_locale);
    void reloadSessionTypes();

    Ui::ClientWindow ui;

    DISALLOW_COPY_AND_ASSIGN(ClientWindow);
};

} // namespace client

#endif // CLIENT__UI__CLIENT_WINDOW_H
