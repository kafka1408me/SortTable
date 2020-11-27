#pragma once

#include <QAbstractItemModel>

using NumType = int;

#define SORTING_BUBBLE "Сортировка пузырьком"

#define SORTING_QUICK "Быстрая сортировка"

#define SORTING_COMB  "Сортировка расческой"

#define SORTING_GNOME  "Гномья сортировка"

#define SORTING_MONKEY "Обезьянья сортировка"

const QStringList sortMethodsList({SORTING_BUBBLE, SORTING_QUICK, SORTING_COMB
                                   , SORTING_GNOME , SORTING_MONKEY});

class MyModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit MyModel(int size, QObject *parent = nullptr);

    QModelIndex index(int row, int column,
                                  const QModelIndex &parent = QModelIndex()) const override;

    QModelIndex parent(const QModelIndex &child) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    Qt::ItemFlags flags(const QModelIndex& index)const override;

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

public slots:
    void slot_fillRandom();

    void slot_setNumMax(NumType max);

    NumType slot_calcMin();

    NumType slot_calcMax();

    double slot_calcSr();

    void slot_sort(const QString& sortingMethod);

private:
    mutable QVector<NumType> vec;
    NumType numMax = INT_MAX;
};


