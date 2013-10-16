#ifndef DOCKLISTVIEW_H
#define DOCKLISTVIEW_H

#include <QDockWidget>
#include "tableinfoelement.h"
#include <QTableWidgetItem>

namespace Ui {
class DockListView;
}

class DockListView : public QDockWidget
{
    Q_OBJECT
    
public:
    explicit DockListView(QWidget *parent = 0);
    ~DockListView();

    void populate(TableInfo List);

private slots:
    void on_tableWidget_doubleClicked(const QModelIndex &index);

    void on_tableWidget_customContextMenuRequested(const QPoint &pos);

private:
    Ui::DockListView *ui;
    bool m_empty;

    void EmptyTable(void);

signals:
    void DoubleClick(QString str);
    void Delete(QString str);
};

#endif // DOCKLISTVIEW_H
