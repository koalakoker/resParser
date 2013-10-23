#ifndef DATAINSPECTORWIDGET_H
#define DATAINSPECTORWIDGET_H

#include <QDialog>
#include "fpoint.h"
#include "rawdata.h"

namespace Ui {
class DataInspectorWidget;
}

class DataInspectorWidget : public QDialog
{
    Q_OBJECT
    
public:
    explicit DataInspectorWidget(QWidget *parent = 0);
    ~DataInspectorWidget();
    void setDataPoints(RawData* pData);
    
private slots:
    void on_tableWidget_customContextMenuRequested(const QPoint &pos);

private:
    Ui::DataInspectorWidget *ui;
    RawData* m_points;

private:
    void UpdateTable(void);

signals:
    void dataChanged(Range r);
};

#endif // DATAINSPECTORWIDGET_H
