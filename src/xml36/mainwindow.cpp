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
#include <QtXmlPatterns/QXmlSchemaValidator>
 #include <QtXmlPatterns/QXmlSchema>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->actionSave->setEnabled(false);
    edit_mode_is_on=false;
    connect(ui->xml_tree,SIGNAL(itemClicked(QTreeWidgetItem*,int)),
            this,SLOT(OnNodeQTreeWidgetPressed(QTreeWidgetItem*,int)));
    connect(ui->attributes_list,SIGNAL(itemClicked(QTreeWidgetItem*,int)),
            this, SLOT(OnAttributesQTreeWidgetPressed(QTreeWidgetItem*,int)));
    connect(ui->xml_tree,SIGNAL(itemExpanded(QTreeWidgetItem*)),
            this,SLOT(OnTreeItemExpanded(QTreeWidgetItem*)));
    connect(ui->xml_tree,SIGNAL(itemCollapsed(QTreeWidgetItem*)),
            this, SLOT(OnTreeItemCollapsed(QTreeWidgetItem*)));
}

MainWindow::~MainWindow()
{
    disconnect(ui->xml_tree,SIGNAL(itemExpanded(QTreeWidgetItem*)),
            this, SLOT(OnTreeItemExpanded(QTreeWidgetItem*)));
    disconnect(ui->xml_tree,SIGNAL(itemCollapsed(QTreeWidgetItem*)),
            this, SLOT(OnTreeItemCollapsed(QTreeWidgetItem*)));
    delete ui;
}

void MainWindow::ClearDocumentAndTrees() {
    xml_document.clear();
    ui->xml_tree->clear();
    ui->attributes_list->clear();
    ClearDocumentAndTreesPointers();
}

void MainWindow::ClearDocumentAndTreesPointers() {
    last_current_tree_item_clicked=0;
    current_working_file_name="";
    current_element_tree_item=0;
    last_current_tree_item_clicked=0;
}

void MainWindow::on_actionOpen_triggered() {
    if((ui->actionSave->isEnabled()) && (!ShowMessageBoxOfferingToSaveData())){ return;}
  QString file_name = QFileDialog::getOpenFileName(this,
                                                   tr("Выберите XML-файл"),
                                                   "..",
                                                   tr("XML-файлы (*.xml)"));

  if (0 == file_name.length())
    return;


  QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
  ClearDocumentAndTrees();
  ui->actionSave->setEnabled(false);
  bool opening_result = OpenXML(file_name);
  QApplication::restoreOverrideCursor();

  if (false == opening_result) {

    QMessageBox::critical(this, tr("Открытие файла"),
                          tr("Ошибка открытия файла %1").arg(file_name),
                          QMessageBox::Ok);
  } else current_working_file_name=file_name;
}

//QDomNode MainWindow::FindNecessaryDomNodeR(QTreeWidgetItem *tree_item_to_find){ // change with saving tree_item->data()
//    if(tree_item_to_find->parent()){
//        QDomNode xml_dom_node=this->FindNecessaryDomNodeR(tree_item_to_find->parent());
//        return xml_dom_node.childNodes().item(tree_item_to_find->parent()->indexOfChild(tree_item_to_find));
//    }
//    else {
//        return xml_document.documentElement();
//    }
//}

void MainWindow::OnTreeItemExpanded(QTreeWidgetItem *expanded_item) {
    //QDomNode xml_node= FindNecessaryDomNodeR(expanded_item);
    QDomNode xml_node=expanded_item->data(0 /*zero column*/, 32 /*user role*/).value<QDomNode>();
    QDomNodeList node_list=xml_node.childNodes();
    expanded_item->takeChild(0 /*first and single node*/);
    for (int iter=0; iter< node_list.count();iter++) {
        if (node_list.at(iter).isElement()) {
           QTreeWidgetItem *child_tree_item=new QTreeWidgetItem(expanded_item);
           child_tree_item->setText(0 /*zero column*/, node_list.at(iter).nodeName());
           QVariant variant;
           variant.setValue(node_list.at(iter));
           child_tree_item->setData(0 /*column*/, 32 /*user role*/, variant);
           if (node_list.at(iter).hasChildNodes()) {
               child_tree_item->addChild(new QTreeWidgetItem());
           }
        }
    }
}

void MainWindow::OnTreeItemCollapsed(QTreeWidgetItem *collapsed_item) {
    int child_count= collapsed_item->childCount();
    for (int iter=0; iter<child_count;iter++) {
        collapsed_item->removeChild(collapsed_item->child(0 ));
    }
    OnNodeQTreeWidgetPressed(collapsed_item, 0 /*zero column*/);
    collapsed_item->setSelected(true);
    collapsed_item->addChild(new QTreeWidgetItem());
}

