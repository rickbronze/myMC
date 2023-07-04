#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_Use96Dpi);
    QApplication a(argc, argv);
    QString scrollSS(QString::fromUtf8(
          "QScrollBar:vertical {"
          "    border: 1px solid #999999;"
          "    background:white;"
          "    width:40px;    "
          "    margin: 0px 0px 0px 0px;"
          "}"
         /*  HANDLE BAR VERTICAL */

         "QScrollBar::handle:vertical { "

         "        background-color: rgb(80, 80, 122);"
         "        min-height: 30px;"
         "        border-radius: 7px;"
         "}"
         "QScrollBar::handle:vertical:hover{"
         "        background-color: rgb(255, 0, 127);"
         "}"
         "QScrollBar::handle:vertical:pressed {"
         "        background-color: rgb(185, 0, 92);"
         "}"
          "QScrollBar::handle:vertical {"
          "    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
          "    stop: 0 rgb(32, 47, 130), stop: 0.5 rgb(32, 47, 130), stop:1 "
          "rgb(32, 47, 130));"
          "    min-height: 0px;"
          "}"
          "QScrollBar::add-line:vertical {"
          "    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
          "    stop: 0 rgb(32, 47, 130), stop: 0.5 rgb(32, 47, 130),  stop:1 "
          "rgb(32, 47, 130));"
          "    height: 0px;"
          "    subcontrol-position: bottom;"
          "    subcontrol-origin: margin;"
          "}"
          "QScrollBar::sub-line:vertical {"
          "    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
          "    stop: 0  rgb(32, 47, 130), stop: 0.5 rgb(32, 47, 130),  stop:1 "
          "rgb(32, 47, 130));"
          "    height: 0 px;"
          "    subcontrol-position: top;"
          "    subcontrol-origin: margin;"
          "}"
                         /*  HANDLE BAR Horizontal */
                         "QScrollBar:horizontal {"
                         "border-color: rgb(227, 227, 227);"
                         "border-width: 1px;"
                         "border-style: solid;"
                         "background-color: rgb(240, 240, 240);"
                         "width: 40px;"
                         "margin: 0px 21px 0 21px;"
                         "}"

                         "QScrollBar::handle:horizontal {"
                         "background-color: rgb(200, 200, 200);"
                         "min-height: 25px;"
                         "}"
                         ));

      a.setStyleSheet(scrollSS);

    a.setQuitOnLastWindowClosed(true);
    MainWindow w;
//w.setWindowState(Qt::WindowFullScreen);
    w.setWindowState(Qt::WindowMaximized);
    w.show();

    return a.exec();
}
