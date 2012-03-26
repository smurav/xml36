#include <QtGui>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QtXml/QDomDocument>
#include <QtXml/QDomNode>
#include <QtXml/QDomElement>
#include <QtXml/QDomAttr>
#include <QtXml/QDomNodeList>
#include <QDebug>
#include <QTreeWidgetItem>
#include <QString>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->xml_tree,SIGNAL(itemClicked(QTreeWidgetItem*,int)),
            this,SLOT(OnNodeQTreeWidgetPressed(QTreeWidgetItem*,int)));
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

QDomNode MainWindow::FindNecessaryDomNodeR(QTreeWidgetItem *clicked_item){
    if(clicked_item->parent()){
        QDomNode xml_dom_node=this->FindNecessaryDomNodeR(clicked_item->parent());
        return xml_dom_node.childNodes().item(clicked_item->parent()->indexOfChild(clicked_item));
    }
    else {
        return xml_document.firstChild().nextSibling();
    }
}

void MainWindow::OnNodeQTreeWidgetPressed(QTreeWidgetItem *clicked_item, int column){
    QDomNode xml_dom_node_to_display=this->FindNecessaryDomNodeR(clicked_item);
    if (!xml_dom_node_to_display.isNull()){
        ui->attributes_list->clear();
        if(xml_dom_node_to_display.isElement()){
            int attribute_tree_column_attribute=0;
            int attribute_tree_column_value=1;
            const int xml_attributes_count=2;
            const QString xml_attributes[xml_attributes_count]={"name","number"};
            for(int i=0;i<xml_attributes_count;i++){
                if(xml_dom_node_to_display.toElement().hasAttribute(xml_attributes[i])){
                    QTreeWidgetItem *attribute_tree_item=new QTreeWidgetItem(ui->attributes_list);
                    attribute_tree_item->setText(attribute_tree_column_attribute,xml_attributes[i]);
                    attribute_tree_item->setText(attribute_tree_column_value, xml_dom_node_to_display.toElement().attributeNode(xml_attributes[i]).value());
                }
            }
        }
    }
}

void MainWindow::BuildXmlNodeTreeR(QDomNode xml_dom_node,QTreeWidgetItem *node_tree_item){
    while(!xml_dom_node.isNull()){
        if (xml_dom_node.isElement()){
            int node_tree_column_node=0;
            node_tree_item->setText(node_tree_column_node, xml_dom_node.toElement().tagName());
            if (xml_dom_node.hasChildNodes()){
                node_tree_item->addChild(new QTreeWidgetItem());
                BuildXmlNodeTreeR(xml_dom_node.firstChild(),node_tree_item->child(node_tree_item->childCount()-1));
            }
            if (!xml_dom_node.nextSibling().isNull()){
                node_tree_item->parent()->addChild((new QTreeWidgetItem()));
                node_tree_item=node_tree_item->parent()->child(node_tree_item->parent()->childCount()-1);
            }
        }
        xml_dom_node=xml_dom_node.nextSibling();
    }
}

bool MainWindow::OpenXML(const QString &fileName){
     QFile input_file(fileName);
     if (!input_file.open(QIODevice::ReadOnly))
         return false;
     if (!xml_document.setContent(&input_file)) {
         input_file.close();
         return false;
     }
     input_file.close();
    QDomNode xml_dom_node = xml_document.firstChild();
    QTreeWidgetItem *node_tree_item=new QTreeWidgetItem(ui->xml_tree);
    BuildXmlNodeTreeR(xml_dom_node, node_tree_item);
   return true;
}


// MY PARSER

//QTreeWidgetItem* attribute;
//void parse(QTreeWidgetItem* it,QString& s,bool& down){
//    down=true;
//    QString subS=">";
//    QString node="";
//    s.remove(0,s.indexOf("<")+1);
//    s.trimmed();
//    if (s[0]!='/'){
//        int min=0;
//        if(s.indexOf(" ")>s.indexOf(">")) min=s.indexOf(">");
//        else min=s.indexOf(" ");
//        it->setText(0,s.mid(0,min));
//        s.remove(0,min);
//        s=" "+s;
//        if (s[s.indexOf(">")-1]=='/') subS="/";
//        while ((s.indexOf(subS)> s.indexOf("=")) && (s.indexOf("=")!=-1)){
//            attribute->addChild(new QTreeWidgetItem());
//            s.trimmed();
//            attribute->child(attribute->childCount()-1)->setText(0,s.mid(0,s.indexOf("=")));
//            s.remove(0,s.indexOf("=")+1);
//            s.trimmed();
//            QString div=" ";
//            if (s[0]=='\'') {div='\''; s.remove(0,1);}
//            if (s[0]=='\"') {div='\"'; s.remove(0,1);}
//            if(s.indexOf(div)>s.indexOf(subS)) min=s.indexOf(subS);
//            else min=s.indexOf(div);
//            attribute->child(attribute->childCount()-1)->setText(1,(div+s.mid(0,min+1)).trimmed());
//            s.remove(0,min+1);
//        }
//        s.remove(0,s.indexOf(">")+1);
//        if(subS=="/") return;
//    }  else {
//        s.remove(0,s.indexOf(">")+1);
//        it->parent()->takeChild(it->parent()->childCount()-1);
//        down=false;
//        return;
//    }
//    while(s.length()>2){
//        if(down){
//            it->addChild(new QTreeWidgetItem());
//            parse(it->child(it->childCount()-1),s,down);
//        }
//        else {
//            it->parent()->addChild(new QTreeWidgetItem());
//            parse(it->parent()->child(it->parent()->childCount()-1),s,down);
//        }
//    }
//    return;
//}

//MainWindow::OpenXML
//    attribute=new QTreeWidgetItem(ui->attibutes_list);
//    QString s;
//    QFile inputFile(fileName);
//    inputFile.open(QIODevice::ReadOnly);
//    QTextStream in(&inputFile);
//        s=in.readAll();
//        s.remove(0,s.indexOf(">")+1);
//        QTreeWidgetItem* item=new QTreeWidgetItem(ui->xml_tree);
//        bool down=true;
//        parse(item,s,down);
