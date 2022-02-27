#pragma once
#include <qdialog.h>
#include "ui_SelectWindowDialog.h"
#include <Windows.h>
#include <qevent.h>

class SelectWindowDialog :
    public QDialog
{
    Q_OBJECT
public:
    SelectWindowDialog(QWidget* parent = Q_NULLPTR);

    static HWND SelectWindow(QWidget* parent = Q_NULLPTR);
    static QString GetWindowTitle(HWND hwnd);

private slots:

    void OnCancelButtonClicked() { hWindow = NULL; this->close(); }
    void OnConfirmButtonClicked() { is_confirm = true; close(); }
    void OnSelectWindowButtonClicked();
private:
    Ui::WindowSelectDialog ui;

    bool is_confirm = false;
    HWND hWindow = NULL;

protected:
    void closeEvent(QCloseEvent* event) {
        if(!is_confirm)
            hWindow = NULL; 
    }
};

