#pragma once
#include <qwidget.h>
#include <qlineedit.h>
#include <qpushbutton.h>

class TabWidget;

class FocusBrowser :
    public QWidget
{
    Q_OBJECT
public:
    FocusBrowser(QWidget* parent = Q_NULLPTR);

    TabWidget* getTabWidget() const { return this->tabWidget; };

private slots:
    void goInputUrl();

private:
    QLineEdit* urlEdit;
    QPushButton* finishedButton;
    QPushButton* backButton;
    QPushButton* forwardButton;
    QPushButton* newPageButton;

    TabWidget* tabWidget;
};

