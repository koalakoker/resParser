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

    void populate(QList<TableInfoElement> List);

private:
    Ui::DockListView *ui;
    bool m_empty;

    void EmptyTable(void);
};

#endif // DOCKLISTVIEW_H
