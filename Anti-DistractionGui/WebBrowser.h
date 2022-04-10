#pragma once
#include <QtWebEngineWidgets\qwebengineview.h>
#include <qtabbar.h>

class TabWidget;

class WebBrowser :
    public QWebEngineView
{
    Q_OBJECT
public:
    explicit WebBrowser(QWidget* parent=Q_NULLPTR);

    void setTabWidget(TabWidget* widget) { tabWidget = widget; }

protected:
    QWebEngineView* createWindow(QWebEnginePage::WebWindowType type);

private:
    QUrl hoveredLink;
    TabWidget* tabWidget = nullptr;
};