void MainWindow::OnQTreeWidgetsChanged(QTreeWidgetItem *changed_item, int changed_column){
    ui->actionSave->setEnabled(true);
    //QDomNode xml_dom_node_to_edit=this->FindNecessaryDomNodeR(ui->xml_tree->currentItem());
    QDomNode xml_dom_node_to_edit=changed_item->data(0 /*column*/, 32 /*user role*/).value<QDomNode>();
    if (changed_item->treeWidget()==ui->xml_tree) {
        xml_dom_node_to_edit.toElement().setTagName(changed_item->text(changed_column));
    } else { // if changed_item in attribute_list
        QDomNamedNodeMap node_map=xml_dom_node_to_edit.attributes();
        if (0==changed_column) {
            QString changed_item_name=changed_item->text(changed_column);
            QString changed_item_value=changed_item->text(1 /* changed_item_value column*/);
            xml_dom_node_to_edit.toElement().removeAttributeNode(node_map.item(changed_item->treeWidget()->indexOfTopLevelItem(changed_item)).toAttr());
            xml_dom_node_to_edit.toElement().setAttribute(changed_item_name, changed_item_value);
        } else {
            node_map.item(changed_item->treeWidget()->indexOfTopLevelItem(changed_item)).toAttr().setValue(changed_item->text(changed_column));
        }
    }
}

void MainWindow::OnCurrentItemChanged(QTreeWidgetItem *current_item, QTreeWidgetItem *previous_item){
    if (0!=previous_item) {
        previous_item->treeWidget()->closePersistentEditor(previous_item,0 /*Zero column*/);
        if (previous_item->treeWidget()==ui->attributes_list) {
            previous_item->treeWidget()->closePersistentEditor(previous_item, 1 /*First column*/);
        }
    }
    if (0!=current_item) {
       current_item->treeWidget()->openPersistentEditor(current_item, 0 /*Zero column*/);
       if (current_item->treeWidget()==ui->attributes_list) {;
          current_item->treeWidget()->openPersistentEditor(current_item, 1 /*First column*/);
       }
    }
}

void MainWindow::OnAttributesQTreeWidgetPressed(QTreeWidgetItem *clicked_item, int /*column*/){
    last_current_tree_item_clicked=clicked_item;
}

void MainWindow::OnNodeQTreeWidgetPressed(QTreeWidgetItem *clicked_item, int /*column*/){
    current_element_tree_item=clicked_item;
    last_current_tree_item_clicked=clicked_item;
    if (edit_mode_is_on) {
        disconnect(ui->xml_tree,SIGNAL(itemChanged(QTreeWidgetItem*,int)),
                   this,SLOT(OnQTreeWidgetsChanged(QTreeWidgetItem*,int)));
        disconnect(ui->attributes_list,SIGNAL(itemChanged(QTreeWidgetItem*,int)),
                   this,SLOT(OnQTreeWidgetsChanged(QTreeWidgetItem*,int)));
    }
    //QDomNode xml_dom_node_to_display=this->FindNecessaryDomNodeR(clicked_item);
    QDomNode xml_dom_node_to_display=clicked_item->data(0 /*zero olumn*/, 32 /*user role*/).value<QDomNode>();
    if (!xml_dom_node_to_display.isNull()){
        ui->attributes_list->clear();
        if(xml_dom_node_to_display.isElement()){
            int attribute_tree_column_attribute=0;
            int attribute_tree_column_value=1;
            QDomNamedNodeMap node_map=xml_dom_node_to_display.attributes();
            for (int node_map_index=0; node_map_index<node_map.count(); node_map_index++) {
                QTreeWidgetItem *attribute_tree_item=new QTreeWidgetItem(ui->attributes_list);
                attribute_tree_item->setText(attribute_tree_column_attribute,node_map.item(node_map_index).toAttr().name());
                attribute_tree_item->setText(attribute_tree_column_value, node_map.item(node_map_index).toAttr().value());
                QVariant variant;
                variant.setValue(xml_dom_node_to_display);
                attribute_tree_item->setData(0 /*zero column*/, 32 /*user role*/, variant);
            }
        }
    }
    if (edit_mode_is_on) {
        connect(ui->xml_tree,SIGNAL(itemChanged(QTreeWidgetItem*,int)),
                this,SLOT(OnQTreeWidgetsChanged(QTreeWidgetItem*,int)));
        connect(ui->attributes_list,SIGNAL(itemChanged(QTreeWidgetItem*,int)),
                this,SLOT(OnQTreeWidgetsChanged(QTreeWidgetItem*,int)));
    }
}

Q_DECLARE_METATYPE(QDomNode)

void MainWindow::CreateTreeRootWithCurrentXMLDocument(){
    QDomNode xml_dom_node = (xml_document.firstChildElement());
    QTreeWidgetItem *node_tree_item=new QTreeWidgetItem(ui->xml_tree);
    node_tree_item->setText(0 /*zero column*/ , xml_dom_node.nodeName());
    QVariant variant;
    variant.setValue(xml_dom_node);
    node_tree_item->setData(0 /*zero column*/, 32 /*user role*/, variant);
    if (xml_dom_node.hasChildNodes()) {
        node_tree_item->addChild(new QTreeWidgetItem());
    }
}

