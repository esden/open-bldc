#include "governorftdi.h"

#include "usb.h"

GovernorFtdiReadThread::GovernorFtdiReadThread(struct ftdi_context *context)
{
    ftdic = context;
}

void GovernorFtdiReadThread::run()
{
    unsigned char data;
    bool data_added = false;
    int ret;

    while(ftdic->usb_dev != NULL){
        if((ret = ftdi_read_data(ftdic, &data, 1)) > 0){
            qDebug("got from ftdi %02X", data);
            buffer.append(data);
            if(buffer.size() <= 1)
            emit readyRead();
            //data_added = true;
        }else{
//            if(ret < 0){
//                qDebug("Error: unable to read data %d (%s)", ret, ftdi_get_error_string(ftdic));
//            }else{
//                if(data_added){
//                    data_added = false;
//                    emit readyRead();
//                    qDebug("data added");
//                }
//                //qDebug("Got %d bytes sleeping.", ret);
//            }
            usleep(200);
        }
    }
    qDebug("dying...");
}

qint64 GovernorFtdiReadThread::read(char *data, qint64 maxlen)
{
    int i;

    for(i=0; i<maxlen && !buffer.isEmpty(); i++)
        data[i] = buffer.takeFirst();

    return i;
}

GovernorFtdi::GovernorFtdi(QObject *parent)
    : QIODevice(parent)
{
}

GovernorFtdi::~GovernorFtdi()
{
    ftdi_usb_close(&ftdic);
    ftdi_deinit(&ftdic);
    ftdic.usb_dev = NULL;
    readThread->wait(0);
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

//    ret = ftdi_read_data_set_chunksize(&ftdic, 1);
//    if(ret < 0){
//        qDebug("Error: unable to set read chunksize: %d (%s)", ret, ftdi_get_error_string(&ftdic));
//    }

//    ret = ftdi_write_data_set_chunksize(&ftdic, 1);
//    if(ret < 0){
//        qDebug("Error: unable to set write chunksize: %d (%s)", ret, ftdi_get_error_string(&ftdic));
//    }

    readThread = new GovernorFtdiReadThread(&ftdic);
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
