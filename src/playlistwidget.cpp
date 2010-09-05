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
    connect(playlistView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(fileDoubleClicked(QModelIndex)));
}

void PlaylistWidget::initGui()
{
    QToolBar *plToolbar = new QToolBar(tr("Playlist actions"));
    plToolbar->addAction(actionAdd);
    plToolbar->addAction(actionRemove);

    playlistView = new QListView;
    playlistModel = new QStringListModel;
    playlistView->setModel(playlistModel);

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
    QStringList videoList = playlistModel->stringList();
    videoList << mediaUrl;
    playlistModel->setStringList(videoList);

    QModelIndex currentIndex = playlistView->currentIndex();
    if (!currentIndex.isValid()) {
        playlistView->setCurrentIndex(playlistModel->index(0, 0, QModelIndex()));
    }

    emit playlistChanged();
}

void PlaylistWidget::add()
{
    QStringList videos = QFileDialog::getOpenFileNames(this,
                                        "Open video files",
                                        QDir::homePath(),
                                        QPCore::phononFilter());

    add(videos);
}

void PlaylistWidget::previousVideo()
{
    qDebug("PlaylistWidget::previousVideo()");
    QModelIndex currentVideo = playlistView->currentIndex();
    int currentRow = currentVideo.row();
    if (currentRow - 1 < 0)
        return;

    QModelIndex previousVideo = playlistModel->index(currentRow - 1, 0, QModelIndex());
    QString filePath = playlistModel->data(previousVideo, Qt::DisplayRole).toString();
    playlistView->setCurrentIndex(previousVideo);
    emit playlistChanged();
    emit videoChanged(filePath);
}

void PlaylistWidget::nextVideo()
{
    qDebug("PlaylistWidget::nextVideo()");

    QModelIndex currentVideo = playlistView->currentIndex();
    int currentRow = currentVideo.row();
    if (currentRow >= playlistModel->rowCount() - 1)
        return;

    QModelIndex nextVideo = playlistModel->index(currentRow + 1, 0, QModelIndex());
    QString filePath = playlistModel->data(nextVideo, Qt::DisplayRole).toString();
    playlistView->setCurrentIndex(nextVideo);
    emit playlistChanged();
    emit videoChanged(filePath);
}

void PlaylistWidget::fileDoubleClicked(const QModelIndex &index)
{
    QString filePath = playlistModel->data(index, Qt::DisplayRole).toString();
    emit videoChanged(filePath);
}

bool PlaylistWidget::hasNext() const
{
    int row = playlistView->currentIndex().row();
    if (row >= playlistModel->rowCount() - 1)
        return false;

    return true;
}

bool PlaylistWidget::hasPrevious() const
{
    int row = playlistView->currentIndex().row();
    if (row == 0)
        return false;

    return true;
}

bool PlaylistWidget::isEmpty() const
{
    if (playlistModel->rowCount() == 0)
        return true;

    return false;
}