bool MainWindow::OpenXML(const QString &fileName){
     QFile input_file(fileName);
     QUrl schemaUrl("file:///home/student/git/xml36/xml/structure.xsd");
          QXmlSchema schema;
          schema.load(schemaUrl);
          if (schema.isValid()) {
              input_file.open(QIODevice::ReadOnly);
              QXmlSchemaValidator validator(schema);
              if (!validator.validate(&input_file, QUrl::fromLocalFile(input_file.fileName()))) {
                  input_file.close();
                  return false;
              }
              input_file.close();
              input_file.open(QIODevice::ReadOnly);
              if (!xml_document.setContent(&input_file)) {
                  input_file.close();
                  return false;
              }
              input_file.close();
             if (!xml_document.isNull()) {
                 CreateTreeRootWithCurrentXMLDocument();
             }
          }
   return true;
}

bool MainWindow::ShowMessageBoxOfferingToSaveData(){  //return false in case of cancel clicked
    QMessageBox message_box;
     message_box.setText("The document has been modified.");
     message_box.setInformativeText("Do you want to save your changes?");
     message_box.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
     message_box.setDefaultButton(QMessageBox::Save);
     int ret = message_box.exec();
     switch(ret){
     case QMessageBox::Save:
         emit on_actionSave_triggered();
         return true;
         break;
     case QMessageBox::Discard:
         return true;
         break;
     case QMessageBox::Cancel:
         return false;
         break;
     }
     return false;
}

void MainWindow::on_actionNew_triggered()
{
    if(ui->actionSave->isEnabled()){
        if(!ShowMessageBoxOfferingToSaveData()) return;
    }
    ui->actionSave->setEnabled(false);
    ClearDocumentAndTrees();
    bool dialog_successful_execution=false;
    QString root_element_name=QInputDialog::getText(this,"Create new document","Enter root element name",QLineEdit::Normal, QDir::home().dirName(), &dialog_successful_execution);
    if ((dialog_successful_execution) && (!root_element_name.isEmpty())) {
        xml_document.appendChild(xml_document.createElement(root_element_name));
        CreateTreeRootWithCurrentXMLDocument();
        //QTreeWidgetItem *root_tree_element_item= new QTreeWidgetItem(ui->xml_tree);
        //root_tree_element_item->setText(0 /*first column*/, root_element_name);
    }
}

bool MainWindow::Valid() {
    QFile temp_file("xmlparsetempfile.xml");
    if (temp_file.open(QFile::WriteOnly | QFile::Truncate)) {
        QTextStream out(&temp_file);
        xml_document.save(out,5);
    }
    temp_file.close();
    QUrl schemaUrl("file:///home/student/git/xml36/xml/structure.xsd");
         QXmlSchema schema;
         schema.load(schemaUrl);
         if (schema.isValid()) {
             temp_file.open(QIODevice::ReadOnly);
             QXmlSchemaValidator validator(schema);
             if (!validator.validate(&temp_file, QUrl::fromLocalFile(temp_file.fileName()))) {
                 temp_file.close();
                 temp_file.deleteLater();
                 QMessageBox::critical(this, tr("saving document"),
                                       tr("Error: xml structure is invalid"),
                                       QMessageBox::Ok);
                 return false;
             } else {
                 temp_file.close();
                 temp_file.deleteLater();
                 return true;
             }
         }
     return false;
    /*QBuffer buffer(&(xml_document.toByteArray()));
    buffer.open(QIODevice::ReadOnly);
    QUrl schemaUrl("file:///home/student/git/xml36/xml/structure.xsd");
    QXmlSchema schema;
    schema.load(schemaUrl);
    if (schema.isValid()) {
         QXmlSchemaValidator validator(schema);
         if (validator.validate(&buffer)) {
             buffer.close();
             return true;
         }
         else {
             buffer.close();
             QMessageBox::critical(this, tr("saving document"),
                                   tr("Error: xml structure is invalid"),
                                   QMessageBox::Ok);
             return false;
         }
    }
    return false;*/
}

void MainWindow::on_actionSave_triggered()
{
    if (!Valid()) return;
    if(""==current_working_file_name) emit on_actionSaveAs_triggered();
    QFile working_file(current_working_file_name);
     if (working_file.open(QFile::WriteOnly | QFile::Truncate)) {
         QTextStream out(&working_file);
         xml_document.save(out,5);
     }
     working_file.close();
     ui->actionSave->setEnabled(false);
}

void MainWindow::on_actionSaveAs_triggered()
{
    QString file_name = QFileDialog::getSaveFileName(this,
                                                     tr("Выберите XML-файл"),
                                                     "..",
                                                     tr("XML-файлы (*.xml)"));
    if (""==file_name) return;
    current_working_file_name=file_name;
    emit on_actionSave_triggered();
}

