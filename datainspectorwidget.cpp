#include "datainspectorwidget.h"
#include "ui_datainspectorwidget.h"
#include <QMenu>

DataInspectorWidget::DataInspectorWidget(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DataInspectorWidget)
{
    m_points = NULL;

    ui->setupUi(this);

    ui->tableWidget->setColumnCount(2);
    QStringList header;
    header << "x" << "y";
    ui->tableWidget->setHorizontalHeaderLabels(header);
    ui->tableWidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    ui->tableWidget->verticalHeader()->hide();
}

DataInspectorWidget::~DataInspectorWidget()
{
    delete ui;
}

void DataInspectorWidget::setDataPoints(RawData* pData)
{
    m_points = pData;
    UpdateTable();
}

void DataInspectorWidget::UpdateTable(void)
{
    ui->tableWidget->reset();
    int row = m_points->count();
    ui->tableWidget->setRowCount(row);
    // Populate table
    int i;
    for (i = 0; i < row; i++)
    {
        HPoint p = m_points->at(i);
        QTableWidgetItem* item = new QTableWidgetItem;
        item->setText(p.x().toString("%.5Rf"));
        ui->tableWidget->setItem(i,0,item);
        item = new QTableWidgetItem;
        item->setText(p.y().toString("%.5Rf"));
        ui->tableWidget->setItem(i,1,item);
    }
}

void DataInspectorWidget::on_tableWidget_customContextMenuRequested(const QPoint &pos)
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
            m_points->remove(row);
            m_points->updateRange();
            UpdateTable();
            emit dataChanged(m_points->RawRange());
        }
    }
}
