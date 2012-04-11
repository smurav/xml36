#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidgetItem>
#include <QtXml/QDomDocument>

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
  void OnNodeQTreeWidgetPressed( QTreeWidgetItem *clicked_item, int column);
  void on_xml_tree_itemExpanded(QTreeWidgetItem *item);
  void on_xml_tree_itemCollapsed(QTreeWidgetItem *item);

  void on_actionSaveAs_triggered();

  void on_actionSave_triggered();

  void on_actionAddAttribute_triggered();

  void on_xml_tree_itemClicked(QTreeWidgetItem *item, int column);

  void on_attributes_list_itemDoubleClicked(QTreeWidgetItem *item, int column);

  void on_actionEdit_triggered();

  void on_actionRemoveAttribute_triggered();

  void on_attributes_list_itemClicked(QTreeWidgetItem *item, int column);

  void on_attributes_list_itemChanged(QTreeWidgetItem *item, int column);

  void on_actionAddNode_triggered();

  void on_xml_tree_itemDoubleClicked(QTreeWidgetItem *item, int column);

  void on_actionRemoveNode_triggered();

  void on_xml_tree_itemChanged(QTreeWidgetItem *item, int column);

  void on_xml_tree_clicked(const QModelIndex &index);

private:
  bool OpenXML(const QString &fileName);
  QDomNode FindNecessaryDomNodeR(QTreeWidgetItem *clicked_item);
  QDomNode FindNecessaryDomNodeR(QTreeWidgetItem *clicked_item, int &index_of_child_in_child_list);
  void BuildXmlNodeTreeR(QDomNode xml_dom_node,QTreeWidgetItem *node_tree_item);

 private:
  Ui::MainWindow *ui;

private:
  QDomDocument xml_document;
  bool saved, to_edit;
  QString xml_file_name;
  QTreeWidgetItem* checked_item;
};

#endif // MAINWINDOW_H
