#ifndef GOVERNORFTDI_H
#define GOVERNORFTDI_H

#include <ftdi.h>

#include <QIODevice>
#include <QThread>
#include <QLinkedList>

class GovernorFtdiReadThread : public QThread
{
    Q_OBJECT
public:
    GovernorFtdiReadThread(struct ftdi_context *context);
    virtual void run();
    qint64 read(char *data, qint64 maxlen);

private:
    struct ftdi_context *ftdic;
    QLinkedList<unsigned char> buffer;

signals:
    void readyRead();
};

class GovernorFtdi : public QIODevice
{
    Q_OBJECT
public:
    GovernorFtdi(QObject *parent = 0);
    ~GovernorFtdi();
    bool open(OpenMode mode);

private:
    struct ftdi_context ftdic;
    GovernorFtdiReadThread *readThread;

protected:
    virtual qint64 writeData(const char *data, qint64 len);
    virtual qint64 readData(char *data, qint64 maxlen);

private slots:
    void on_readThread_readyRead();
};

#endif // GOVERNORFTDI_H
