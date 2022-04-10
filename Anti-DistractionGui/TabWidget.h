#pragma once
#include <qtabwidget.h>
#include <qurl.h>

class WebBrowser;

class TabWidget :
    public QTabWidget
{
    Q_OBJECT
public:
    TabWidget(QWidget* parent = Q_NULLPTR);

    void setAllowedDomainsList(const QString& domains) { allowedDomainsList = domains.split(';'); }
    WebBrowser* currentWebBrowser() const;
    QUrl currentUrl()const;
    void setDefaultUrl(const QString& url);

public slots:
    bool setUrl(const QString& url);

    void handleCurrentChanged(int index);

    WebBrowser* createTab();
    WebBrowser* createBackgroundTab();

    void closeTab(int index);

    void back();
    void forward();

signals:
    void urlChanged(const QUrl& url);
private:
    void setupBrowser(WebBrowser* browser);
    WebBrowser* webBrowser(int index)const;

    QStringList allowedDomainsList;
    QUrl defaultUrl;

    bool isVaildUrl(const QUrl& url) const;
};