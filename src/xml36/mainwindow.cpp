#include <QtGui>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QDebug>
#include <QTextStream>
#include <QStringList>

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

QString Complex_tag(QString& str, bool IsShitItem)
{
    QString res;
    int k=0;
    str.remove(0,str.indexOf("="));
    if (str.contains("="))
    {
        for (int i=0; i<str.size(); i++)
        {
            if (str[i] == '\'') k++;
            if (k == 1)
                res+=str[i];
            if (k == 2)
            {
                k = 0;
                res+=" ";
            }

        }
    }
    res.remove("'");
    if (IsShitItem)
    res.remove("/");
    return res;
}

bool MainWindow::OpenXML(const QString &fileName) {
  ui->xml_tree->addTopLevelItem(new QTreeWidgetItem(QStringList(fileName)));
  QFile file(fileName);
  if (!file.open(QIODevice::ReadOnly))
      qDebug()<<"Failed!"<<endl;
  QTextStream stream(&file);
  QString line = stream.readAll();
  QTreeWidgetItem* root;
  QTreeWidgetItem* current;
  QString att, val;
  line.remove(line.indexOf("<?"),line.indexOf("?>")+2); //удаляем cofig
  QString s(line);
  int i=0;
  int level;
  while (line.size() != 0)
  {
      QString str = line.mid(line.indexOf("<"),line.indexOf(">"));
      line.remove(str); //удаляем тег из строки line
      bool IsShitItem = str.contains("/>"); //true, если строка является листком
      str.remove("<").remove(">");   
      if (str.contains("="))
      {
          QString buff(str);
          att = str.mid(0,str.indexOf("="));   //выводим атрибут
          val = Complex_tag(buff, IsShitItem);   //выводим значение
          buff = att.mid(0,att.indexOf(" "));
          buff = "</"+buff+">";
          if (!IsShitItem)
             level = s.count(buff);
          else
              level = s.count("/>");
          line.remove(buff);
          QTreeWidgetItem* newItem;
          if (i < level)
          {
              i++;
              newItem = new QTreeWidgetItem (current);
              newItem->setText(0,att);
              newItem->setText(1,val);
          }
          if (i == level)
          {
              i=0;
              current = newItem;
          }
       }
      else
      {
          root = new QTreeWidgetItem(ui->attibutes_list);
          current = root;
          root->setText(0,str);
          line.remove("</"+str+">");   //удаляем корень
      }
  }
  return true;
}
