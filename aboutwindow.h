#ifndef ABOUTWINDOW_H
#define ABOUTWINDOW_H

#include <QDialog>

namespace Ui {
class AboutWindow;
}

class AboutWindow : public QDialog
{
    Q_OBJECT

public:
    explicit AboutWindow(QWidget *parent = nullptr);
    ~AboutWindow();

private slots:
    void on_lbContactEmail_linkActivated(const QString &link);

    void on_lbProjectLink_linkActivated(const QString &link);

private:
    Ui::AboutWindow *ui;
};

#endif // ABOUTWINDOW_H
