#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QThread>
#include <QtCharts/QPieSeries>
#include <QtCharts/QChart>

#include <QGraphicsBlurEffect>
#include <QGraphicsScene>
#include <QGraphicsView>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("SMART-FLASH GUI");

    setupPieChart(ui->disk_chart, 0, 0);

    //создаем основной процесс
    collectData = new CollectData();
    QThread *processThread = new QThread(this);
    collectData->moveToThread(processThread);

    //подключаем все слоты к сигналам
    connect(processThread, &QThread::started, collectData, &CollectData::main_process);
    connect(collectData, &CollectData::valueChanged, this, &MainWindow::onValueChanged);
    connect(collectData, &CollectData::finished, processThread, &QThread::quit);
    connect(collectData, &CollectData::finished, collectData, &CollectData::deleteLater);
    connect(processThread, &QThread::finished, processThread, &QThread::deleteLater);

    //запускаем процесс
    processThread->start();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupPieChart(QChartView* disk_chart, float used, float free) {
    // Создаем серию данных для диаграммы
    QPieSeries* series = new QPieSeries();

    // Заполняем серию данными (значениями и подписями)
    series->append("", used);
    series->append("", free);

    //добавляем данные и задаем цвета секторов
    QPieSlice *slice1 = series->slices().at(0);
    slice1->setColor(QColor(208, 149, 76));

    QPieSlice *slice2 = series->slices().at(1);
    slice2->setColor(QColor(18, 191, 104));

/*
    // Создаем градиенты для неонового эффекта
       QPointF center1 = QPointF(0.5, 0.5) + QPointF(0.5 * cos((slice1->startAngle() + slice1->angleSpan() / 2) * M_PI / 180),
                                                    0.5 * sin((slice1->startAngle() + slice1->angleSpan() / 2) * M_PI / 180));
       QRadialGradient gradient1(center1, 0.5);
       gradient1.setColorAt(0, QColor(208, 149, 76).lighter(150));
       gradient1.setColorAt(1, QColor(208, 149, 76));

       QPointF center2 = QPointF(0.5, 0.5) + QPointF(0.5 * cos((slice2->startAngle() + slice2->angleSpan() / 2) * M_PI / 180),
                                                    0.5 * sin((slice2->startAngle() + slice2->angleSpan() / 2) * M_PI / 180));
       QRadialGradient gradient2(center2, 0.5);
       gradient2.setColorAt(0, QColor(18, 191, 104).lighter(150));
       gradient2.setColorAt(1, QColor(18, 191, 104));

       // Применяем градиенты к секторам
       slice1->setBrush(gradient1);
       slice2->setBrush(gradient2);

       // Создаем и настраиваем эффект размытия для неонового эффекта
       QGraphicsBlurEffect* blurEffect1 = new QGraphicsBlurEffect();
       blurEffect1->setBlurRadius(5);

       QGraphicsBlurEffect* blurEffect2 = new QGraphicsBlurEffect();
       blurEffect2->setBlurRadius(5);

       // Создаем QGraphicsScene и добавляем диаграмму
       QGraphicsScene* scene = new QGraphicsScene();
       QChart* chart = new QChart();
       disk_chart->setChart(chart);
       scene->addItem(chart);

       // Применяем эффект размытия к секторам через QGraphicsScene
       chart->setGraphicsEffectForIndex(0, blurEffect1);
       chart->setGraphicsEffectForIndex(1, blurEffect2);

       // Устанавливаем сцену для disk_chart
       disk_chart->setScene(scene);

*/
    slice1->setBorderColor(QColor(53, 53, 53)); // Заменяем значения RGB на нужный цвет границы
    slice1->setBorderWidth(1); // Задаем ширину границы
    slice2->setBorderColor(QColor(53, 53, 53)); // Заменяем значения RGB на нужный цвет границы
    slice2->setBorderWidth(1); // Задаем ширину границы


    // Создаем объект диаграммы и добавляем серию данных
    QChart* chart = new QChart();
    chart->addSeries(series);
    //chart->setTitle("Disk Space Distribution");
    chart->setBackgroundBrush(QColor(53, 53, 53));
    chart->legend()->hide();

    // Настраиваем объект disk_chart
    disk_chart->setChart(chart);
    disk_chart->setRenderHint(QPainter::Antialiasing);
}

//функция приведения данных в удобный формат
QString MainWindow::formatBytes(float value) {
    const char *units[] = {"байт", "КБ", "МБ", "ГБ", "ТБ"};
    int unitIndex = 0;

    while (value >= 1024 && unitIndex < 4) {
        value /= 1024;
        ++unitIndex;
    }

    char buffer[64];
    snprintf(buffer, sizeof(buffer), "%.2f %s", value, units[unitIndex]);

    return QString::fromStdString(buffer);
}

//функция, выполняемая при выработке сигнала
void MainWindow::onValueChanged(float value1, float value2, float value3, float value4) {

    //выводим текст в приложение
    ui->label->setText(MainWindow::formatBytes(value1));
    ui->label_2->setText(MainWindow::formatBytes(value2));
    ui->label_2->setStyleSheet("color: rgb(208, 149, 76)");

    ui->label_3->setText(MainWindow::formatBytes(value3));
    ui->label_3->setStyleSheet("color: rgb(18, 191, 104)");

    ui->label_9->setText("");

    //выводим значение на прогресс бар
    ui->progressBar->setValue(value4);

    //создаем график
    setupPieChart(ui->disk_chart, value2, value3);

}
