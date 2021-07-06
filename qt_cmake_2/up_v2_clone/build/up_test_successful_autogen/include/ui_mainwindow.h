/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.9.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <qcustomplot.h>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QCustomPlot *widget;
    QGroupBox *groupBox;
    QPushButton *pushButton_close;
    QCheckBox *checkBox;
    QPushButton *pushButton_back;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout_3;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_2;
    QLineEdit *line_autor;
    QPushButton *pushButton_input;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *pushButton_tend;
    QPushButton *pushButton_tstart;
    QPushButton *pushButton_clear;
    QTextBrowser *textBrowser;
    QWidget *layoutWidget1;
    QVBoxLayout *verticalLayout;
    QFormLayout *formLayout_2;
    QLabel *labelX;
    QLineEdit *LineX;
    QFormLayout *formLayout;
    QLabel *LinelY;
    QLineEdit *LineY;
    QPushButton *pushButton;
    QWidget *layoutWidget2;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QTextBrowser *mouse_hit;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1180, 734);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        widget = new QCustomPlot(centralWidget);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setGeometry(QRect(380, 30, 761, 581));
        widget->setInputMethodHints(Qt::ImhNone);
        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(20, 10, 351, 601));
        groupBox->setMouseTracking(false);
        groupBox->setStyleSheet(QStringLiteral("selection-color: rgb(138, 226, 52);"));
        pushButton_close = new QPushButton(groupBox);
        pushButton_close->setObjectName(QStringLiteral("pushButton_close"));
        pushButton_close->setEnabled(true);
        pushButton_close->setGeometry(QRect(10, 560, 80, 31));
        pushButton_close->setStyleSheet(QLatin1String("border-color: rgb(12, 29, 241);\n"
"background-color: rgb(115, 210, 22);\n"
"selection-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(9, 41, 4, 255), stop:0.085 rgba(2, 79, 0, 255), stop:0.19 rgba(50, 147, 22, 255), stop:0.275 rgba(236, 191, 49, 255), stop:0.39 rgba(243, 61, 34, 255), stop:0.555 rgba(135, 81, 60, 255), stop:0.667 rgba(121, 75, 255, 255), stop:0.825 rgba(164, 255, 244, 255), stop:0.885 rgba(104, 222, 71, 255), stop:1 rgba(93, 128, 0, 255));"));
        checkBox = new QCheckBox(groupBox);
        checkBox->setObjectName(QStringLiteral("checkBox"));
        checkBox->setEnabled(true);
        checkBox->setGeometry(QRect(170, 560, 170, 23));
        checkBox->setAcceptDrops(false);
        checkBox->setAutoFillBackground(false);
        checkBox->setChecked(true);
        checkBox->setTristate(false);
        pushButton_back = new QPushButton(groupBox);
        pushButton_back->setObjectName(QStringLiteral("pushButton_back"));
        pushButton_back->setGeometry(QRect(270, 40, 61, 91));
        layoutWidget = new QWidget(groupBox);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(10, 20, 258, 140));
        verticalLayout_3 = new QVBoxLayout(layoutWidget);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        line_autor = new QLineEdit(layoutWidget);
        line_autor->setObjectName(QStringLiteral("line_autor"));

        horizontalLayout_2->addWidget(line_autor);

        pushButton_input = new QPushButton(layoutWidget);
        pushButton_input->setObjectName(QStringLiteral("pushButton_input"));

        horizontalLayout_2->addWidget(pushButton_input);


        verticalLayout_2->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        pushButton_tend = new QPushButton(layoutWidget);
        pushButton_tend->setObjectName(QStringLiteral("pushButton_tend"));

        horizontalLayout_3->addWidget(pushButton_tend);

        pushButton_tstart = new QPushButton(layoutWidget);
        pushButton_tstart->setObjectName(QStringLiteral("pushButton_tstart"));

        horizontalLayout_3->addWidget(pushButton_tstart);

        pushButton_clear = new QPushButton(layoutWidget);
        pushButton_clear->setObjectName(QStringLiteral("pushButton_clear"));

        horizontalLayout_3->addWidget(pushButton_clear);


        verticalLayout_2->addLayout(horizontalLayout_3);


        verticalLayout_3->addLayout(verticalLayout_2);

        textBrowser = new QTextBrowser(layoutWidget);
        textBrowser->setObjectName(QStringLiteral("textBrowser"));

        verticalLayout_3->addWidget(textBrowser);

        layoutWidget1 = new QWidget(groupBox);
        layoutWidget1->setObjectName(QStringLiteral("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(10, 180, 162, 93));
        verticalLayout = new QVBoxLayout(layoutWidget1);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        formLayout_2 = new QFormLayout();
        formLayout_2->setSpacing(6);
        formLayout_2->setObjectName(QStringLiteral("formLayout_2"));
        labelX = new QLabel(layoutWidget1);
        labelX->setObjectName(QStringLiteral("labelX"));

        formLayout_2->setWidget(0, QFormLayout::LabelRole, labelX);

        LineX = new QLineEdit(layoutWidget1);
        LineX->setObjectName(QStringLiteral("LineX"));

        formLayout_2->setWidget(0, QFormLayout::FieldRole, LineX);


        verticalLayout->addLayout(formLayout_2);

        formLayout = new QFormLayout();
        formLayout->setSpacing(6);
        formLayout->setObjectName(QStringLiteral("formLayout"));
        LinelY = new QLabel(layoutWidget1);
        LinelY->setObjectName(QStringLiteral("LinelY"));

        formLayout->setWidget(0, QFormLayout::LabelRole, LinelY);

        LineY = new QLineEdit(layoutWidget1);
        LineY->setObjectName(QStringLiteral("LineY"));

        formLayout->setWidget(0, QFormLayout::FieldRole, LineY);


        verticalLayout->addLayout(formLayout);

        pushButton = new QPushButton(layoutWidget1);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        verticalLayout->addWidget(pushButton);

        layoutWidget2 = new QWidget(groupBox);
        layoutWidget2->setObjectName(QStringLiteral("layoutWidget2"));
        layoutWidget2->setGeometry(QRect(0, 300, 334, 194));
        horizontalLayout = new QHBoxLayout(layoutWidget2);
        horizontalLayout->setSpacing(5);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
        horizontalLayout->setContentsMargins(0, 5, 5, 5);
        label = new QLabel(layoutWidget2);
        label->setObjectName(QStringLiteral("label"));
        label->setContextMenuPolicy(Qt::NoContextMenu);
        label->setStyleSheet(QStringLiteral("background-color: qlineargradient(spread:repeat, x1:0, y1:1, x2:1, y2:0, stop:0.262887 rgba(97, 203, 228, 255), stop:0.778351 rgba(198, 224, 234, 255));"));
        label->setTextFormat(Qt::RichText);

        horizontalLayout->addWidget(label);

        mouse_hit = new QTextBrowser(layoutWidget2);
        mouse_hit->setObjectName(QStringLiteral("mouse_hit"));

        horizontalLayout->addWidget(mouse_hit);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1180, 28));
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
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", Q_NULLPTR));
        groupBox->setTitle(QApplication::translate("MainWindow", "\346\225\260\346\215\256\350\276\223\345\205\245", Q_NULLPTR));
        pushButton_close->setText(QApplication::translate("MainWindow", "Close", Q_NULLPTR));
        checkBox->setText(QApplication::translate("MainWindow", "Lock original position", Q_NULLPTR));
        pushButton_back->setText(QApplication::translate("MainWindow", "GoDault", Q_NULLPTR));
        pushButton_input->setText(QApplication::translate("MainWindow", "input(y)", Q_NULLPTR));
        pushButton_tend->setText(QApplication::translate("MainWindow", "stop", Q_NULLPTR));
        pushButton_tstart->setText(QApplication::translate("MainWindow", "start", Q_NULLPTR));
        pushButton_clear->setText(QApplication::translate("MainWindow", "clear", Q_NULLPTR));
        labelX->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-weight:600;\">x</span></p></body></html>", Q_NULLPTR));
        LinelY->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-weight:600;\">y</span></p></body></html>", Q_NULLPTR));
        pushButton->setText(QApplication::translate("MainWindow", "input", Q_NULLPTR));
        label->setText(QApplication::translate("MainWindow", "Mouse HIt", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
