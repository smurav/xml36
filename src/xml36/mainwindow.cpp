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
  xml_file_name = QFileDialog::getOpenFileName(this,
                                                   tr("Выберите XML-файл"),
                                                   "..",
                                                   tr("XML-файлы (*.xml)"));

  if (0 == xml_file_name.length())
    return;

  QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
  bool opening_result = OpenXML(xml_file_name);
  QApplication::restoreOverrideCursor();

  if (false == opening_result) {

    QMessageBox::critical(this, tr("Открытие файла"),
                          tr("Ошибка открытия файла %1").arg(xml_file_name),
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
            QDomNamedNodeMap xml_attributes=xml_dom_node_to_display.attributes();
            for(int i=0;i<xml_attributes.length();i++){
                QTreeWidgetItem *attribute_tree_item=new QTreeWidgetItem(ui->attributes_list);
                attribute_tree_item->setText(attribute_tree_column_attribute,xml_attributes.item(i).nodeName());
                attribute_tree_item->setText(attribute_tree_column_value, xml_attributes.item(i).nodeValue());
                attribute_tree_item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
            }
        }
    }
}

void MainWindow::BuildXmlNodeTreeR(QDomNode xml_dom_node,QTreeWidgetItem *node_tree_item){
    while(!xml_dom_node.isNull()){
        if (xml_dom_node.isElement()){
            int node_tree_column_node=0;
            node_tree_item->setText(node_tree_column_node, xml_dom_node.toElement().tagName());
            node_tree_item->setIcon(0, QIcon(":/node_closed.png"));
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
    if (node_tree_item) {
      node_tree_item->setIcon(0, QIcon(":/node_closed.png"));
      BuildXmlNodeTreeR(xml_dom_node, node_tree_item);
    }
   return true;
}


void MainWindow::on_xml_tree_itemExpanded(QTreeWidgetItem *item)
{
    item->setIcon(0, QIcon(":/node_opened.png"));
}

void MainWindow::on_xml_tree_itemCollapsed(QTreeWidgetItem *item)
{
    item->setIcon(0, QIcon(":/node_closed.png"));
}

void MainWindow::on_actionSaveAs_triggered()
{
    const int IndentSize = 4;
    QString file_name=QFileDialog::getSaveFileName(this, tr("Выберите файл"), "..", tr("XML-файлы (*.xml)"));
    QFile file(file_name);
    if(file.open(QIODevice::WriteOnly))
    {
        QTextStream out(&file);
        xml_document.save(out, IndentSize);
        xml_file_name=file_name;
    }
}

void MainWindow::on_actionSave_triggered()
{
    const int IndentSize = 4;
    QFile file(xml_file_name);
    if(file.open(QIODevice::WriteOnly))
    {
        QTextStream out(&file);
        xml_document.save(out, IndentSize);
    }
}

void MainWindow::on_actionAddAttribute_triggered()
{
    QDomNode xml_dom_node_to_edit=this->FindNecessaryDomNodeR(checked_item);
    xml_dom_node_to_edit.toElement().setAttribute("[NoData]", "[NoData]");

    QTreeWidgetItem* new_item = new QTreeWidgetItem;
    new_item->setText(0, xml_dom_node_to_edit.attributes().item(0).nodeName());
    new_item->setText(1, xml_dom_node_to_edit.attributes().item(0).nodeValue());


    ui->attributes_list->insertTopLevelItem(0, new_item);
}




void MainWindow::on_xml_tree_itemClicked(QTreeWidgetItem *item, int column)
{
    checked_item = item;
    if(to_edit) item->treeWidget()->openPersistentEditor(item, column);
}

void MainWindow::on_attributes_list_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    item->treeWidget()->openPersistentEditor(item, column);
}

void MainWindow::on_actionEdit_triggered()
{
    to_edit = true;
}

void MainWindow::on_actionRemoveAttribute_triggered()
{
    QDomNode xml_dom_node_to_edit=this->FindNecessaryDomNodeR(checked_item);
    xml_dom_node_to_edit.removeChild(xml_dom_node_to_edit.attributes().item(0));//ui->attributes_list->indexOfTopLevelItem(ui->attributes_list->currentItem())));
}

void MainWindow::on_attributes_list_itemClicked(QTreeWidgetItem *item, int column)
{
    if(to_edit) item->treeWidget()->openPersistentEditor(item, column);
}

void MainWindow::on_attributes_list_itemChanged(QTreeWidgetItem *changed_item, int changed_column)
{
    changed_item->treeWidget()->closePersistentEditor(changed_item, changed_column);
    to_edit = false;
    QDomNode xml_dom_node_to_edit=this->FindNecessaryDomNodeR(checked_item);
    QDomNamedNodeMap node_map=xml_dom_node_to_edit.toElement().attributes();
    if(1 == changed_column)
        node_map.item(changed_item->treeWidget()->indexOfTopLevelItem(changed_item)).setNodeValue(changed_item->text(1));
}

void MainWindow::on_actionAddNode_triggered()
{
    QTreeWidgetItem* new_item = new QTreeWidgetItem;
    new_item->setText(0, "new");
    new_item->setIcon(0, QIcon(":/node_closed.png"));
    checked_item->addChild(new_item);
}

void MainWindow::on_xml_tree_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    item->treeWidget()->openPersistentEditor(item, column);
}

void MainWindow::on_actionRemoveNode_triggered()
{
    if(checked_item != NULL)
        if(-1 == ui->xml_tree->indexOfTopLevelItem(checked_item)){
            QDomNode node_to_edit = this->FindNecessaryDomNodeR(checked_item);
            node_to_edit.parentNode().removeChild(node_to_edit);
            checked_item->parent()->removeChild(checked_item);
        }
        else {
            xml_document.clear();
            ui->xml_tree->clear();
        }
}

void MainWindow::on_xml_tree_itemChanged(QTreeWidgetItem *item, int column)
{
    item->treeWidget()->closePersistentEditor(item, column);
    to_edit = false;
}

void MainWindow::on_xml_tree_clicked(const QModelIndex &index)
{

}
