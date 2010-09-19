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
#include <QBrush>

#include <QDebug>

Playlist::Playlist(QObject *parent) :
    QAbstractListModel(parent)
{
    playRow = -1;
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
    } else if (role == Qt::BackgroundRole) {
        if (index.row() == playRow)
            return QBrush(Qt::lightGray);
    } else if (role == Qt::ForegroundRole) {
        if (playedVideos.contains(index))
            return QBrush(Qt::darkGray);
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

    if (!firstIndex.isValid())
        firstIndex = index(0, 0);

    beginInsertRows(QModelIndex(), row, row);
    filePaths << videoPath;
    endInsertRows();

    lastIndex = index(rowCount() - 1, 0);

    if (playRow == -1) {
        playRow = 0;
        emit loadVideo(filePaths.value(0));
    }

    emit nextVideoStatusChange(hasNext());
    emit previousVideoStatusChange(hasPrevious());
    return true;
}

bool Playlist::hasNext() const
{
    if (playRow == -1 || playRow == filePaths.size() - 1)
        return false;

    return true;
}

bool Playlist::hasPrevious() const
{
    if (playRow < 1)
        return false;

    return true;
}

void Playlist::videoPlayed()
{
    playedVideos << index(playRow, 0);

    if (!nextVideo()) {
        playRow = -1;
        emit dataChanged(firstIndex, lastIndex);
    }
}

bool Playlist::nextVideo()
{
    if (!hasNext())
        return false;

    playRow++;
    emit playVideo(filePaths.value(playRow));
    emit nextVideoStatusChange(hasNext());
    emit previousVideoStatusChange(hasPrevious());
    emit dataChanged(firstIndex, lastIndex);
    return true;
}

bool Playlist::previousVideo()
{
    if (!hasPrevious())
        return false;

    playRow--;
    emit playVideo(filePaths.value(playRow));
    emit nextVideoStatusChange(hasNext());
    emit previousVideoStatusChange(hasPrevious());
    emit dataChanged(firstIndex, lastIndex);
    return true;
}

void Playlist::playIndex(const QModelIndex &videoIndex)
{
    playRow = videoIndex.row();
    emit playVideo(filePaths.value(playRow));
    emit nextVideoStatusChange(hasNext());
    emit previousVideoStatusChange(hasNext());
    emit dataChanged(firstIndex, lastIndex);
}

bool Playlist::removeRows(int row, int count, const QModelIndex &parent)
{
    if (rowCount() <= (row + count - 1))
        return false;

    beginRemoveRows(QModelIndex(), row, row + count - 1);
    for (int i = 1; i <= count; i++) {
        filePaths.removeAt(row);
    }
    endRemoveRows();

    return true;
}
