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

extern "C" {
#include <lg/types.h>
#include <lg/gpdef.h>
}

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
    unsigned short value;

    for(int i=0; i<32; i++){
        value = governorMaster->getRegisterMapValue(i);
        registerModel.setRegisterValue(i, value);
    }

    connect(&registerModel, SIGNAL(itemChanged(QStandardItem *)), this, SLOT(on_guiRegisterChanged(QStandardItem *)));

    ui->registerTableView->setModel(&registerModel);
    ui->registerTableView->resizeColumnsToContents();
    ui->registerTableView->resizeRowsToContents();

    /* governor output data */
    ui->outputTableView->setModel(&outputModel);
    ui->outputTableView->resizeColumnsToContents();
    ui->outputTableView->resizeRowsToContents();

    /* governor input data */
    ui->inputTableView->setModel(&inputModel);
    ui->inputTableView->resizeColumnsToContents();
    ui->inputTableView->resizeRowsToContents();

    /* Dialog initialization */
    connectDialog = new ConnectDialog(this);

    /* Actions */
    updateRegister = new QAction(tr("Update"), this);
    updateRegister->setStatusTip(tr("Read register content from client."));
    updateAllRegisters = new QAction(tr("Update All"), this);
    updateAllRegisters->setStatusTip(tr("Read all register contents from client."));
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

void MainWindow::on_outputTriggered()
{
    signed short data;
    QByteArray qdata;

    while((data = governorMaster->pickupByte()) != -1){
        outputModel.handleByte(data);
        qdata.append(data);
    }

    governorInterface->write(qdata);

    ui->outputTableView->resizeColumnsToContents();
    ui->outputTableView->resizeRowsToContents();
    ui->outputTableView->scrollToBottom();
}

void MainWindow::on_registerChanged(unsigned char addr)
{
    registerModel.setRegisterValue(addr, governorMaster->getRegisterMapValue(addr));
    switch(addr){
    case 0:
        ui->forcedCommCheckBox->setChecked(governorMaster->getRegisterMapValue(0) & (1 << 1));
        ui->ADCCommCheckBox->setChecked(governorMaster->getRegisterMapValue(0) & (1 << 2));
        break;
    case 1:
        ui->PWMOffsetSpinBox->setValue(governorMaster->getRegisterMapValue(addr));
        break;
    case 2:
        ui->PWMDutyCycleSpinBox->setValue(governorMaster->getRegisterMapValue(addr));
        break;
    case 3:
        ui->forcedCommTimValSpinBox->setValue(governorMaster->getRegisterMapValue(addr));
        break;
    case 4:
        ui->ADCLevelspinBox->setValue(governorMaster->getRegisterMapValue(addr));
        break;
    }
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
    case 3:
        governorMaster->sendGetCont(item->row());
        return;
        break;
    }

    if(conversion_ok && connected){
        registerModel.setRegisterValue(item->row(), value);
        if(governorMaster->getRegisterMapValue(item->row()) != value)
            governorMaster->sendSet(item->row(), value);
    }else{
        registerModel.setRegisterValue(item->row(), governorMaster->getRegisterMapValue(item->row()));
        if(!connected)
            ui->statusBar->showMessage(tr("Please connect first before changing register values..."), 5000);
    }
}

void MainWindow::on_registerTableView_customContextMenuRequested(QPoint pos)
{
    QModelIndex index = ui->registerTableView->indexAt(pos);
    QMenu menu(this);
    QAction *action;

    if(index.isValid()){
        menu.addAction(updateRegister);
        menu.addAction(updateAllRegisters);
    }else{
        menu.addAction(updateAllRegisters);
    }

    action = menu.exec(ui->registerTableView->mapToGlobal(pos));

    if(action == updateRegister)
        governorMaster->sendGet(index.row());
    else if(action == updateAllRegisters)
        for(int i=0; i<32; i++)
            governorMaster->sendGet(i);
}

void MainWindow::on_actionAbout_Qt_triggered()
{
    QMessageBox::aboutQt(this, tr("About Qt"));
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this, tr("About"),
                       tr("<center>qgovernor 0.1</center><br />\n"
                          "<center>Copyright (C) 2010 "
                          "Piotr Esden-Tempski &lt;piotr@esden.net&gt;</center>\n"
                          "<center>GNU GPL, version 3 or later</center><br />\n"
                          "<center><a href=\"http://open-bldc.org\">"
                          "http://open-bldc.org</a></center>"));
}

