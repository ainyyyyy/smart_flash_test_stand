#include "collect_data.h"
#include <QThread>
#include <iostream>
#include <windows.h>
#include "tchar.h"
#include <string.h>
#include <QFile>
#include <QTextStream>


CollectData::CollectData(QObject *parent) : QObject(parent)
{
}

void CollectData::main_process()
{
//////////////////////////При наличии расхождений необходимо заменить эти 2 переменные на актуальные (см. README репозитория)
    char *path = "E:\\";
    LPCTSTR sPortName = _T("COM5");
//////////////////////////

    float rx_buffer1 = 0;
    float a;
    float all = 100;
    std::string strr;
    //strr += '\r';
    //strr += '\n';
    char *cstr;
  DWORD TWR = 0;

    char input[1];
    int count = 0;
    std::string str = "";
    DWORD bytesRead, bytesWrite;
    HANDLE hComm, hSerial;

hSerial = ::CreateFile(sPortName, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

//watching files

unsigned __int64 TotalNumberOfBytes;
unsigned __int64 TotalNumberOfFreeBytes;
ULARGE_INTEGER free;


  std::cout << "watching for changes... " << path << std::endl;

  HANDLE file = CreateFileA(path,
    FILE_LIST_DIRECTORY,
    FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
    NULL,
    OPEN_EXISTING,
    FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED,
    NULL);
  OVERLAPPED overlapped;
  overlapped.hEvent = CreateEvent(NULL, FALSE, 0, NULL);

  uint8_t change_buf[1024];
  BOOL success = ReadDirectoryChangesW(
    file, change_buf, 1024, TRUE,
    FILE_NOTIFY_CHANGE_FILE_NAME  |
    FILE_NOTIFY_CHANGE_DIR_NAME   |
    FILE_NOTIFY_CHANGE_LAST_WRITE,
    NULL, &overlapped, NULL);

  for (;;) {
        DWORD result = WaitForSingleObject(overlapped.hEvent, 0);

        if (result == WAIT_OBJECT_0) {
          DWORD bytes_transferred;
          GetOverlappedResult(file, &overlapped, &bytes_transferred, FALSE);

          FILE_NOTIFY_INFORMATION *event = (FILE_NOTIFY_INFORMATION*)change_buf;

          for (;;) {

                if((event->Action != FILE_ACTION_ADDED) and (event->Action != FILE_ACTION_RENAMED_OLD_NAME)){
                    GetDiskFreeSpaceExA(path,&free,
                    (PULARGE_INTEGER)&TotalNumberOfBytes,
                    (PULARGE_INTEGER)&TotalNumberOfFreeBytes);

//new code
                    char *cstr = &(strr[0]);
                    // DWORD data = DWORD(TotalNumberOfBytes - TotalNumberOfFreeBytes - 16384);
                    char buffer[50];
                    char buffer_mem_1[15];
                    char buffer_free[15];
                    char buffer_occupied[15];
                    char buffer_all[15];
                    char str[20];
                    //float cycle = 100000;
                    // strcpy(buffer, cstr);
                      // strcat(buffer, "\n\n");
                    snprintf(buffer_mem_1, sizeof buffer_mem_1, "%.10f",(((float)(TotalNumberOfBytes - TotalNumberOfFreeBytes)/(float)(TotalNumberOfBytes))/1000));
                    snprintf(buffer_free, sizeof buffer_free, "%llu",(TotalNumberOfFreeBytes));
                    snprintf(buffer_occupied, sizeof buffer_occupied, "%llu",((TotalNumberOfBytes - TotalNumberOfFreeBytes - 16384)));
                    snprintf(buffer_all, sizeof buffer_all, "%llu",(TotalNumberOfBytes));
                    sprintf(buffer,"%s %s", buffer_mem_1,buffer_free);
                    sprintf(buffer,"%s %s", buffer,buffer_occupied);
                    sprintf(buffer,"%s %s", buffer,buffer_all);
                    // strcat(buffer, buffer_mem);
                    //std::cout<<buffer<<std::endl;

                    // itoa(data,buffer,20);
                    DCB dcbSerialParams = { 0 };
                    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
                    if(!GetCommState(hSerial, &dcbSerialParams)){
                        std::cout << "getting state error\n";
                    }
                    dcbSerialParams.BaudRate = CBR_115200;
                    dcbSerialParams.ByteSize = 8;
                    dcbSerialParams.StopBits = ONESTOPBIT;
                    dcbSerialParams.Parity = NOPARITY;
                    if(!SetCommState(hSerial, &dcbSerialParams)){
                        std::cout << "error setting serial port state\n";
                    }
                    // char data[] = {dbcc_size};  // строка для передачи
                    DWORD dwSize = sizeof(buffer);   // размер этой строки
                    DWORD dwBytesWritten;    // тут будет количество собственно переданных байт
                    DWORD dwBytesRead;

                    QString in_data;
                    QString in_data2;
                    QFile file("data.txt");
                    //QFile file2("data2.txt");
                    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
                        {
                            QTextStream in(&file);
                            in_data = in.readLine();
                            in_data2 = in.readLine();
                            file.close();
                        }

                    if (in_data != ""){
                        all = in_data.toFloat();
                    }

                    if (in_data2 != ""){
                        rx_buffer1 = in_data2.toFloat();
                    }


                    a = ((float)(TotalNumberOfBytes - TotalNumberOfFreeBytes)/(float)(TotalNumberOfBytes))/1000;
                    //convert_rx_buff += fabs(a*1000 - rx_buffer2);
                    all -= fabs(a - rx_buffer1);
                    rx_buffer1 = a;
                    //rx_buffer2 = a*1000;
                    uint8_t str1[20];
                    // uint8_t str2[20];
                    //strcpy((char*)str1,uint64_to_string(convert_rx_buff));
                    //sprintf((char*)str2,"%.3f", convert_rx_buff);
                    sprintf((char*)str1,"%.5f", all);
                    //strcpy((char*)str1,uint64_to_string(convert_rx_buff));
                    memset(str, 0, sizeof(str));
                    sprintf((char*)str,"%s", (char*)str1);



                    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
                        {
                            QTextStream out(&file);
                            out << all << "\n" << rx_buffer1;
                            file.close();
                        }

                    //std::cout<<buffer<<"       write"<<std::endl<<std::endl;
                    BOOL iRet = WriteFile(hSerial, buffer, dwSize, &dwBytesWritten, NULL);
                    // std::cout<<iRet<<std::endl;
                    /*
                    if (iRet){
                    BOOL oRet = ReadFile(hSerial, str, dwSize, &dwBytesRead, NULL);
                    // std::cout<<oRet<<std::endl;
                    std::cout<<str<<"       read"<<std::endl<<std::endl;
                    }
*/
                    emit valueChanged(TotalNumberOfBytes,
                                      TotalNumberOfBytes - TotalNumberOfFreeBytes,
                                      TotalNumberOfFreeBytes,
                                      atof(str));
/*
                    std::cout << "wr:" << std::endl;
                    strr = std::to_string(TotalNumberOfBytes - TotalNumberOfFreeBytes - 16384) + " "
                            + std::to_string(TotalNumberOfBytes) + " "
                            + std::to_string(TotalNumberOfFreeBytes);
                    std::cout << send_to_stm(hSerial, strr) << std::endl;
/*
                    const char *strr_ch = strr.c_str();
                    char firstDigit[50], secondDigit[50], thirdDigit[50];
                    char *space = strstr(strr_ch, " ");
                    int digitLen = space - strr_ch;
                    int otherStringLen = strlen(strr_ch) - digitLen - 1;
                    snprintf(firstDigit, digitLen + 1, strr_ch);

                    char rest_of_strr[50];
                    //snprintf(rest_of_rx_buffer, otherStringLen + 1, &space[1]);
                    strncpy(rest_of_strr, &space[1], otherStringLen + 1);

                    space = strstr(rest_of_strr, " ");
                    digitLen = space - rest_of_strr;
                    otherStringLen = strlen(rest_of_strr) - digitLen - 1;
                    snprintf(secondDigit, digitLen + 1, rest_of_strr);
                    strncpy(thirdDigit, &space[1], otherStringLen + 1);

*/

                }

                // Are there more events to handle?
               if (event->NextEntryOffset) {
                  *((uint8_t**)&event) += event->NextEntryOffset;
                } else {
                  break;
                }


          }
/*
                std::cout << "TotalNumberOfBytes:" << std::endl;
                strr = std::to_string(TotalNumberOfBytes);
                std::cout << send_to_stm(hSerial, strr) << std::endl;


                    std::cout << "TotalNumberOfFreeBytes:" << std::endl;
                    strr = std::to_string(TotalNumberOfFreeBytes);
                    std::cout << send_to_stm(hSerial, strr) << std::endl;
*/




          // Queue the next event
          BOOL success = ReadDirectoryChangesW(
                file, change_buf, 1024, TRUE,
                FILE_NOTIFY_CHANGE_FILE_NAME  |
                FILE_NOTIFY_CHANGE_DIR_NAME   |
                FILE_NOTIFY_CHANGE_LAST_WRITE,
                NULL, &overlapped, NULL);

        }

        // Do other loop stuff here...
  }
    CloseHandle(hComm);
    emit finished();
}
