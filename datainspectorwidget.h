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
    
private slots:
    void on_tableWidget_customContextMenuRequested(const QPoint &pos);

private:
    Ui::DataInspectorWidget *ui;
    QVector<HPoint>* m_points;

private:
    void UpdateTable(void);
};

#endif // DATAINSPECTORWIDGET_H
