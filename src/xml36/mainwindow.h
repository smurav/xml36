#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDomNode>

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

 private:
  bool OpenXML(const QString &fileName);
  void GetChilds(QDomNode node);

 private:
  Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
