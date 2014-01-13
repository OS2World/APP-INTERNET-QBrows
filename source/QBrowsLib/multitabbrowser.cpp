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

#include "multitabbrowser.h"
#include <QPushButton>
#include <QWebHistory>
#include <QLayout>

MultiTabBrowser::MultiTabBrowser(QWidget *parent)
	:QTabWidget(parent)
{
	connect(this,SIGNAL(currentChanged(int)),
			this,SLOT(tabChanged(int)));

	MultiTabWebView *firstTab=new MultiTabWebView(this);
	setupWebView(firstTab);
	addTab(firstTab,tr("Empty tab"));
	currentUsedView=views[0];
	schemesWithAutoLoad.append("http");
	schemesWithAutoLoad.append("https");
}

TabItem::ItemType MultiTabBrowser::currentTabItemType()
{
	if (currentUsedView!=0) return currentUsedView->type;
	return TabItem::NO_BROWSER;
}

int MultiTabBrowser::findTabItemIndex(FSBrowserController *fsView)
{
	for (int i=0;i<views.count();i++)
		if ((views[i]->type==TabItem::FS_BROWSER)&&
			(views[i]->fsBrowser==fsView))
			return i;
	return -1;
}

int MultiTabBrowser::findTabItemIndex(MultiTabWebView *webView)
{
	for (int i=0;i<views.count();i++)
		if ((views[i]->type==TabItem::WEB_BROWSER)&&
			(views[i]->webBrowser==webView))
			return i;
	return -1;
}

TabItem* MultiTabBrowser::findTabItemForTabWidget(QWidget *widget)
{
	for (int i=0;i<views.count();i++)
	{
		if (views[i]->type==TabItem::FS_BROWSER)
		{
			if (widget==views[i]->fsBrowser->widget())
				return views[i];
		}
		else if (views[i]->type==TabItem::WEB_BROWSER)
		{
			if (widget==views[i]->webBrowser)
				return views[i];
		}
	}
	return 0;
}

void MultiTabBrowser::closeTab()
{
	if (currentUsedView==0) return;
	if (views.count()<=1)
	{
		webBrowserGoTo(QUrl());
		return;
	}
	views.removeAt(views.indexOf(currentUsedView));
	delete currentUsedView;
	currentUsedView=views[currentIndex()];
}

void MultiTabBrowser::addBrowserTab()
{
	int index=currentIndex();
	MultiTabWebView *newView=new MultiTabWebView;
	setupWebView(newView);
	insertTab(index+1,newView,tr("Empty tab"));
}

void MultiTabBrowser::addFSTab()
{
	int index=currentIndex();
	FSBrowserController *newView=new FSBrowserController;
	setupFSView(newView);
	insertTab(index+1,newView->widget(),tr("Empty tab"));
	tabChanged(index);
}

void MultiTabBrowser::webViewCreating(MultiTabWebView *newView)
{
	newView->setParent(this);
	setupWebView(newView);
	addTab(newView,tr("Empty tab"));
	setCurrentWidget(newView);
	emit currentTabChanged();
}

void MultiTabBrowser::setupWebView(MultiTabWebView *view)
{
	TabItem *newItem=new TabItem(view);
	views.append(newItem);

	connect(view,SIGNAL(newWebBrowserInstance(MultiTabWebView*)),
			this,SLOT(webViewCreating(MultiTabWebView*)));
	connect(view->page(),SIGNAL(windowCloseRequested()),
			this,SLOT(webViewClosing()));
	connect(view,SIGNAL(titleChanged(QString)),
			this,SLOT(titleChanged(QString)));
	connect(view,SIGNAL(urlChanged(QUrl)),
			this,SLOT(onWebUrlChanged(QUrl)));
	connect(view,SIGNAL(loadStarted()),this,SIGNAL(beginLoading()));
	connect(view,SIGNAL(loadFinished(bool)),this,SIGNAL(endLoading()));
	connect(view,SIGNAL(linkClicked(QUrl)),
			this,SLOT(webPageRequested(QUrl)));
}

void MultiTabBrowser::webViewClosing()
{
	MultiTabWebView *closingView=(MultiTabWebView*)sender();
	deleteWebView(closingView);
}

void MultiTabBrowser::deleteWebView(MultiTabWebView *view)
{
	if (views.count()>1)
	{
		int index=findTabItemIndex(view);
		if (index!=-1)
		{
			if (views[index]==currentUsedView) closeTab();
			else
			{
				delete views[index];
				views.removeAt(index);
				removeTab(indexOf(view));
			}
		}
	}
	else view->setUrl(QUrl());
}

