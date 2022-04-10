#include "FocusBrowser.h"
#include <qlayout.h>
#include <qregexp.h>


FocusBrowser::FocusBrowser(QWidget* parent):
	QWidget(parent)
{
	
	browser = new WebBrowser(this);
	urlEdit = new QLineEdit(this);
	finishedButton = new QPushButton(tr("GO"), this);
	backButton = new QPushButton(tr("Back"), this);
	aheadButton = new QPushButton(tr("Ahead"), this);


	QVBoxLayout* mainLayout = new QVBoxLayout(this);
	QHBoxLayout* urlLayout = new QHBoxLayout(this);

	urlLayout->addWidget(backButton);
	urlLayout->addWidget(aheadButton);
	urlLayout->addWidget(urlEdit);
	urlLayout->addWidget(finishedButton);

	mainLayout->addLayout(urlLayout);
	mainLayout->addWidget(browser);

	this->setLayout(mainLayout);

	browser->setUrl(QStringLiteral("about:blank"));
	
	connect(browser, &WebBrowser::urlChanged, this, &FocusBrowser::onBrowserUrlChanged);
	connect(urlEdit, &QLineEdit::returnPressed, this, &FocusBrowser::goInputUrl);
	connect(finishedButton, &QPushButton::clicked, this, &FocusBrowser::goInputUrl);
	connect(backButton, &QPushButton::clicked, browser, &WebBrowser::back);
	connect(aheadButton, &QPushButton::clicked, browser, &WebBrowser::forward);
	
	setWindowFlag(Qt::Window, true);
}

bool FocusBrowser::setUrl(const QString & urlStr)
{
	QUrl url = QUrl::fromUserInput(urlStr);
	if (allowedDomains.contains(url.host())) {
		browser->setUrl(url);
		return true;
	}
	return false;
}

void FocusBrowser::onBrowserUrlChanged(const QUrl& url)
{
	if (!allowedDomains.contains(url.host())) {
		browser->back();
	}
	urlEdit->setText(url.toDisplayString());
}
void FocusBrowser::goInputUrl()
{
	if (!setUrl(urlEdit->text())) {
		urlEdit->setText(browser->url().toDisplayString());
	}
}
