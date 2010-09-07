/* QuantumPlayer - Qt and Phonon based multimedia player
 * Copyright (C) 2010  Ville Leskinen
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */

#include "playlist.h"

#include <QFileInfo>

Playlist::Playlist(QObject *parent) :
    QAbstractListModel(parent)
{
}

QVariant Playlist::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DisplayRole) {
        QFileInfo fileInfo(filePaths.value(index.row()));
        return fileInfo.fileName();
    } else if (role == Qt::UserRole) {
        return filePaths.value(index.row());
    }

    return QVariant();
}

Qt::ItemFlags Playlist::flags(const QModelIndex &index) const
{
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

QVariant Playlist::headerData(int section, Qt::Orientation orientation, int role) const
{
    return QVariant();
}

int Playlist::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    else
        return filePaths.size();
}

bool Playlist::addVideo(const QString &videoPath)
{
    int row = filePaths.size();

    beginInsertRows(QModelIndex(), row, row);
    filePaths << videoPath;
    endInsertRows();
}
