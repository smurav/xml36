/********************************************************************************
** Form generated from reading UI file 'addnodedialog.ui'
**
** Created: Tue Apr 17 18:19:26 2012
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ADDNODEDIALOG_H
#define UI_ADDNODEDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>

QT_BEGIN_NAMESPACE

class Ui_AddNodeDialog
{
public:
    QGridLayout *gridLayout;
    QDialogButtonBox *buttonBox;
    QLineEdit *node_name;
    QLabel *label;

    void setupUi(QDialog *AddNodeDialog)
    {
        if (AddNodeDialog->objectName().isEmpty())
            AddNodeDialog->setObjectName(QString::fromUtf8("AddNodeDialog"));
        AddNodeDialog->resize(400, 79);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/xml.png"), QSize(), QIcon::Normal, QIcon::Off);
        AddNodeDialog->setWindowIcon(icon);
        gridLayout = new QGridLayout(AddNodeDialog);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        buttonBox = new QDialogButtonBox(AddNodeDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        gridLayout->addWidget(buttonBox, 1, 1, 1, 1);

        node_name = new QLineEdit(AddNodeDialog);
        node_name->setObjectName(QString::fromUtf8("node_name"));

        gridLayout->addWidget(node_name, 0, 1, 1, 1);

        label = new QLabel(AddNodeDialog);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);


        retranslateUi(AddNodeDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), AddNodeDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), AddNodeDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(AddNodeDialog);
    } // setupUi

    void retranslateUi(QDialog *AddNodeDialog)
    {
        AddNodeDialog->setWindowTitle(QApplication::translate("AddNodeDialog", "\320\224\320\276\320\261\320\260\320\262\320\273\320\265\320\275\320\270\320\265 \321\203\320\267\320\273\320\260", 0, QApplication::UnicodeUTF8));
        node_name->setText(QApplication::translate("AddNodeDialog", "item", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("AddNodeDialog", "\320\230\320\274\321\217 \321\203\320\267\320\273\320\260", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class AddNodeDialog: public Ui_AddNodeDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ADDNODEDIALOG_H
