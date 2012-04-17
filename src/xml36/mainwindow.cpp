#include <QtGui>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
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
}

MainWindow::~MainWindow() {
  FreeXMLDocument();
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
  ui_->actionAddNode->setEnabled(0 != xml_doc_ptr_);
}

void MainWindow::on_actionOpen_triggered() {
  if (!MaybeSave())
    return;

  QString file_name_ = QFileDialog::getOpenFileName(this,
                                                    tr("Выберите XML-файл"),
                                                    "..",
                                                    tr("XML-файлы (*.xml)"));
  OpenXMLDocument(file_name_);
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

  return SaveXMLDocument(file_name);
}

bool MainWindow::OpenXMLDocument(const QString &file_name) {
  FreeXMLDocument();
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

void MainWindow::on_xml_tree_itemExpanded(QTreeWidgetItem *item)
{
    item->setIcon(0, QIcon(":/node_opened.png"));
}

void MainWindow::on_xml_tree_itemCollapsed(QTreeWidgetItem *item)
{
    item->setIcon(0, QIcon(":/node_closed.png"));
}

xmlNodePtr  MainWindow::GetNode(QTreeWidgetItem *item) {
  if (0 == item)
    return 0;

  return (xmlNodePtr)((void*)item->data(0, Qt::UserRole).toLongLong());
}

bool MainWindow::SetNode(QTreeWidgetItem *item, xmlNodePtr node) {
  if (0 == item)
    return false;

  item->setData(0, Qt::UserRole, (qint64)node);
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

  xmlNodePtr root_node = xmlDocGetRootElement(xml_doc_ptr_);
  if (0 == root_node) { // Пустой документ
    xmlDocSetRootElement(xml_doc_ptr_, new_node);
  } else {
    QTreeWidgetItem *current_item = ui_->xml_tree->currentItem();
    xmlNodePtr parent_node = current_item ? GetNode(current_item): root_node;
    if (0 == xmlAddChild(parent_node, new_node)) {
      QMessageBox::critical(this, tr("Создание узла XML"),
                            tr("Ошибка создания дочернего узла %1")
                              .arg(node_name),
                            QMessageBox::Ok);
      return;
    }
  }

  SetModified(true);
}
