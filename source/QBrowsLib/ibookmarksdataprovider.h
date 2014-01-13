/*******************************************
This file is part of QBrows.

QBrows is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

QBrows is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with QBrows. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef IBOOKMARKSDATAPROVIDER_H
#define IBOOKMARKSDATAPROVIDER_H

#include <QtGlobal>
#include <QStringList>

class IBookmarksDataProvider
{
public:
	struct Catergory
	{
		QString path;
		QString title;
	};

	struct Bookmark
	{
		QString path;
		QString title;
		QStringList tags;
		QString urlString;
	};

public:
	virtual bool loadCatergories(const QString &path,
								 QList<Catergory> &list)=0;
	virtual bool loadBookmarks(const QString &path,
							   QList<Bookmark> &bookmarks)=0;
	virtual bool addCategory(Catergory &category)=0;//fill id
	virtual bool addBookmark(Bookmark &bookmark)=0;//fill id
	virtual bool delCatergory(const Catergory &category)=0;
	virtual bool delBookmark(const Bookmark &bookmark)=0;
};

#endif // IBOOKMARKSDATAPROVIDER_H
