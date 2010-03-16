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

#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <QtGui>
#include <QDialog>

#include "governorclient.h"

namespace Ui {
    class Simulator;
}

class Simulator : public QDialog {
    Q_OBJECT
public:
    Simulator(QWidget *parent = 0);
    ~Simulator();
    int handleByte(unsigned char byte);

protected:
    void changeEvent(QEvent *e);

    QStandardItemModel registerModel;
    GovernorClient *governorClient;

private:
    Ui::Simulator *ui;

private slots:
    void on_outputTriggered();
    void on_registerChanged(unsigned char addr);
    void on_guiRegisterChanged(QStandardItem *item);

signals:
    void newOutput(unsigned char data);
};

#endif // SIMULATOR_H
