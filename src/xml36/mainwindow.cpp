#include <QtGui>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QDebug>
#include <QTreeWidgetItem>
#include <QString>
#include <QAbstractItemView>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->xml_tree, SIGNAL(itemClicked(QTreeWidgetItem*,int)),
            this,SLOT(OnNodeQTreeWidgetPressed(QTreeWidgetItem*,int)));
       ui->actionSave->setEnabled(false);
       ui->actionSaveAs->setEnabled(false);
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
  currentFilePath = file_name;
  bool opening_result = OpenXML(file_name);
  QApplication::restoreOverrideCursor();
  setCurrentFile(file_name);
  isDocumentModified = false;
  ui->actionSaveAs->setEnabled(true);

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
     if (!input_file.open(QFile::ReadOnly | QFile::Text))
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


void MainWindow::setCurrentFile(const QString &fileName) {
    currentFilePath = fileName;
    setWindowModified(false);

    QString shownName = currentFilePath;
    if (true == currentFilePath.isEmpty())
        shownName = tr("Безымянный.xml");
    setWindowTitle("XML36 " +shownName);
}


QString MainWindow::strippedName(const QString &fullName)
{
    return QFileInfo(fullName).fileName();
}


void MainWindow::on_xml_tree_itemExpanded(QTreeWidgetItem *item)
{
    item->setIcon(0, QIcon(":/node_opened.png"));
}


void MainWindow::on_xml_tree_itemCollapsed(QTreeWidgetItem *item)
{
    item->setIcon(0, QIcon(":/node_closed.png"));
}


void MainWindow::on_actionNew_triggered()
{
      if (true == isDocumentModified)
          MainWindow::maybeSave();
      else {
            xml_document.clear();
            ui->attributes_list->clear();
            ui->xml_tree->clear();
            setCurrentFile("");
    }
}


bool MainWindow::maybeSave() {
    QMessageBox::StandardButton alternative;
    alternative = QMessageBox::warning(this, tr("XML36"),
                 tr("Xml документ был изменен.\n"
                    "Хотите сохранить изменения?"),
                 QMessageBox::Save  | QMessageBox::Cancel);
    if (alternative == QMessageBox::Save)
        return saveFile(currentFilePath);
    else if (alternative == QMessageBox::Cancel);
    return false;
}


bool MainWindow::saveAs() {
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Сохранить файл"),
                                                    QDir::currentPath(),
                                                    tr("Xml-files (*.xml)"));
    if (true == fileName.isEmpty())
        return false;
    return saveFile(fileName);
}


bool MainWindow::saveFile(const QString &fileName) {
    isDocumentModified = false;
    QFile file(fileName);
    if (false == file.open(QFile::ReadWrite | QFile::Text)) {
        QMessageBox::warning(this, tr("XML36"),
                             tr("Невозможно сохранить файл %1\n.")
                             //.arg(fileName)
                             .arg(file.errorString()));
        return false;
    }

    QTextStream out(&file);

    QApplication::setOverrideCursor(Qt::WaitCursor);
    out << xml_document << endl;
    QApplication::restoreOverrideCursor();

    setCurrentFile(fileName);
    QMessageBox::information(this,tr("XML36"),tr("Файл был успешно сохранен").arg(fileName));
    return true;
}


bool MainWindow::on_actionSaveAs_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this);
    if (true == fileName.isEmpty())
        return false;

    return saveFile(fileName);
}


bool MainWindow::on_actionSave_triggered()
{
    if (true == currentFilePath.isEmpty())
        return saveAs();
    else
        saveFile(currentFilePath);
}


void MainWindow::on_attributes_list_doubleClicked(const QModelIndex &index)
{
    qDebug()<<"double clicked"<<endl;
    bool okKey;
    QString newItem = QInputDialog::getText(this,
                                            tr("Редактирование атрибута"),
                                            tr("Введите новое значение атрибута"),
                                            QLineEdit::Normal,
                                            tr("Новый атрибут"), &okKey);
    if (true == okKey) {
        QString oldItem = ui -> attributes_list -> currentItem() ->text(index.column());
        ui -> attributes_list -> currentItem() -> setText(index.column() ,newItem);
    }
        ui->actionSave->setEnabled(true);
}
