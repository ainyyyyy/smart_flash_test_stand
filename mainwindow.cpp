#include "mainwindow.h"
#include "ui_mainwindow.h"
//#include <QTimer>
#include <QThread>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

/*
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::onTimerTimeout);
    timer->start(1000); // Запуск таймера с интервалом 1000 мс (1 секунда)
*/
    collectData = new CollectData();
    QThread *processThread = new QThread(this);

    collectData->moveToThread(processThread);

    connect(processThread, &QThread::started, collectData, &CollectData::main_process);
    connect(collectData, &CollectData::valueChanged, this, &MainWindow::onValueChanged);
    connect(collectData, &CollectData::finished, processThread, &QThread::quit);
    connect(collectData, &CollectData::finished, collectData, &CollectData::deleteLater);
    connect(processThread, &QThread::finished, processThread, &QThread::deleteLater);

    processThread->start();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onValueChanged(QString value)
{
    ui->label->setText(value);
}
/*
void MainWindow::on_pushButton_clicked()
{
    ui->label->setText("Кнопка нажата");
}

void MainWindow::updateLabelText(QString qstr)
{
    //static int counter = 0;
    //counter++;
    QString newText = qstr.arg(qstr);
    ui->label->setText(newText);
}

void MainWindow::onTimerTimeout()
{
    // Ваш код для выполнения в бесконечном цикле



}
*/
