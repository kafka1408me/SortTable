#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include "mymodel.h"
#include <QTableView>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    slot_ResetNums();

    int countNums = ui->countNumspinBox->value();
    myModel = new MyModel(countNums, this);

    ui->tableView->setModel(myModel);

    ui->tableView->setColumnWidth(0, 160);

    ui->sortCmbx->addItems(sortMethodsList);

    connect(ui->countNumspinBox, SIGNAL(valueChanged(int)), this, SLOT(slot_countNumsChanged(int)));
    connect(ui->fillRandBtn, SIGNAL(clicked()), myModel, SLOT(slot_fillRandom()));
    connect(ui->calcMinBtn, SIGNAL(clicked()), this, SLOT(slot_setMinNum()));
    connect(ui->calcMaxBtn, SIGNAL(clicked()), this, SLOT(slot_setMaxNum()));
    connect(ui->calcSrBtn, SIGNAL(clicked()), this, SLOT(slot_setSrNum()));

    connect(myModel, SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &, const QVector<int> &)), this, SLOT(slot_ResetNums()));
    connect(myModel, SIGNAL(rowsInserted(const QModelIndex &, int, int)), this, SLOT(slot_ResetNums()));
    connect(myModel, SIGNAL(rowsRemoved(const QModelIndex &, int, int)), this, SLOT(slot_ResetNums()));

    connect(ui->sortBtn, SIGNAL(clicked()), this, SLOT(slot_sort()));

    setWindowTitle("My program");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slot_countNumsChanged(int n)
{
    int countRow = myModel->rowCount();
    int diff = n - countRow;

    if(diff > 0)
    {
        myModel->insertRows(countRow, diff);
    }
    else
    {
        myModel->removeRows(countRow - 1 + diff, -diff);
    }

}

void MainWindow::slot_setMinNum()
{
    auto min = myModel->slot_calcMin();
    ui->minLineEdit->setText(QString::number(min));
}

void MainWindow::slot_setMaxNum()
{
    auto max = myModel->slot_calcMax();
    ui->maxLineEdit->setText(QString::number(max));
}

void MainWindow::slot_setSrNum()
{
    auto sr = myModel->slot_calcSr();
    ui->srLineEdit->setText(QString::number(sr));
}

void MainWindow::slot_ResetNums()
{
    static QString resetStr("---");
    ui->minLineEdit->setText(resetStr);
    ui->maxLineEdit->setText(resetStr);
    ui->srLineEdit->setText(resetStr);
}

void MainWindow::slot_sort()
{
    QString sortingMethod = ui->sortCmbx->currentText();
    myModel->slot_sort(sortingMethod);
}


