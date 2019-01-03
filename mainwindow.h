#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QtSerialPort>
#include <QtDebug>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_setting_clicked();
    void uart_emulator_handleError(QSerialPort::SerialPortError error);
    void uart_receiver_readData();
    void uart_receiver_handleError(QSerialPort::SerialPortError error);

    void on_pushButton_send_clicked();

private:
    Ui::MainWindow *ui;

    QSerialPort *uart_emulator;
    QSerialPort *uart_receiver;
    QByteArray uart_receiver_rxdata;

};

#endif // MAINWINDOW_H
