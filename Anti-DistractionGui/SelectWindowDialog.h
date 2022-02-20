#pragma once
#include <qdialog.h>
#include "ui_SelectWindowDialog.h"
#include <Windows.h>

class SelectWindowDialog :
    public QDialog
{
    Q_OBJECT
public:
    SelectWindowDialog(QWidget* parent = Q_NULLPTR);

    static HWND SelectWindow(QWidget* parent = Q_NULLPTR);
    static QString GetWindowTitle(HWND hwnd);

private slots:

    void OnCancelButtonClicked() { hWindow = NULL; close(); }
    void OnConfirmButtonClicked() { close(); }
    void OnSelectWindowButtonClicked();
private:
    Ui::WindowSelectDialog ui;

    HWND hWindow = NULL;
};

