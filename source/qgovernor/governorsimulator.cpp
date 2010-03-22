#include "governorsimulator.h"

GovernorSimulator::GovernorSimulator(QObject *parent)
    : QIODevice(parent)
{
    simulator.show();
    connect(&simulator, SIGNAL(readyRead()), this, SLOT(on_simulatorReadyRead()));
    connect(&simulator, SIGNAL(shutdown()), this, SLOT(on_simulatorShutdown()));
    connect(&simulator, SIGNAL(rejected()), this, SLOT(on_simulatorShutdown()));
}

GovernorSimulator::~GovernorSimulator()
{
}

qint64 GovernorSimulator::writeData(const char *data, qint64 len)
{
    qint64 i;

    for(i=0; i<len; i++){
        if(simulator.handleByte(data[i])){
            return -1;
        }
    }

    return i;
}

qint64 GovernorSimulator::readData(char *data, qint64 maxlen)
{
    qint64 byte, count = 0;

    while((byte = simulator.readByte()) != -1){
        data[count++] = byte;
        if(count == maxlen){
            return count;
        }
    }

    return count;
}

void GovernorSimulator::on_simulatorReadyRead()
{
    emit readyRead();
}

void GovernorSimulator::on_simulatorShutdown()
{
    emit aboutToClose();
}
