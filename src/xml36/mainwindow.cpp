#include <QtGui>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QDebug>
#include <QTextStream>
#include <QTreeWidgetItem>
#include <QString>

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

bool MainWindow::OpenXML(const QString &fileName)
{
    //ui->xml_tree->addTopLevelItem(new QTreeWidgetItem(QStringList(fileName)));
    QFile file(fileName);
    file.open(QIODevice::ReadOnly);
    QTextStream in(&file);
    while (!in.atEnd())
    {
        QString s, s1;
        in >> s;
        int n1,n2,k=0;
        s1=s;
        if(s.length()!=0 && s.length()!=1 && s[0]=='<')
        {
            if(s[1]=='?');
            else
            {
                n1=s.indexOf('<');
                s.remove(0,n1);
                n2=s.indexOf(' ');
                if(n2==-1)
                    n2=s.indexOf('>');
                if(s1[n1+1]!='/')
                {
                    QTreeWidgetItem* in=new QTreeWidgetItem(QStringList(s1.mid(n1+1,n2-1)));
                    if(k==0)ui->xml_tree->addTopLevelItem(in);
                    else ui->xml_tree->insertTopLevelItem(2, in);
                    if(s1[s1.indexOf('>')-1]!='/')	k++;
                }
                else k--;
            }
        }
   }
   file.close();
   return true;
}

