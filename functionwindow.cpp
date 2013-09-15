#include "functionwindow.h"
#include "ui_functionwindow.h"

FunctionWindow::FunctionWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FunctionWindow)
{
    ui->setupUi(this);
}

FunctionWindow::~FunctionWindow()
{
    delete ui;
}

void FunctionWindow::functionListPopulate(QStringList list)
{
    ui->functionsList->addItems(list);
}
