/* QuantumPlayer - Qt and Phonon based multimedia player
 * Copyright (C) 2010-2012 Ville Leskinen
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

#include <QFileInfo>
#include <Phonon/MediaObject>
#include <QDebug>

#include "video.h"

Video::Video(const QString &path)
{
    videoPath = path;
    getVideoInformation();
}

Video::~Video()
{
}

void Video::getVideoInformation()
{
    QFileInfo fileInfo(videoPath);
    videoName = fileInfo.fileName();

    Phonon::MediaObject mediaObject;
    mediaObject.setCurrentSource(videoPath);
    // TODO: This doesn't work. Media needs to be loaded
    qint64 totalMs = mediaObject.totalTime();
    qint64 totSeconds = totalMs / 1000;
    qint64 seconds = totSeconds % 60;
    qint64 totMinutes = totSeconds / 60;
    qint64 minutes = totMinutes % 60;
    qint64 hours = totMinutes / 60;

    videoTotalTime = QTime(hours, minutes, seconds);
    qDebug() << videoTotalTime;
}
