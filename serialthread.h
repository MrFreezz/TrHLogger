#ifndef SERIALTHREAD_H
#define SERIALTHREAD_H
#include <QThread>
#include <QMutex>
#include <QWaitCondition>

//! [0]
class SerialThread : public QThread
{
    Q_OBJECT

public:
    explicit SerialThread(QObject *parent = nullptr);
    ~SerialThread();

    void transaction(const QString &portName, int waitTimeout);
    void run() Q_DECL_OVERRIDE;

signals:
    void response(const QString &s);
    void error(const QString &s);
    void timeout(const QString &s);

private:
    QString portName;
    //QString request;
    int waitTimeout;
    QMutex mutex;
    QWaitCondition cond;
    bool quit;
};
#endif // SERIALTHREAD_H
