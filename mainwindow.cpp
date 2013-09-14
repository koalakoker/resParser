#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QPalette p = ui->line_input->palette();
    p.setColor(QPalette::Text, Qt::darkGreen);
    ui->line_input->setPalette(p);
    ui->statusBar->showMessage(QString(tr("Type \"usage\" for help...\n")),10000);

    connect (ui->line_input,SIGNAL(keyUpPressed()),this,SLOT(keyUpPress()));
    connect (ui->line_input,SIGNAL(keyDownPressed()),this,SLOT(keyDownPress()));
    connect (ui->line_input,SIGNAL(keyOperator()),this,SLOT(keyOperatorPress()));
    connect (ui->line_input,SIGNAL(keyEscPressed()),this,SLOT(close()));
}

MainWindow::~MainWindow()
{
    delete ui;
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