void MainWindow::on_governorInterface_aboutToClose()
{
    ui->statusBar->showMessage(tr("Connection closed."), 5000);
    if(connectDialog->getInterfaceId() == 0)
        delete governorInterface;
    ui->actionConnect->setText(tr("Connect..."));
    ui->actionConnect->setIconText(tr("Connect"));
    ui->actionConnect->setToolTip(tr("Connect"));
    ui->actionConnect->setChecked(false);
    ui->registerTableView->setDisabled(true);
    ui->commGroupBox->setDisabled(true);
    connected = false;
}

void MainWindow::on_governorInterface_readyRead()
{
    char data[1024];
    qint64 size;

    size = governorInterface->read(data, sizeof(data));

    for(int i=0; i<size; i++){
        inputModel.handleByte(data[i]);
        governorMaster->handleByte(data[i]);
    }

    ui->inputTableView->resizeColumnsToContents();
    ui->inputTableView->resizeRowsToContents();
    ui->inputTableView->scrollToBottom();
}

void MainWindow::on_actionConnect_triggered(bool checked)
{
    if(checked){
        if(connectDialog->exec()){
            ui->statusBar->showMessage(tr("Connecting to interface %1 ...").arg(connectDialog->getInterfaceId()));
            switch(connectDialog->getInterfaceId()){
            case 0:
                governorInterface = new GovernorSimulator(this);
                break;
            case 1:
                governorInterface = new GovernorFtdi(this);
                break;
            }

            if(governorInterface->open(QIODevice::ReadWrite)){
                connect(governorInterface, SIGNAL(readyRead()), this, SLOT(on_governorInterface_readyRead()));
                connect(governorInterface, SIGNAL(aboutToClose()), this, SLOT(on_governorInterface_aboutToClose()));

                for(int i=0; i<32; i++)
                    governorMaster->sendGet(i);
                ui->registerTableView->setEnabled(true);
                ui->commGroupBox->setEnabled(true);
                connected = true;
                ui->actionConnect->setText(tr("Disconnect..."));
                ui->actionConnect->setIconText(tr("Disconnect"));
                ui->actionConnect->setToolTip(tr("Disconnect"));
                ui->statusBar->showMessage(tr("Connection to interface %1 established.").arg(connectDialog->getInterfaceId()), 3000);
            }else{
                ui->actionConnect->setChecked(false);
                delete governorInterface;
                ui->statusBar->showMessage(tr("Connection failed to interface %1.").arg(connectDialog->getInterfaceId()), 3000);
            }
        }else{
            ui->actionConnect->setChecked(false);
        }
    }else{
        ui->statusBar->showMessage(tr("Connection closed."), 5000);
        if(connectDialog->getInterfaceId() == 0 ||
           connectDialog->getInterfaceId() == 1)
            delete governorInterface;
        ui->registerTableView->setDisabled(true);
        ui->commGroupBox->setDisabled(true);
        connected = false;
        ui->actionConnect->setText(tr("Connect..."));
        ui->actionConnect->setIconText(tr("Connect"));
        ui->actionConnect->setToolTip(tr("Connect"));
    }
}

void MainWindow::on_forcedCommCheckBox_clicked(bool checked)
{
    if(checked){
        registerModel.setRegisterValue(0, governorMaster->getRegisterMapValue(0) | (1 << 1));
    }else{
        registerModel.setRegisterValue(0, governorMaster->getRegisterMapValue(0) & ~(1 << 1));
    }
}

void MainWindow::on_ADCCommCheckBox_clicked(bool checked)
{
    if(checked){
        registerModel.setRegisterValue(0, governorMaster->getRegisterMapValue(0) | (1 << 2));
    }else{
        registerModel.setRegisterValue(0, governorMaster->getRegisterMapValue(0) & ~(1 << 2));
    }
}

void MainWindow::on_PWMDutyCycleSpinBox_valueChanged(int value)
{
    registerModel.setRegisterValue(2, value);
}

void MainWindow::on_PWMOffsetSpinBox_valueChanged(int value)
{
    registerModel.setRegisterValue(1, value);
}

void MainWindow::on_forcedCommTimValSpinBox_valueChanged(int value)
{
    registerModel.setRegisterValue(3, value);
}

void MainWindow::on_ADCLevelspinBox_valueChanged(int value)
{
    registerModel.setRegisterValue(4, value);
}

void MainWindow::on_triggerCommPushButton_clicked()
{
    registerModel.setRegisterValue(0, governorMaster->getRegisterMapValue(0) | (1 << 0));
    registerModel.setRegisterValue(0, governorMaster->getRegisterMapValue(0) & ~(1 << 0));
}

void MainWindow::on_forcedCommTimIncSpinBox_valueChanged(int step)
{
    ui->forcedCommTimValSpinBox->setSingleStep(step);
}
