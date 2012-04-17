#include "addnodedialog.h"
#include "ui_addnodedialog.h"
#include <QtGui>

AddNodeDialog::AddNodeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddNodeDialog)
{
    ui->setupUi(this);
}

AddNodeDialog::~AddNodeDialog()
{
    delete ui;
}

void AddNodeDialog::on_node_name_textChanged(const QString &arg1) {
  ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(arg1.length());
}

QString AddNodeDialog::GetNodeName() const {
  return ui->node_name->text();
}

