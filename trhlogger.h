#ifndef TRHLOGGER_H
#define TRHLOGGER_H

#include <QMainWindow>
#include <QBasicTimer>
#include "serialthread.h"


namespace Ui {
class TrhLogger;
}

class TrhLogger : public QMainWindow
{
    Q_OBJECT

public:
    explicit TrhLogger(QWidget *parent = 0);
    ~TrhLogger();

private slots:
    void on_quitButton_clicked();

    void on_actionOpen_triggered();

    void on_actionSave_triggered();

    void on_StartButton_clicked();

    void showResponse(const QString &s);
    void processError(const QString &s);
    void processTimeout(const QString &s);

    void on_StopButton_clicked();

protected:
    void timerEvent(QTimerEvent *event) override;

private:
    Ui::TrhLogger *ui;
    SerialThread thread;
    QBasicTimer timer;
};

#endif // TRHLOGGER_H
