#include "mainwindow.h"
//#include "rgbcode.h"
#include <QApplication>

//static RGB rgb;
int main(int argc, char *argv[])
{
   //rgb.SetAll(COLORS::BLUE);
   // rgb.Render();
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();

}
