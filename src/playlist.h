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

#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <QAbstractListModel>

#include <QStringList>
#include <QList>

class Playlist : public QAbstractListModel
{
Q_OBJECT
public:
    explicit Playlist(QObject *parent = 0);

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    bool addVideo(const QString &videoPath);


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
    QStringList filePaths;

    int playRow;
    QModelIndex firstIndex;
    QModelIndex lastIndex;

    QList<QModelIndex> playedVideos;
};

#endif // PLAYLIST_H
