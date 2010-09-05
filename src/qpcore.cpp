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

#include "qpcore.h"

#include <QtCore>
#include <Phonon/BackendCapabilities>

QString PhononFilter;

void initFilter()
{
    QString allFilter(QObject::tr("All multimedia files ("));;
    QStringList mimeTypes = Phonon::BackendCapabilities::availableMimeTypes();
    QStringList videoTypes = mimeTypes.filter("video");

    if (videoTypes.contains("video/x-msvideo") || videoTypes.contains("video/msvideo")
        || videoTypes.contains("video/avi")) {
        qDebug("AVI support found");
        allFilter.append("*.avi ");
        PhononFilter.append(QObject::tr("AVI files (*.avi);;"));
    }

    if (videoTypes.contains("video/mpeg")) {
        qDebug("MPEG support found");
        allFilter.append("*.mpg *.mpeg ");
        PhononFilter.append(QObject::tr("MPEG files (*.mpg *.mpeg);;"));
    }

    if (videoTypes.contains("video/mp4")) {
        qDebug("MPEG-4 support found");
        allFilter.append("*.mp4 ");
        PhononFilter.append(QObject::tr("MPEG-4 files (*.mp4);;"));
    }

    if (videoTypes.contains("video/ogg")) {
        // *.ogg is sometimes used for video, although it is not recomended
        qDebug("Ogg support found");
        allFilter.append("*.ogg *.ogv ");
        PhononFilter.append(QObject::tr("Ogg files (*.ogg *.ogv);;"));
    }

    if (videoTypes.contains("video/x-ms-wmv")) {
        qDebug("Windows Media Video support found");
        allFilter.append("*.wmv ");
        PhononFilter.append(QObject::tr("Windows Media Video (*.wmv);;"));
    }

    allFilter.append(");;");
    PhononFilter.prepend(allFilter);
    PhononFilter.append(QObject::tr("All files (*.*)"));
}

void QPCore::initialize()
{
    initFilter();
}

QString QPCore::phononFilter()
{
    return PhononFilter;
}
