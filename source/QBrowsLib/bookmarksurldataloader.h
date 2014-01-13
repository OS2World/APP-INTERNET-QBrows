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

#ifndef BOOKMARKSURLDATALOADER_H
#define BOOKMARKSURLDATALOADER_H

#include "iurldataloader.h"
#include "bookmarkslocaldataprovider.h"

class BookmarksUrlDataLoader
	:public IUrlDataLoader
{
public:
	BookmarksUrlDataLoader();
	~BookmarksUrlDataLoader();

public:
	virtual bool loadData(QUrl &url,QByteArray *data);
	bool fillList(QString cat,QByteArray *data);
	bool addCategory(QUrl &url,QByteArray *data);
	bool delCategory(QUrl &url,QByteArray *data);
	bool addBookmark(QUrl &url,QByteArray *data);
	bool delBookmark(QUrl &url,QByteArray *data);

private:
	BookmarksLocalDataProvider *dataProvider;
};

#endif // BOOKMARKSURLDATALOADER_H
