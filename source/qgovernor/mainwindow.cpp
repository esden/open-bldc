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

    /* Variable initialization */
    connected = false;

    /* Governor */
    governorMaster = new GovernorMaster();
    connect(governorMaster, SIGNAL(outputTriggered()), this, SLOT(on_outputTriggered()));
    connect(governorMaster, SIGNAL(registerChanged(unsigned char)), this, SLOT(on_registerChanged(unsigned char)));

    /* register display table */
    registerModel.setHorizontalHeaderLabels(
            QStringList() << QApplication::translate("qgovernor", "Dec")
                          << QApplication::translate("qgovernor", "Hex")
                          << QApplication::translate("qgovernor", "Bin")
                          << QApplication::translate("qgovernor", "Mon")
                          << QApplication::translate("qgovernor", "Dis"));


    unsigned short value;

    for(int i=0; i<32; i++){
        value = governorMaster->getRegisterMapValue(i);
        registerModel.setVerticalHeaderItem(i, new QStandardItem(QString::number(i, 10).rightJustified(3, '0', false)));
        registerModel.setItem(i, 0, new QStandardItem(QString::number(value, 10).rightJustified(5, '0', false)));
        registerModel.setItem(i, 1, new QStandardItem(QString::number(value, 16).rightJustified(4, '0', false)));
        registerModel.setItem(i, 2, new QStandardItem(QString::number(value >> 8, 2).rightJustified(8, '0', false)
                                                      .append(" ")
                                                      .append(QString::number(value & 0xFF, 2).rightJustified(8, '0', false))));
        registerModel.setItem(i, 3, new QStandardItem());
        registerModel.item(i, 3)->setCheckable(true);
        registerModel.setItem(i, 4, new QStandardItem());
        registerModel.item(i, 4)->setCheckable(true);
    }

    connect(&registerModel, SIGNAL(itemChanged(QStandardItem *)), this, SLOT(on_guiRegisterChanged(QStandardItem *)));

    ui->registerTableView->setModel(&registerModel);
    ui->registerTableView->resizeColumnsToContents();
    ui->registerTableView->resizeRowsToContents();

    /* governor output data */
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

    /* governor input data */
    inputModel.setHorizontalHeaderLabels(
            QStringList() << QApplication::translate("qgovernor", "Addr")
                          << QApplication::translate("qgovernor", "Val Hex")
                          << QApplication::translate("qgovernor", "Val Dec")
                          << QApplication::translate("qgovernor", "Val Bin"));

    ui->inputTableView->setModel(&inputModel);
    ui->inputTableView->resizeColumnsToContents();
    ui->inputTableView->resizeRowsToContents();

    /* Dialog initialization */
    connectDialog = new ConnectDialog(this);

    /* Simulator initialization */
    simulator = new Simulator(this);
    connect(simulator, SIGNAL(newOutput(unsigned char)), this, SLOT(on_simulatorInput(unsigned char)));
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
        ui->statusBar->showMessage(tr("Connecting to interface %1 ...").arg(connectDialog->getInterfaceId()));
        if(connectDialog->getInterfaceId() == 0)
            simulator->show();

        for(int i=0; i<32; i++)
            governorMaster->sendGet(i);
        ui->disconnectPushButton->setDisabled(false);
        ui->registerTableView->setDisabled(false);
        connected = true;
        ui->statusBar->showMessage(tr("Connection to interface %1 established.").arg(connectDialog->getInterfaceId()), 5000);
    }else{
        ui->connectPushButton->setDisabled(false);
    }
}

void MainWindow::on_disconnectPushButton_clicked()
{
    ui->statusBar->showMessage(tr("Connection closed."), 5000);
    if(connectDialog->getInterfaceId() == 0)
        simulator->hide();
    ui->connectPushButton->setDisabled(false);
    ui->disconnectPushButton->setDisabled(true);
    ui->registerTableView->setDisabled(true);
    connected = false;
}

void MainWindow::addInput(unsigned char addr, unsigned short value)
{
    inputModel.appendRow(
            QList<QStandardItem *>() << new QStandardItem(QString::number(addr, 10).rightJustified(3, '0', false))
                                     << new QStandardItem(QString::number(value, 16).rightJustified(4, '0', false))
                                     << new QStandardItem(QString::number(value, 10).rightJustified(5, '0', false))
                                     << new QStandardItem(QString::number(value >> 8, 2).rightJustified(8, '0', false)
                                                          .append(" ")
                                                          .append(QString::number(value & 0xFF, 2).rightJustified(8, '0', false))));
    ui->inputTableView->resizeColumnsToContents();
    ui->inputTableView->scrollToBottom();
    ui->inputTableView->resizeRowsToContents();
}

