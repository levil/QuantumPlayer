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

#ifndef VIDEO_H
#define VIDEO_H

#include <QString>
#include <QTime>

class Video
{
public:
    Video(const QString &path);
    ~Video();

    QString path() const { return videoPath; }
    QString name() const { return videoName; }
    QTime totalTime() const { return videoTotalTime; }
    QString totalTimeString() const { return videoTotalTime.toString("HH:mm:ss"); }

private:
    void getVideoInformation();

    QString videoPath;
    QString videoName;
    QTime videoTotalTime;
};

#endif // VIDEO_H
