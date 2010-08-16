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
#include <Phonon/VideoPlayer>
#include <Phonon/MediaSource>
#include <Phonon/MediaObject>
#include <Phonon/SeekSlider>

#include <QtGui/QVBoxLayout>
#include <QtGui/QToolBar>
#include <QtGui/QAction>
#include <QtGui/QIcon>

#include "player.h"

Player::Player(QWidget *parent) :
    QWidget(parent)
{
    initActions();
    initGui();
    initConnections();
}

void Player::initConnections()
{
    connect(vp, SIGNAL(finished()), vp, SLOT(deleteLater()));
    connect(playPauseAction, SIGNAL(triggered()), this, SLOT(handlePlayPause()));
    connect(stopAction, SIGNAL(triggered()), vp, SLOT(stop()));
}

void Player::initActions()
{
    playPauseAction = new QAction(QIcon::fromTheme("media-playback-start"), tr("Play"), this);
    playPauseAction->setEnabled(false);

    stopAction = new QAction(QIcon::fromTheme("media-playback-stop"), tr("Stop"), this);
    stopAction->setEnabled(false);
}

void Player::initGui()
{
    vp = new Phonon::VideoPlayer(Phonon::VideoCategory);

    Phonon::SeekSlider *seekSlider = new Phonon::SeekSlider(vp->mediaObject());

    controlBar = new QToolBar;
    controlBar->addAction(playPauseAction);
    controlBar->addAction(stopAction);
    controlBar->addWidget(seekSlider);

    vLayout = new QVBoxLayout;
    vLayout->addWidget(vp);
    vLayout->addWidget(controlBar);

    setLayout(vLayout);
}

void Player::loadMedia(const QString &mediaUrl)
{
    vp->load(mediaUrl);
    playPauseAction->setEnabled(true);
}

void Player::handlePlayPause()
{
    if (vp->isPlaying()) {
        vp->pause();
        playPauseAction->setIcon(QIcon::fromTheme("media-playback-start"));
        playPauseAction->setText(tr("Play"));
    } else {
        vp->play();
        playPauseAction->setIcon(QIcon::fromTheme("media-playback-pause"));
        playPauseAction->setText(tr("Pause"));
        stopAction->setEnabled(true);
    }
}