void MultiTabBrowser::fsViewCreating(FSBrowserController *newFSCtl)
{
	newFSCtl->setParent(this);
	setupFSView(newFSCtl);
	addTab(newFSCtl->widget(),tr("Empty tab"));
	setCurrentWidget(newFSCtl->widget());
	emit currentTabChanged();
}

void MultiTabBrowser::setupFSView(FSBrowserController *view)
{
	TabItem *newItem=new TabItem(view);
	views.append(newItem);

	connect(view,SIGNAL(newFSBrowserInstance(FSBrowserController*)),
			this,SLOT(fsViewCreating(FSBrowserController*)));
	connect(view,SIGNAL(closeRequest()),
			this,SLOT(fsViewClosing()));
	connect(view,SIGNAL(titleChanged(QString)),
			this,SLOT(titleChanged(QString)));
	connect(view,SIGNAL(pathChanged(QString)),
			this,SLOT(onFSPathChanged(QString)));
	connect(view,SIGNAL(beginLoading()),this,SIGNAL(beginLoading()));
	connect(view,SIGNAL(endLoading()),this,SIGNAL(endLoading()));
}

void MultiTabBrowser::fsViewClosing()
{
	FSBrowserController *closingView=(FSBrowserController*)sender();
	deleteFSView(closingView);
}

void MultiTabBrowser::deleteFSView(FSBrowserController *view)
{
	if (views.count()>1)
	{
		int index=findTabItemIndex(view);
		if (index!=-1)
		{
			if (views[index]==currentUsedView) closeTab();
			else
			{
				delete views[index];
				views.removeAt(index);
				removeTab(indexOf(view->widget()));
			}
		}
	}
	else view->setCurrentDir(QDir::root());
}

void MultiTabBrowser::titleChanged(const QString &title)
{
	QString titleStr=title;
	if (titleStr.isEmpty()) titleStr=tr("Empty tab");
	if (sender()->metaObject()->className()==
		MultiTabWebView::staticMetaObject.className())
	{
		MultiTabWebView *view=(MultiTabWebView*)sender();
		setTabText(indexOf(view),titleStr);
	}
	else if (sender()->metaObject()->className()==
			 FSBrowserController::staticMetaObject.className())
	{
		FSBrowserController *view=(FSBrowserController*)sender();
		setTabText(indexOf(view->widget()),titleStr);
	}
}

//void MultiTabBrowser::iconChanged()
//{
//	MultiTabWebView *view=(MultiTabWebView*)sender();
//	if (!views.contains(view)) return;
//	setTabIcon(indexOf(view),view->icon());
//}

void MultiTabBrowser::tabChanged(int index)
{
	QWidget *tabWidget=widget(index);
	currentUsedView=findTabItemForTabWidget(tabWidget);
	emit currentTabChanged();
}

void MultiTabBrowser::webBrowserGoTo(const QUrl &url)
{
	QUrl loadingUrl=url;
	if (currentUsedView==0) return;
	if (currentUsedView->type!=TabItem::WEB_BROWSER)
	{
		int tabIndex=currentIndex();
		views.removeAt(views.indexOf(currentUsedView));
		if (currentUsedView->type==TabItem::FS_BROWSER)
			delete currentUsedView->fsBrowser;
		MultiTabWebView *newView=new MultiTabWebView(this);
		setupWebView(newView);
		insertTab(tabIndex,newView,tr("Empty tab"));
		setCurrentWidget(newView);
	}
	if (!schemesWithAutoLoad.contains(url.scheme()))
	{
		currentUsedView->webBrowser->page()->
				setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
		QByteArray data;
		emit webDataRequested(loadingUrl,&data);
		currentUsedView->webBrowser->setHtml(data,loadingUrl);
	}
	else
	{
		currentUsedView->webBrowser->page()->
				setLinkDelegationPolicy(QWebPage::DontDelegateLinks);
		currentUsedView->webBrowser->setUrl(loadingUrl);
	}
}

void MultiTabBrowser::webBrowserSetPageData(const QString &urlString,
											const QByteArray &data)
{
	if (currentUsedView==0) return;
	if (currentUsedView->type!=TabItem::WEB_BROWSER)
	{
		int tabIndex=currentIndex();
		views.removeAt(views.indexOf(currentUsedView));
		if (currentUsedView->type==TabItem::FS_BROWSER)
			delete currentUsedView->fsBrowser;
		MultiTabWebView *newView=new MultiTabWebView(this);
		setupWebView(newView);
		insertTab(tabIndex,newView,tr("Empty tab"));
		setCurrentWidget(newView);
	}
	currentUsedView->webBrowser->setHtml(QString::fromUtf8(data),urlString);
	emit webUrlChanged(QUrl(urlString));
}

