/*  QuantumPlayer - Qt and Phonon based multimedia player
 *  Copyright (C) 2010-2012  Ville Leskinen
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>
 */

#ifndef QUANTUMPLAYER_H
#define QUANTUMPLAYER_H

#include <QMainWindow>

class Player;
class PlaylistWidget;

class QAction;
class QDockWidget;

class QuantumPlayer : public QMainWindow
{
Q_OBJECT
public:
    explicit QuantumPlayer(QWidget *parent = 0);

signals:

public slots:
    void changeFullScreen(bool fullScreen);

private slots:
    void handleOpen();

protected:
    void closeEvent(QCloseEvent *event);

private:
    void initGui();
    void initActions();
    void initMenus();
    void initConnections();

    void readSettings();
    void writeSettings();

    Player *player;
    PlaylistWidget *playlistWidget;

    QAction *actionOpen;
    QAction *actionQuit;
    QAction *actionViewPlaylist;
    QAction *actionAboutQt;

    QDockWidget *playlistDock;

    bool playlistWasVisible;
};

#endif // QUANTUMPLAYER_H
