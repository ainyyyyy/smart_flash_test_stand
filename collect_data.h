#ifndef COLLECT_DATA_H
#define COLLECT_DATA_H


#include <QObject>
#include <iostream>
#include <windows.h>

class CollectData : public QObject
{
    Q_OBJECT

public:
    explicit CollectData(QObject *parent = nullptr);

public slots:
    void main_process();
    //int send_to_stm(HANDLE hSerial, std::string strr);

signals:
    void valueChanged(float value1, float value2, float value3, float value4);
    void finished();
};


#endif // COLLECT_DATA_H
