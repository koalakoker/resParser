#include "docklistview.h"
#include "ui_docklistview.h"
#include <QMenu>
#include <QDebug>

DockListView::DockListView(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::DockListView)
{
    ui->setupUi(this);
    m_empty = true;

    ui->tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
}

DockListView::~DockListView()
{
    delete ui;
}

void DockListView::populate(TableInfo List)
{
    if (!m_empty)
    {
        EmptyTable();
    }
    ui->tableWidget->setRowCount(List.row());
    ui->tableWidget->setColumnCount(List.column());
    int i;
    for (i = 0; i < List.row(); i++)
    {
        TableInfoElement info = List.at(i);
        int j;
        for (j = 0;  j < List.column(); j++)
        {
            QString str;
            if (j  < info.m_value.count())
            {
                str = info.m_value.at(j);
            }
            else
            {
                str = "";
            }
            QTableWidgetItem* item = new QTableWidgetItem(str);
            item->setToolTip(str);
            QFont font;
            font.setBold(true);
            item->setFont(font);
            ui->tableWidget->setItem(i,j,item);
        }
    }
    for (i = 0; i < List.column(); i++)
    {
        ui->tableWidget->resizeColumnToContents(i);
    }
    m_empty = false;
}

void DockListView::EmptyTable(void)
{
    ui->tableWidget->reset();
    m_empty = true;
}

void DockListView::on_tableWidget_doubleClicked(const QModelIndex &index)
{
    emit DoubleClick(ui->tableWidget->item(index.row(),0)->text());
}

void DockListView::on_tableWidget_customContextMenuRequested(const QPoint &pos)
{
    QPoint globalPos = ui->tableWidget->mapToGlobal(pos);

    QMenu myMenu;
    myMenu.addAction("Delete");

    QAction* selectedItem = myMenu.exec(globalPos);
    if (selectedItem)
    {
        if (selectedItem->text() == "Delete")
        {
            int row = ui->tableWidget->itemAt(pos)->row();
            emit Delete(ui->tableWidget->item(row,0)->text());
        }
    }
}