void MultiTabBrowser::fsBrowserGoTo(const QString &path)
{
	if (currentUsedView==0) return;
	if (currentUsedView->type!=TabItem::FS_BROWSER)
	{
		int tabIndex=currentIndex();
		views.removeAt(views.indexOf(currentUsedView));
		if (currentUsedView->type==TabItem::WEB_BROWSER)
		{
			delete currentUsedView->webBrowser;
		}
		FSBrowserController *newView=new FSBrowserController(this);
		setupFSView(newView);
		insertTab(tabIndex,newView->widget(),tr("Empty tab"));
		setCurrentWidget(newView->widget());
	}
	currentUsedView->fsBrowser->setCurrentDir(path);
}

void MultiTabBrowser::onWebUrlChanged(const QUrl &url)
{
	MultiTabWebView *view=(MultiTabWebView*)sender();
	if (currentUsedView==0) return;
	if (currentUsedView->webBrowser==view)
		emit webUrlChanged(url);
}

void MultiTabBrowser::onFSPathChanged(const QString &path)
{
	FSBrowserController *view=(FSBrowserController*)sender();
	if (currentUsedView==0) return;
	if (currentUsedView->fsBrowser==view)
		emit fsPathChanged(path);
}

bool MultiTabBrowser::canWebBrowserGoBack()
{
	if (currentUsedView==0) return false;
	if (currentUsedView->type!=TabItem::WEB_BROWSER) return false;
	return currentUsedView->webBrowser->history()->canGoBack();
}

bool MultiTabBrowser::canWebBrowserGoForward()
{
	if (currentUsedView==0) return false;
	if (currentUsedView->type!=TabItem::WEB_BROWSER) return false;
	return currentUsedView->webBrowser->history()->canGoForward();
}

void MultiTabBrowser::webBrowserBack()
{
	if (currentUsedView==0) return;
	if (currentUsedView->type!=TabItem::WEB_BROWSER) return;
	currentUsedView->webBrowser->back();
}

void MultiTabBrowser::webBrowserForward()
{
	if (currentUsedView==0) return;
	if (currentUsedView->type!=TabItem::WEB_BROWSER) return;
	currentUsedView->webBrowser->forward();
}

QUrl MultiTabBrowser::webBrowserCurrentUrl()
{
	if (currentUsedView==0) return QUrl();
	if (currentUsedView->type!=TabItem::WEB_BROWSER) return QUrl();
	return currentUsedView->webBrowser->url();
}

bool MultiTabBrowser::canFSBrowserGoBack()
{
	if (currentUsedView==0) return false;
	if (currentUsedView->type!=TabItem::FS_BROWSER) return false;
	return currentUsedView->fsBrowser->canGoBack();
}

bool MultiTabBrowser::canFSBrowserGoForward()
{
	if (currentUsedView==0) return false;
	if (currentUsedView->type!=TabItem::FS_BROWSER) return false;
	return currentUsedView->fsBrowser->canGoForward();
}

void MultiTabBrowser::fsBrowserGoBack()
{
	if (currentUsedView==0) return;
	if (currentUsedView->type!=TabItem::FS_BROWSER) return;
	currentUsedView->fsBrowser->goBack();
}

void MultiTabBrowser::fsBrowserGoForward()
{
	if (currentUsedView==0) return;
	if (currentUsedView->type!=TabItem::FS_BROWSER) return;
	currentUsedView->fsBrowser->goForward();
}

QString MultiTabBrowser::fsBrowserCurrentPath()
{
	if (currentUsedView==0) return QString();
	if (currentUsedView->type!=TabItem::FS_BROWSER) return QString();
	return currentUsedView->fsBrowser->getCurrentDir();
}

void MultiTabBrowser::addWebSchemeWithAutoLoad(const QString &scheme)
{
	if (!schemesWithAutoLoad.contains(scheme))
		schemesWithAutoLoad.append(scheme);
}

void MultiTabBrowser::webPageRequested(const QUrl &url)
{
	QUrl loadingUrl=url;
	MultiTabWebView *view=qobject_cast<MultiTabWebView*>(sender());
	if (view==0) return;
	if (!schemesWithAutoLoad.contains(url.scheme()))
	{
		QByteArray data;
		emit webDataRequested(loadingUrl,&data);
		view->setHtml(data,loadingUrl);
	}
	else
	{
		view->page()->setLinkDelegationPolicy(QWebPage::DontDelegateLinks);
		view->setUrl(loadingUrl);
	}
}
