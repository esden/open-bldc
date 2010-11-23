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

#include <QTextStream>
#include "log.h"
#include "governormaster.h"
#include <sys/time.h>
#include <stdio.h>

#define NUM_REGISTERS 32
#define BUFFER_SIZE 2048

QGLogger::QGLogger(const QString &name) : QFile(name)
{
  QTextStream err(stderr, (QIODevice::ReadWrite));

  if (!(this->open(QIODevice::WriteOnly | QIODevice::Text)))
    err << "Warning!  Couldn't open log file '" << name << "' for writing.\n";
}

QGLogger::~QGLogger()
{
  this->close();
}

void
QGLogger::writeRegisters(s32 (*getreg)(unsigned char))
{
  QString line;
  QTextStream out(this);

  struct timeval tv;
  time_t tim;
  time(&tim);
  struct tm *ts;
  gettimeofday(&tv, NULL);
  ts = localtime(&tim);
  
  out << ts->tm_year << ts->tm_mon << ts->tm_mday;

  out << ":" << ts->tm_hour << ":" << ts->tm_min;
  out << ":" << ts->tm_sec << ":" << tv.tv_usec << "\t";

  for(int j=0; j<32; j++) {
    out << getreg(j) << "\t";
  } 

  out << "\n";
}
