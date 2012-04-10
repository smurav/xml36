#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidgetItem>
#include <QDomDocument>
#include <QString>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

 private slots:
  void on_actionOpen_triggered();
  void OnNodeQTreeWidgetPressed( QTreeWidgetItem *clicked_item, int column );
  void on_xml_tree_itemExpanded(QTreeWidgetItem *item);
  void on_xml_tree_itemCollapsed(QTreeWidgetItem *item);
  void on_actionNew_triggered();
  bool on_actionSave_triggered();
  bool on_actionSaveAs_triggered();

  void on_attributes_list_doubleClicked(const QModelIndex &index);

private:
  bool OpenXML(const QString &fileName);
  QDomNode FindNecessaryDomNodeR(QTreeWidgetItem *clicked_item);
  QDomNode FindNecessaryDomNodeR(QTreeWidgetItem *clicked_item, int &index_of_child_in_child_list);
  void BuildXmlNodeTreeR(QDomNode xml_dom_node,QTreeWidgetItem *node_tree_item);

  void setCurrentFile(const QString &fileName);
  QString strippedName(const QString &fullName);

  bool saveFile(const QString &file);
  bool saveAs();
  bool maybeSave();

 private:
  Ui::MainWindow *ui;
  QDomDocument xml_document;
  QString currentFilePath;         //текущий путь файла
  bool isDocumentModified;
};

#endif // MAINWINDOW_H
