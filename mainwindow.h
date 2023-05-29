#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "collect_data.h"

//#include <QtCharts/QChartView>
#include <QWidget>
#include <QtCharts/QChartView>
#include <string>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT


public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    //QtCharts::QChartView* MainWindow::createPieChart();

    void setupPieChart(QChartView* disk_chart, float used, float free);
    QString formatBytes(float value);
    void updateLabelText(QString qstr);

private slots:
    //void on_pushButton_clicked();
    //void onTimerTimeout();
    void onValueChanged(float value1, float value2, float value3, float value4);




private:
    Ui::MainWindow *ui;
    CollectData *collectData;


    //QTimer *timer;
};
#endif // MAINWINDOW_H
