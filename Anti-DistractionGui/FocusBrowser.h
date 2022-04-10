#pragma once
#include <qwidget.h>
#include "WebBrowser.h"
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qstack.h>
#include <qtabwidget.h>

class FocusBrowser :
    public QWidget
{
    Q_OBJECT
public:
    FocusBrowser(QWidget* parent = Q_NULLPTR);

    bool setUrl(const QString & urlStr);
    void setAllowedDomains(const QString& domains) { allowedDomains = domains.split(";"); }


private slots:
    void goInputUrl();
    void onBrowserUrlChanged(const QUrl& url);

private:
    WebBrowser* browser;
    QLineEdit* urlEdit;
    QPushButton* finishedButton;
    QPushButton* backButton;
    QPushButton* aheadButton;

    QStringList allowedDomains;

};