void MainWindow::on_actionDelete_triggered()
{
    if (0!=last_current_tree_item_clicked) {
        ui->actionSave->setEnabled(true);
        if (last_current_tree_item_clicked->treeWidget()==ui->xml_tree) {
            if (last_current_tree_item_clicked==ui->xml_tree->topLevelItem(0)) {
                ClearDocumentAndTrees();
            } else {
                QDomNode node_to_delete=last_current_tree_item_clicked->data(0 /*zero column*/, 32 /*user role*/).value<QDomNode>(); //QDomNode node_to_delete= FindNecessaryDomNodeR(last_current_tree_item_clicked);
                node_to_delete.parentNode().removeChild(node_to_delete);
                ui->attributes_list->clear();
                last_current_tree_item_clicked->parent()->removeChild(last_current_tree_item_clicked);
                current_element_tree_item=ui->xml_tree->currentItem();
                last_current_tree_item_clicked=current_element_tree_item;
            }
        } else { //last_current_tree_item->treeWidger()==ui->attributes_list
            QDomNode node_with_attribute_to_delete=current_element_tree_item->data(0 /*zero column*/, 32 /*user role*/).value<QDomNode>();//QDomNode node_with_attribute_to_delete=FindNecessaryDomNodeR(current_element_tree_item);
            node_with_attribute_to_delete.toElement().removeAttribute(last_current_tree_item_clicked->text(0 /* column with name*/));
            last_current_tree_item_clicked->treeWidget()->takeTopLevelItem(last_current_tree_item_clicked->treeWidget()->indexOfTopLevelItem(last_current_tree_item_clicked));
            last_current_tree_item_clicked=ui->attributes_list->currentItem();
        }
    }
}

void MainWindow::on_actionEdit_toggled(bool edit_is_on)
{
    edit_mode_is_on=edit_is_on;
    if (edit_mode_is_on) {
        connect(ui->xml_tree,SIGNAL(itemChanged(QTreeWidgetItem*,int)),
                this,SLOT(OnQTreeWidgetsChanged(QTreeWidgetItem*,int)));
        connect(ui->attributes_list,SIGNAL(itemChanged(QTreeWidgetItem*,int)),
                this,SLOT(OnQTreeWidgetsChanged(QTreeWidgetItem*,int)));
        connect(ui->xml_tree,SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)),
                this,SLOT(OnCurrentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)));
        connect(ui->attributes_list,SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)),
                this,SLOT(OnCurrentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)));
        if (0 != ui->xml_tree->currentItem()) {
            ui->xml_tree->openPersistentEditor(ui->xml_tree->currentItem(),0 /* zero column*/);
        }
        if (0!= ui->attributes_list->currentItem()) {
            ui->attributes_list->openPersistentEditor(ui->attributes_list->currentItem(),0 /*zero column*/);
            ui->attributes_list->openPersistentEditor(ui->attributes_list->currentItem(),1 /*first column*/);
        }
    } else {
        last_current_tree_item_clicked->treeWidget()->setCurrentItem(last_current_tree_item_clicked,0);
        disconnect(ui->xml_tree,SIGNAL(itemChanged(QTreeWidgetItem*,int)),
                   this,SLOT(OnQTreeWidgetsChanged(QTreeWidgetItem*,int)));
        disconnect(ui->attributes_list,SIGNAL(itemChanged(QTreeWidgetItem*,int)),
                   this,SLOT(OnQTreeWidgetsChanged(QTreeWidgetItem*,int)));
        disconnect(ui->xml_tree,SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)),
                this,SLOT(OnCurrentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)));
        disconnect(ui->attributes_list,SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)),
                this,SLOT(OnCurrentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)));
        if (0 != ui->xml_tree->currentItem()) {
            ui->xml_tree->closePersistentEditor(ui->xml_tree->currentItem(),0 /* zero column*/);
        }
        if (0!= ui->attributes_list->currentItem()) {
            ui->attributes_list->closePersistentEditor(ui->attributes_list->currentItem(),0 /*zero column*/);
            ui->attributes_list->closePersistentEditor(ui->attributes_list->currentItem(),1 /*first column*/);
        }
    }
}

QTreeWidgetItem* MainWindow::CreateNewTreeWidgetElement(QDomNode new_xml_node){
    QTreeWidgetItem *new_tree_item=new QTreeWidgetItem();
    new_tree_item->setText(0,"NewElement");
    QVariant variant;
    variant.setValue(new_xml_node);
    new_tree_item->setData(0 /*zero column*/, 32 /*user role*/, variant);
    return new_tree_item;
}
QTreeWidgetItem* MainWindow::CreateNewTreeWidgetAttribute(QDomNode new_xml_node){
    QTreeWidgetItem *new_tree_item=new QTreeWidgetItem();
    new_tree_item->setText(0 /*column_attribute*/ , "NewName" );
    new_tree_item->setText(1 /*column_value*/ , "NewValue");
    QVariant variant;
    variant.setValue(new_xml_node);
    new_tree_item->setData(0 /*zero column*/, 32 /*user role*/, variant);
    return new_tree_item;
}

