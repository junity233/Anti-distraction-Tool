#include "WebBrowser.h"

WebBrowser::WebBrowser(QWidget* parent) :
    QWebEngineView(parent)
{
    connect(page(), &QWebEnginePage::linkHovered, [this](const QString& url) {
        hoveredLink = url;
        });
}

QWebEngineView* WebBrowser::createWindow(QWebEnginePage::WebWindowType type)
{
    if (type == QWebEnginePage::WebBrowserTab)
    {
        setUrl(hoveredLink);
        return nullptr;
    }
    return this;
}
