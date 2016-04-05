/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout_6;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout;
    QLabel *lblServer;
    QLineEdit *lneServer;
    QLabel *lblUsername;
    QLineEdit *lneUsername;
    QLabel *lblPassword;
    QLineEdit *lnePassword;
    QLabel *lblPort;
    QLineEdit *lnePort;
    QPushButton *btConnect;
    QTextEdit *textMsg;
    QHBoxLayout *horizontalLayout_5;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QLabel *lblLocalPath;
    QLineEdit *lneLocalPath;
    QTreeWidget *twLocalFile;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_4;
    QLabel *lblRemotePath;
    QLineEdit *lneRemotePath;
    QTreeWidget *twRemoteFile;
    QHBoxLayout *horizontalLayout_3;
    QLabel *lblSatus;
    QSpacerItem *horizontalSpacer;
    QLabel *lblProcessInfo;
    QProgressBar *barProcess;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(918, 590);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        horizontalLayout_6 = new QHBoxLayout(centralWidget);
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        lblServer = new QLabel(centralWidget);
        lblServer->setObjectName(QStringLiteral("lblServer"));

        horizontalLayout->addWidget(lblServer);

        lneServer = new QLineEdit(centralWidget);
        lneServer->setObjectName(QStringLiteral("lneServer"));

        horizontalLayout->addWidget(lneServer);

        lblUsername = new QLabel(centralWidget);
        lblUsername->setObjectName(QStringLiteral("lblUsername"));

        horizontalLayout->addWidget(lblUsername);

        lneUsername = new QLineEdit(centralWidget);
        lneUsername->setObjectName(QStringLiteral("lneUsername"));

        horizontalLayout->addWidget(lneUsername);

        lblPassword = new QLabel(centralWidget);
        lblPassword->setObjectName(QStringLiteral("lblPassword"));

        horizontalLayout->addWidget(lblPassword);

        lnePassword = new QLineEdit(centralWidget);
        lnePassword->setObjectName(QStringLiteral("lnePassword"));

        horizontalLayout->addWidget(lnePassword);

        lblPort = new QLabel(centralWidget);
        lblPort->setObjectName(QStringLiteral("lblPort"));

        horizontalLayout->addWidget(lblPort);

        lnePort = new QLineEdit(centralWidget);
        lnePort->setObjectName(QStringLiteral("lnePort"));
        lnePort->setMaximumSize(QSize(133, 16777215));

        horizontalLayout->addWidget(lnePort);

        btConnect = new QPushButton(centralWidget);
        btConnect->setObjectName(QStringLiteral("btConnect"));

        horizontalLayout->addWidget(btConnect);


        verticalLayout_3->addLayout(horizontalLayout);

        textMsg = new QTextEdit(centralWidget);
        textMsg->setObjectName(QStringLiteral("textMsg"));

        verticalLayout_3->addWidget(textMsg);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        lblLocalPath = new QLabel(centralWidget);
        lblLocalPath->setObjectName(QStringLiteral("lblLocalPath"));

        horizontalLayout_2->addWidget(lblLocalPath);

        lneLocalPath = new QLineEdit(centralWidget);
        lneLocalPath->setObjectName(QStringLiteral("lneLocalPath"));

        horizontalLayout_2->addWidget(lneLocalPath);


        verticalLayout->addLayout(horizontalLayout_2);

        twLocalFile = new QTreeWidget(centralWidget);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QStringLiteral("1"));
        twLocalFile->setHeaderItem(__qtreewidgetitem);
        twLocalFile->setObjectName(QStringLiteral("twLocalFile"));
        twLocalFile->setContextMenuPolicy(Qt::CustomContextMenu);
        twLocalFile->setIndentation(0);

        verticalLayout->addWidget(twLocalFile);


        horizontalLayout_5->addLayout(verticalLayout);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setSizeConstraint(QLayout::SetFixedSize);
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        lblRemotePath = new QLabel(centralWidget);
        lblRemotePath->setObjectName(QStringLiteral("lblRemotePath"));

        horizontalLayout_4->addWidget(lblRemotePath);

        lneRemotePath = new QLineEdit(centralWidget);
        lneRemotePath->setObjectName(QStringLiteral("lneRemotePath"));

        horizontalLayout_4->addWidget(lneRemotePath);


        verticalLayout_2->addLayout(horizontalLayout_4);

        twRemoteFile = new QTreeWidget(centralWidget);
        twRemoteFile->setObjectName(QStringLiteral("twRemoteFile"));
        twRemoteFile->setContextMenuPolicy(Qt::CustomContextMenu);
        twRemoteFile->setIndentation(0);
        twRemoteFile->header()->setVisible(true);
        twRemoteFile->header()->setStretchLastSection(false);

        verticalLayout_2->addWidget(twRemoteFile);


        horizontalLayout_5->addLayout(verticalLayout_2);


        verticalLayout_3->addLayout(horizontalLayout_5);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        lblSatus = new QLabel(centralWidget);
        lblSatus->setObjectName(QStringLiteral("lblSatus"));

        horizontalLayout_3->addWidget(lblSatus);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer);

        lblProcessInfo = new QLabel(centralWidget);
        lblProcessInfo->setObjectName(QStringLiteral("lblProcessInfo"));

        horizontalLayout_3->addWidget(lblProcessInfo);

        barProcess = new QProgressBar(centralWidget);
        barProcess->setObjectName(QStringLiteral("barProcess"));
        barProcess->setValue(24);

        horizontalLayout_3->addWidget(barProcess);


        verticalLayout_3->addLayout(horizontalLayout_3);


        horizontalLayout_6->addLayout(verticalLayout_3);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 918, 23));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        lblServer->setText(QApplication::translate("MainWindow", "Server Ip", 0));
        lblUsername->setText(QApplication::translate("MainWindow", "Username", 0));
        lblPassword->setText(QApplication::translate("MainWindow", "password", 0));
        lblPort->setText(QApplication::translate("MainWindow", "port", 0));
        btConnect->setText(QApplication::translate("MainWindow", "connect", 0));
        lblLocalPath->setText(QApplication::translate("MainWindow", "Local Path", 0));
        lblRemotePath->setText(QApplication::translate("MainWindow", "Remote Path", 0));
        QTreeWidgetItem *___qtreewidgetitem = twRemoteFile->headerItem();
        ___qtreewidgetitem->setText(0, QApplication::translate("MainWindow", "1", 0));
        lblSatus->setText(QApplication::translate("MainWindow", "TextLabel", 0));
        lblProcessInfo->setText(QApplication::translate("MainWindow", "TextLabel", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
