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

#include <QAction>
#include <QToolBar>
#include <QListView>
#include <QStringListModel>
#include <QVBoxLayout>
#include <QFileDialog>

#include "playlistwidget.h"
#include "playlist.h"
#include "qpcore.h"

PlaylistWidget::PlaylistWidget(QWidget *parent) :
    QWidget(parent)
{
    initActions();
    initGui();
    initConnections();
}

void PlaylistWidget::initActions()
{
    actionAdd = new QAction(QIcon::fromTheme("list-add"), tr("Add video"), this);
    actionRemove = new QAction(QIcon::fromTheme("list-remove"), tr("Remove video"), this);
    actionRemove->setEnabled(false);
}

void PlaylistWidget::initConnections()
{
    connect(actionAdd, SIGNAL(triggered()), this, SLOT(add()));
    connect(actionRemove, SIGNAL(triggered()), this, SLOT(removeCurrentIndex()));

    connect(playlistView, SIGNAL(doubleClicked(QModelIndex)), playlist, SLOT(playIndex(QModelIndex)));
}

void PlaylistWidget::initGui()
{
    QToolBar *plToolbar = new QToolBar(tr("Playlist actions"));
    plToolbar->addAction(actionAdd);
    plToolbar->addAction(actionRemove);

    playlistView = new QListView;
    playlist = new Playlist(this);
    playlistView->setModel(playlist);
    playlistView->setSelectionMode(QAbstractItemView::SingleSelection);

    QVBoxLayout *vLayout = new QVBoxLayout;
    vLayout->addWidget(plToolbar);
    vLayout->addWidget(playlistView);

    setLayout(vLayout);
}

void PlaylistWidget::add(const QStringList &mediaUrls)
{
    QStringListIterator urlIterator(mediaUrls);
    while (urlIterator.hasNext())
        add(urlIterator.next());
}

void PlaylistWidget::add(const QString &mediaUrl)
{
    actionRemove->setEnabled(true);
    playlist->addVideo(mediaUrl);

    QModelIndex currentIndex = playlistView->currentIndex();
    if (!currentIndex.isValid()) {
        playlistView->setCurrentIndex(playlist->index(0, 0, QModelIndex()));
    }
}

void PlaylistWidget::add()
{
    QStringList videos = QFileDialog::getOpenFileNames(this,
                                        "Open video files",
                                        QDir::homePath(),
                                        QPCore::phononFilter());

    add(videos);
}

bool PlaylistWidget::hasNext() const
{
    return playlist->hasNext();
}

bool PlaylistWidget::hasPrevious() const
{
    return playlist->hasPrevious();
}

bool PlaylistWidget::isEmpty() const
{
    if (playlist->rowCount() == 0)
        return true;

    return false;
}

Playlist* PlaylistWidget::playlistModel() const
{
    return playlist;
}

void PlaylistWidget::removeCurrentIndex()
{
    QModelIndex currentIndex = playlistView->currentIndex();
    if (!currentIndex.isValid())
        return;

    playlist->removeRow(currentIndex.row());

    if (isEmpty())
        actionRemove->setEnabled(false);
}
