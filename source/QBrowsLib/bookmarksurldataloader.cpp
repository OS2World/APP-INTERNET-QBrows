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

#include "bookmarksurldataloader.h"
#include <QDebug>

BookmarksUrlDataLoader::BookmarksUrlDataLoader()
{
	dataProvider=BookmarksLocalDataProvider::getInstance();
}

BookmarksUrlDataLoader::~BookmarksUrlDataLoader()
{
}

bool BookmarksUrlDataLoader::loadData(QUrl &url,QByteArray *data)
{
	qDebug()<<"Url= "<<url;
	QString path=url.path();
	qDebug()<<"path= "<<path;
	if (path.startsWith("/list"))
		return fillList(path.mid(5),data);
	else if (path.startsWith("/cat_add"))
		return addCategory(url,data);
	else if (path.startsWith("/cat_add"))
		return delCategory(url,data);
	else if (path.startsWith("/book_add"))
		return addBookmark(url,data);
	else if (path.startsWith("/book_del"))
		return delBookmark(url,data);
	return false;
}

bool BookmarksUrlDataLoader::fillList(QString cat,QByteArray *data)
{
	while(cat.endsWith('/')) cat.chop(1);
	while(cat.startsWith('/')) cat.remove(0,1);
	qDebug()<<"cat= "<<cat;
	QString parentPath="/"+cat;
	parentPath=parentPath.left(parentPath.lastIndexOf('/')+1);
	qDebug()<<parentPath;
	*data="<html>\n"
		  "<head><title>/"+cat.toUtf8()+"</title></head>\n"
		  "<body bgcolor=white>\n";
	if (!cat.isEmpty())
		data->append("<p><a href=\"bookmarks:/list"+
					 parentPath.toUtf8()+"\">..</a></p>\n");
	data->append("<font color=red>");
	data->append("Categories");
	data->append("</font>\n");
	data->append("<p>\n");
	data->append("<a href='bookmarks:/cat_add?path=/'"+
				 cat.toUtf8()+"'>New category</a><br/>");
	QList<IBookmarksDataProvider::Catergory> categories;
	dataProvider->loadCatergories(cat,categories);
	for (int i=0;i<categories.count();i++)
	{
		data->append("<a href=\"bookmarks:/list"+
					 categories[i].path.toUtf8()+"\">");
		data->append(categories[i].title);
		data->append("</a><br/>\n");
	}
	data->append("</p>\n");

	data->append("<font color=red>");
	data->append("Bookmarks");
	data->append("</font>\n");
	data->append("<p>\n");
	data->append("<a href='bookmarks:/book_add?path=/'"+
				 cat.toUtf8()+"'>New bookmark</a><br/>");
	QList<IBookmarksDataProvider::Bookmark> bookmarks;
	dataProvider->loadBookmarks(cat,bookmarks);
	for (int i=0;i<bookmarks.count();i++)
	{
		data->append("<a href=\""+bookmarks[i].urlString.toUtf8()+"\">");
		data->append(bookmarks[i].title);
		data->append("</a><br/>\n");
	}
	data->append("</p>");
	data->append("</body>");
	return true;
}

bool BookmarksUrlDataLoader::addCategory(QUrl &url,
										 QByteArray *data)
{
	QString path=url.queryItemValue("path");
	QString title=url.queryItemValue("title");
	while(path.startsWith('/')) path.remove(0,1);
	while(path.endsWith('/')) path.chop(1);
	path.insert(0,'/');
	if (title.isEmpty())
	{
		*data="<html>\n"
			  "<head>"
			  "<title>Add category</title>\n"
			  "<script language=\"javascript\">\n"
			  "<!--\n"
			  "function update()\n"
			  "{\n"
			  "	document.getElementById('link').href=\""
			  "bookmarks:/cat_add?path="+path.toUtf8()+
			  "&title=\"+document.getElementById('catTitle').value;\n"
			  "}\n"
			  "//-->\n"
			  "</script>\n"
			  "</head>\n"
			  "<body bgcolor=white>\n"
			  "<p>Category title</p>\n"
			  "<p><input id='catTitle' onchange=\"update();\"/></p>\n"
			  "<p><a id='link' href='bookmarks:/cat_add?path="+
			  path.toUtf8()+"&title='>Add</a></p>\n"
			  "</body>\n"
			  "</html>\n";
	}
	else
	{
		IBookmarksDataProvider::Catergory cat;
		cat.path=path;
		cat.title=title;
		dataProvider->addCategory(cat);
		url=QUrl("bookmarks:/list"+path);
		emit redirect(url,data);
	}
	return true;
}

