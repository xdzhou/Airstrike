/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Wed Nov 6 21:35:44 2013
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QFormLayout>
#include <QtGui/QFrame>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QProgressBar>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QStatusBar>
#include <QtGui/QTextEdit>
#include <QtGui/QToolBar>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_3;
    QFormLayout *formLayout;
    QLabel *label;
    QLineEdit *ipEdit;
    QLabel *label_2;
    QLineEdit *portEdit;
    QLabel *label_4;
    QLineEdit *nameEdit;
    QCheckBox *checkBoxBot;
    QComboBox *comboBoxTeam;
    QLabel *label_10;
    QFrame *frame;
    QVBoxLayout *verticalLayout_6;
    QHBoxLayout *horizontalLayout;
    QPushButton *connectButton;
    QPushButton *disconnectButton;
    QSpacerItem *verticalSpacer;
    QGroupBox *groupBox_4;
    QFormLayout *formLayout_3;
    QLabel *label_5;
    QLabel *playerIdLabel;
    QLabel *label_9;
    QLabel *equipe;
    QLabel *label_8;
    QLabel *sprite;
    QVBoxLayout *verticalLayout_5;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_2;
    QFormLayout *formLayout_2;
    QLabel *label_3;
    QLabel *playerScore;
    QLabel *teamScore;
    QLabel *label_6;
    QLabel *label_7;
    QProgressBar *healthPoints;
    QSpacerItem *verticalSpacer_2;
    QGroupBox *groupBox_3;
    QVBoxLayout *verticalLayout_4;
    QTextEdit *debugTextEdit;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(568, 401);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        horizontalLayout_2 = new QHBoxLayout(centralWidget);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        verticalLayout_3 = new QVBoxLayout(groupBox);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        formLayout = new QFormLayout();
        formLayout->setSpacing(6);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        formLayout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));

        formLayout->setWidget(1, QFormLayout::LabelRole, label);

        ipEdit = new QLineEdit(groupBox);
        ipEdit->setObjectName(QString::fromUtf8("ipEdit"));

        formLayout->setWidget(1, QFormLayout::FieldRole, ipEdit);

        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        formLayout->setWidget(2, QFormLayout::LabelRole, label_2);

        portEdit = new QLineEdit(groupBox);
        portEdit->setObjectName(QString::fromUtf8("portEdit"));

        formLayout->setWidget(2, QFormLayout::FieldRole, portEdit);

        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        formLayout->setWidget(3, QFormLayout::LabelRole, label_4);

        nameEdit = new QLineEdit(groupBox);
        nameEdit->setObjectName(QString::fromUtf8("nameEdit"));
        nameEdit->setContextMenuPolicy(Qt::CustomContextMenu);
        nameEdit->setMaxLength(8);

        formLayout->setWidget(3, QFormLayout::FieldRole, nameEdit);

        checkBoxBot = new QCheckBox(groupBox);
        checkBoxBot->setObjectName(QString::fromUtf8("checkBoxBot"));

        formLayout->setWidget(5, QFormLayout::FieldRole, checkBoxBot);

        comboBoxTeam = new QComboBox(groupBox);
        comboBoxTeam->setObjectName(QString::fromUtf8("comboBoxTeam"));
        comboBoxTeam->setContextMenuPolicy(Qt::PreventContextMenu);
        comboBoxTeam->setSizeAdjustPolicy(QComboBox::AdjustToContents);

        formLayout->setWidget(4, QFormLayout::FieldRole, comboBoxTeam);

        label_10 = new QLabel(groupBox);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        formLayout->setWidget(4, QFormLayout::LabelRole, label_10);


        verticalLayout_3->addLayout(formLayout);


        verticalLayout->addWidget(groupBox);

        frame = new QFrame(centralWidget);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        verticalLayout_6 = new QVBoxLayout(frame);
        verticalLayout_6->setSpacing(6);
        verticalLayout_6->setContentsMargins(11, 11, 11, 11);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(-1, 0, -1, -1);
        connectButton = new QPushButton(frame);
        connectButton->setObjectName(QString::fromUtf8("connectButton"));

        horizontalLayout->addWidget(connectButton);

        disconnectButton = new QPushButton(frame);
        disconnectButton->setObjectName(QString::fromUtf8("disconnectButton"));

        horizontalLayout->addWidget(disconnectButton);


        verticalLayout_6->addLayout(horizontalLayout);


        verticalLayout->addWidget(frame);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        groupBox_4 = new QGroupBox(centralWidget);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        formLayout_3 = new QFormLayout(groupBox_4);
        formLayout_3->setSpacing(6);
        formLayout_3->setContentsMargins(11, 11, 11, 11);
        formLayout_3->setObjectName(QString::fromUtf8("formLayout_3"));
        formLayout_3->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
        label_5 = new QLabel(groupBox_4);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        formLayout_3->setWidget(0, QFormLayout::LabelRole, label_5);

        playerIdLabel = new QLabel(groupBox_4);
        playerIdLabel->setObjectName(QString::fromUtf8("playerIdLabel"));

        formLayout_3->setWidget(0, QFormLayout::FieldRole, playerIdLabel);

        label_9 = new QLabel(groupBox_4);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        formLayout_3->setWidget(1, QFormLayout::LabelRole, label_9);

        equipe = new QLabel(groupBox_4);
        equipe->setObjectName(QString::fromUtf8("equipe"));

        formLayout_3->setWidget(1, QFormLayout::FieldRole, equipe);

        label_8 = new QLabel(groupBox_4);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        formLayout_3->setWidget(2, QFormLayout::LabelRole, label_8);

        sprite = new QLabel(groupBox_4);
        sprite->setObjectName(QString::fromUtf8("sprite"));

        formLayout_3->setWidget(2, QFormLayout::FieldRole, sprite);


        verticalLayout->addWidget(groupBox_4);


        horizontalLayout_2->addLayout(verticalLayout);

        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        groupBox_2 = new QGroupBox(centralWidget);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        verticalLayout_2 = new QVBoxLayout(groupBox_2);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        formLayout_2 = new QFormLayout();
        formLayout_2->setSpacing(6);
        formLayout_2->setObjectName(QString::fromUtf8("formLayout_2"));
        formLayout_2->setContentsMargins(0, 0, -1, -1);
        label_3 = new QLabel(groupBox_2);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        formLayout_2->setWidget(0, QFormLayout::LabelRole, label_3);

        playerScore = new QLabel(groupBox_2);
        playerScore->setObjectName(QString::fromUtf8("playerScore"));
        QFont font;
        font.setPointSize(16);
        playerScore->setFont(font);
        playerScore->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout_2->setWidget(0, QFormLayout::FieldRole, playerScore);

        teamScore = new QLabel(groupBox_2);
        teamScore->setObjectName(QString::fromUtf8("teamScore"));
        teamScore->setFont(font);
        teamScore->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout_2->setWidget(1, QFormLayout::FieldRole, teamScore);

        label_6 = new QLabel(groupBox_2);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        formLayout_2->setWidget(1, QFormLayout::LabelRole, label_6);

        label_7 = new QLabel(groupBox_2);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        formLayout_2->setWidget(2, QFormLayout::LabelRole, label_7);

        healthPoints = new QProgressBar(groupBox_2);
        healthPoints->setObjectName(QString::fromUtf8("healthPoints"));
        QFont font1;
        font1.setPointSize(20);
        healthPoints->setFont(font1);
        healthPoints->setValue(100);

        formLayout_2->setWidget(2, QFormLayout::FieldRole, healthPoints);


        verticalLayout_2->addLayout(formLayout_2);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer_2);


        verticalLayout_5->addWidget(groupBox_2);

        groupBox_3 = new QGroupBox(centralWidget);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        verticalLayout_4 = new QVBoxLayout(groupBox_3);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        debugTextEdit = new QTextEdit(groupBox_3);
        debugTextEdit->setObjectName(QString::fromUtf8("debugTextEdit"));

        verticalLayout_4->addWidget(debugTextEdit);


        verticalLayout_5->addWidget(groupBox_3);


        horizontalLayout_2->addLayout(verticalLayout_5);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 568, 23));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);
        QWidget::setTabOrder(ipEdit, portEdit);
        QWidget::setTabOrder(portEdit, nameEdit);
        QWidget::setTabOrder(nameEdit, connectButton);
        QWidget::setTabOrder(connectButton, disconnectButton);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("MainWindow", "Param\303\250tres de connection", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("MainWindow", "Adresse IP", 0, QApplication::UnicodeUTF8));
        ipEdit->setText(QApplication::translate("MainWindow", "127.0.0.1", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("MainWindow", "Port", 0, QApplication::UnicodeUTF8));
        portEdit->setText(QApplication::translate("MainWindow", "1234", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("MainWindow", "Pseudo", 0, QApplication::UnicodeUTF8));
        checkBoxBot->setText(QApplication::translate("MainWindow", "Bot", 0, QApplication::UnicodeUTF8));
        comboBoxTeam->clear();
        comboBoxTeam->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "Vert Clair", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Rouge", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Bleu", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Jaunes", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Turquoise", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Violet", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Orange", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Gris", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Vert Fonc\303\251", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Bleu Fonc\303\251", 0, QApplication::UnicodeUTF8)
        );
        label_10->setText(QApplication::translate("MainWindow", "\303\211quipe", 0, QApplication::UnicodeUTF8));
        connectButton->setText(QApplication::translate("MainWindow", "Connecter", 0, QApplication::UnicodeUTF8));
        disconnectButton->setText(QApplication::translate("MainWindow", "D\303\251connecter", 0, QApplication::UnicodeUTF8));
        groupBox_4->setTitle(QApplication::translate("MainWindow", "Infos", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("MainWindow", "Joueur num\303\251ro", 0, QApplication::UnicodeUTF8));
        playerIdLabel->setText(QApplication::translate("MainWindow", "0", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("MainWindow", "Equipe", 0, QApplication::UnicodeUTF8));
        equipe->setText(QApplication::translate("MainWindow", "0", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("MainWindow", "Sprite", 0, QApplication::UnicodeUTF8));
        sprite->setText(QApplication::translate("MainWindow", "Image", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("MainWindow", "Jeu", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("MainWindow", "Points joueur", 0, QApplication::UnicodeUTF8));
        playerScore->setText(QApplication::translate("MainWindow", "0", 0, QApplication::UnicodeUTF8));
        teamScore->setText(QApplication::translate("MainWindow", "0", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("MainWindow", "Points \303\251quipe", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("MainWindow", "Vie", 0, QApplication::UnicodeUTF8));
        groupBox_3->setTitle(QApplication::translate("MainWindow", "Debug", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
