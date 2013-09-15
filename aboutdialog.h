#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>

namespace Ui {
class AboutDialog;
}

class AboutDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit AboutDialog(QWidget *parent = 0);
    ~AboutDialog();
    void SetAboutTxt(QString txt);
    void SetIconAuthorTxt(QString iconAuthorTxt);
        
private slots:
    void on_label_4_linkActivated(const QString &link);

private:
    Ui::AboutDialog *ui;
    QString iconAuthorTxt;
};

#endif // ABOUTDIALOG_H
