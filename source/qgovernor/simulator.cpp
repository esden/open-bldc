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

#include <QStandardItem>

#include "simulator.h"
#include "ui_simulator.h"

Simulator::Simulator(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Simulator)
{
    ui->setupUi(this);

    /* Governor */
    governorClient = new GovernorClient();
    connect(governorClient, SIGNAL(outputTriggered()), this, SLOT(on_outputTriggered()));
    connect(governorClient, SIGNAL(registerChanged(unsigned char)), this, SLOT(on_registerChanged(unsigned char)));

    /* initialize client register with some arbitary numbers so we see some connection action */
    for(int i=0; i<16; i++)
        governorClient->setRegister(i, 1 << i);
    for(int i=0; i<16; i++)
        governorClient->setRegister(16+i, 0x8000 >> i);

    /* register display table */
    unsigned short value;

    for(int i=0; i<32; i++){
        value = governorClient->getRegisterMapValue(i);
        registerModel.setRegisterValue(i, value);
    }

    connect(&registerModel, SIGNAL(itemChanged(QStandardItem *)), this, SLOT(on_guiRegisterChanged(QStandardItem *)));

    ui->registerTableView->setModel(&registerModel);
    ui->registerTableView->resizeColumnsToContents();
    ui->registerTableView->resizeRowsToContents();

}

Simulator::~Simulator()
{
    delete ui;
}

int Simulator::handleByte(unsigned char byte)
{
    return governorClient->handleByte(byte);
}

void Simulator::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void Simulator::on_outputTriggered()
{
    signed short data;
    while((data = governorClient->pickupByte()) != -1){
        emit newOutput(data);
    }
}

void Simulator::on_registerChanged(unsigned char addr)
{
    registerModel.setRegisterValue(addr, governorClient->getRegisterMapValue(addr));
}

void Simulator::on_guiRegisterChanged(QStandardItem *item)
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

    if(conversion_ok){
        registerModel.setRegisterValue(item->row(), value);
        if(governorClient->getRegisterMapValue(item->row()) != value)
            governorClient->setRegister(item->row(), value);
    }else{
        registerModel.setRegisterValue(item->row(), governorClient->getRegisterMapValue(item->row()));
    }
}

void Simulator::on_pushButton_clicked()
{
    emit shutdown();
}
