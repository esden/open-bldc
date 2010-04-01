#ifndef GOVERNORFTDI_H
#define GOVERNORFTDI_H

#include <ftdi.h>

#include <QIODevice>
#include <QThread>
#include <QMutex>
#include <QLinkedList>

class GovernorFtdiReadThread : public QThread
{
    Q_OBJECT
public:
    GovernorFtdiReadThread(QObject *parent, struct ftdi_context *context);
    ~GovernorFtdiReadThread();

    virtual void run();
    qint64 read(char *data, qint64 maxlen);
    void shutdown();

private:
    struct ftdi_context *ftdic;
    QMutex mutex;
    QLinkedList<unsigned char> buffer;
    bool quit;

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