void MainWindow::on_actionAdd_triggered()
{
    if (xml_document.isNull()) {
        emit on_actionNew_triggered();
    } else {
        if (0!=current_element_tree_item) {
            ui->actionSave->setEnabled(true);
            QDomNode selected_node=current_element_tree_item->data(0 /*zero column*/, 32 /*user role*/).value<QDomNode>(); //QDomNode selected_node=FindNecessaryDomNodeR(current_element_tree_item);
            if (current_element_tree_item==ui->xml_tree->topLevelItem(0)) {
                QDomNode new_xml_node=xml_document.createElement("NewElement");
                selected_node.appendChild(new_xml_node);
                current_element_tree_item->addChild(CreateNewTreeWidgetElement(new_xml_node));
            } else {
                QMessageBox message_box;
                message_box.setText("Choose");
                message_box.setInformativeText("Choose action");
                QPushButton *add_attribute_button=message_box.addButton(tr("add attribute"), QMessageBox::ActionRole);
                QPushButton *add_node_above_button=message_box.addButton(tr("add node above"), QMessageBox::ActionRole);
                QPushButton *add_node_below_button=message_box.addButton(tr("add node below"), QMessageBox::ActionRole);
                QPushButton *add_node_inside_button=message_box.addButton(tr("add node inside"), QMessageBox::ActionRole);
                QPushButton *cancel_button=message_box.addButton(tr("cancel"), QMessageBox::ActionRole);
                message_box.exec();
                if (message_box.clickedButton()==add_attribute_button) {            //TODO Divide blocks to different functions
                    selected_node.toElement().setAttribute("NewName","NewValue");
                    ui->attributes_list->addTopLevelItem(CreateNewTreeWidgetAttribute(selected_node));
                    OnNodeQTreeWidgetPressed(current_element_tree_item, 0 /*column*/);           //TODO NEED TO BE REWRITE!!!
                } else if (message_box.clickedButton()==add_node_above_button) {
                    QDomNode new_node=xml_document.createElement("NewElement");
                    selected_node.parentNode().insertBefore(new_node,selected_node);
                    current_element_tree_item->parent()->insertChild(
                                current_element_tree_item->parent()->indexOfChild(current_element_tree_item), CreateNewTreeWidgetElement(new_node));
                } else if (message_box.clickedButton()==add_node_below_button) {
                    QDomNode new_node=xml_document.createElement("NewElement");
                    selected_node.parentNode().insertAfter(new_node,selected_node);
                    current_element_tree_item->parent()->insertChild(
                                current_element_tree_item->parent()->indexOfChild(current_element_tree_item)+1 , CreateNewTreeWidgetElement(new_node));
                } else if (message_box.clickedButton()==add_node_inside_button) {
                    QDomNode new_node=xml_document.createElement("NewElement");
                    selected_node.appendChild(new_node);
                    current_element_tree_item->addChild(CreateNewTreeWidgetElement(new_node));
                } else if (message_box.clickedButton()==cancel_button) {
                    return;
                }
            }
       }
    }
}


//#include <QtGui>
//#include "mainwindow.h"
//#include "ui_mainwindow.h"
//#include <QFile>
//#include <QtXml/QDomDocument>
//#include <QtXml/QDomNode>
//#include <QtXml/QDomElement>
//#include <QtXml/QDomAttr>
//#include <QtXml/QDomNodeList>
//#include <QDebug>
//#include <QTreeWidgetItem>
//#include <QString>


//MainWindow::MainWindow(QWidget *parent) :
//    QMainWindow(parent),
//    ui(new Ui::MainWindow)
//{
//    ui->setupUi(this);
//    ClearDocumentAndTreesPointers();
//    document_is_modified=false;
//    edit_mode_is_on=false;
//    connect(ui->xml_tree,SIGNAL(itemClicked(QTreeWidgetItem*,int)),
//            this,SLOT(OnNodeQTreeWidgetPressed(QTreeWidgetItem*,int)));
//    connect(ui->attributes_list,SIGNAL(itemClicked(QTreeWidgetItem*,int)),
//            this, SLOT(OnAttributesQTreeWidgetPressed(QTreeWidgetItem*,int)));
//}

//void MainWindow::ClearDocumentAndTrees() {
//    xml_document.clear();
//    ui->xml_tree->clear();
//    ui->attributes_list->clear();
//    ClearDocumentAndTreesPointers();
//}

//void MainWindow::ClearDocumentAndTreesPointers() {
//    last_current_tree_item_clicked=0;
//    current_working_file_name="";
//    current_element_tree_item=0;
//    last_current_tree_item_clicked=0;
//}

//MainWindow::~MainWindow()
//{
//    delete ui;
//}

//void MainWindow::on_actionOpen_triggered() {
//    if(document_is_modified){
//        if(!ShowMessageBoxOfferingToSaveData()) return;
//    }
//  QString file_name = QFileDialog::getOpenFileName(this,
//                                                   tr("Выберите XML-файл"),
//                                                   "..",
//                                                   tr("XML-файлы (*.xml)"));

//  if (0 == file_name.length())
//    return;

//  QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
//  ClearDocumentAndTrees();
//  document_is_modified=false;
//  bool opening_result = OpenXML(file_name);
//  QApplication::restoreOverrideCursor();

//  if (false == opening_result) {

//    QMessageBox::critical(this, tr("Открытие файла"),
//                          tr("Ошибка открытия файла %1").arg(file_name),
//                          QMessageBox::Ok);
//  } else current_working_file_name=file_name;
//}

//QDomNode MainWindow::FindNecessaryDomNodeR(QTreeWidgetItem *clicked_item){
//    if(clicked_item->parent()){
//        QDomNode xml_dom_node=this->FindNecessaryDomNodeR(clicked_item->parent());
//        return xml_dom_node.childNodes().item(clicked_item->parent()->indexOfChild(clicked_item));
//    }
//    else {
//        return xml_document.documentElement();
//    }
//}

