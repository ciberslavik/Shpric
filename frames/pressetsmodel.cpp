#include "pressetsmodel.h"

PressetsModel::PressetsModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}

QVariant PressetsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
            if (section == 0) {
                return QString("Имя");
            } else if (section == 1) {
                return QString("Логика");
            } else if (section == 2) {
                return QString("Объем");
            }
        }
        return QVariant();
}

bool PressetsModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    if (value != headerData(section, orientation, role)) {
        // FIXME: Implement me!
        emit headerDataChanged(orientation, section, section);
        return true;
    }
    return false;
}


int PressetsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return _pressets.count();

    // FIXME: Implement me!
}

int PressetsModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 3;
    // FIXME: Implement me!
}

QVariant PressetsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || role != Qt::DisplayRole) {
        return QVariant();
    }
    if (index.column() == 0) {
        return _pressets[index.row()]->PressetName();
    } else if (index.column() == 1) {
        return logicToString((Logic)_pressets[index.row()]->Logic());
    } else if (index.column() == 2) {
        return QString::number(_pressets[index.row()]->Mass());
    }
    return QVariant();
}

bool PressetsModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (data(index, role) != value) {
        // FIXME: Implement me!
        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    }
    return false;
}

Qt::ItemFlags PressetsModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
             return Qt::ItemIsEnabled;

         return QAbstractItemModel::flags(index);
}

bool PressetsModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count - 1);
    // FIXME: Implement me!
    endInsertRows();
    return true;
}

bool PressetsModel::insertColumns(int column, int count, const QModelIndex &parent)
{
    beginInsertColumns(parent, column, column + count - 1);
    // FIXME: Implement me!
    endInsertColumns();
    return true;
}

bool PressetsModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);
    // FIXME: Implement me!
    endRemoveRows();
    return true;
}

bool PressetsModel::removeColumns(int column, int count, const QModelIndex &parent)
{
    beginRemoveColumns(parent, column, column + count - 1);
    // FIXME: Implement me!
    endRemoveColumns();
    return true;
}

void PressetsModel::populateData(const QList<PressetConfig *> &pressets)
{
    _pressets = pressets;
}

void PressetsModel::clear()
{
    _pressets.clear();
}

QString PressetsModel::logicToString(Logic logic) const
{
    switch (logic) {
    case Logic::Swiper:
        return "Перекрут";
        break;
    case Logic::Portion:
        return "Порционирование";
        break;
    case Logic::Stuffing:
        return "Набивка";
        break;
    case Logic::None:
        return "NULL";
        break;
    default:
        return "";
    }
}
