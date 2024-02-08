#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    // Creating application and setting window icon
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/img/resources/Img/Icon.ico"));

    // Creating main window
    MainWindow w;
    w.setWindowTitle("Image Overlay");
    w.setStyleSheet("background-color: rgb(166, 144, 176);");
    w.show();

    // Starting the process
    return a.exec();
}
