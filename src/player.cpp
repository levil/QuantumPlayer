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
#include <Phonon/VolumeSlider>

#include <QtGui>

#include <QDebug>

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
    connect(vp, SIGNAL(finished()), this, SIGNAL(playerFinished()));
    connect(vp->mediaObject(), SIGNAL(tick(qint64)), this, SLOT(handleTick(qint64)));
    connect(vp->mediaObject(), SIGNAL(totalTimeChanged(qint64)), this, SLOT(handleTotalTimeChange(qint64)));

    connect(actionPlayPause, SIGNAL(triggered()), this, SLOT(handlePlayPause()));
    connect(actionStop, SIGNAL(triggered()), this, SLOT(handleStop()));
    connect(actionToggleFullscreen, SIGNAL(toggled(bool)), this, SIGNAL(toggleFullScreen(bool)));
    connect(actionSkipBackward, SIGNAL(triggered()), this, SIGNAL(skipBackward()));
    connect(actionSkipForward, SIGNAL(triggered()), this, SIGNAL(skipForward()));
}

void Player::initActions()
{
    actionPlayPause = new QAction(QIcon::fromTheme("media-playback-start"), tr("Play"), this);
    actionPlayPause->setEnabled(false);
    QList<QKeySequence> playPauseShortcuts;
    playPauseShortcuts << QKeySequence(Qt::Key_MediaPlay) << QKeySequence(Qt::Key_Space);
    actionPlayPause->setShortcuts(playPauseShortcuts);

    actionStop = new QAction(QIcon::fromTheme("media-playback-stop"), tr("Stop"), this);
    actionStop->setEnabled(false);
    actionStop->setShortcut(QKeySequence(Qt::Key_MediaStop));

    actionSkipBackward = new QAction(QIcon::fromTheme("media-skip-backward"), tr("Skip backward"), this);
    actionSkipBackward->setEnabled(false);

    actionSkipForward = new QAction(QIcon::fromTheme("media-skip-forward"), tr("Skip forward"), this);
    actionSkipForward->setEnabled(false);

    actionToggleFullscreen = new QAction(QIcon::fromTheme("view-fullscreen"), tr("Fullscreen"), this);
    actionToggleFullscreen->setCheckable(true);
    actionToggleFullscreen->setChecked(false);
    actionToggleFullscreen->setShortcut(QKeySequence(Qt::Key_F));
}

void Player::initGui()
{
    vp = new Phonon::VideoPlayer(Phonon::VideoCategory);

    Phonon::SeekSlider *seekSlider = new Phonon::SeekSlider(vp->mediaObject());
    Phonon::VolumeSlider *volumeSlider = new Phonon::VolumeSlider(vp->audioOutput());
    volumeSlider->setMaximumWidth(100);

    totalTime = "00:00:00";
    timeLabel = new QLabel("00:00:00/" + totalTime);

    controlBar = new QToolBar;
    controlBar->addAction(actionPlayPause);
    controlBar->addAction(actionStop);
    controlBar->addAction(actionSkipBackward);
    controlBar->addAction(actionSkipForward);
    controlBar->addWidget(volumeSlider);
    controlBar->addWidget(seekSlider);
    controlBar->addWidget(timeLabel);
    controlBar->addAction(actionToggleFullscreen);

    vLayout = new QVBoxLayout;
    vLayout->addWidget(vp);
    vLayout->addWidget(controlBar);

    setLayout(vLayout);
}

void Player::loadMedia(const QString &mediaUrl)
{
    vp->load(mediaUrl);
    actionPlayPause->setEnabled(true);
}

void Player::play(const QString &mediaUrl)
{
    if (vp->isPlaying())
        handleStop();

    loadMedia(mediaUrl);
    handlePlayPause(true);
}

void Player::handlePlayPause(bool forcePlay)
{
    if (vp->isPlaying() && !forcePlay) {
        vp->pause();
        changePlayPause(true);
    } else {
        vp->play();
        changePlayPause(false);
        actionStop->setEnabled(true);
    }
}

void Player::handleStop()
{
    vp->stop();
    actionStop->setEnabled(false);
    changePlayPause(true);
}

void Player::changePlayPause(bool showPlay)
{
    if (showPlay) {
        actionPlayPause->setIcon(QIcon::fromTheme("media-playback-start"));
        actionPlayPause->setText(tr("Play"));
    } else {
        actionPlayPause->setIcon(QIcon::fromTheme("media-playback-pause"));
        actionPlayPause->setText(tr("Pause"));
    }
}

void Player::setPlayEnabled(bool enabled)
{
    actionPlayPause->setEnabled(enabled);
}

void Player::setSkipBackwardEnabled(bool enabled)
{
    actionSkipBackward->setEnabled(enabled);
}

void Player::setSkipForwardEnabled(bool enabled)
{
    actionSkipForward->setEnabled(enabled);
}

QString Player::msToString(qint64 ms)
{
    int totSeconds = ms / 1000;
    int seconds = totSeconds % 60;
    int totMinutes = totSeconds / 60;
    int minutes = totMinutes % 60;
    int hours = totMinutes / 60;

    return QTime(hours, minutes, seconds).toString("HH:mm:ss");
}

void Player::handleTick(qint64 time)
{
    QString currentTime = msToString(time);
    timeLabel->setText(currentTime + "/" + totalTime);
}

void Player::handleTotalTimeChange(qint64 newTotalTime)
{
    qDebug() << "Total time in ms: " << newTotalTime;
    totalTime = msToString(newTotalTime);
    timeLabel->setText("00:00:00/" + totalTime);
}


void Player::wheelEvent(QWheelEvent *event)
{
    if (event->orientation() == Qt::Horizontal) {
        event->ignore();
        return;
    }

    int numSteps = (event->delta() / 8) / 15;
    vp->seek(vp->currentTime() + 10000*numSteps);

    event->accept();
}
