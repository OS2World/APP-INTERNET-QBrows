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

#ifndef BOOKMARKSLOCALDATAPROVIDER_H
#define BOOKMARKSLOCALDATAPROVIDER_H

#include "ibookmarksdataprovider.h"
#include <QDir>

class BookmarksLocalDataProvider
	:public IBookmarksDataProvider
{
public:
	static BookmarksLocalDataProvider* getInstance();
	static void destroy();

private:
	static BookmarksLocalDataProvider *instance;

private:
	BookmarksLocalDataProvider();
	~BookmarksLocalDataProvider();

private:
	QDir bookmarksDBPath;

public:
	virtual bool loadCatergories(const QString &path,QList<Catergory> &list);
	virtual bool loadBookmarks(const QString &path,
							   QList<Bookmark> &bookmarks);
	virtual bool addCategory(Catergory &category);//fill id
	virtual bool addBookmark(Bookmark &bookmark);//fill id
	virtual bool delCatergory(const Catergory &category);
	virtual bool delBookmark(const Bookmark &bookmark);
};

#endif // BOOKMARKSLOCALDATAPROVIDER_H
