#ifndef GOVERNORSIMULATOR_H
#define GOVERNORSIMULATOR_H

#include <QIODevice>

#include "simulator.h"

class GovernorSimulator : public QIODevice
{
    Q_OBJECT
public:
    GovernorSimulator(QObject *parent = 0);
    ~GovernorSimulator();

private:
    Simulator simulator;

protected:
    virtual qint64 readData(char *data, qint64 maxlen);
    virtual qint64 writeData(const char *data, qint64 len);

private slots:
    void on_simulatorReadyRead();
    void on_simulatorShutdown();
};

#endif // GOVERNORSIMULATOR_H