bool BookmarksUrlDataLoader::addBookmark(QUrl &url,
										 QByteArray *data)
{
	QString path=url.queryItemValue("path");
	QString title=url.queryItemValue("title");
	QString urlString=url.queryItemValue("url");
	QString tags=url.queryItemValue("tags");
	while(path.startsWith('/')) path.remove(0,1);
	while(path.endsWith('/')) path.chop(1);
	path.insert(0,'/');
	if (title.isEmpty()||url.isEmpty())
	{
		*data="<html>\n"
			  "<head>"
			  "<title>Add category</title>\n"
			  "<script language=\"javascript\">\n"
			  "<!--\n"
			  "function update()\n"
			  "{\n"
			  "	document.getElementById('link').href=\""
			  "bookmarks:/book_add?path="+path.toUtf8()+
			  "&title=\"+document.getElementById('bookmarkTitle').value+"
			  "\"&url=\"+document.getElementById('bookmarkUrl').value+"
			  "\"&tags=\"+document.getElementById('bookmarkTags').value;\n"
			  "}\n"
			  "//-->\n"
			  "</script>\n"
			  "</head>\n"
			  "<body bgcolor=white>\n"
			  "<p>Bookmark title</p>\n"
			  "<p><input id='bookmarkTitle' onchange=\"update();\"/></p>\n"
			  "<p>Bookmark url</p>\n"
			  "<p><input id='bookmarkUrl' onchange=\"update();\"/></p>\n"
			  "<p>Bookmark tags</p>\n"
			  "<p><input id='bookmarkTags' onchange=\"update();\"/></p>\n"
			  "<p><a id='link' href='bookmarks:/book_add?path="+
			  path.toUtf8()+"'>Add</a></p>\n"
			  "</body>\n"
			  "</html>\n";
		qDebug()<<*data;
	}
	else
	{
		IBookmarksDataProvider::Bookmark bookmark;
		bookmark.path=path;
		bookmark.title=title;
		bookmark.urlString=urlString;
		bookmark.tags=tags.split(',');
		dataProvider->addBookmark(bookmark);
		url=QUrl("bookmarks:/list"+path);
		emit redirect(url,data);
	}
	return true;
}

bool BookmarksUrlDataLoader::delCategory(QUrl &url,QByteArray *data)
{
	QString path=url.queryItemValue("path");
	while(path.startsWith('/')) path.remove(0,1);
	while(path.endsWith('/')) path.chop(1);
	path.insert(0,'/');
	bool retVal;
	if (path!="/")
	{
		IBookmarksDataProvider::Catergory cat;
		cat.path=path;
		retVal=dataProvider->delCatergory(cat);
	}
	url=QUrl("bookmarks:/list"+path);
	emit redirect(url,data);
	return retVal;
}

bool BookmarksUrlDataLoader::delBookmark(QUrl &url,QByteArray *data)
{
	QString path=url.queryItemValue("path");
	while(path.startsWith('/')) path.remove(0,1);
	while(path.endsWith('/')) path.chop(1);
	path.insert(0,'/');
	bool retVal;
	if (path!="/")
	{
		IBookmarksDataProvider::Bookmark book;
		book.path=path;
		retVal=dataProvider->delBookmark(book);
	}
	url=QUrl("bookmarks:/list"+path);
	emit redirect(url,data);
	return retVal;
}
