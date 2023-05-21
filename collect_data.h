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
    int send_to_stm(HANDLE hSerial, std::string strr);

signals:
    void valueChanged(QString value);
    void finished();
};


#endif // COLLECT_DATA_H
