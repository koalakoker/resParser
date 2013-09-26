#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QToolTip"
#include "aboutdialog.h"
#include <QDebug>

#define PRJ_NAME    "resParser"
#define PRJ_VERSION "0.1.0"
#define PRJ_WINDOWS_TITLE (QString("%1 - v%2").arg(QString(PRJ_NAME)).arg(QString(PRJ_VERSION)))
#define HYST_FILENAME "hyst.rp"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    updateOutputFormatSetting();

    this->setWindowTitle(PRJ_WINDOWS_TITLE);

    QPalette p = ui->line_input->palette();
    p.setColor(QPalette::Text, Qt::darkGreen);
    ui->line_input->setPalette(p);
    ui->statusBar->showMessage(QString(tr("Type \"usage\" for help...\n")),10000);

    m_funcWin.functionListPopulate(m_cmdMngr.BuiltInFunctionList());

    connect (ui->line_input,SIGNAL(keyUpPressed()),this,SLOT(keyUpPress()));
    connect (ui->line_input,SIGNAL(keyDownPressed()),this,SLOT(keyDownPress()));
    connect (ui->line_input,SIGNAL(keyOperator()),this,SLOT(keyOperatorPress()));
    connect (ui->line_input,SIGNAL(keyEscPressed()),this,SLOT(close()));
    connect (m_cmdMngr.Parser(),SIGNAL(functionListUpdate(QStringList)),&m_funcWin,SLOT(functionListPopulate(QStringList)));

    on_action_Load_triggered();
}

MainWindow::~MainWindow()
{
    delete ui;
    m_funcWin.close();
}

void MainWindow::on_line_input_returnPressed()
{
    QString qsInput = ui->line_input->text();
    ui->output_pane->appendHtml(m_cmdMngr.AddNewCommand(qsInput));
    ui->line_input->setText("");
}

void MainWindow::keyUpPress()
{
    ui->line_input->setText(m_cmdMngr.GetPreviousCommand());
}

void MainWindow::keyDownPress()
{
    ui->line_input->setText(m_cmdMngr.GetNextCommand());
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
    QToolTip::showText(ui->line_input->mapToGlobal(QPoint(10,-45)),m_cmdMngr.PreviewResult(arg1));
}

void MainWindow::on_actionFunctions_toggled(bool arg1)
{
    if (arg1)
    {
        m_funcWin.functionListPopulate(m_cmdMngr.BuiltInFunctionList());
        m_funcWin.move(this->pos().x()+this->size().width()+20,this->pos().y()+10);
        m_funcWin.show();
    }
    else
    {
        m_funcWin.hide();
    }
}

void MainWindow::on_actionAbout_activated()
{
    AboutDialog diag;
    diag.SetAboutTxt(PRJ_WINDOWS_TITLE);
    diag.exec();
}

void MainWindow::on_actionAuto_toggled(bool arg1)
{
    if (arg1)
    {
        m_cmdMngr.SetFormat(Auto);
    }
    updateOutputFormatSetting();
    ui->statusBar->showMessage(QString(tr("Switched to Decimal (Auto)")),10000);
}

void MainWindow::on_actionFloating_toggled(bool arg1)
{
    if (arg1)
    {
        m_cmdMngr.SetFormat(Fixed);
    }
    updateOutputFormatSetting();
    ui->statusBar->showMessage(QString(tr("Switched to Decimal (Floating)")),10000);
}

void MainWindow::on_actionScientific_toggled(bool arg1)
{
    if (arg1)
    {
        m_cmdMngr.SetFormat(Scientific);
    }
    updateOutputFormatSetting();
    ui->statusBar->showMessage(QString(tr("Switched to Decimal (Scientific)")),10000);
}

void MainWindow::on_actionHexadecimal_toggled(bool arg1)
{
    if (arg1)
    {
        m_cmdMngr.SetFormat(Hexadecimal);
    }
    updateOutputFormatSetting();
    ui->statusBar->showMessage(QString(tr("Switched to Hexadecimal")),10000);
}


void MainWindow::updateOutputFormatSetting(void)
{
    formatOutput_t format = m_cmdMngr.Format();
    ui->actionAuto->setChecked(format == Auto);
    ui->actionFloating->setChecked(format == Fixed);
    ui->actionScientific->setChecked(format == Scientific);
    ui->actionHexadecimal->setChecked(format == Hexadecimal);
    updateOutputPaneAndPreview();

}

void MainWindow::updateOutputPaneAndPreview(void)
{
    ui->output_pane->clear();
    ui->output_pane->appendHtml(m_cmdMngr.OutputPaneReprint());
    on_line_input_textChanged(ui->line_input->text());
}

void MainWindow::on_action_Save_triggered()
{
    m_cmdMngr.Save(HYST_FILENAME);
}

void MainWindow::on_action_Load_triggered()
{
    m_cmdMngr.Load(HYST_FILENAME);
    updateOutputPaneAndPreview();
}

void MainWindow::closeEvent (QCloseEvent* event)
{
    on_action_Save_triggered();
    event->setAccepted(true);
}
