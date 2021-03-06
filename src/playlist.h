/* QuantumPlayer - Qt and Phonon based multimedia player
 * Copyright (C) 2010-2012  Ville Leskinen
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

#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <QAbstractItemModel>

#include <QStringList>
#include <QList>

#include "video.h"

class Playlist : public QAbstractItemModel
{
Q_OBJECT
public:
    explicit Playlist(QObject *parent = 0);
    ~Playlist();

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    Qt::ItemFlags flags(const QModelIndex &) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent) const;

    bool addVideo(const QString &videoPath);
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());

    bool hasNext() const;
    bool hasPrevious() const;

signals:
    void loadVideo(const QString &mediaUrl);
    void playVideo(const QString &mediaUrl);

    void playStatusChange(bool enabled);
    void nextVideoStatusChange(bool enabled);
    void previousVideoStatusChange(bool enabled);

public slots:
    void videoPlayed();
    bool nextVideo();
    bool previousVideo();

    void playIndex(const QModelIndex &videoIndex);

private:
    QString videoPathAt(int index) const;

    QStringList filePaths;
    QList<Video*> playlistVideos;

    int playRow;
    QModelIndex firstIndex;
    QModelIndex lastIndex;

    QList<QModelIndex> playedVideos;
};

#endif // PLAYLIST_H
