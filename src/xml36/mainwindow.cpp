#include <QtGui>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QDebug>
#include <QTextStream>
#include <QStringList>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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
  bool opening_result = OpenXML(file_name);
  QApplication::restoreOverrideCursor();

  if (false == opening_result) {

    QMessageBox::critical(this, tr("Открытие файла"),
                          tr("Ошибка открытия файла %1").arg(file_name),
                          QMessageBox::Ok);
  }
}


void MainWindow::GetChilds(QDomNode node)
{
    if (node.hasChildNodes()) {
      QDomNodeList tempNodeList = node.childNodes();
      for (int i = 0; i<tempNodeList.count(); ++i) {
        if (tempNodeList.at(i).isComment()) {
          node.removeChild(tempNodeList.at(i));
        } else if (tempNodeList.at(i).hasChildNodes()) {
          GetChilds(tempNodeList.at(i));
        }
      }
    }
}


bool MainWindow::OpenXML(const QString &fileName) {

  QDomDocument document;
  {
    QFile file(fileName);
    if (false == file.open(QIODevice::ReadOnly))
          qDebug()<<"Failed!"<<endl;
    QTextStream inputStream(&file);
    QString inputString = inputStream.readAll();
    document.setContent(inputString);
  }

  QDomNodeList nodeList = document.childNodes();
  for (int i = 0; i<nodeList.count(); ++i) {

    if (nodeList.at(i).isProcessingInstruction())  // удаляем Processing Instruction
      document.removeChild(nodeList.at(i));

    if (nodeList.at(i).isComment())    // удаляем комментарии
      document.removeChild(nodeList.at(i));

    if (nodeList.at(i).hasChildNodes())
       GetChilds(nodeList.at(i));
  }

  for (int i=0; i<nodeList.count(); i++) {
      ui->xml_tree->addTopLevelItem(new QTreeWidgetItem(QStringList(nodeList.at(i).nodeName())));
      ui->attibutes_list->addTopLevelItem(new QTreeWidgetItem(QStringList(nodeList.at(i).nodeValue())));
}
    return true;
}


