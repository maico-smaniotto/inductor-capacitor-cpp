#include "helpwindow.h"
#include "ui_helpwindow.h"

HelpWindow *HelpWindow::helpWindow = nullptr;

HelpWindow::HelpWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HelpWindow)
{
    ui->setupUi(this);

    setFixedSize(width(), height());

    // Connects the button clicked() signal to the inherited close() slot
    connect(ui->btClose, SIGNAL(clicked()), this, SLOT(close()));
}

HelpWindow::~HelpWindow()
{
    helpWindow = nullptr;
    delete ui;
}
