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
  void OnNodeQTreeWidgetPressed( QTreeWidgetItem *clicked_item, int column );

 private:
  bool OpenXML(const QString &fileName);
  QDomNode FindNecessaryDomNodeR(QTreeWidgetItem *clicked_item);
  QDomNode FindNecessaryDomNodeR(QTreeWidgetItem *clicked_item, int &index_of_child_in_child_list);
  void BuildXmlNodeTreeR(QDomNode xml_dom_node,QTreeWidgetItem *node_tree_item);

 private:
  Ui::MainWindow *ui;

private:
  QDomDocument xml_document;
};

#endif // MAINWINDOW_H
