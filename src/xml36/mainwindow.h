#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidgetItem>
#include <QString>
#include "libxml/xmlreader.h"
#include "libxml/xpath.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

 protected:
  void closeEvent(QCloseEvent *event);

 private slots:
  void on_actionNew_triggered();
  void on_actionOpen_triggered();
  bool on_actionSave_triggered();
  bool on_actionSaveAs_triggered();
  void on_xml_tree_itemExpanded(QTreeWidgetItem *item);
  void on_xml_tree_itemCollapsed(QTreeWidgetItem *item);

  void on_actionAddNode_triggered();

private:
  bool OpenXMLDocument(const QString &file_name);
  bool SaveXMLDocument(const QString &file_name);
  void SetCurrentFileName(const QString &file_name, bool untitled);
  void SetModified(bool modified);
  void UpdateWindowTitle();
  bool MaybeSave();
  void FreeXMLDocument();
  xmlNodePtr GetNode(QTreeWidgetItem *item);
  bool SetNode(QTreeWidgetItem *item, xmlNodePtr node);

 private:
  Ui::MainWindow *ui_;
  QString         file_name_;   // Текущее имя файла
  bool            untitled_;    // Имя файла явно не задано
  bool            modified_;    // Присутствуют несохраненные изменения
  xmlDocPtr       xml_doc_ptr_; // XML документ
};

#endif // MAINWINDOW_H
