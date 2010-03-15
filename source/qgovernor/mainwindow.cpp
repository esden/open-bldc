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

#include <lg/types.h>
#include <lg/gpdef.h>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    registerModel.setHorizontalHeaderLabels(
            QStringList() << QApplication::translate("qgovernor", "Dec")
                          << QApplication::translate("qgovernor", "Hex")
                          << QApplication::translate("qgovernor", "Bin")
                          << QApplication::translate("qgovernor", "Mon")
                          << QApplication::translate("qgovernor", "Dis"));

    for(int i=0; i<32; i++){
        registerModel.setVerticalHeaderItem(i, new QStandardItem(QString::number(i, 10).rightJustified(3, '0', false)));
        registerModel.setItem(i, 0, new QStandardItem(QString::number(0, 10).rightJustified(5, '0', false)));
        registerModel.setItem(i, 1, new QStandardItem(QString::number(0, 16).rightJustified(4, '0', false)));
        registerModel.setItem(i, 2, new QStandardItem(QString::number(0 >> 8, 2).rightJustified(8, '0', false)
                                                      .append(" ")
                                                      .append(QString::number(0 & 0xFF, 2).rightJustified(8, '0', false))));
        registerModel.setItem(i, 3, new QStandardItem());
        registerModel.item(i, 3)->setCheckable(true);
        registerModel.setItem(i, 4, new QStandardItem());
        registerModel.item(i, 4)->setCheckable(true);
    }

    ui->registerTableView->setModel(&registerModel);
    ui->registerTableView->resizeColumnsToContents();
    ui->registerTableView->resizeRowsToContents();

    outputModel.setHorizontalHeaderLabels(
            QStringList() << QApplication::translate("qgovernor", "Mon")
                          << QApplication::translate("qgovernor", "R/W")
                          << QApplication::translate("qgovernor", "Addr")
                          << QApplication::translate("qgovernor", "Val Hex")
                          << QApplication::translate("qgovernor", "Val Dec")
                          << QApplication::translate("qgovernor", "Val Bin"));

    ui->outputTableView->setModel(&outputModel);
    ui->outputTableView->resizeColumnsToContents();
    ui->outputTableView->resizeRowsToContents();

    inputModel.setHorizontalHeaderLabels(
            QStringList() << QApplication::translate("qgovernor", "Mon")
                          << QApplication::translate("qgovernor", "R/W")
                          << QApplication::translate("qgovernor", "Addr")
                          << QApplication::translate("qgovernor", "Val Hex")
                          << QApplication::translate("qgovernor", "Val Dec")
                          << QApplication::translate("qgovernor", "Val Bin"));

    ui->inputTableView->setModel(&inputModel);
    ui->inputTableView->resizeColumnsToContents();
    ui->inputTableView->resizeRowsToContents();

    /* Dialog initialization */
    connectDialog = new ConnectDialog(this);

    /* Governor */
    governorMaster = new GovernorMaster();
    connect(governorMaster, SIGNAL(outputTriggered()), this, SLOT(outputTriggeredSignal()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWindow::on_connectPushButton_clicked()
{
    ui->connectPushButton->setDisabled(true);
    if(connectDialog->exec()){
        ui->statusBar->showMessage(tr("Connection accepted ... connecting to interface %1 ...").arg(connectDialog->getInterfaceId()));
        governorMaster->sendSet(10, 20);
        ui->connectPushButton->setDisabled(false);
    }else{
        ui->statusBar->showMessage(tr("Connection rejected..."));
        ui->connectPushButton->setDisabled(false);
    }
}

void MainWindow::addInput(bool monitor, QChar r_w, unsigned char addr, unsigned short value)
{
    inputModel.appendRow(
            QList<QStandardItem *>() << new QStandardItem()
                                     << new QStandardItem(r_w)
                                     << new QStandardItem(QString::number(addr, 10).rightJustified(2, '0', false))
                                     << new QStandardItem(QString::number(value, 16).rightJustified(4, '0', false))
                                     << new QStandardItem(QString::number(value, 10).rightJustified(4, '0', false))
                                     << new QStandardItem(QString::number(value >> 8, 2).rightJustified(8, '0', false)
                                                          .append(" ")
                                                          .append(QString::number(value & 0xFF, 2).rightJustified(8, '0', false))));
    ui->inputTableView->resizeColumnsToContents();
    ui->inputTableView->scrollToBottom();
    ui->inputTableView->resizeRowsToContents();
}

void MainWindow::addOutput(bool monitor, QChar r_w, unsigned char addr, unsigned short value)
{
    outputModel.appendRow(
            QList<QStandardItem *>() << new QStandardItem()
                                     << new QStandardItem(r_w)
                                     << new QStandardItem(QString::number(addr, 10).rightJustified(3, '0', false))
                                     << new QStandardItem(QString::number(value, 16).rightJustified(4, '0', false))
                                     << new QStandardItem(QString::number(value, 10).rightJustified(4, '0', false))
                                     << new QStandardItem(QString::number(value >> 8, 2).rightJustified(8, '0', false)
                                                          .append(" ")
                                                          .append(QString::number(value & 0xFF, 2).rightJustified(8, '0', false))));
    ui->outputTableView->resizeColumnsToContents();
    ui->outputTableView->resizeRowsToContents();
    ui->outputTableView->scrollToBottom();
}

void MainWindow::outputTriggeredSignal()
{
    signed short data;
    static int state = 0;
    bool monitor;
    unsigned char addr;
    unsigned short value;

    while((data = governorMaster->pickupByte()) != -1){
        printf("data value: %d\n", data);
        switch(state){
        case 0:
            if((data & GP_MODE_MASK) == GP_MODE_WRITE){
                addr = data & GP_ADDR_MASK;
                state = 1;
            }
            break;
        case 1:
            value = data;
            state = 2;
            break;
        case 2:
            value |= data << 8;
            addOutput(false, 'W', addr, value);
            state = 0;
            break;
        }
    }
}
