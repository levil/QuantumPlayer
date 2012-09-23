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
#include <Phonon/AudioOutput>

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
    connect(actionMute, SIGNAL(toggled(bool)), this, SLOT(handleMuteButtonToggle(bool)));
    connect(actionToggleFullscreen, SIGNAL(toggled(bool)), this, SIGNAL(toggleFullScreen(bool)));
    connect(this, SIGNAL(toggleFullScreen(bool)), this, SLOT(changeFullScreen(bool)));
    connect(actionSkipBackward, SIGNAL(triggered()), this, SIGNAL(skipBackward()));
    connect(actionSkipForward, SIGNAL(triggered()), this, SIGNAL(skipForward()));

    connect(volumeSlider, SIGNAL(sliderMoved(int)), this, SLOT(handleVolumeSliderMove(int)));
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

    actionMute = new QAction(QIcon::fromTheme("audio-volume-medium"), tr("Mute"), this);
    actionMute->setCheckable(true);

    actionToggleFullscreen = new QAction(QIcon::fromTheme("view-fullscreen"), tr("Fullscreen"), this);
    actionToggleFullscreen->setCheckable(true);
    actionToggleFullscreen->setChecked(false);
    actionToggleFullscreen->setShortcut(QKeySequence(Qt::Key_F));
}

void Player::initGui()
{
    volumeIcons << QIcon::fromTheme("audio-volume-muted")
                << QIcon::fromTheme("audio-volume-low")
                << QIcon::fromTheme("audio-volume-medium")
                << QIcon::fromTheme("audio-volume-high");
    vp = new Phonon::VideoPlayer(Phonon::VideoCategory);

    Phonon::SeekSlider *seekSlider = new Phonon::SeekSlider(vp->mediaObject());
    //Phonon::VolumeSlider *volumeSlider = new Phonon::VolumeSlider(vp->audioOutput());
    //volumeSlider->setMaximumWidth(100);

    volumeSlider = new QSlider(Qt::Horizontal);
    volumeSlider->setRange(0, 100);
    volumeSlider->setMaximumWidth(100);
    int volumeValue = (int)(vp->audioOutput()->volume()*100);
    volumeSlider->setSliderPosition((int)(volumeValue));
    if (volumeValue <= 0) {
        actionMute->setIcon(volumeIcons.at((int)Player::IconVolMuted));
    } else if (volumeValue < 33) {
        actionMute->setIcon(volumeIcons.at((int)Player::IconVolLow));
    } else if (volumeValue < 66) {
        actionMute->setIcon(volumeIcons.at((int)Player::IconVolMedium));
    } else {
        actionMute->setIcon(volumeIcons.at((int)Player::IconVolHigh));
    }

    totalTime = "00:00:00";
    timeLabel = new QLabel("00:00:00/" + totalTime);

    controlBar = new QToolBar;
    controlBar->addAction(actionPlayPause);
    controlBar->addAction(actionStop);
    controlBar->addAction(actionSkipBackward);
    controlBar->addAction(actionSkipForward);
    controlBar->addAction(actionMute);
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

void Player::changeFullScreen(bool fullScreen)
{
    if (fullScreen)
        showFullScreen();
    else
        showNormal();
}

void Player::handleVolumeSliderMove(int value)
{
    if (value <= 0) {
        actionMute->setIcon(volumeIcons.at((int)Player::IconVolMuted));
    } else if (value < 33) {
        actionMute->setIcon(volumeIcons.at((int)Player::IconVolLow));
    } else if (value < 66) {
        actionMute->setIcon(volumeIcons.at((int)Player::IconVolMedium));
    } else
        actionMute->setIcon(volumeIcons.at((int)Player::IconVolHigh));

    vp->audioOutput()->setVolume((qreal)value/100);
    qDebug() << "Volume is now: " << vp->audioOutput()->volume();
}

void Player::handleMuteButtonToggle(bool checked)
{
    if (checked) {
        actionMute->setIcon(volumeIcons.at((int)Player::IconVolMuted));
        oldVolume = volumeSlider->value();
    } else {
        if (oldVolume <= 0) {
            actionMute->setIcon(volumeIcons.at((int)Player::IconVolMuted));
        } else if (oldVolume < 33) {
            actionMute->setIcon(volumeIcons.at((int)Player::IconVolLow));
        } else if (oldVolume < 66) {
            actionMute->setIcon(volumeIcons.at((int)Player::IconVolMedium));
        } else
            actionMute->setIcon(volumeIcons.at((int)Player::IconVolHigh));

        vp->audioOutput()->setVolume((qreal)oldVolume/100);
    }

    vp->audioOutput()->setMuted(checked);
    volumeSlider->setEnabled(!checked);

    qDebug() << "Volume is now: " << vp->audioOutput()->volume();
}
