/*****************
 * # for using HDMI lcd, add below line to /boot/config.txt
hdmi_mode=16
hdmi_group=2
# for using usb_serial ports
# add user to dialout group for using serial port (ttyUSB0, ...)
sudo usermod -a -G dialout pi
# for using ttyS0 (mini serial port on GPIO)
sudo usermod -a -G tty pi
    # then edit the /boot/cmdline.txt: console=serial0 -> console=serial1
 ***********************/
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // UART emulator
    uart_emulator = new QSerialPort(this);
    connect(uart_emulator, SIGNAL(error(QSerialPort::SerialPortError)), this,
            SLOT(uart_emulator_handleError(QSerialPort::SerialPortError)));
    // UART receiver
    uart_receiver = new QSerialPort(this);
    connect(uart_receiver, SIGNAL(error(QSerialPort::SerialPortError)), this,
            SLOT(uart_receiver_handleError(QSerialPort::SerialPortError)));
    connect(uart_receiver, SIGNAL(readyRead()), this, SLOT(uart_receiver_readData()));

    // setup default parameters for uart connectors
    // UART emulator = ttyUSB0
    uart_emulator->setPortName("ttyUSB0");
    uart_emulator->setBaudRate(QSerialPort::Baud9600);
    uart_emulator->setDataBits(QSerialPort::Data8);
    uart_emulator->setParity(QSerialPort::Parity::NoParity);
    uart_emulator->setStopBits(QSerialPort::StopBits::OneStop);
    uart_emulator->setFlowControl(QSerialPort::FlowControl::NoFlowControl);

    // UART receiver = ttyS0
    uart_receiver->setPortName("ttyS0");
    uart_receiver->setBaudRate(QSerialPort::Baud9600);
    uart_receiver->setDataBits(QSerialPort::Data8);
    uart_receiver->setParity(QSerialPort::Parity::NoParity);
    uart_receiver->setStopBits(QSerialPort::StopBits::OneStop);
    uart_receiver->setFlowControl(QSerialPort::FlowControl::NoFlowControl);

    // Connect receiver ports
    if (uart_receiver->open(QIODevice::ReadWrite)) {
        ui->statusBar->showMessage(QString("Connected to %1 : %2")
                                   .arg(uart_receiver->portName())
                                   .arg(uart_receiver->baudRate()));
    } else {
        QMessageBox::critical(this, tr("Error"), uart_receiver->errorString());
        ui->statusBar->showMessage("Open error ttyS0");
    }


    // connect emulator port
    if (uart_emulator->open(QIODevice::ReadWrite)) {
        ui->statusBar->showMessage(QString("Connected to %1 : %2")
                                   .arg(uart_emulator->portName())
                                   .arg(uart_emulator->baudRate()));
    } else {
        QMessageBox::critical(this, tr("Error"), uart_emulator->errorString());
        ui->statusBar->showMessage("Open error ttyUSB0");
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_setting_clicked()
{
    // connect emulator port
    if (uart_emulator->open(QIODevice::ReadWrite)) {
        ui->statusBar->showMessage(QString("Connected to %1 : %2")
                                   .arg(uart_emulator->portName())
                                   .arg(uart_emulator->baudRate()));
    } else {
        QMessageBox::critical(this, tr("Error"), uart_emulator->errorString());
        ui->statusBar->showMessage("Open error ttyUSB0");
    }
    // Connect receiver ports
    if (uart_receiver->open(QIODevice::ReadWrite)) {
        ui->statusBar->showMessage(QString("Connected to %1 : %2")
                                   .arg(uart_receiver->portName())
                                   .arg(uart_receiver->baudRate()));
    } else {
        QMessageBox::critical(this, tr("Error"), uart_receiver->errorString());
        ui->statusBar->showMessage("Open error ttyS0");
    }

}

void MainWindow::uart_emulator_handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
    QMessageBox::critical(this, QString("Critical Error emulator"), uart_emulator->errorString());
    }
}

void MainWindow::uart_receiver_handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
    QMessageBox::critical(this, QString("Critical Error receiver"), uart_receiver->errorString());
    }
}

void MainWindow::uart_receiver_readData()
{
    QByteArray data;
        data.append( uart_receiver->readAll());
        while (uart_receiver->waitForReadyRead(200))
                data.append(uart_receiver->readAll());

        if (uart_receiver->error() == QSerialPort::ReadError) {
            qDebug("Failed to read from port "+uart_receiver->portName().toLatin1()+", error: "+uart_receiver->errorString().toLatin1());
        } else if (uart_receiver->error() == QSerialPort::TimeoutError && data.isEmpty()) {
            qDebug("No data was currently available for reading from port "+uart_receiver->portName().toLatin1());
        }
        qDebug("Data successfully received from port "+uart_receiver->portName().toLatin1());

        qDebug(data);
        ui->label_data1->setText(QString(data));

        // Xử lý dữ liệu

}

void MainWindow::on_pushButton_send_clicked()
{
    QString str;
    str = ui->lineEdit_test->text();
    qDebug() << str;
    // send out to receiver
    uart_emulator->write(str.toLocal8Bit());
}
