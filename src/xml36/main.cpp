#include <QtGui>
#include "mainwindow.h"

int main(int argc, char *argv[]) {
  QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));

  QApplication a(argc, argv);
  a.setStyle("windows");
  MainWindow w;
  w.show();

  return a.exec();
}
