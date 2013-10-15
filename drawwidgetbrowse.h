#ifndef DRAWWIDGETBROWSE_H
#define DRAWWIDGETBROWSE_H

#include <QMainWindow>
#include <QDoubleSpinBox>
#include "fpoint.h"
#include "drawwidget.h"

namespace Ui {
class DrawWidgetBrowse;
}

class DrawWidgetBrowse : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit DrawWidgetBrowse(QWidget *parent = 0);
    ~DrawWidgetBrowse();

    DrawWidget* drawWidget(void);
    void setXmin(float value);
    void setYmin(float value);
    void setXmax(float value);
    void setYmax(float value);
    void checkAutoX(void);
    void checkAutoY(void);
    
private slots:
    void on_autoXCheck_stateChanged(int arg1);
    void on_autoYCheck_stateChanged(int arg1);
    void on_ymin_valueChanged(double arg1);
    void on_ymax_valueChanged(double arg1);
    void on_xmin_valueChanged(double arg1);
    void on_xmax_valueChanged(double arg1);
    void on_cursor1x_valueChanged(double arg1);
    void on_cursor2x_valueChanged(double arg1);
    void on_action_Data_triggered();

    void spin_ymin(int step);
    void spin_xmin(int step);

    void cursorValueChanged(int cursor, hfloat x, hfloat y);

    void on_actionHiglight_Points_toggled(bool arg1);

    void on_actionLabel_X_axe_toggled(bool arg1);

    void on_actionShow_labels_Y_axe_toggled(bool arg1);

    void on_actionShow_grid_orizzontal_toggled(bool arg1);

    void on_actionShow_grid_vertical_toggled(bool arg1);

private:
    Ui::DrawWidgetBrowse *ui;
    static void setValueQuiet(QDoubleSpinBox* w,double value);
};

#endif // DRAWWIDGETBROWSE_H
