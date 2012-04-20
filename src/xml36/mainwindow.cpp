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
#include <QAbstractItemView>
#include "addnodedialog.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui_(new Ui::MainWindow) {
  ui_->setupUi(this);
  untitled_ = false;
  modified_ = false;
  xml_doc_ptr_ = 0;
  schema_valid_ctxt_ = 0;
}

MainWindow::~MainWindow() {
  FreeXMLDocument();
  FreeXMLSchema();
  delete ui_;
  ui_ = 0;
}

void MainWindow::closeEvent(QCloseEvent* pEv) {
  if (MaybeSave())
    pEv->accept();
  else
    pEv->ignore();
}

void MainWindow::on_actionNew_triggered() {
  if (!MaybeSave())
    return;

  static int sequence_number = 1;
  SetCurrentFileName(tr("doc%1.xml").arg(sequence_number++), true);
  SetModified(true);

  FreeXMLDocument();
  xml_doc_ptr_ = xmlNewDoc(BAD_CAST "1.0");
  UpdateButtons();
}

void MainWindow::on_actionOpen_triggered() {
  if (!MaybeSave())
    return;

  QString file_name = QFileDialog::getOpenFileName(this,
                                                   tr("Выберите XML-файл"),
                                                   "..",
                                                   tr("XML-файлы (*.xml)"));
  SetCurrentFileName(file_name, false);
  OpenXMLDocument(file_name);
}

bool MainWindow::on_actionSave_triggered() {
  if (untitled_)
    return on_actionSaveAs_triggered();
  else
    return SaveXMLDocument(file_name_);
}

bool MainWindow::on_actionSaveAs_triggered() {
  QString file_name = QFileDialog::getSaveFileName(this,
                                                   tr("Сохранить как"),
                                                   ".",
                                                   tr("XML-файлы (*.xml)"));
  if (0 == file_name.length())
    return false;

  return SaveXMLDocument(file_name);
}

bool MainWindow::OpenXMLDocument(const QString &file_name) {
  FreeXMLDocument();

  xmlParserCtxtPtr parser = xmlNewParserCtxt();
  if (0 == parser)
    return false;

  xml_doc_ptr_ = xmlCtxtReadFile(parser,
                                 file_name.toLocal8Bit().data(),
                                 NULL,
                                 XML_PARSE_NOBLANKS);// | XML_PARSE_DTDVALID);
  if (false == parser->valid) {
    QMessageBox::critical(this, tr("Открытие XML документа"),
                          tr("Структура файла %1 не соответствует "
                          "DTD-диаграмме").arg(file_name),
                          QMessageBox::Ok);
    SetCurrentFileName("", false);
    xmlFreeParserCtxt(parser);
    FreeXMLDocument();
    return false;
  }
  AddItem(xmlDocGetRootElement(xml_doc_ptr_));
  xmlFreeParserCtxt(parser);
  UpdateButtons();
  return true;
}

bool MainWindow::SaveXMLDocument(const QString &file_name) {
  if (0 == xml_doc_ptr_)
    return false;

  if (-1 == xmlSaveFormatFileEnc(file_name.toLocal8Bit(),
                                 xml_doc_ptr_,
                                 "UTF-8",
                                 1)) {
    QMessageBox::critical(this, tr("Сохранение XML документа"),
                          tr("Ошибка сохранения файла %1").arg(file_name),
                          QMessageBox::Ok);
    return false;
  }

  SetCurrentFileName(file_name, false);
  SetModified(false);
  return true;
}

void MainWindow::SetCurrentFileName(const QString &file_name, bool untitled) {
  file_name_ = file_name;
  untitled_ = untitled;
  UpdateWindowTitle();
}

void MainWindow::SetModified(bool modified) {
  modified_ = modified;
  ui_->actionSave->setEnabled(modified);
  ui_->actionSaveAs->setEnabled(modified);
  UpdateWindowTitle();
}