void MainWindow::addOutput(unsigned char addr, unsigned short value)
{
    outputModel.appendRow(
            QList<QStandardItem *>() << new QStandardItem()
                                     << new QStandardItem("W")
                                     << new QStandardItem(QString::number(addr, 10).rightJustified(3, '0', false))
                                     << new QStandardItem(QString::number(value, 16).rightJustified(4, '0', false))
                                     << new QStandardItem(QString::number(value, 10).rightJustified(5, '0', false))
                                     << new QStandardItem(QString::number(value >> 8, 2).rightJustified(8, '0', false)
                                                          .append(" ")
                                                          .append(QString::number(value & 0xFF, 2).rightJustified(8, '0', false))));
    ui->outputTableView->resizeColumnsToContents();
    ui->outputTableView->resizeRowsToContents();
    ui->outputTableView->scrollToBottom();
}

void MainWindow::addOutput(bool monitor, unsigned char addr)
{
    outputModel.appendRow(
            QList<QStandardItem *>() << new QStandardItem()
                                     << new QStandardItem("R")
                                     << new QStandardItem(QString::number(addr, 10).rightJustified(3, '0', false)));
    ui->outputTableView->resizeColumnsToContents();
    ui->outputTableView->resizeRowsToContents();
    ui->outputTableView->scrollToBottom();
}

void MainWindow::on_outputTriggered()
{
    signed short data;
    static int state = 0;
    unsigned char addr;
    unsigned short value;

    while((data = governorMaster->pickupByte()) != -1){
        switch(state){
        case 0:
            if((data & GP_MODE_MASK) == GP_MODE_WRITE){
                addr = data & GP_ADDR_MASK;
                state = 1;
            }else if((data & GP_MODE_MASK) == GP_MODE_READ | GP_MODE_PEEK){
                addOutput(false, data & GP_ADDR_MASK);
            }
            break;
        case 1:
            value = data;
            state = 2;
            break;
        case 2:
            value |= data << 8;
            addOutput(addr, value);
            state = 0;
            break;
        }

        simulator->handleByte(data);
    }
}

void MainWindow::on_registerChanged(unsigned char addr)
{
    int value = governorMaster->getRegisterMapValue(addr);

    registerModel.item(addr, 1)->setData(QString::number(value, 16).rightJustified(4, '0', false), Qt::DisplayRole);
    registerModel.item(addr, 0)->setData(QString::number(value, 10).rightJustified(5, '0', false), Qt::DisplayRole);
    registerModel.item(addr, 2)->setData(QString::number(value >> 8, 2).rightJustified(8, '0', false)
                                                .append(" ")
                                                .append(QString::number(value & 0xFF, 2)
                                                        .rightJustified(8, '0', false)),
                                                Qt::DisplayRole);
}

void MainWindow::on_guiRegisterChanged(QStandardItem *item)
{
    int value;
    bool conversion_ok;

    switch(item->column()){
    case 0:
        value = item->data(Qt::DisplayRole).toString().toInt(&conversion_ok, 10);
        break;
    case 1:
        value = item->data(Qt::DisplayRole).toString().toInt(&conversion_ok, 16);
        break;
    case 2:
        value = item->data(Qt::DisplayRole).toString().remove(QChar(' '), Qt::CaseInsensitive).toInt(&conversion_ok, 2);
        break;
    }

    if(conversion_ok && connected){
        registerModel.item(item->row(), 0)->setData(QString::number(value, 10).rightJustified(5, '0', false), Qt::DisplayRole);
        registerModel.item(item->row(), 1)->setData(QString::number(value, 16).rightJustified(4, '0', false), Qt::DisplayRole);
        registerModel.item(item->row(), 2)->setData(QString::number(value >> 8, 2).rightJustified(8, '0', false)
                                                    .append(" ")
                                                    .append(QString::number(value & 0xFF, 2)
                                                            .rightJustified(8, '0', false)),
                                                    Qt::DisplayRole);
        if(governorMaster->getRegisterMapValue(item->row()) != value)
            governorMaster->sendSet(item->row(), value);
    }else{
        value = governorMaster->getRegisterMapValue(item->row());
        registerModel.item(item->row(), 0)->setData(QString::number(value, 10).rightJustified(5, '0', false), Qt::DisplayRole);
        registerModel.item(item->row(), 1)->setData(QString::number(value, 16).rightJustified(4, '0', false), Qt::DisplayRole);
        registerModel.item(item->row(), 2)->setData(QString::number(value >> 8, 2).rightJustified(8, '0', false)
                                                    .append(" ")
                                                    .append(QString::number(value & 0xFF, 2)
                                                            .rightJustified(8, '0', false)),
                                                    Qt::DisplayRole);
        if(!connected)
            ui->statusBar->showMessage(tr("Please connect first before changing register values..."), 5000);
    }
}

void MainWindow::on_simulatorInput(unsigned char data)
{
    static int state = 0;
    static unsigned char addr;
    static unsigned short value;

    switch(state){
    case 0:
        addr = data & GP_ADDR_MASK;
        state = 1;
        break;
    case 1:
        value = data;
        state = 2;
        break;
    case 2:
        value |= data << 8;
        addInput(addr, value);
        state = 0;
        break;
    }

    governorMaster->handleByte(data);
}
