#include "FocusBrowser.h"
#include <qlayout.h>
#include <qregexp.h>
#include <qapplication.h>
#include <qdesktopwidget.h>
#include <qevent.h>
#include <qaction.h>
#include "TabWidget.h"
#include <qtabbar.h>

FocusBrowser::FocusBrowser(QWidget* parent):
	QWidget(parent)
{
	
	tabWidget = new TabWidget(this);
	urlEdit = new QLineEdit(this);
	finishedButton = new QPushButton(tr("GO"), this);
	backButton = new QPushButton(tr("Back"), this);
	forwardButton = new QPushButton(tr("Forward"), this);
	newPageButton = new QPushButton(tr("New"), this);

	QVBoxLayout* mainLayout = new QVBoxLayout(this);
	QHBoxLayout* urlLayout = new QHBoxLayout(this);

	urlLayout->addWidget(backButton);
	urlLayout->addWidget(forwardButton);
	urlLayout->addWidget(newPageButton);
	urlLayout->addWidget(urlEdit);
	urlLayout->addWidget(finishedButton);

	mainLayout->addLayout(urlLayout);
	mainLayout->addWidget(tabWidget);

	this->setLayout(mainLayout);

	QAction* newPageAction = new QAction(this);
	QAction* closePageAction = new QAction(this);

	newPageAction->setShortcut(QKeySequence::AddTab);
	closePageAction->setShortcut(QKeySequence::Close);

	
	connect(newPageButton, &QPushButton::clicked, tabWidget, &TabWidget::createTab);
	connect(urlEdit, &QLineEdit::returnPressed, this, &FocusBrowser::goInputUrl);
	connect(finishedButton, &QPushButton::clicked, this, &FocusBrowser::goInputUrl);
	connect(backButton, &QPushButton::clicked, tabWidget, &TabWidget::back);
	connect(forwardButton, &QPushButton::clicked, tabWidget, &TabWidget::forward);
	connect(tabWidget, &TabWidget::urlChanged, [this](const QUrl& url) {
		urlEdit->setText(url.toDisplayString());
		});
	connect(newPageAction, &QAction::triggered, tabWidget, &TabWidget::newTab);
	connect(closePageAction, &QAction::triggered, tabWidget, &TabWidget::closeCurrentTab);
	
	setWindowFlag(Qt::Window, true);
	setWindowFlag(Qt::WindowStaysOnTopHint);
	setWindowFlag(Qt::FramelessWindowHint, true);

	tabWidget->tabBar()->addAction(closePageAction);
	tabWidget->tabBar()->addAction(newPageAction);

	tabWidget->createTab();
}

void FocusBrowser::closeEvent(QCloseEvent* event)
{
	event->ignore();
}

void FocusBrowser::goInputUrl()
{
	if (!tabWidget->setUrl(urlEdit->text()))
		urlEdit->setText(tabWidget->currentUrl().toDisplayString());
}
