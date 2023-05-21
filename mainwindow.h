#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "collect_data.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void updateLabelText(QString qstr);

private slots:
    //void on_pushButton_clicked();
    //void onTimerTimeout();
    void onValueChanged(QString value);



private:
    Ui::MainWindow *ui;
    CollectData *collectData;
    //QTimer *timer;
};
#endif // MAINWINDOW_H
