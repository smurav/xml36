#include <QtGui>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QDebug>
#include <QTextStream>
#include <QTreeWidgetItem>

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
  QString att, val;
  line.remove(line.indexOf("<?"),line.indexOf("?>")+2);
      qDebug()<<line<<endl;
   while (!line.isEmpty())
    {
      QString str = line.mid(line.indexOf("<"),line.indexOf(">"));
      line.remove(str,Qt::CaseInsensitive);
      bool list= str.contains("/>",Qt::CaseInsensitive); //true, если дошли до листка
      str.remove("<",Qt::CaseInsensitive).remove(">",Qt::CaseInsensitive);
      qDebug()<<"str "<<str<<" "<<list<<endl;
      str.remove("<",Qt::CaseInsensitive).remove(">",Qt::CaseInsensitive); //очищаем строку от ">" и "<"
        if ((str.indexOf("=")<str.size()) && (str.indexOf("=")>0))
          {
            QString buff = str;
            qDebug()<<"buff "<<buff<<endl;
            buff.simplified();
            att = str.remove(str.indexOf("="),str.size()).simplified(); //атрибут - строка от начала до "="
            qDebug()<<"att"<<att<<endl;
            ui->attibutes_list->addTopLevelItem(new QTreeWidgetItem(QStringList(att))); //выводим значение атрибута
            val = buff.remove(att, Qt::CaseInsensitive).simplified();
            val.remove("='").remove("'").simplified(); //очищаем значение атрибута от лишнего
            qDebug()<<"val "<<val<<endl;
            str = att.remove(att.indexOf(" "),att.size());
            qDebug()<<"deleting element"<<str<<endl;
            str.simplified();
            if (!list)
                line.remove(" </"+str+"> ",Qt::CaseInsensitive);
            qDebug()<<"line after remove //"<<line<<endl;
          }
       else
         {
           ui->attibutes_list->addTopLevelItem(new QTreeWidgetItem(QStringList(str))); //случай корня
           line.remove("</"+str+">",Qt::CaseInsensitive);
         }
        qDebug()<<line<<endl;
    }
   return true;
}

