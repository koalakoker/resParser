#ifndef DATAINSPECTORWIDGET_H
#define DATAINSPECTORWIDGET_H

#include <QDialog>
#include "fpoint.h"

namespace Ui {
class DataInspectorWidget;
}

class DataInspectorWidget : public QDialog
{
    Q_OBJECT
    
public:
    explicit DataInspectorWidget(QWidget *parent = 0);
    ~DataInspectorWidget();
    void setDataPoints(QVector<HPoint>* pData);
    
private:
    Ui::DataInspectorWidget *ui;
    QVector<HPoint>* m_points;
};

#endif // DATAINSPECTORWIDGET_H
