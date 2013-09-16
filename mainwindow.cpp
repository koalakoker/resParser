#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QToolTip"
#include "aboutdialog.h"
#include <QDebug>

#define PRJ_NAME    "resParser"
#define PRJ_VERSION "0.1.1"
#define PRJ_WINDOWS_TITLE (QString("%1 - v%2").arg(QString(PRJ_NAME)).arg(QString(PRJ_VERSION)))

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowTitle(PRJ_WINDOWS_TITLE);

    QPalette p = ui->line_input->palette();
    p.setColor(QPalette::Text, Qt::darkGreen);
    ui->line_input->setPalette(p);
    ui->statusBar->showMessage(QString(tr("Type \"usage\" for help...\n")),10000);

    funcWin.functionListPopulate(cmdMngr.builtInFunctionList());

    connect (ui->line_input,SIGNAL(keyUpPressed()),this,SLOT(keyUpPress()));
    connect (ui->line_input,SIGNAL(keyDownPressed()),this,SLOT(keyDownPress()));
    connect (ui->line_input,SIGNAL(keyOperator()),this,SLOT(keyOperatorPress()));
    connect (ui->line_input,SIGNAL(keyEscPressed()),this,SLOT(close()));
}

MainWindow::~MainWindow()
{
    delete ui;
    funcWin.close();
}

void MainWindow::on_line_input_returnPressed()
{
    QString qsInput = ui->line_input->text();
    ui->output_pane->appendHtml(qsInput);
    ui->output_pane->appendHtml(cmdMngr.AddNewCommand(qsInput));
    ui->line_input->setText("");
}

void MainWindow::keyUpPress()
{
    ui->line_input->setText(cmdMngr.GetPreviousCommand());
}

void MainWindow::keyDownPress()
{
    ui->line_input->setText(cmdMngr.GetNextCommand());
}

void MainWindow::keyOperatorPress()
{
    if (ui->line_input->text().length()==0)
    {
        ui->line_input->setText("ans");
    }
}

void MainWindow::on_line_input_textChanged(const QString &arg1)
{
    hfloat result = cmdMngr.PreviewResult(arg1);
    if (!result.isNan())
    {
        QToolTip::showText(ui->line_input->mapToGlobal(QPoint(10,-45)),QString("ans=") + result.toString());
    }
    else
    {
        QToolTip::showText(ui->line_input->mapToGlobal(QPoint(10,-45)),QString(""));
    }
}

void MainWindow::on_actionFunctions_toggled(bool arg1)
{
    if (arg1)
    {
        funcWin.move(this->pos().x()+this->size().width()+20,this->pos().y()+10);
        funcWin.show();
    }
    else
    {
        funcWin.hide();
    }
}

void MainWindow::on_actionAbout_activated()
{
    AboutDialog diag;
    diag.SetAboutTxt(PRJ_WINDOWS_TITLE);
    diag.exec();
}