void MainWindow::UpdateWindowTitle() {
  QString title = tr("xml36 - %1").arg(QFileInfo(file_name_).fileName());
  if (modified_)
    title.append("*");
  setWindowTitle(title);
}

bool MainWindow::MaybeSave() {
  if (modified_) {
    QMessageBox::StandardButton result = QMessageBox::warning(this,
      tr("xml36"),
      tr("Документ '%1' был изменен.\n"
         "Хотите ли вы его сохранить?").arg(QFileInfo(file_name_).fileName()),
      QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

    if (QMessageBox::Save == result)
      return on_actionSave_triggered();

    if (QMessageBox::Cancel == result)
       return false;
  }

  SetModified(false);
  return true;
}

void MainWindow::FreeXMLSchema() {
  if (schema_valid_ctxt_) {
    xmlSchemaFreeValidCtxt(schema_valid_ctxt_);
    schema_valid_ctxt_ = 0;
  }
}

void MainWindow::FreeXMLDocument() {
  if (xml_doc_ptr_) {
    xmlFreeDoc(xml_doc_ptr_);
    xml_doc_ptr_ = 0;
  }

  ui_->xml_tree->clear();
  ui_->attributes_list->clear();

  ui_->actionAddNode->setEnabled(false);
  ui_->actionRemoveNode->setEnabled(false);
  ui_->actionAddAttribute->setEnabled(false);
  ui_->actionRemoveAttribute->setEnabled(false);
  ui_->actionEdit->setEnabled(false);
}

void MainWindow::UpdateButtons() {
  ui_->actionAddNode->setEnabled(0 != xml_doc_ptr_);

  bool node_selected = (0 != ui_->xml_tree->currentItem());
  ui_->actionRemoveNode->setEnabled(node_selected);
  ui_->actionAddAttribute->setEnabled(node_selected);

  bool attribute_selected = (0 != ui_->attributes_list->currentItem());
  ui_->actionRemoveAttribute->setEnabled(attribute_selected);
  ui_->actionEdit->setEnabled(node_selected || attribute_selected);
}

void MainWindow::on_xml_tree_itemExpanded(QTreeWidgetItem *item) {
  item->setIcon(0, QIcon(":/node_opened.png"));
  for (int i = 0; i < item->childCount(); i++) {
    item->removeChild(item->child(i));
  }
  xmlNodePtr current_node = GetNode(item);
  if (0 == current_node)
    return;
  for (xmlNodePtr child_node = current_node->children;
       0 != child_node;
       child_node = child_node->next) {
    AddItem(child_node, item);
  }
}

void MainWindow::on_xml_tree_itemCollapsed(QTreeWidgetItem *item) {
  item->setIcon(0, QIcon(":/node_closed.png"));
  for (int i = 1; i < item->childCount(); i++) {
    item->removeChild(item->child(i));
  }
}

xmlNodePtr  MainWindow::GetNode(QTreeWidgetItem *item) {
  if (0 == item)
    return 0;

  qlonglong data = item->data(0, Qt::UserRole).toLongLong();
  return (xmlNodePtr)((void*)data);
}

bool MainWindow::AddItem(xmlNodePtr node, QTreeWidgetItem *parent_item) {
  if (0 == node)
    return false;

  QTreeWidgetItem *new_item = new QTreeWidgetItem();
  new_item->setText(0, QString::fromUtf8((char *)node->name));
  new_item->setIcon(0, QIcon(":/node_closed.png"));
  new_item->setData(0, Qt::UserRole, (qlonglong)node);

  if (node->children) { // Есть дочерние элементы
    new_item->addChild(new QTreeWidgetItem());
  }

  if (parent_item) {
    parent_item->addChild(new_item);
  } else {
    ui_->xml_tree->addTopLevelItem(new_item);
  }
  //ui_->xml_tree->setCurrentItem(new_item, 0);
  return true;
}

void MainWindow::on_actionAddNode_triggered() {
  if (0 == xml_doc_ptr_)
    return;

  AddNodeDialog dialog;
  if (QDialog::Accepted != dialog.exec())
    return;

  QString node_name = dialog.GetNodeName();
  xmlNodePtr new_node = xmlNewNode(0, BAD_CAST node_name.toUtf8().data());
  if (0 == new_node) {
    QMessageBox::critical(this, tr("Создание узла XML"),
                          tr("Ошибка создания узла %1").arg(node_name),
                          QMessageBox::Ok);
    return;
  }

  QTreeWidgetItem *current_item = 0;
  xmlNodePtr root_node = xmlDocGetRootElement(xml_doc_ptr_);
  if (0 == root_node) { // Пустой документ
    xmlDocSetRootElement(xml_doc_ptr_, new_node);
  } else {
    current_item = ui_->xml_tree->currentItem();
    xmlNodePtr parent_node = current_item ? GetNode(current_item): root_node;
    if (0 == xmlAddChild(parent_node, new_node)) {
      QMessageBox::critical(this, tr("Создание узла XML"),
                            tr("Ошибка создания дочернего узла %1")
                              .arg(node_name),
                            QMessageBox::Ok);
      return;
    }
  }

  AddItem(new_node, current_item);
  SetModified(true);
}

void MainWindow::on_xml_tree_currentItemChanged(QTreeWidgetItem *current,
                                                QTreeWidgetItem */*previous*/) {
  ui_->attributes_list->clear();

  xmlNodePtr current_node = GetNode(current);
  if ((0 == current_node) || (0 == current_node->properties))
    return;
  for (xmlAttrPtr attribute = current_node->properties;
       0 != attribute;
       attribute = attribute->next) {
    QTreeWidgetItem *new_item = new QTreeWidgetItem();
    new_item->setText(0, QString::fromUtf8((char *)attribute->name));
    QString value = QString::fromUtf8((char *)xmlGetProp(current_node,
                                                         attribute->name));
    new_item->setText(1, value);
    ui_->attributes_list->addTopLevelItem(new_item);
  }
  UpdateButtons();
}

void MainWindow::on_attributes_list_currentItemChanged(QTreeWidgetItem */*current*/,
                                                       QTreeWidgetItem */*previous*/) {
  UpdateButtons();
}

void MainWindow::on_actionLoadSchema_triggered() {
  FreeXMLSchema();
  ui_->actionCheckSchema->setEnabled(false);
  QString file_name = QFileDialog::getOpenFileName(this,
                                                   tr("Выберите XML-схему"),
                                                   "..",
                                                   tr("XSD-файлы (*.xsd)"));
  //file_name.push_front("file://");
  xmlSchemaParserCtxtPtr schema_parser = xmlSchemaNewParserCtxt(file_name.toUtf8().data());
  if (0 == schema_parser)
    return;

  xmlSchemaPtr schema = xmlSchemaParse(schema_parser);
  if (0 == schema) {
    xmlSchemaFreeParserCtxt(schema_parser);
    schema_parser = 0;
    return;
  }

  schema_valid_ctxt_ = xmlSchemaNewValidCtxt(schema);
  if (0 == schema_valid_ctxt_) {
    xmlSchemaFreeParserCtxt(schema_parser);
    schema_parser = 0;
    xmlSchemaFree(schema);
    schema = 0;
    return;
  }

  ui_->actionCheckSchema->setEnabled(true);
}

void MainWindow::on_actionCheckSchema_triggered() {
  if ((0 == schema_valid_ctxt_) || (0 == xml_doc_ptr_))
    return;

  if (0 == xmlSchemaValidateDoc(schema_valid_ctxt_, xml_doc_ptr_)) {
    QMessageBox::information(this, tr("Проверка схемы XML"),
                             tr("Структура документа корректна"),
                             QMessageBox::Ok);
  } else {
    QMessageBox::critical(this, tr("Проверка схемы XML"),
                             tr("Структура документа не соответствует схеме"),
                             QMessageBox::Ok);
  }
}
