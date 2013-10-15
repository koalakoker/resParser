#include "drawwidgetbrowse.h"
#include "ui_drawwidgetbrowse.h"
#include "datainspectorwidget.h"
#include "math.h"

DrawWidgetBrowse::DrawWidgetBrowse(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DrawWidgetBrowse)
{
    ui->setupUi(this);

    ui->actionLabel_X_axe->setChecked(true);
    ui->actionShow_labels_Y_axe->setChecked(true);
    ui->actionShow_grid_orizzontal->setChecked(true);
    ui->actionShow_grid_vertical->setChecked(true);

    connect (ui->drawWidget,SIGNAL(cursorPositionChanged(int,hfloat,hfloat)),this,SLOT(cursorValueChanged(int,hfloat,hfloat)));
    connect (ui->xmin,SIGNAL(userStepBy(int)),this,SLOT(spin_xmin(int)));
    connect (ui->ymin,SIGNAL(userStepBy(int)),this,SLOT(spin_ymin(int)));
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
        HPoint xrange = ui->drawWidget->getXRange();
        setValueQuiet(ui->xmin,xrange.x().toFloat());
        setValueQuiet(ui->xmax,xrange.y().toFloat());
        ui->drawWidget->setXmin(xrange.x());
        ui->drawWidget->setXmax(xrange.y());
        ui->drawWidget->repaint();
    }
}

void DrawWidgetBrowse::on_autoYCheck_stateChanged(int arg1)
{
    if (arg1)
    {
        HPoint yrange = ui->drawWidget->getYRange();
        setValueQuiet(ui->ymin,yrange.x().toFloat());
        setValueQuiet(ui->ymax,yrange.y().toFloat());
        ui->drawWidget->setYmin(yrange.x());
        ui->drawWidget->setYmax(yrange.y());
        ui->drawWidget->repaint();
    }
}

void DrawWidgetBrowse::on_ymin_valueChanged(double arg1)
{
    ui->drawWidget->setYmin(arg1);
    ui->drawWidget->repaint();
    ui->autoYCheck->setCheckState(Qt::Unchecked);
    ui->ymin->setSingleStep(fabs(arg1)/10);
}

void DrawWidgetBrowse::on_ymax_valueChanged(double arg1)
{
    ui->drawWidget->setYmax(arg1);
    ui->drawWidget->repaint();
    ui->autoYCheck->setCheckState(Qt::Unchecked);
    ui->ymax->setSingleStep(fabs(arg1)/10);
}

void DrawWidgetBrowse::on_xmin_valueChanged(double arg1)
{
    ui->drawWidget->setXmin(arg1);
    ui->drawWidget->repaint();
    ui->autoXCheck->setCheckState(Qt::Unchecked);
    ui->xmin->setSingleStep(fabs(arg1)/10);
}

void DrawWidgetBrowse::on_xmax_valueChanged(double arg1)
{
    ui->drawWidget->setXmax(arg1);
    ui->drawWidget->repaint();
    ui->autoXCheck->setCheckState(Qt::Unchecked);
    ui->xmax->setSingleStep(fabs(arg1)/10);
}

void DrawWidgetBrowse::on_cursor1x_valueChanged(double arg1)
{
    ui->drawWidget->setCursorX(0,arg1);
    ui->cursor1y->setValue(ui->drawWidget->CursorY(0).toFloat());
    ui->drawWidget->repaint();
}

void DrawWidgetBrowse::on_cursor2x_valueChanged(double arg1)
{
    ui->drawWidget->setCursorX(1,arg1);
    ui->cursor2y->setValue(ui->drawWidget->CursorY(1).toFloat());
    ui->drawWidget->repaint();
}

void DrawWidgetBrowse::cursorValueChanged(int cursor, hfloat x, hfloat y)
{
    if (cursor == 0)
    {
        ui->cursor1x->setValue(x.toFloat());
        ui->cursor1y->setValue(y.toFloat());
    }
    else if (cursor == 1)
    {
        ui->cursor2x->setValue(x.toFloat());
        ui->cursor2y->setValue(y.toFloat());
    }
}

void DrawWidgetBrowse::on_action_Data_triggered()
{
    DataInspectorWidget* d = new DataInspectorWidget;
    d->show();
    d->setDataPoints(ui->drawWidget->Points());
}

void DrawWidgetBrowse::spin_ymin(int step)
{
    if (step > 0)
    {
        ui->ymax->stepDown();
    }
    else if (step < 0)
    {
        ui->ymax->stepUp();
    }
}

void DrawWidgetBrowse::spin_xmin(int step)
{
    if (step > 0)
    {
        ui->xmax->stepDown();
    }
    else if (step < 0)
    {
        ui->xmax->stepUp();
    }
}

void DrawWidgetBrowse::on_actionHiglight_Points_toggled(bool arg1)
{
    ui->drawWidget->setHighlightPoints(arg1);
}

void DrawWidgetBrowse::on_actionLabel_X_axe_toggled(bool arg1)
{
    ui->drawWidget->setShowLabelX(arg1);
}

void DrawWidgetBrowse::on_actionShow_labels_Y_axe_toggled(bool arg1)
{
    ui->drawWidget->setShowLabelY(arg1);
}

void DrawWidgetBrowse::on_actionShow_grid_orizzontal_toggled(bool arg1)
{
    ui->drawWidget->setShowGridOrizzontal(arg1);
}

void DrawWidgetBrowse::on_actionShow_grid_vertical_toggled(bool arg1)
{
    ui->drawWidget->setShowGridVertical(arg1);
}
