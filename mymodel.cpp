#include "mymodel.h"
#include <QDebug>
#include <random>
#include <algorithm>

//---------------------- SORTING METHODS
// Сортировка пузырьком
void sorting_Bubble(QVector<NumType>& a)
{
    int r = a.size() - 1;

    for(int i = 0; i < r; ++i)
    {
        for(int j = r; j > i; --j)
        {
            if(a[j-1] > a[j])
            {
                std::swap(a[j-1], a[j]);
            }
        }
    }
}

// Быстрая сортировка
int partition(QVector<NumType>& a, int l, int r)
{
    int i = l - 1, j = r;
    NumType v = a[r];

    for(;;)
    {
        while(a[++i] < v);
        while(v < a[--j])
        {
            if(j == l)
            {
                break;
            }
        }
        if(i >=j)
        {
            break;
        }
        std::swap(a[i], a[j]);
    }
    std::swap(a[i], a[r]);
    return i;
}


void sorting_quick(QVector<NumType>& a, int l, int r)
{
    if(r <= l)
    {
        return;
    }

    int i = partition(a, l, r);
    sorting_quick(a, l, i - 1);
    sorting_quick(a, i+1, r);
}

// Сортировка расческой
void sorting_comb(QVector<NumType> &data)
{
    double factor = 1.2473309; // фактор уменьшения
    int step = data.size() - 1; // шаг сортировки

    while (step >= 1)
    {
        for (int i = 0; i + step < data.size(); i++)
        {
            if (data[i] > data[i + step])
            {
                std::swap(data[i], data[i + step]);
            }
        }
        step /= factor;
    }
}

// Гномья сортировка
void sorting_gnome(QVector<NumType>& a)
{
    int i = 1;
    int j = 2;
    int size = a.size();

    while(i < size)
    {
        if(a[i-1] < a[i])
        {
            i = j;
            j = j + 1;
        }
        else
        {
            std::swap(a[i-1], a[i]);
            i = i - 1;
            if(i == 0)
            {
                i = j;
                j = j + 1;
            }
        }
    }
}

// Обезьянья сортировка
int correct(QVector<NumType>& arr)
{
    int size = arr.size();
    while (--size > 0)
    {
        if (arr[size - 1] > arr[size])
        {
            return 0;
        }
    }
    return 1;
}

void shuffle(QVector<NumType>& arr)
{
    int i;
    const int size = arr.size();
    for (i = 0; i < size; ++i)
    {
        std::swap(arr[i], arr[(rand() % size)]);
    }
}

void bogoSort(QVector<NumType>& arr)
{
    while (!correct(arr))
    {
        shuffle(arr);
    }
}
//---------------------- SORTING METHODS

MyModel::MyModel(int size, QObject *parent) :
    QAbstractItemModel(parent)
{
    vec.reserve(200);
    for(int i = 0; i < size; ++i)
    {
        vec.append(i);
    }
    srand(time(nullptr));
}

QModelIndex MyModel::index(int row, int column, const QModelIndex &parent) const
{
    if(parent.isValid())
    {
        return QModelIndex();
    }

    if(row < 0 || row >= vec.size() || column != 0)
    {
        return QModelIndex();
    }

    return createIndex(row, column, reinterpret_cast<void*>(&vec[row]));
}

QModelIndex MyModel::parent(const QModelIndex &child) const
{
    Q_UNUSED(child)
    return QModelIndex();
}

int MyModel::rowCount(const QModelIndex &parent) const
{
    if(parent.isValid())
    {
        return 0;
    }
    return vec.size();
}

int MyModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 1;
}

QVariant MyModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
    {
        return QVariant();
    }

    if(index.row() < 0 || index.row() >= vec.size())
    {
        return QVariant();
    }

    return (role == Qt::DisplayRole || role == Qt::EditRole) ? vec.at(index.row()) : QVariant();
}

bool MyModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(index.isValid() && role == Qt::EditRole)
    {
        vec.replace(index.row(), value.value<NumType>());
        emit dataChanged(index, index);
        return true;
    }
    return false;
}

bool MyModel::insertRows(int row, int count, const QModelIndex &parent)
{
    if(parent.isValid())
    {
        return false;
    }
    beginInsertRows(QModelIndex(), row, row + count - 1);
    vec.insert(row, count, NumType(0));
    endInsertRows();

    return true;
}

bool MyModel::removeRows(int row, int count, const QModelIndex &parent)
{
    if(parent.isValid())
    {
        return false;
    }

    beginRemoveRows(QModelIndex(), row, row + count - 1);
    vec.remove(row, count);
    endRemoveRows();

    return true;
}

Qt::ItemFlags MyModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = QAbstractItemModel::flags(index);

    return index.isValid() ? (flags | Qt::ItemIsEditable): flags;
}

QVariant MyModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role != Qt::DisplayRole)
    {
        return QVariant();
    }

    return (orientation == Qt::Horizontal) ? QString("Число") : QString::number(section);
}

void MyModel::slot_fillRandom()
{
    NumType num;
    const int size = vec.size();
    for(int i = 0; i < size; ++i)
    {
        num = rand() % numMax;
        vec[i] = num;
    }
    emit dataChanged(index(0,0), index(size-1, 0));
}

void MyModel::slot_setNumMax(NumType max)
{
    numMax = max;
}

NumType MyModel::slot_calcMin()
{
    return *std::min_element(vec.begin(), vec.end());
}

NumType MyModel::slot_calcMax()
{
    return *std::max_element(vec.begin(), vec.end());
}

double MyModel::slot_calcSr()
{
    NumType sum = std::accumulate(vec.begin(), vec.end(), NumType(0));
    return double(sum)/vec.size();
}

void MyModel::slot_sort(const QString &sortingMethod)
{
    static QVector<NumType> myVec;
    myVec = vec;

    int size = vec.size();

    if(sortingMethod == SORTING_BUBBLE)
    {
        sorting_Bubble(myVec);
    }
    else if(sortingMethod == SORTING_QUICK)
    {
        sorting_quick(myVec, 0, size - 1);
    }
    else if(sortingMethod == SORTING_COMB)
    {
        sorting_comb(myVec);
    }
    else if(sortingMethod == SORTING_GNOME)
    {
        sorting_gnome(myVec);
    }
    else // if(sortingMethod == SORTING_MONKEY)
    {
        bogoSort(myVec);
    }

    if(!memcmp(myVec.data(), vec.data(), size*sizeof(NumType)))  // Если векторы равны
    {
        return;
    }

    QModelIndex modelIndex;
    for(int i = 0; i < size ;++i)
    {
        if(vec[i] != myVec[i])
        {
            vec[i] = myVec[i];
            modelIndex = index(i,0);
            emit dataChanged(modelIndex, modelIndex);
        }
    }
    //    emit dataChanged(index(0,0), index(vec.size()-1, 0));
}



