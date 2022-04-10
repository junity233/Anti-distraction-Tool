#include "WebBrowser.h"
#include "TabWidget.h"

WebBrowser::WebBrowser(QWidget* parent) :
    QWebEngineView(parent)
{
    connect(page(), &QWebEnginePage::linkHovered, [this](const QString& url) {
        hoveredLink = url;
        });
}

QWebEngineView* WebBrowser::createWindow(QWebEnginePage::WebWindowType type)
{
    switch (type) {
    case QWebEnginePage::WebBrowserBackgroundTab:
        return tabWidget->createBackgroundTab();
    case QWebEnginePage::WebBrowserTab:
        return tabWidget->createTab();
    case QWebEnginePage::WebBrowserWindow:
        return this;
    }
    return nullptr;
}
