#include <QtGui>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QDebug>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOpen_triggered() {
  QString file_name = QFileDialog::getOpenFileName(this,
                                                   tr("Выберите XML-файл"),
                                                   "..",
                                                   tr("XML-файлы (*.xml)"));

  if (0 == file_name.length())
    return;

  QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
  bool opening_result = OpenXML(file_name);
  QApplication::restoreOverrideCursor();

  if (false == opening_result) {

    QMessageBox::critical(this, tr("Открытие файла"),
                          tr("Ошибка открытия файла %1").arg(file_name),
                          QMessageBox::Ok);
  }
}

bool MainWindow::OpenXML(const QString &fileName) {
  ui->xml_tree->addTopLevelItem(new QTreeWidgetItem(QStringList(fileName)));
  QFile file(fileName);
  if (!file.open(QIODevice::ReadOnly))
      qDebug()<<"Failed!"<<endl;
  QTextStream stream(&file);
  QString line = stream.readAll();
      qDebug()<<line<<endl;
  /*QString att, root, str;
  int i=0;
  while(i<line.length())
  {
      if (line[i]!="<")
          str+=line[i];
      else
      {
          i++;
          root+=line[i];
      }
      if (line[i]==">")

  } */

  return true;
}
