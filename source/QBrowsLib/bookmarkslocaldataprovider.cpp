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

#include "bookmarkslocaldataprovider.h"
#include <QSettings>
#include <QTemporaryFile>
#include <QDebug>

BookmarksLocalDataProvider *BookmarksLocalDataProvider::instance=0;

BookmarksLocalDataProvider* BookmarksLocalDataProvider::getInstance()
{
	if (instance==0)
		instance=new BookmarksLocalDataProvider;
	return instance;
}

void BookmarksLocalDataProvider::destroy()
{
	if (instance!=0)
	{
		delete instance;
		instance=0;
	}
}

BookmarksLocalDataProvider::BookmarksLocalDataProvider()
	:bookmarksDBPath(QDir::homePath()+"/.QBrows/BookmarksDB")
{
	if (!bookmarksDBPath.exists())
		bookmarksDBPath.mkpath(bookmarksDBPath.path());
}

BookmarksLocalDataProvider::~BookmarksLocalDataProvider()
{
}

bool BookmarksLocalDataProvider::loadCatergories(const QString &path,
												 QList<Catergory> &list)
{
	list.clear();
	QString cdPath=path;
	while (cdPath.startsWith("/")) cdPath.remove(0,1);
	QDir workDir=bookmarksDBPath;
	if (!cdPath.isEmpty())
		if (!workDir.cd(cdPath)) return false;
	if (!workDir.path().contains(bookmarksDBPath.path()))
		workDir=bookmarksDBPath;
	QStringList dirs=workDir.entryList(QDir::Dirs|QDir::NoDotAndDotDot);
	for (int i=0;i<dirs.count();i++)
	{
		if (cdPath!=0)
		{
			Catergory cat={"/"+cdPath+"/"+dirs[i],dirs[i]};
			list.append(cat);
		}
		else
		{
			Catergory cat={"/"+dirs[i],dirs[i]};
			list.append(cat);
		}
	}
	return true;
}

bool BookmarksLocalDataProvider::loadBookmarks(const QString &path,
											   QList<Bookmark> &bookmarks)
{
	bookmarks.clear();
	QString cdPath=path;
	while (cdPath.startsWith("/")) cdPath.remove(0,1);
	QDir workDir=bookmarksDBPath;
	if (!cdPath.isEmpty())
		if (!workDir.cd(cdPath)) return false;
	if (!workDir.path().contains(bookmarksDBPath.path()))
		workDir=bookmarksDBPath;
	QStringList files=workDir.entryList(QStringList("*.bookmark"),QDir::Files);
	for (int i=0;i<files.count();i++)
	{
		QSettings bookmarkSettings(workDir.absolutePath()+"/"+files[i],
								   QSettings::IniFormat);
		QString title=bookmarkSettings.value("title","Empty").toString();
		QString url=bookmarkSettings.value("url","about:blank").toString();
		QStringList tags=bookmarkSettings.
						 value("tags",QStringList("")).toStringList();
		Bookmark bookmark={path,title,tags,url};
		bookmarks.append(bookmark);
	}
	return true;
}

bool BookmarksLocalDataProvider::addCategory(Catergory &category)
{
	QString cdPath=category.path;
	while (cdPath.startsWith("/")) cdPath.remove(0,1);
	QDir workDir=bookmarksDBPath;
	if (!cdPath.isEmpty())
		if (!workDir.cd(cdPath)) return false;
	if (!workDir.path().contains(bookmarksDBPath.path()))
		workDir=bookmarksDBPath;
	QFileInfo fi(category.title);
	category.title=fi.fileName();
	return workDir.mkdir(category.title);
}

bool BookmarksLocalDataProvider::addBookmark(Bookmark &bookmark)
{
	QString cdPath=bookmark.path;
	while (cdPath.startsWith("/")) cdPath.remove(0,1);
	QDir workDir=bookmarksDBPath;
	if (!cdPath.isEmpty())
		if (!workDir.cd(cdPath)) return false;
	if (!workDir.path().contains(bookmarksDBPath.path()))
		workDir=bookmarksDBPath;
	QTemporaryFile file("XXXXXX.bookmark");
	file.open();
	file.close();
	file.setAutoRemove(false);
	QSettings bookmarkSettings(workDir.absolutePath()+"/"+file.fileName(),
							   QSettings::IniFormat);
	bookmarkSettings.setValue("title",bookmark.title);
	bookmarkSettings.setValue("url",bookmark.urlString);
	bookmarkSettings.setValue("tags",bookmark.tags);
	bookmark.path+="/"+file.fileName();
	return true;
}

bool BookmarksLocalDataProvider::delCatergory(const Catergory &category)
{
	QString cdPath=category.path;
	while (cdPath.startsWith("/")) cdPath.remove(0,1);
	QDir workDir=bookmarksDBPath;
	if (!cdPath.isEmpty())
		if (!workDir.cd(cdPath)) return false;
	if (!workDir.path().contains(bookmarksDBPath.path()))
		workDir=bookmarksDBPath;
	QList<Catergory> cats;
	loadCatergories(category.path,cats);
	for (int i=0;i<cats.count();i++)
		delCatergory(cats[i]);
	QStringList files=workDir.entryList(QDir::Files);
	for (int i=0;i<files.count();i++)
		workDir.remove(files[i]);
	return workDir.rmdir(".");
}

bool BookmarksLocalDataProvider::delBookmark(const Bookmark &bookmark)
{
	QFileInfo fi(bookmark.path);
	QString cdPath=fi.dir().path();
	while (cdPath.startsWith("/")) cdPath.remove(0,1);
	QDir workDir=bookmarksDBPath;
	if (!cdPath.isEmpty())
		if (!workDir.cd(cdPath)) return false;
	if (!workDir.path().contains(bookmarksDBPath.path()))
		workDir=bookmarksDBPath;
	return workDir.remove(fi.fileName());
}
