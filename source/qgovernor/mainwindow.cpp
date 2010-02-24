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
        registerModel.setItem(i, 2, new QStandardItem(QString::number(0, 2).rightJustified(16, '0', false)));
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
    ui->statusBar->showMessage("Connecting...");

    outputModel.appendRow(
            QList<QStandardItem *>() << new QStandardItem()
                                     << new QStandardItem("R")
                                     << new QStandardItem(QString::number(10, 10).rightJustified(3, '0', false))
                                     << new QStandardItem(QString::number(12, 16).rightJustified(4, '0', false))
                                     << new QStandardItem(QString::number(12, 10).rightJustified(4, '0', false))
                                     << new QStandardItem(QString::number(12, 2).rightJustified(16, '0', false)));
    ui->outputTableView->resizeColumnsToContents();
    ui->outputTableView->resizeRowsToContents();
    ui->outputTableView->scrollToBottom();

}
