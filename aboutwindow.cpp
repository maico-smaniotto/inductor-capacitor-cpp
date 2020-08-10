#include "aboutwindow.h"
#include "ui_aboutwindow.h"
#include <QDesktopServices>
#include <QUrl>

AboutWindow::AboutWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutWindow)
{
    ui->setupUi(this);
    setFixedSize(width(), height());

    // Connects the button clicked() signal to the inherited close() slot
    connect(ui->btClose, SIGNAL(clicked()), this, SLOT(close()));
}

AboutWindow::~AboutWindow()
{
    delete ui;
}

void AboutWindow::on_lbContactEmail_linkActivated(const QString &link)
{
    QDesktopServices::openUrl("mailto:" + link + "?subject=" + ui->lbProgramTitle->text() + "&body=");
}

void AboutWindow::on_lbProjectLink_linkActivated(const QString &link)
{
    QDesktopServices::openUrl(link);
}