//void MainWindow::OnQTreeWidgetsChanged(QTreeWidgetItem *changed_item, int changed_column){
//    document_is_modified=true;
//    QDomNode xml_dom_node_to_edit=this->FindNecessaryDomNodeR(ui->xml_tree->currentItem());
//    if (changed_item->treeWidget()==ui->xml_tree) {
//        xml_dom_node_to_edit.toElement().setTagName(changed_item->text(changed_column));
//    } else { // if changed_item in attribute_list
//        QDomNamedNodeMap node_map=xml_dom_node_to_edit.attributes();
//        if (0==changed_column) {
//            QString changed_item_name=changed_item->text(changed_column);
//            QString changed_item_value=changed_item->text(1 /* changed_item_value column*/);
//            xml_dom_node_to_edit.toElement().removeAttributeNode(node_map.item(changed_item->treeWidget()->indexOfTopLevelItem(changed_item)).toAttr());
//            xml_dom_node_to_edit.toElement().setAttribute(changed_item_name, changed_item_value);
//        } else {
//            node_map.item(changed_item->treeWidget()->indexOfTopLevelItem(changed_item)).toAttr().setValue(changed_item->text(changed_column));
//        }
//    }
//}

//void MainWindow::OnCurrentItemChanged(QTreeWidgetItem *current_item, QTreeWidgetItem *previous_item){
//    if (0!=previous_item) {
//        previous_item->treeWidget()->closePersistentEditor(previous_item,0 /*Zero column*/);
//        if (previous_item->treeWidget()==ui->attributes_list) {
//            previous_item->treeWidget()->closePersistentEditor(previous_item, 1 /*First column*/);
//        }
//    }
//    if (0!=current_item) {
//    current_item->treeWidget()->openPersistentEditor(current_item, 0 /*Zero column*/);
//    if (current_item->treeWidget()==ui->attributes_list) {;
//        current_item->treeWidget()->openPersistentEditor(current_item, 1 /*First column*/);
//    }}
//}

//void MainWindow::OnAttributesQTreeWidgetPressed(QTreeWidgetItem *clicked_item, int column){
//    last_current_tree_item_clicked=clicked_item;
//}

//void MainWindow::OnNodeQTreeWidgetPressed(QTreeWidgetItem *clicked_item, int column){
//    current_element_tree_item=clicked_item;
//    last_current_tree_item_clicked=clicked_item;
//    if (edit_mode_is_on) {
//        disconnect(ui->xml_tree,SIGNAL(itemChanged(QTreeWidgetItem*,int)),
//                   this,SLOT(OnQTreeWidgetsChanged(QTreeWidgetItem*,int)));
//        disconnect(ui->attributes_list,SIGNAL(itemChanged(QTreeWidgetItem*,int)),
//                   this,SLOT(OnQTreeWidgetsChanged(QTreeWidgetItem*,int)));
//    }
//    QDomNode xml_dom_node_to_display=this->FindNecessaryDomNodeR(clicked_item);
//    if (!xml_dom_node_to_display.isNull()){
//        ui->attributes_list->clear();
//        if(xml_dom_node_to_display.isElement()){
//            int attribute_tree_column_attribute=0;
//            int attribute_tree_column_value=1;
//            QDomNamedNodeMap node_map=xml_dom_node_to_display.attributes();
//            for (int node_map_index=0; node_map_index<node_map.count(); node_map_index++) {
//                QTreeWidgetItem *attribute_tree_item=new QTreeWidgetItem(ui->attributes_list);
//                attribute_tree_item->setText(attribute_tree_column_attribute,node_map.item(node_map_index).toAttr().name());
//                attribute_tree_item->setText(attribute_tree_column_value, node_map.item(node_map_index).toAttr().value());
//            }
//        }
//    }
//    if (edit_mode_is_on) {
//        connect(ui->xml_tree,SIGNAL(itemChanged(QTreeWidgetItem*,int)),
//                this,SLOT(OnQTreeWidgetsChanged(QTreeWidgetItem*,int)));
//        connect(ui->attributes_list,SIGNAL(itemChanged(QTreeWidgetItem*,int)),
//                this,SLOT(OnQTreeWidgetsChanged(QTreeWidgetItem*,int)));
//    }
//}

//void MainWindow::BuildXmlNodeTreeR(QDomNode xml_dom_node,QTreeWidgetItem *node_tree_item){
//    while(!xml_dom_node.isNull()){
//        if (xml_dom_node.isElement()){
//            int node_tree_column_node=0;
//            node_tree_item->setText(node_tree_column_node, xml_dom_node.toElement().tagName());
//            if (xml_dom_node.hasChildNodes()){
//                node_tree_item->addChild(new QTreeWidgetItem());
//                BuildXmlNodeTreeR(xml_dom_node.firstChild(),node_tree_item->child(node_tree_item->childCount()-1));
//            }
//            if (!xml_dom_node.nextSiblingElement().isNull()){
//                node_tree_item->parent()->addChild((new QTreeWidgetItem()));
//                node_tree_item=node_tree_item->parent()->child(node_tree_item->parent()->childCount()-1);
//            }
//        }
//        xml_dom_node=xml_dom_node.nextSiblingElement();
//    }
//}

