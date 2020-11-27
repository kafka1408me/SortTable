#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>


class MyModel;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QTableWidgetItem;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void slot_countNumsChanged(int n);

    void slot_setMinNum();

    void slot_setMaxNum();

    void slot_setSrNum();

    void slot_ResetNums();

    void slot_sort();


private:
    Ui::MainWindow *ui;
    MyModel* myModel;
};
#endif // MAINWINDOW_H
