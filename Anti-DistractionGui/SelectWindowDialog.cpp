#include "SelectWindowDialog.h"
#include <qdatetime.h>

static HWND getCursorWindow() {
	POINT p;
	GetCursorPos(&p);
	return WindowFromPoint(p);
}

SelectWindowDialog::SelectWindowDialog(QWidget* parent) :
	QDialog(parent)
{
	ui.setupUi(this);

}

void SelectWindowDialog::OnSelectWindowButtonClicked()
{
	Sleep(3000);
	hWindow = getCursorWindow();


	ui.handleLineEdit->setText("0x" + QString::number((qulonglong)hWindow, 16));
	ui.titleLineEdit->setText(GetWindowTitle(hWindow));

}

HWND SelectWindowDialog::SelectWindow(QWidget* parent)
{
	SelectWindowDialog dialog(parent);
	dialog.exec();
	return dialog.hWindow;
}

QString SelectWindowDialog::GetWindowTitle(HWND hwnd)
{

	WCHAR titleBuf[1024];
	GetWindowText(hwnd, titleBuf, 1024);

	return QString::fromWCharArray(titleBuf);
}
