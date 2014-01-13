#	This file is part of QBrows.
#
#	QBrows is free software: you can redistribute it and/or modify
#	it under the terms of the GNU General Public License as published by
#	the Free Software Foundation, either version 3 of the License, or
#	(at your option) any later version.
#
#	QBrows is distributed in the hope that it will be useful,
#	but WITHOUT ANY WARRANTY; without even the implied warranty of
#	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
#	GNU General Public License for more details.
#
#	You should have received a copy of the GNU General Public License
#	along with QBrows. If not, see <http://www.gnu.org/licenses/>.

appName=QBrows
include(../qmakeapp.macro.pro)
QT += network webkit

libName=QBrows
libPath=QBrowsLib
include(../qmake.uselib.macro.pro)

SOURCES += \
	main.cpp
