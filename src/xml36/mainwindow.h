#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidgetItem>
#include <QtXml/QDomDocument>
#include <QFile>

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
  void OnNodeQTreeWidgetPressed(QTreeWidgetItem *clicked_item, int column);
  void OnAttributesQTreeWidgetPressed(QTreeWidgetItem *clicked_item, int column);
  void OnQTreeWidgetsChanged(QTreeWidgetItem *changed_item, int changed_column);
  void OnCurrentItemChanged(QTreeWidgetItem *current_item, QTreeWidgetItem *previous_item);
  void OnTreeItemExpanded(QTreeWidgetItem *expanded_item);
  void OnTreeItemCollapsed(QTreeWidgetItem *collapsed_item);

  void on_actionNew_triggered();
  void on_actionSave_triggered();
  void on_actionSaveAs_triggered();
  void on_actionDelete_triggered();
  void on_actionEdit_toggled(bool edit_is_on);

  void on_actionAdd_triggered();

private:
  bool OpenXML(const QString &fileName);
  void CreateTreeRootWithCurrentXMLDocument();
  QTreeWidgetItem* CreateNewTreeWidgetElement(QDomNode);
  QTreeWidgetItem* CreateNewTreeWidgetAttribute(QDomNode);
//  QDomNode FindNecessaryDomNodeR(QTreeWidgetItem *clicked_item);
//  QDomNode FindNecessaryDomNodeR(QTreeWidgetItem *clicked_item, int &index_of_child_in_child_list);
  void BuildXmlNodeTreeR(QDomNode xml_dom_node,QTreeWidgetItem *node_tree_item);
  void ClearDocumentAndTreesPointers();
  void ClearDocumentAndTrees();
  bool ShowMessageBoxOfferingToSaveData();  //return false in case of cancel clicked

  bool Valid();

 private:
  Ui::MainWindow *ui;

private:
  QDomDocument xml_document;
  QTreeWidgetItem *current_element_tree_item;
  QTreeWidgetItem *last_current_tree_item_clicked;
  QString current_working_file_name;
  bool edit_mode_is_on;
};

#endif // MAINWINDOW_H
