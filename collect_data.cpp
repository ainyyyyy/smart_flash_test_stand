#include "collect_data.h"
#include <QThread>
#include <iostream>
#include <windows.h>
#include "tchar.h"

CollectData::CollectData(QObject *parent) : QObject(parent)
{
}

int CollectData::send_to_stm(HANDLE hSerial, std::string strr){

    std::cout << strr << std::endl;
    char *cstr = &(strr[0]);
    std::cout << *cstr << std::endl;
    // DWORD data = DWORD(TotalNumberOfBytes - TotalNumberOfFreeBytes - 16384);
    char buffer[50];
    char str[50];
    strcpy(buffer, cstr);

    std::cout << buffer << std::endl;

    std::cout << *buffer << std::endl;
    // itoa(data,buffer,20);
    DCB dcbSerialParams = { 0 };
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    if(!GetCommState(hSerial, &dcbSerialParams)){
        std::cout << "getting state error\n";
    }
    dcbSerialParams.BaudRate = CBR_9600;
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

/*
    uint8_t firstDigit[50], secondDigit[50];


    char *space = strstr(buffer, " ");
    int digitLen = space - buffer;
    int otherStringLen = strlen(buffer) - digitLen - 1;
    snprintf((char*)firstDigit, digitLen + 1, buffer);

    char rest_of_rx_buffer[50];
    //snprintf(rest_of_rx_buffer, otherStringLen + 1, &space[1]);
    strncpy(rest_of_rx_buffer, &space[1], otherStringLen + 1);

    space = strstr(rest_of_rx_buffer, " ");
    digitLen = space - rest_of_rx_buffer;
    //otherStringLen = strlen(rest_of_rx_buffer) - digitLen - 1;
    snprintf((char*)secondDigit, digitLen + 1, rest_of_rx_buffer);


    cout << "first digit = " << firstDigit << endl;

    cout << "snd digit = " << secondDigit << endl;
*/

    std::cout<<buffer<<"       write"<<std::endl<<std::endl;
    BOOL iRet = WriteFile(hSerial, buffer, dwSize, &dwBytesWritten, NULL);
    // std::cout<<iRet<<std::endl;
    if (iRet){
    BOOL oRet = ReadFile(hSerial, str, dwSize, &dwBytesRead, NULL);
    // std::cout<<oRet<<std::endl;
    std::cout<<str<<"       read"<<std::endl<<std::endl;


    memset(str, 0, sizeof(str));



    oRet = ReadFile(hSerial, str, dwSize, &dwBytesRead, NULL);
    // std::cout<<oRet<<std::endl;
    std::cout<<str<<"       read"<<std::endl<<std::endl;


    memset(str, 0, sizeof(str));

    oRet = ReadFile(hSerial, str, dwSize, &dwBytesRead, NULL);
    // std::cout<<oRet<<std::endl;
    std::cout<<str<<"       read"<<std::endl<<std::endl;


    memset(str, 0, sizeof(str));

    }
    return 0;
}

void CollectData::main_process()
{
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

  LPCTSTR sPortName = _T("COM5");
hSerial = ::CreateFile(sPortName, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    // hComm = CreateFileA("\\\\.\\COM3",
    // 		    GENERIC_READ | GENERIC_WRITE,
    // 		    0,
    // 		    NULL,
    // 		    OPEN_EXISTING,
    // 		    0,
    // 		    NULL);
    // if (hComm == INVALID_HANDLE_VALUE){
    // 	printf("Error in opening serial port");
    // 	std::cout << std::endl;
    // }
    // else{
    // 	printf("Opening serial port successful");
    // 	std::cout << std::endl;
    // }

//watching files

unsigned __int64 TotalNumberOfBytes;
unsigned __int64 TotalNumberOfFreeBytes;
ULARGE_INTEGER free;


char *path = "C:\\Users\\User\\Desktop\\project\\over2";
  std::cout << "watching for changes... " << path << std::endl;

  HANDLE file = CreateFileA(path,
    FILE_LIST_DIRECTORY,
    FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
    NULL,
    OPEN_EXISTING,
    FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED,
    NULL);
  //assert(file != INVALID_HANDLE_VALUE);
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

                    std::cout << "wr:" << std::endl;
                    strr = std::to_string(TotalNumberOfBytes - TotalNumberOfFreeBytes - 16384) + " "
                            + std::to_string(TotalNumberOfBytes) + " "
                            + std::to_string(TotalNumberOfFreeBytes);
                    std::cout << send_to_stm(hSerial, strr) << std::endl;
                    emit valueChanged(QString::fromStdString(strr));

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
