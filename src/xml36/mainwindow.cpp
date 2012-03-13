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
QTreeWidgetItem* attribute;




void parse(QTreeWidgetItem* it,QString& s,bool& down){
    down=true;
    qDebug()<<s;
    QString subS=">";
    QString node="";
    QStringList attributes;
    QStringList attributesValues;
    s.remove(0,s.indexOf("<")+1);
    s.trimmed();
    if (s[0]!='/'){
        int min=0;
        if(s.indexOf(" ")>s.indexOf(">")) min=s.indexOf(">");
        else min=s.indexOf(" ");
        node=s.mid(0,min);
        it->setText(0,node);
        s.remove(0,min);
        s=" "+s;
        if (s[s.indexOf(">")-1]=='/') subS="/";
        while ((s.indexOf(subS)> s.indexOf("=")) && (s.indexOf("=")!=-1)){
            attribute->addChild(new QTreeWidgetItem());
            s.trimmed();
            attributes.append(s.mid(0,s.indexOf("=")));
            attribute->child(attribute->childCount()-1)->setText(0,s.mid(0,s.indexOf("=")));
            s.remove(0,s.indexOf("=")+1);
            s.trimmed();
            QString div=" ";
            if (s[0]=='\'') {div='\''; s.remove(0,1);}
            if (s[0]=='\"') {div='\"'; s.remove(0,1);}
            if(s.indexOf(div)>s.indexOf(subS)) min=s.indexOf(subS);
            else min=s.indexOf(div);
            attributesValues.append((div+s.mid(0,min)).trimmed());
            attribute->child(attribute->childCount()-1)->setText(1,(div+s.mid(0,min+1)).trimmed());
            s.remove(0,min+1);
        }
        s.remove(0,s.indexOf(">")+1);
        if(subS=="/") return;

    }  else {
        s.remove(0,s.indexOf(">")+1);
        it->parent()->takeChild(it->parent()->childCount()-1);
        down=false;
        return;
    }
    while(s.length()>2){
        if(down){
            it->addChild(new QTreeWidgetItem());
            parse(it->child(it->childCount()-1),s,down);
        }
        else {
            it->parent()->addChild(new QTreeWidgetItem());
            parse(it->parent()->child(it->parent()->childCount()-1),s,down);
        }
    }
    return;
}

bool MainWindow::OpenXML(const QString &fileName)
{
    attribute=new QTreeWidgetItem(ui->attibutes_list);
    QString s;
    QFile inputFile(fileName);
    inputFile.open(QIODevice::ReadOnly);
    QTextStream in(&inputFile);
        s=in.readAll();
        s.remove(0,s.indexOf(">")+1);
        QTreeWidgetItem* item=new QTreeWidgetItem(ui->xml_tree);
        bool down=true;
        parse(item,s,down);

    ui->xml_tree->addTopLevelItem(new QTreeWidgetItem(QStringList(fileName)));
   return true;


}

