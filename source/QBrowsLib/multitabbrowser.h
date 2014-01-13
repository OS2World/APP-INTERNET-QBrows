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

#ifndef MULTITABBROWSER_H
#define MULTITABBROWSER_H

#include <QTabWidget>
#include <QSet>
#include "multitabwebview.h"
#include "fsbrowsercontroller.h"

struct TabItem
{
	enum ItemType{NO_BROWSER,WEB_BROWSER,FS_BROWSER} type;
	union
	{
		MultiTabWebView *webBrowser;
		FSBrowserController *fsBrowser;
	};

	TabItem()
	{
		type=NO_BROWSER;
		webBrowser=0;
	}

	TabItem(MultiTabWebView *web)
	{
		type=WEB_BROWSER;
		webBrowser=web;
	}

	TabItem(FSBrowserController *fs)
	{
		type=FS_BROWSER;
		fsBrowser=fs;
	}

	~TabItem()
	{
		if (type==FS_BROWSER)
			delete fsBrowser;
		else if (type==WEB_BROWSER)
			delete webBrowser;
	}
};

class QPushButton;

class MultiTabBrowser
	:public QTabWidget
{
	Q_OBJECT
public:
	explicit MultiTabBrowser(QWidget *parent=0);

public:
	TabItem::ItemType currentTabItemType();

public slots:
	void closeTab();
	void addBrowserTab();
	void addFSTab();

public://web browser
	bool canWebBrowserGoBack();
	bool canWebBrowserGoForward();
	void webBrowserGoTo(const QUrl &);
	void webBrowserSetPageData(const QString &urlString,const QByteArray &data);
	void webBrowserBack();
	void webBrowserForward();
	QUrl webBrowserCurrentUrl();
	void addWebSchemeWithAutoLoad(const QString &scheme);

public://file system
	bool canFSBrowserGoBack();
	bool canFSBrowserGoForward();
	void fsBrowserGoTo(const QString &path);
	void fsBrowserGoBack();
	void fsBrowserGoForward();
	QString fsBrowserCurrentPath();

signals:
	void currentTabChanged();
	void webUrlChanged(const QUrl &);
	void fsPathChanged(const QString &);
	void windowCloseRequest();//нафига??
	void beginLoading();
	void endLoading();
	void webDataRequested(QUrl &url,QByteArray *data);

private slots://web browser
	void webViewClosing();
	void webViewCreating(MultiTabWebView *newView);
	void onWebUrlChanged(const QUrl &url);
	void webPageRequested(const QUrl &url);

private slots://fs browser
	void fsViewClosing();
	void fsViewCreating(FSBrowserController *newFSCtl);
	void onFSPathChanged(const QString &path);

private slots:
	void titleChanged(const QString &);//???
	void tabChanged(int);

private:
	void setupWebView(MultiTabWebView *view);
	void deleteWebView(MultiTabWebView *view);
	void setupFSView(FSBrowserController *view);
	void deleteFSView(FSBrowserController *view);
	int findTabItemIndex(FSBrowserController *fsView);
	int findTabItemIndex(MultiTabWebView *webView);
	TabItem* findTabItemForTabWidget(QWidget *widget);
	void checkWebUrlPolicy(const QUrl &url);

private:
	QList<TabItem*> views;
	TabItem *currentUsedView;
	QList<QString> schemesWithAutoLoad;
};

#endif // MULTITABBROWSER_H
