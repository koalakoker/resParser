#include "docklistview.h"
#include "ui_docklistview.h"

DockListView::DockListView(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::DockListView)
{
    ui->setupUi(this);
    m_empty = true;
}

DockListView::~DockListView()
{
    delete ui;
}

void DockListView::populate(QList<TableInfoElement> List)
{
    if (!m_empty)
    {
        EmptyTable();
    }
    ui->tableWidget->setRowCount(List.count());
    ui->tableWidget->setColumnCount(2);
    int i;
    for (i = 0; i < List.count(); i++)
    {
        TableInfoElement info = List.at(i);
        QTableWidgetItem* item = new QTableWidgetItem(info.m_name);
        item->setToolTip(info.m_name);
        QFont font;
        font.setBold(true);
        item->setFont(font);
        ui->tableWidget->setItem(i,0,item);
        item = new QTableWidgetItem(info.m_value);
        item->setToolTip(info.m_value);
        item->setFont(font);
        ui->tableWidget->setItem(i,1,item);
    }
    ui->tableWidget->resizeColumnToContents(0);
    ui->tableWidget->resizeColumnToContents(1);
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
