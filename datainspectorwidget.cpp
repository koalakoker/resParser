#include "datainspectorwidget.h"
#include "ui_datainspectorwidget.h"

DataInspectorWidget::DataInspectorWidget(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DataInspectorWidget)
{
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

void DataInspectorWidget::setDataPoints(QVector<FPoint>* pData)
{
    m_points = pData;
    int row = pData->count();
    ui->tableWidget->setRowCount(row);
    // Populate table
    int i;
    for (i = 0; i < row; i++)
    {
        FPoint p = m_points->at(i);
        QTableWidgetItem* item = new QTableWidgetItem;
        item->setText(QString("%1").arg(p.x()));
        ui->tableWidget->setItem(i,0,item);
        item = new QTableWidgetItem;
        item->setText(QString("%1").arg(p.y()));
        ui->tableWidget->setItem(i,1,item);
    }
}
