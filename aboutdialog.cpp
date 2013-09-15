#include "aboutdialog.h"
#include "ui_aboutdialog.h"
#include "QMessageBox"

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
}

AboutDialog::~AboutDialog()
{
    delete ui;
}

void AboutDialog::SetAboutTxt(QString txt)
{
    QString title = tr("About ");
    title.append(txt);
    this->setWindowTitle(title);
    ui->AboutText->setText(txt);
}

void AboutDialog::SetIconAuthorTxt(QString iconAuthorTxt)
{
    this->iconAuthorTxt = iconAuthorTxt;
}

void AboutDialog::on_label_4_linkActivated(const QString &link)
{
    QMessageBox msg;
    msg.setWindowTitle(link);
    msg.setText(iconAuthorTxt);
    msg.exec();
}
