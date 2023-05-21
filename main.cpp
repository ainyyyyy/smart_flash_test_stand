#include "mainwindow.h"
#include <QApplication>
#include <iostream>
//#include <QtCore>
//#include <QtGui>
#include <QtWidgets>

using namespace std;





int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    /*QDialog *dialog = new QDialog;
        QLabel *label = new QLabel(dialog);
        label->setText("<font color=red>Hello, World!</font>");
        dialog->show();
        */
    MainWindow w;
    w.show();

/*
    while (true) {
            // Ваш код здесь
            // Например, обработка событий:
        static int counter = 0;
        counter++;
        QString qstr = QString::number(counter);

        w.updateLabelText(qstr);
        a.processEvents();
    }
*/
    return a.exec();

/*
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

              a.processEvents();
                if((event->Action != FILE_ACTION_ADDED) and (event->Action != FILE_ACTION_RENAMED_OLD_NAME)){
                    GetDiskFreeSpaceExA(path,&free,
                    (PULARGE_INTEGER)&TotalNumberOfBytes,
                    (PULARGE_INTEGER)&TotalNumberOfFreeBytes);

                    std::cout << "wr:" << std::endl;
                    strr = std::to_string(TotalNumberOfBytes - TotalNumberOfFreeBytes - 16384) + " "
                            + std::to_string(TotalNumberOfBytes) + " "
                            + std::to_string(TotalNumberOfFreeBytes);
                    std::cout << send_to_stm2(hSerial, strr) << std::endl;
                }

                // Are there more events to handle?
               if (event->NextEntryOffset) {
                  *((uint8_t**)&event) += event->NextEntryOffset;
                } else {
                  break;
                }


               w.updateLabelText(QString::fromStdString(strr));
          }
/*
                std::cout << "TotalNumberOfBytes:" << std::endl;
                strr = std::to_string(TotalNumberOfBytes);
                std::cout << send_to_stm2(hSerial, strr) << std::endl;


                    std::cout << "TotalNumberOfFreeBytes:" << std::endl;
                    strr = std::to_string(TotalNumberOfFreeBytes);
                    std::cout << send_to_stm2(hSerial, strr) << std::endl;
*/



/*
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
*/
}
