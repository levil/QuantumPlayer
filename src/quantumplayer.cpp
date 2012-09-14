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

#include <QtGui>
#include <QSettings>

#include "quantumplayer.h"
#include "playlistwidget.h"
#include "playlist.h"
#include "player.h"
#include "qpcore.h"

QuantumPlayer::QuantumPlayer(QWidget *parent) :
    QMainWindow(parent)
{
    initGui();
    initActions();
    initMenus();
    initConnections();

    readSettings();

    QPCore::initialize();
}

void QuantumPlayer::initGui()
{
    player = new Player();
    setCentralWidget(player);
    setGeometry(100, 100, 800, 600);

    playlistDock = new QDockWidget(tr("Playlist"), this);
    playlistDock->setObjectName("playlistDock");
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

    QMenu *viewMenu = menuBar()->addMenu(tr("&View"));
    viewMenu->addAction(actionViewPlaylist);
    viewMenu->addAction(player->toggleFullscreenAct());

    QMenu *playbackMenu = menuBar()->addMenu(tr("&Playback"));
    playbackMenu->addAction(player->playPauseAct());
    playbackMenu->addAction(player->stopAct());

    QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(actionAboutQt);
}

void QuantumPlayer::initActions()
{
    actionOpen = new QAction(QIcon::fromTheme("document-open"), tr("&Open"), this);
    actionOpen->setShortcut(QKeySequence::Open);
    actionQuit = new QAction(QIcon::fromTheme("application-exit"), tr("&Quit"), this);
    actionQuit->setShortcut(QKeySequence::Quit);

    actionViewPlaylist = playlistDock->toggleViewAction();

    actionAboutQt = new QAction(tr("About &Qt"), this);
}

void QuantumPlayer::initConnections()
{
    connect(actionQuit, SIGNAL(triggered()), qApp, SLOT(quit()));
    connect(actionOpen, SIGNAL(triggered()), this, SLOT(handleOpen()));
    connect(actionAboutQt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    Playlist *pl = playlistWidget->playlistModel();
    connect(pl, SIGNAL(playVideo(QString)), player, SLOT(play(QString)));
    connect(pl, SIGNAL(loadVideo(QString)), player, SLOT(loadMedia(QString)));
    connect(pl, SIGNAL(nextVideoStatusChange(bool)), player, SLOT(setSkipForwardEnabled(bool)));
    connect(pl, SIGNAL(previousVideoStatusChange(bool)), player, SLOT(setSkipBackwardEnabled(bool)));
    connect(player, SIGNAL(playerFinished()), pl, SLOT(videoPlayed()));
    connect(player, SIGNAL(skipBackward()), pl, SLOT(previousVideo()));
    connect(player, SIGNAL(skipForward()), pl, SLOT(nextVideo()));

    connect(player, SIGNAL(toggleFullScreen(bool)), this, SLOT(changeFullScreen(bool)));
}

void QuantumPlayer::handleOpen()
{
    QString fileUrl = QFileDialog::getOpenFileName(this, tr("Open media file"), QDir::homePath(),
                        QPCore::phononFilter());

    if (fileUrl.isNull())
        return;

    if (!windowTitle().isEmpty())
        setWindowTitle(QString());

    setWindowFilePath(fileUrl);
    player->loadMedia(fileUrl);
}

void QuantumPlayer::changeFullScreen(bool fullScreen)
{
    menuBar()->setVisible(!fullScreen);
    playlistDock->setVisible(!fullScreen);
    if (fullScreen)
        showFullScreen();
    else
        showNormal();
}

void QuantumPlayer::closeEvent(QCloseEvent *event)
{
    writeSettings();
    event->accept();
}

void QuantumPlayer::writeSettings()
{
    QSettings settings;

    settings.beginGroup("MainWindow");
    settings.setValue("geometry", this->saveGeometry());
    settings.setValue("windowState", this->saveState());
    settings.endGroup();
}

void QuantumPlayer::readSettings()
{
    QSettings settings;

    settings.beginGroup("MainWindow");
    restoreGeometry(settings.value("geometry").toByteArray());
    restoreState(settings.value("windowState").toByteArray());
}