//bool MainWindow::OpenXML(const QString &fileName){
//     QFile input_file(fileName);
//     if (!input_file.open(QIODevice::ReadOnly))
//         return false;
//     if (!xml_document.setContent(&input_file)) {
//         input_file.close();
//         return false;
//     }
//     input_file.close();
//    QDomNode xml_dom_node = xml_document.firstChild();
//    QTreeWidgetItem *node_tree_item=new QTreeWidgetItem(ui->xml_tree);
//    BuildXmlNodeTreeR(xml_dom_node, node_tree_item);
//   return true;
//}

//bool MainWindow::ShowMessageBoxOfferingToSaveData(){  //return false in case of cancel clicked
//    QMessageBox message_box;
//     message_box.setText("The document has been modified.");
//     message_box.setInformativeText("Do you want to save your changes?");
//     message_box.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
//     message_box.setDefaultButton(QMessageBox::Save);
//     int ret = message_box.exec();
//     switch(ret){
//     case QMessageBox::Save:
//         emit on_actionSave_triggered();
//         return true;
//         break;
//     case QMessageBox::Discard:
//         return true;
//         break;
//     case QMessageBox::Cancel:
//         return false;
//         break;
//     }
//}

//void MainWindow::on_actionNew_triggered()
//{
//    if(document_is_modified){
//        if(!ShowMessageBoxOfferingToSaveData()) return;
//    }
//    document_is_modified=false;
//    ClearDocumentAndTrees();
//    bool dialog_successful_execution=false;
//    QString root_element_name=QInputDialog::getText(this,"Create new document","Enter root element name",QLineEdit::Normal, QDir::home().dirName(), &dialog_successful_execution);
//    if ((dialog_successful_execution) && (!root_element_name.isEmpty())) {
//        xml_document.appendChild(xml_document.createElement(root_element_name));
//        QTreeWidgetItem *root_tree_element_item= new QTreeWidgetItem(ui->xml_tree);
//        root_tree_element_item->setText(0 /*first column*/, root_element_name);
//    }
//}

//void MainWindow::on_actionSave_triggered()
//{
//    if(""==current_working_file_name) emit on_actionSaveAs_triggered();
//    QFile working_file(current_working_file_name);
//     if (working_file.open(QFile::WriteOnly | QFile::Truncate)) {
//         QTextStream out(&working_file);
//         xml_document.save(out,5);
//     }
//     working_file.close();
//     document_is_modified=false;
//}

//void MainWindow::on_actionSaveAs_triggered()
//{
//    QString file_name = QFileDialog::getSaveFileName(this,
//                                                     tr("Выберите XML-файл"),
//                                                     "..",
//                                                     tr("XML-файлы (*.xml)"));
//    if (""==file_name) return;
//    current_working_file_name=file_name;
//    emit on_actionSave_triggered();
//}

//void MainWindow::on_actionDelete_triggered()
//{
//    if (0!=last_current_tree_item_clicked) {
//        document_is_modified=true;
//        if (last_current_tree_item_clicked->treeWidget()==ui->xml_tree) {
//            if (last_current_tree_item_clicked==ui->xml_tree->topLevelItem(0)) {
//                ClearDocumentAndTrees();
//            } else {
//                QDomNode node_to_delete= FindNecessaryDomNodeR(last_current_tree_item_clicked);
//                node_to_delete.parentNode().removeChild(node_to_delete);
//                ui->attributes_list->clear();
//                last_current_tree_item_clicked->parent()->removeChild(last_current_tree_item_clicked);
//                current_element_tree_item=ui->xml_tree->currentItem();
//                last_current_tree_item_clicked=current_element_tree_item;
//            }
//        } else { //last_current_tree_item->treeWidger()==ui.attributes_list
//            QDomNode node_with_attribute_to_delete=FindNecessaryDomNodeR(current_element_tree_item);
//            node_with_attribute_to_delete.toElement().removeAttribute(last_current_tree_item_clicked->text(0 /* column with name*/));
//            last_current_tree_item_clicked->treeWidget()->takeTopLevelItem(last_current_tree_item_clicked->treeWidget()->indexOfTopLevelItem(last_current_tree_item_clicked));
//            last_current_tree_item_clicked=ui->attributes_list->currentItem();
//        }
//    }
//}

