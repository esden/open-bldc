/*
 * qgovernor - QT based Open-BLDC PC interface tool
 * Copyright (C) 2010 by Piotr Esden-Tempski <piotr@esden.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <QTcpSocket>

#include "governormaster.h"

#include "registermodel.h"
#include "protocolmodel.h"
#include "connectdialog.h"
#include "simulator.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::MainWindow *ui;

    ConnectDialog *connectDialog;
    Simulator *simulator;
    RegisterModel registerModel;
    ProtocolModel outputModel;
    ProtocolModel inputModel;
    QTcpSocket *tcpSocket;

    GovernorMaster *governorMaster;
    bool connected;

    QAction *updateRegister;
    QAction *updateAllRegisters;

private slots:
    void on_action_Simulator_triggered();
    void on_actionAbout_triggered();
    void on_actionAbout_Qt_triggered();
    void on_registerTableView_customContextMenuRequested(QPoint pos);
    void on_connectPushButton_clicked();
    void on_disconnectPushButton_clicked();
    void on_outputTriggered();
    void on_registerChanged(unsigned char addr);
    void on_guiRegisterChanged(QStandardItem *item);
    void on_simulatorInput(unsigned char data);
};

#endif // MAINWINDOW_H
