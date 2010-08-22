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

#include <QtGui>

#include "quantumplayer.h"
#include "playlistwidget.h"
#include "player.h"

QuantumPlayer::QuantumPlayer(QWidget *parent) :
    QMainWindow(parent)
{
    initActions();
    initGui();
    initMenus();
    initConnections();
}

void QuantumPlayer::initGui()
{
    player = new Player();
    setCentralWidget(player);
    setGeometry(100, 100, 800, 600);

    QDockWidget *playlistDock = new QDockWidget(tr("Playlist"), this);
    playlistDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    playlistWidget = new PlaylistWidget(playlistDock);
    playlistDock->setWidget(playlistWidget);
    addDockWidget(Qt::LeftDockWidgetArea, playlistDock);

    setWindowTitle(qApp->applicationName());
}

void QuantumPlayer::initMenus()
{
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(actionOpen);
    fileMenu->addSeparator();
    fileMenu->addAction(actionQuit);

    QMenu *playbackMenu = menuBar()->addMenu(tr("&Playback"));
    playbackMenu->addAction(player->actionPlayPause());
    playbackMenu->addAction(player->actionStop());

    QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(actionAboutQt);
}

void QuantumPlayer::initActions()
{
    actionOpen = new QAction(QIcon::fromTheme("document-open"), tr("&Open"), this);
    actionOpen->setShortcut(QKeySequence::Open);
    actionQuit = new QAction(QIcon::fromTheme("application-exit"), tr("&Quit"), this);
    actionQuit->setShortcut(QKeySequence::Quit);

    actionAboutQt = new QAction(tr("About &Qt"), this);
}

void QuantumPlayer::initConnections()
{
    connect(actionQuit, SIGNAL(triggered()), qApp, SLOT(quit()));
    connect(actionOpen, SIGNAL(triggered()), this, SLOT(handleOpen()));
    connect(actionAboutQt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    connect(playlistWidget, SIGNAL(videoChanged(QString)), player, SLOT(play(QString)));
    connect(player, SIGNAL(playerFinished()), playlistWidget, SLOT(nextVideo()));
}

void QuantumPlayer::handleOpen()
{
    QString fileUrl = QFileDialog::getOpenFileName(this, tr("Open media file"), QDir::homePath());

    if (fileUrl.isNull())
        return;

    if (!windowTitle().isEmpty())
        setWindowTitle(QString());

    setWindowFilePath(fileUrl);
    player->loadMedia(fileUrl);
}
