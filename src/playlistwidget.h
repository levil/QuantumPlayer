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

#ifndef PLAYLISTWIDGET_H
#define PLAYLISTWIDGET_H

#include <QWidget>

class QAction;
class QTreeView;
class QStringListModel;
class QModelIndex;

class Playlist;

class PlaylistWidget : public QWidget
{
Q_OBJECT
public:
    explicit PlaylistWidget(QWidget *parent = 0);

    bool hasNext() const;
    bool hasPrevious() const;
    bool isEmpty() const;

    Playlist* playlistModel() const;

signals:

public slots:
    void add(const QStringList &mediaUrls);
    void add(const QString &mediaUrl);
    void add();

    void removeCurrentIndex();

private slots:
    void playlistContextMenu(const QPoint &pos);

private:
    void initGui();
    void initActions();
    void initConnections();

    QAction *actionAdd;
    QAction *actionRemove;

    QTreeView *playlistView;
    Playlist *playlist;
};

#endif // PLAYLISTWIDGET_H
