#pragma once
#include <QtWebEngineWidgets\qwebengineview.h>

class WebBrowser :
    public QWebEngineView
{
    Q_OBJECT
public:
    explicit WebBrowser(QWidget* parent=Q_NULLPTR);

protected:
    QWebEngineView* createWindow(QWebEnginePage::WebWindowType type);

private:
    QUrl hoveredLink;
};

