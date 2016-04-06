/********************************************************************************
** Form generated from reading UI file 'ldapclientqt.ui'
**
** Created by: Qt User Interface Compiler version 5.3.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LDAPCLIENTQT_H
#define UI_LDAPCLIENTQT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LDAPClientQTClass
{
public:
    QTabWidget *tabLdap;
    QWidget *tab;
    QGroupBox *grpConnect;
    QLabel *lblIP;
    QLabel *lblPort;
    QLineEdit *txtIP;
    QLineEdit *txtPort;
    QPushButton *pushConnect;
    QLabel *lblTest;
    QWidget *tab_2;
    QPlainTextEdit *txtASN;
    QPushButton *parseButton;
    QWidget *tab_3;
    QPlainTextEdit *txtHex;
    QPushButton *parseHexButton;

    void setupUi(QDialog *LDAPClientQTClass)
    {
        if (LDAPClientQTClass->objectName().isEmpty())
            LDAPClientQTClass->setObjectName(QStringLiteral("LDAPClientQTClass"));
        LDAPClientQTClass->setWindowModality(Qt::NonModal);
        LDAPClientQTClass->resize(654, 544);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(LDAPClientQTClass->sizePolicy().hasHeightForWidth());
        LDAPClientQTClass->setSizePolicy(sizePolicy);
        LDAPClientQTClass->setSizeGripEnabled(false);
        LDAPClientQTClass->setModal(false);
        tabLdap = new QTabWidget(LDAPClientQTClass);
        tabLdap->setObjectName(QStringLiteral("tabLdap"));
        tabLdap->setGeometry(QRect(0, 0, 651, 541));
        tabLdap->setTabShape(QTabWidget::Rounded);
        tabLdap->setElideMode(Qt::ElideNone);
        tabLdap->setUsesScrollButtons(false);
        tabLdap->setDocumentMode(false);
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        grpConnect = new QGroupBox(tab);
        grpConnect->setObjectName(QStringLiteral("grpConnect"));
        grpConnect->setGeometry(QRect(0, 10, 341, 71));
        lblIP = new QLabel(grpConnect);
        lblIP->setObjectName(QStringLiteral("lblIP"));
        lblIP->setGeometry(QRect(20, 20, 46, 13));
        lblPort = new QLabel(grpConnect);
        lblPort->setObjectName(QStringLiteral("lblPort"));
        lblPort->setGeometry(QRect(20, 40, 46, 13));
        txtIP = new QLineEdit(grpConnect);
        txtIP->setObjectName(QStringLiteral("txtIP"));
        txtIP->setGeometry(QRect(60, 20, 113, 20));
        txtIP->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        txtPort = new QLineEdit(grpConnect);
        txtPort->setObjectName(QStringLiteral("txtPort"));
        txtPort->setGeometry(QRect(60, 40, 113, 20));
        txtPort->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        pushConnect = new QPushButton(grpConnect);
        pushConnect->setObjectName(QStringLiteral("pushConnect"));
        pushConnect->setGeometry(QRect(190, 20, 75, 23));
        pushConnect->setStyleSheet(QLatin1String("background-color: lightgreen;\n"
"border: 2px solid rgba(205, 92, 92, 255);"));
        pushConnect->setFlat(false);
        lblTest = new QLabel(tab);
        lblTest->setObjectName(QStringLiteral("lblTest"));
        lblTest->setGeometry(QRect(0, 80, 46, 16));
        tabLdap->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        txtASN = new QPlainTextEdit(tab_2);
        txtASN->setObjectName(QStringLiteral("txtASN"));
        txtASN->setGeometry(QRect(0, 0, 651, 481));
        txtASN->setLineWrapMode(QPlainTextEdit::WidgetWidth);
        txtASN->setTextInteractionFlags(Qt::LinksAccessibleByMouse|Qt::TextEditable|Qt::TextEditorInteraction|Qt::TextSelectableByKeyboard|Qt::TextSelectableByMouse);
        parseButton = new QPushButton(tab_2);
        parseButton->setObjectName(QStringLiteral("parseButton"));
        parseButton->setGeometry(QRect(570, 490, 75, 23));
        tabLdap->addTab(tab_2, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QStringLiteral("tab_3"));
        txtHex = new QPlainTextEdit(tab_3);
        txtHex->setObjectName(QStringLiteral("txtHex"));
        txtHex->setGeometry(QRect(0, 0, 651, 481));
        txtHex->setLineWrapMode(QPlainTextEdit::WidgetWidth);
        parseHexButton = new QPushButton(tab_3);
        parseHexButton->setObjectName(QStringLiteral("parseHexButton"));
        parseHexButton->setGeometry(QRect(570, 490, 75, 23));
        tabLdap->addTab(tab_3, QString());

        retranslateUi(LDAPClientQTClass);
        QObject::connect(pushConnect, SIGNAL(clicked()), LDAPClientQTClass, SLOT(pushButton_clicked()));

        tabLdap->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(LDAPClientQTClass);
    } // setupUi

    void retranslateUi(QDialog *LDAPClientQTClass)
    {
        LDAPClientQTClass->setWindowTitle(QApplication::translate("LDAPClientQTClass", "LDAPTool", 0));
        grpConnect->setTitle(QApplication::translate("LDAPClientQTClass", "Remote PCAP", 0));
        lblIP->setText(QApplication::translate("LDAPClientQTClass", "IP : ", 0));
        lblPort->setText(QApplication::translate("LDAPClientQTClass", "Port : ", 0));
        txtIP->setText(QApplication::translate("LDAPClientQTClass", "192.168.220.129", 0));
        txtPort->setText(QApplication::translate("LDAPClientQTClass", "1113", 0));
        pushConnect->setText(QApplication::translate("LDAPClientQTClass", "Connect", 0));
        lblTest->setText(QApplication::translate("LDAPClientQTClass", "TextLabel", 0));
        tabLdap->setTabText(tabLdap->indexOf(tab), QApplication::translate("LDAPClientQTClass", "Remote", 0));
        txtASN->setDocumentTitle(QString());
        txtASN->setPlainText(QApplication::translate("LDAPClientQTClass", "BindRequest::= {	\n"
"	Version	3,		\n"
"	Name	'sdfrun',	\n"
"	authentication \n"
"		sasl ::= \n"
"		{ mechanism 'mechanism',\n"
"		credentials 'credentials' \n"
"		}\n"
"	}", 0));
        parseButton->setText(QApplication::translate("LDAPClientQTClass", "Parse", 0));
        tabLdap->setTabText(tabLdap->indexOf(tab_2), QApplication::translate("LDAPClientQTClass", "ASN", 0));
        txtHex->setDocumentTitle(QString());
        txtHex->setPlainText(QApplication::translate("LDAPClientQTClass", "30 2c 02 01 01 60 27 02 01 03 04 06 73 64 66 72 75\n"
"6e 80 1a 30 18 04 09 6d 65 63 68 61 6e 69 73 \n"
"6d 04 0b 63 72 65 64 65 6e74 69 61 6c 73", 0));
        parseHexButton->setText(QApplication::translate("LDAPClientQTClass", "Parse", 0));
        tabLdap->setTabText(tabLdap->indexOf(tab_3), QApplication::translate("LDAPClientQTClass", "Hex", 0));
    } // retranslateUi

};

namespace Ui {
    class LDAPClientQTClass: public Ui_LDAPClientQTClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LDAPCLIENTQT_H
