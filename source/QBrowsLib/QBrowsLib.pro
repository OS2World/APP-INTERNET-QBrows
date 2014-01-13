#	This file is part of QBrows.
#
#	QBrows is free software: you can redistribute it and/or modify
#	it under the terms of the GNU General Public License as published by
#	the Free Software Foundation, either version 3 of the License, or
#	(at your option) any later version.
#
#	QBrows is distributed in the hope that it will be useful,
#	but WITHOUT ANY WARRANTY; without even the implied warranty of
#	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#	GNU General Public License for more details.
#
#	You should have received a copy of the GNU General Public License
#	along with QBrows. If not, see <http://www.gnu.org/licenses/>.

libName=QBrows
include(../qmakeslib.macro.pro)

QT += network webkit

HEADERS += \
	browserwidget.h \
	browsercontroller.h \
	fsbrowsercontroller.h \
	multitabwebview.h \
	multitabbrowser.h \
    ibookmarksdataprovider.h \
    iurldataloader.h \
    bookmarksurldataloader.h \
    bookmarkslocaldataprovider.h \
    settingswidget.h

SOURCES += \
	browserwidget.cpp \
	browsercontroller.cpp \
	fsbrowsercontroller.cpp \
	multitabwebview.cpp \
	multitabbrowser.cpp \
    bookmarksurldataloader.cpp \
    bookmarkslocaldataprovider.cpp \
    settingswidget.cpp

RESOURCES += \
	Resources/qbrowscommon.qrc
