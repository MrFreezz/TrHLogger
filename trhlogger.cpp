#include "TrhLogger.h"
#include "ui_TrhLogger.h"
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QtSerialPort/QSerialPortInfo>

TrhLogger::TrhLogger(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TrhLogger)
{
    ui->setupUi(this);
    //disable the stop button
    ui->StopButton->setDisabled(true);
    //find available serial ports
    const auto infos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &info : infos)
        ui->serialPortComboBox->addItem(info.portName());

    ui->serialPortComboBox->setFocus();
    ui->serialPortComboBox->setEnabled(true);

    connect(&thread, &SerialThread::response, this, &TrhLogger::showResponse);
    connect(&thread, &SerialThread::error, this, &TrhLogger::processError);
    connect(&thread, &SerialThread::timeout, this, &TrhLogger::processTimeout);

}

TrhLogger::~TrhLogger()
{
    delete ui;
}

void TrhLogger::on_quitButton_clicked()
{
  qApp->quit();
}

void TrhLogger::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QString(),
              tr("Text Files (*.txt);;C++ Files (*.cpp *.h)"));

      if (!fileName.isEmpty()) {
          QFile file(fileName);
          if (!file.open(QIODevice::ReadOnly)) {
              QMessageBox::critical(this, tr("Error"), tr("Could not open file"));
              return;
          }
          QTextStream in(&file);
          ui->textEdit->setText(in.readAll());
          file.close();
      }
}

void TrhLogger::on_actionSave_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), QString(),
              tr("Text Files (*.txt);;C++ Files (*.cpp *.h)"));

      if (!fileName.isEmpty()) {
          QFile file(fileName);
          if (!file.open(QIODevice::WriteOnly)) {
              // error message
          } else {
              QTextStream stream(&file);
              stream << ui->textEdit->toPlainText();
              stream.flush();
              file.close();
          }
      }
}

void TrhLogger::on_StartButton_clicked()
{
  int iInterval = ui->SampleIntervalSpinBox->value();
  //start timer here
  timer.start(iInterval*1000, this);
  //disable the start button
  ui->StartButton->setDisabled(true);
  //enable the stop button
  ui->StopButton->setDisabled(false);
  //disable the SampleIntervalSpinBox
  ui->SampleIntervalSpinBox->setDisabled(true);

}

void TrhLogger::showResponse(const QString &s)
{
  ui->textEdit->append(s);
}

void TrhLogger::processError(const QString &s)
{
  ui->textEdit->append(s);
  timer.stop();
  //re-enable the start button
  ui->StartButton->setDisabled(false);
  //disable the stop button
  ui->StopButton->setDisabled(true);

}

void TrhLogger::processTimeout(const QString &s)
{
  ui->textEdit->append(s);
  timer.stop();
  //re-enable the start button
  ui->StartButton->setDisabled(false);
  //disable the stop button
  ui->StopButton->setDisabled(true);
  //enable the SampleIntervalSpinBox
  ui->SampleIntervalSpinBox->setDisabled(false);

}

void TrhLogger::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == timer.timerId()) {
        //start serial read thread here
        thread.transaction(ui->serialPortComboBox->currentText(),1000);
    } else {
        QWidget::timerEvent(event);
    }

}

void TrhLogger::on_StopButton_clicked()
{
  timer.stop();
  //re-enable the start button
  ui->StartButton->setDisabled(false);
  //disable the stop button
  ui->StopButton->setDisabled(true);
  //enable the SampleIntervalSpinBox
  ui->SampleIntervalSpinBox->setDisabled(false);

}
