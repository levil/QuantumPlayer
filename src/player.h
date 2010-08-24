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
#ifndef PLAYER_H
#define PLAYER_H

#include <QWidget>

namespace Phonon {
    class VideoPlayer;
};

class QHBoxLayout;
class QVBoxLayout;
class QAction;
class QToolBar;

class Player : public QWidget
{
Q_OBJECT
public:
    explicit Player(QWidget *parent = 0);

    void setPlayEnabled(bool enabled);
    void setSkipBackwardEnabled(bool enabled);
    void setSkipForwardEnabled(bool enabled);

    QAction *playPauseAct() const { return actionPlayPause; };
    QAction *stopAct() const { return actionStop; };

signals:
    void skipBackward();
    void skipForward();
    void playerFinished();
    void toggleFullScreen(bool fullScreen);

public slots:
    void loadMedia(const QString &mediaUrl);
    void play(const QString &mediaUrl);

private slots:
    void handlePlayPause();
    void handleStop();

private:
    void initConnections();
    void initActions();
    void initGui();

    void changePlayPause(bool showPlay);

    Phonon::VideoPlayer *vp;

    QAction *actionPlayPause;
    QAction *actionStop;
    QAction *actionSkipBackward;
    QAction *actionSkipForward;
    QAction *actionToggleFullscreen;

    QVBoxLayout *vLayout;
    QToolBar *controlBar;
};

#endif // PLAYER_H
