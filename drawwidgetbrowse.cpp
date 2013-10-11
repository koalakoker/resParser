#include "drawwidgetbrowse.h"
#include "ui_drawwidgetbrowse.h"
#include "datainspectorwidget.h"

DrawWidgetBrowse::DrawWidgetBrowse(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DrawWidgetBrowse)
{
    ui->setupUi(this);

    connect (ui->drawWidget,SIGNAL(cursorPositionChanged(int,float,float)),this,SLOT(cursorValueChanged(int,float,float)));
}

DrawWidgetBrowse::~DrawWidgetBrowse()
{
    delete ui;
}

DrawWidget* DrawWidgetBrowse::drawWidget(void)
{
    return (ui->drawWidget);
}

void DrawWidgetBrowse::setXmin(float value)
{
    ui->xmin->setValue(value);
}

void DrawWidgetBrowse::setYmin(float value)
{
    ui->ymin->setValue(value);
}

void DrawWidgetBrowse::setXmax(float value)
{
    ui->xmax->setValue(value);
}

void DrawWidgetBrowse::setYmax(float value)
{
    ui->ymax->setValue(value);
}

void DrawWidgetBrowse::checkAutoX(void)
{
    ui->autoXCheck->setCheckState(Qt::Checked);
}

void DrawWidgetBrowse::checkAutoY(void)
{
    ui->autoYCheck->setCheckState(Qt::Checked);
}

// Private

void DrawWidgetBrowse::setValueQuiet(QDoubleSpinBox *w, double value)
{
    bool signalState = w->signalsBlocked();
    w->blockSignals(true);
    w->setValue(value);
    w->blockSignals(signalState);
}

void DrawWidgetBrowse::on_autoXCheck_stateChanged(int arg1)
{
    if (arg1)
    {
        FPoint xrange = ui->drawWidget->getXRange();
        setValueQuiet(ui->xmin,xrange.x());
        setValueQuiet(ui->xmax,xrange.y());
        ui->drawWidget->m_xmin = xrange.x();
        ui->drawWidget->m_xmax = xrange.y();
        ui->drawWidget->repaint();
    }
}

void DrawWidgetBrowse::on_autoYCheck_stateChanged(int arg1)
{
    if (arg1)
    {
        FPoint yrange = ui->drawWidget->getYRange();
        setValueQuiet(ui->ymin,yrange.x());
        setValueQuiet(ui->ymax,yrange.y());
        ui->drawWidget->m_ymin = yrange.x();
        ui->drawWidget->m_ymax = yrange.y();
        ui->drawWidget->repaint();
    }
}

void DrawWidgetBrowse::on_ymin_valueChanged(double arg1)
{
    ui->drawWidget->m_ymin = arg1;
    ui->drawWidget->repaint();
    ui->autoYCheck->setCheckState(Qt::Unchecked);
}

void DrawWidgetBrowse::on_ymax_valueChanged(double arg1)
{
    ui->drawWidget->m_ymax = arg1;
    ui->drawWidget->repaint();
    ui->autoYCheck->setCheckState(Qt::Unchecked);
}

void DrawWidgetBrowse::on_xmin_valueChanged(double arg1)
{
    ui->drawWidget->m_xmin = arg1;
    ui->drawWidget->repaint();
    ui->autoXCheck->setCheckState(Qt::Unchecked);
}

void DrawWidgetBrowse::on_xmax_valueChanged(double arg1)
{
    ui->drawWidget->m_xmax = arg1;
    ui->drawWidget->repaint();
    ui->autoXCheck->setCheckState(Qt::Unchecked);
}

void DrawWidgetBrowse::on_cursor1x_valueChanged(double arg1)
{
    ui->drawWidget->setCursorX(0,arg1);
    ui->cursor1y->setValue(ui->drawWidget->CursorY(0));
    ui->drawWidget->repaint();
}

void DrawWidgetBrowse::on_cursor2x_valueChanged(double arg1)
{
    ui->drawWidget->setCursorX(1,arg1);
    ui->cursor2y->setValue(ui->drawWidget->CursorY(1));
    ui->drawWidget->repaint();
}

void DrawWidgetBrowse::cursorValueChanged(int cursor, float x, float y)
{
    if (cursor == 0)
    {
        ui->cursor1x->setValue(x);
        ui->cursor1y->setValue(y);
    }
    else if (cursor == 1)
    {
        ui->cursor2x->setValue(x);
        ui->cursor2y->setValue(y);
    }
}

void DrawWidgetBrowse::on_action_Data_triggered()
{
    DataInspectorWidget* d = new DataInspectorWidget;
    d->show();
    d->setDataPoints(ui->drawWidget->Points());
}
