#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QToolTip"
#include "aboutdialog.h"
#include <QDebug>

#define PRJ_NAME    "resParser"
#define PRJ_VERSION "0.1.3"
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

    connect (ui->line_input,SIGNAL(keyUpPressed()),this,SLOT(keyUpPress()));
    connect (ui->line_input,SIGNAL(keyDownPressed()),this,SLOT(keyDownPress()));
    connect (ui->line_input,SIGNAL(keyOperator()),this,SLOT(keyOperatorPress()));
    connect (ui->line_input,SIGNAL(keyEscPressed()),this,SLOT(close()));
    connect (m_cmdMngr.Parser(),SIGNAL(variablesUpdate()),this,SLOT(variableUpdates()));
    connect (m_cmdMngr.Parser(),SIGNAL(functionListUpdate(QStringList)),this,SLOT(userDefinedFunctionUpdates()));

    on_action_Load_triggered();

    // Prepare dock
    addDockWidget(Qt::RightDockWidgetArea,&m_dockListVariables,Qt::Vertical);
    tabifyDockWidget(&m_dockListVariables,&m_dockListUserDefinedFunctions);
    tabifyDockWidget(&m_dockListVariables,&m_dockListBuiltinFunctions);
    m_dockListVariables.setVisible(false);
    m_dockListUserDefinedFunctions.setVisible(false);
    m_dockListBuiltinFunctions.setVisible(false);
    m_isExtended = false;

    m_dockListVariables.setWindowTitle("Variables");
    m_dockListUserDefinedFunctions.setWindowTitle("User functions");
    m_dockListBuiltinFunctions.setWindowTitle("Built-in functions");

    m_dockListVariables.populate(m_cmdMngr.VariableInfo());
    m_dockListBuiltinFunctions.populate(m_cmdMngr.BuiltInFunctionInfo());
    m_dockListUserDefinedFunctions.populate(m_cmdMngr.UserFunctionInfo());

    connect (&m_dockListVariables,SIGNAL(visibilityChanged(bool)),this,SLOT(dockVisibilityChange(bool)));
    connect (&m_dockListVariables,SIGNAL(DoubleClick(QString)),this,SLOT(addStrToLineInput(QString)));
    connect (&m_dockListVariables,SIGNAL(Delete(QString)),this,SLOT(clearSelected(QString)));
    connect (&m_dockListUserDefinedFunctions,SIGNAL(visibilityChanged(bool)),this,SLOT(dockVisibilityChange(bool)));
    connect (&m_dockListUserDefinedFunctions,SIGNAL(DoubleClick(QString)),this,SLOT(addStrToLineInput(QString)));
    connect (&m_dockListUserDefinedFunctions,SIGNAL(Delete(QString)),this,SLOT(clearSelected(QString)));
    connect (&m_dockListBuiltinFunctions,SIGNAL(visibilityChanged(bool)),this,SLOT(dockVisibilityChange(bool)));
    connect (&m_dockListBuiltinFunctions,SIGNAL(DoubleClick(QString)),this,SLOT(addStrToLineInput(QString)));
}

void MainWindow::variableUpdates(void)
{
    m_dockListVariables.populate(m_cmdMngr.VariableInfo());
}

void MainWindow::userDefinedFunctionUpdates(void)
{
    m_dockListUserDefinedFunctions.populate(m_cmdMngr.UserFunctionInfo());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent (QCloseEvent* event)
{
    on_action_Save_triggered();
    event->setAccepted(true);
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
        m_cmdMngr.Parser()->SetFormat(Auto);
    }
    updateOutputFormatSetting();
    ui->statusBar->showMessage(QString(tr("Switched to Decimal (Auto)")),10000);
}

void MainWindow::on_actionFloating_toggled(bool arg1)
{
    if (arg1)
    {
        m_cmdMngr.Parser()->SetFormat(Fixed);
    }
    updateOutputFormatSetting();
    ui->statusBar->showMessage(QString(tr("Switched to Decimal (Floating)")),10000);
}

void MainWindow::on_actionScientific_toggled(bool arg1)
{
    if (arg1)
    {
        m_cmdMngr.Parser()->SetFormat(Scientific);
    }
    updateOutputFormatSetting();
    ui->statusBar->showMessage(QString(tr("Switched to Decimal (Scientific)")),10000);
}

void MainWindow::on_actionHexadecimal_toggled(bool arg1)
{
    if (arg1)
    {
        m_cmdMngr.Parser()->SetFormat(Hexadecimal);
    }
    updateOutputFormatSetting();
    ui->statusBar->showMessage(QString(tr("Switched to Hexadecimal")),10000);
}


void MainWindow::updateOutputFormatSetting(void)
{
    formatOutput_t format = m_cmdMngr.Parser()->Format();
    ui->actionAuto->setChecked(format == Auto);
    ui->actionFloating->setChecked(format == Fixed);
    ui->actionScientific->setChecked(format == Scientific);
    ui->actionHexadecimal->setChecked(format == Hexadecimal);
    updateOutputPaneAndPreview();
    variableUpdates();
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

void MainWindow::dockVisibilityChange(bool visible)
{
    visible = (m_dockListVariables.isVisible()||m_dockListUserDefinedFunctions.isVisible()||m_dockListBuiltinFunctions.isVisible());
    if (visible)
    {
        if (!m_isExtended)
        {
            QRect rect = geometry();
            rect.setWidth(rect.width()+m_dockListVariables.geometry().width());
            setGeometry(rect);
            m_isExtended = true;
        }
    }
    else
    {
        if (m_isExtended)
        {
            QRect rect = geometry();
            rect.setWidth(rect.width()-m_dockListVariables.geometry().width());
            setGeometry(rect);
            m_isExtended = false;
        }
    }
}

void MainWindow::addStrToLineInput(QString str)
{
    ui->line_input->insert(str);
    ui->line_input->setFocus();
}

void MainWindow::on_actionVariables_toggled(bool arg1)
{
    m_dockListVariables.setVisible(arg1);
    m_dockListVariables.raise();
}

void MainWindow::on_actionFunctions_toggled(bool arg1)
{
    m_dockListBuiltinFunctions.setVisible(arg1);
    m_dockListBuiltinFunctions.raise();
}

void MainWindow::on_actionUser_Functions_toggled(bool arg1)
{
    m_dockListUserDefinedFunctions.setVisible(arg1);
    m_dockListUserDefinedFunctions.raise();
}

void MainWindow::clearSelected(QString str)
{
    m_cmdMngr.AddNewCommand(QString("clear %1").arg(str));
}
