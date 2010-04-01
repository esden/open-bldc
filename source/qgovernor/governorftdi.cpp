#include "governorftdi.h"

#include "usb.h"

GovernorFtdiReadThread::GovernorFtdiReadThread(QObject *parent, struct ftdi_context *context)
    : QThread(parent), ftdic(context), quit(false)
{
}

GovernorFtdiReadThread::~GovernorFtdiReadThread()
{
    quit = true;
}

void GovernorFtdiReadThread::run()
{
    unsigned char data;
    int ret;

    while(!quit){
        if((ret = ftdi_read_data(ftdic, &data, 1)) > 0){
            mutex.lock();
            buffer.append(data);
            if(buffer.size() <= 1)
                emit readyRead();
            mutex.unlock();
        }else{
            usleep(200);
        }
    }
}

qint64 GovernorFtdiReadThread::read(char *data, qint64 maxlen)
{
    QMutexLocker lock(&mutex);
    int i;

    for(i=0; i<maxlen && !buffer.isEmpty(); i++)
        data[i] = buffer.takeFirst();

    return i;
}

void GovernorFtdiReadThread::shutdown(void)
{
    quit = true;
    wait();
}

GovernorFtdi::GovernorFtdi(QObject *parent)
    : QIODevice(parent)
{
}

GovernorFtdi::~GovernorFtdi()
{
    if(readThread)
        readThread->shutdown();
    ftdi_usb_close(&ftdic);
    ftdi_deinit(&ftdic);
    ftdic.usb_dev = NULL;
}

bool GovernorFtdi::open(OpenMode mode)
{
    int ret;

    if(ftdi_init(&ftdic) < 0)
        qDebug("ftdi_init failed");

    ret = ftdi_set_interface(&ftdic, INTERFACE_B);
    if(ret < 0){
        qDebug("Error: unable to set interface: %d (%s)", ret, ftdi_get_error_string(&ftdic));
        return false;
    }

    ret = ftdi_usb_open(&ftdic, 0x0403, 0x6010);
    if(ret < 0){
        qDebug("Error: unable to open ftdi device: %d (%s)", ret, ftdi_get_error_string(&ftdic));
        return false;
    }

    ret = ftdi_set_baudrate(&ftdic, 9600);
    if(ret < 0){
        qDebug("Error: unable to set baudrate: %d (%s)", ret, ftdi_get_error_string(&ftdic));
        return false;
    }

    readThread = new GovernorFtdiReadThread(this, &ftdic);
    connect(readThread, SIGNAL(readyRead()), this, SLOT(on_readThread_readyRead()));
    readThread->start();

    QIODevice::open(mode);

    return true;
}

qint64 GovernorFtdi::writeData(const char *data, qint64 len)
{
    return ftdi_write_data(&ftdic, (unsigned char *)data, len);
}

qint64 GovernorFtdi::readData(char *data, qint64 maxlen)
{
    return readThread->read(data, maxlen);
}

void GovernorFtdi::on_readThread_readyRead()
{
    emit readyRead();
}
