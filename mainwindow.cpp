#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    parser = ParserClass();
    cmdMngr = CommandMngrClass();

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
    ui->output_pane->appendPlainText(qsInput);

    cmdMngr.AddNewCommand(qsInput);

    if ((qsInput.contains("list"))||(qsInput.contains("ls")))
    {
        int i;
        for (i = 0; i < parser.VariableCreated(); i++)
        {
            Variable var = parser.GetVariableAtIndex(i);
            ui->output_pane->appendPlainText(var.ToString());
        }
        ui->output_pane->appendPlainText("");
    }
    else if (qsInput.contains("clear"))
    {
        parser.Clear();
        ui->output_pane->clear();
    }
    else if (qsInput == "E12")
    {
        ui->output_pane->appendPlainText(Resistor::E12ValuesToString());
        ui->output_pane->appendPlainText("");
    }
    else if (qsInput == "E24")
    {
        ui->output_pane->appendPlainText(Resistor::E24ValuesToString());
        ui->output_pane->appendPlainText("");
    }
    else if (qsInput.toLower() == "usage")
    {
        ui->output_pane->appendPlainText("list or ls to show varaibles.");
        ui->output_pane->appendPlainText("clear to delete all varaibles.");
        ui->output_pane->appendPlainText("E12 to show all E12 resitor values.");
        ui->output_pane->appendPlainText("E24 to show all E24 resitor values.");
        ui->output_pane->appendPlainText("->E12 to round to nearest E12 resitor values.");
        ui->output_pane->appendPlainText(": parallel operator.\n");
    }
    else
    {
        hfloat retVal = parser.Parse(qsInput);
        ui->output_pane->appendPlainText(QString("ans=%1\n").arg(retVal.toString()));
    }

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