//void MainWindow::on_actionEdit_toggled(bool edit_is_on)
//{
//    edit_mode_is_on=edit_is_on;
//    if (edit_mode_is_on) {
//        connect(ui->xml_tree,SIGNAL(itemChanged(QTreeWidgetItem*,int)),
//                this,SLOT(OnQTreeWidgetsChanged(QTreeWidgetItem*,int)));
//        connect(ui->attributes_list,SIGNAL(itemChanged(QTreeWidgetItem*,int)),
//                this,SLOT(OnQTreeWidgetsChanged(QTreeWidgetItem*,int)));
//        connect(ui->xml_tree,SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)),
//                this,SLOT(OnCurrentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)));
//        connect(ui->attributes_list,SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)),
//                this,SLOT(OnCurrentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)));
//        if (0 != ui->xml_tree->currentItem()) {
//            ui->xml_tree->openPersistentEditor(ui->xml_tree->currentItem(),0 /* zero column*/);
//        }
//        if (0!= ui->attributes_list->currentItem()) {
//            ui->attributes_list->openPersistentEditor(ui->attributes_list->currentItem(),0 /*zero column*/);
//            ui->attributes_list->openPersistentEditor(ui->attributes_list->currentItem(),1 /*first column*/);
//        }
//    } else {
//        disconnect(ui->xml_tree,SIGNAL(itemChanged(QTreeWidgetItem*,int)),
//                   this,SLOT(OnQTreeWidgetsChanged(QTreeWidgetItem*,int)));
//        disconnect(ui->attributes_list,SIGNAL(itemChanged(QTreeWidgetItem*,int)),
//                   this,SLOT(OnQTreeWidgetsChanged(QTreeWidgetItem*,int)));
//        disconnect(ui->xml_tree,SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)),
//                this,SLOT(OnCurrentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)));
//        disconnect(ui->attributes_list,SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)),
//                this,SLOT(OnCurrentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)));
//        if (0 != ui->xml_tree->currentItem()) {
//            ui->xml_tree->closePersistentEditor(ui->xml_tree->currentItem(),0 /* zero column*/);
//        }
//        if (0!= ui->attributes_list->currentItem()) {
//            ui->attributes_list->closePersistentEditor(ui->attributes_list->currentItem(),0 /*zero column*/);
//            ui->attributes_list->closePersistentEditor(ui->attributes_list->currentItem(),1 /*first column*/);
//        }
//    }
//}

//void MainWindow::on_actionAdd_triggered()
//{
//    if (xml_document.isNull()) {
//        emit on_actionNew_triggered();
//    } else {
//        if (0!=current_element_tree_item) {
//            document_is_modified=true;
//            QDomNode selected_node=FindNecessaryDomNodeR(current_element_tree_item);
//            if (current_element_tree_item==ui->xml_tree->topLevelItem(0)) {
//                selected_node.appendChild(xml_document.createElement("NewElement"));
//                QTreeWidgetItem *new_tree_item=new QTreeWidgetItem();
//                new_tree_item->setText(0,"NewElement");
//                current_element_tree_item->addChild(new_tree_item);
//            } else {
//                QMessageBox message_box;
//                message_box.setText("Choose");
//                message_box.setInformativeText("Choose action");
//                QPushButton *add_attribute_button=message_box.addButton(tr("add attribute"), QMessageBox::ActionRole);
//                QPushButton *add_node_above_button=message_box.addButton(tr("add node above"), QMessageBox::ActionRole);
//                QPushButton *add_node_below_button=message_box.addButton(tr("add node below"), QMessageBox::ActionRole);
//                QPushButton *add_node_inside_button=message_box.addButton(tr("add node inside"), QMessageBox::ActionRole);
//                QPushButton *cancel_button=message_box.addButton(tr("cancel"), QMessageBox::ActionRole);
//                message_box.exec();
//                if (message_box.clickedButton()==add_attribute_button) {            //TODO Divide blocks to different functions
//                    selected_node.toElement().setAttribute("NewName","NewValue");
//                    QTreeWidgetItem *new_tree_item=new QTreeWidgetItem();
//                    new_tree_item->setText(0 /*column_attribute*/ , "NewName" );
//                    new_tree_item->setText(1 /*column_value*/ , "NewValue");
//                    ui->attributes_list->addTopLevelItem(new_tree_item);
//                    OnNodeQTreeWidgetPressed(current_element_tree_item, 0 /*column*/);           //TODO NEED TO BE REWRITE!!!
//                } else if (message_box.clickedButton()==add_node_above_button) {
//                    QDomNode new_node=xml_document.createElement("NewElement");
//                    selected_node.parentNode().insertBefore(new_node,selected_node);
//                    QTreeWidgetItem *new_tree_item=new QTreeWidgetItem();
//                    new_tree_item->setText(0,"NewElement");
//                    current_element_tree_item->parent()->insertChild(current_element_tree_item->parent()->indexOfChild(current_element_tree_item), new_tree_item);
//                } else if (message_box.clickedButton()==add_node_below_button) {
//                    selected_node.parentNode().insertAfter(xml_document.createElement("NewElement"),selected_node);
//                    QTreeWidgetItem *new_tree_item=new QTreeWidgetItem();
//                    new_tree_item->setText(0,"NewElement");
//                    current_element_tree_item->parent()->insertChild(current_element_tree_item->parent()->indexOfChild(current_element_tree_item)+1 ,new_tree_item);
//                } else if (message_box.clickedButton()==add_node_inside_button) {
//                    selected_node.appendChild(xml_document.createElement("NewElement"));
//                    QTreeWidgetItem *new_tree_item=new QTreeWidgetItem();
//                    new_tree_item->setText(0,"NewElement");
//                    current_element_tree_item->addChild(new_tree_item);
//                } else if (message_box.clickedButton()==cancel_button) {
//                    return;
//                }
//            }
//       }
//    }
//}
