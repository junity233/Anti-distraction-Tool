#include "TabWidget.h"
#include "WebBrowser.h"
#include <qtabbar.h>
#include <qurl.h>
#include <qwebenginehistory.h>

TabWidget::TabWidget(QWidget* parent) :
    QTabWidget(parent),
    defaultUrl("about:blank")
{
    QTabBar* tabBar = this->tabBar();
    tabBar->setTabsClosable(true);
    tabBar->setSelectionBehaviorOnRemove(QTabBar::SelectPreviousTab);
    tabBar->setMovable(true);
    tabBar->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(tabBar, &QTabBar::tabBarDoubleClicked, [this](int index) {
        if (index == -1)
            createTab();
        });
    connect(tabBar, &QTabBar::tabCloseRequested, this, &TabWidget::closeTab);
    connect(this, &TabWidget::currentChanged, this, &TabWidget::handleCurrentChanged);
}

void TabWidget::handleCurrentChanged(int index)
{
    if (index != -1) {
        WebBrowser* browser = webBrowser(index);
        if (!browser->url().isEmpty())
            browser->setFocus();
        emit urlChanged(browser->url());
    }
    else {
        emit urlChanged(QUrl());
    }
}

bool TabWidget::setUrl(const QString& urlStr) {
    QUrl url = QUrl::fromUserInput(urlStr);
    if (isVaildUrl(url)) {
        if (currentIndex() != -1) {
            webBrowser(currentIndex())->setUrl(url);
        }
        else {
            createTab();
            webBrowser(currentIndex())->setUrl(url);
        }
        return true;
    }
    return false;
}

void TabWidget::setupBrowser(WebBrowser* browser)
{
    connect(browser, &WebBrowser::titleChanged, [this, browser](const QString& title) {
        int index = this->indexOf(browser);
        if (index != -1) {
            setTabText(index, title);
            setTabToolTip(index, title);
        }
        });

    connect(browser, &WebBrowser::urlChanged, [this,browser](const QUrl& url) {
        int index = this->indexOf(browser);
        if (index != -1) {
            tabBar()->setTabData(index, url);
        }
        if (index == currentIndex()) {
            emit urlChanged(browser->url());
        }
        });

    connect(browser, &WebBrowser::iconChanged, [this, browser](const QIcon& icon) {
        int index = this->indexOf(browser);
        if (index != -1) {
            setTabIcon(index, icon);
        }
        });

    connect(browser, &WebBrowser::loadStarted, [this, browser] {
        qDebug() << browser->url().toString();
        if (!isVaildUrl(browser->url())) {
            if (browser->history()->canGoBack())
                browser->back();
            else closeTab(indexOf(browser));
        }
        });
    //一部分网页会在打开后跳转到另一个网页，故只用loadStarted会检测不到
    connect(browser, &WebBrowser::urlChanged, [this, browser](const QUrl& url) {
        if (!isVaildUrl(browser->url())) {
            if (browser->history()->canGoBack())
                browser->back();
            else closeTab(indexOf(browser));
        }
        });
}

WebBrowser* TabWidget::webBrowser(int index) const
{
    return qobject_cast<WebBrowser*>(widget(index));
}

bool TabWidget::isVaildUrl(const QUrl& url) const
{
    if (url.scheme().toLower() == "about")
        return true;
    if (allowedDomainsList.contains(url.host()))
        return true;

    return false;
}

WebBrowser* TabWidget::currentWebBrowser() const
{
    return webBrowser(currentIndex());
}

QUrl TabWidget::currentUrl() const { 
    WebBrowser* browser = currentWebBrowser();
    if (browser != nullptr)
        return browser->url();
    return QUrl();
}

void TabWidget::setDefaultUrl(const QString& urlStr)
{
    QUrl url = QUrl::fromUserInput(urlStr);
    defaultUrl = url;
}

WebBrowser* TabWidget::createTab()
{
    WebBrowser* browser = createBackgroundTab();
    setCurrentWidget(browser);
    return browser;
}

WebBrowser* TabWidget::createBackgroundTab()
{
    WebBrowser* browser = new WebBrowser;
    browser->setTabWidget(this);
    browser->setUrl(defaultUrl);
    
    int index = addTab(browser, tr("(Untitled)"));
    setupBrowser(browser);


    browser->resize(currentWidget()->size());
    browser->show();

    return browser;
}

void TabWidget::closeTab(int index)
{
    if (WebBrowser* view = webBrowser(index)) {
        bool hasFocus = view->hasFocus();
        removeTab(index);
        if (hasFocus && count() > 0)
            currentWebBrowser()->setFocus();
        if (count() == 0)
            createTab();
        view->deleteLater();
    }
}

void TabWidget::newTab()
{
    createTab();
}

void TabWidget::closeCurrentTab()
{
    closeTab(currentIndex());
}

void TabWidget::back()
{
    WebBrowser* browser = currentWebBrowser();
    if (browser != nullptr)
        browser->back();
}

void TabWidget::forward()
{
    WebBrowser* browser = currentWebBrowser();
    if (browser != nullptr)
        browser->forward();
}
