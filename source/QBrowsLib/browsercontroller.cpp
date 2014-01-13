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

#include "browsercontroller.h"
#include "multitabbrowser.h"
#include "bookmarksurldataloader.h"
#include "settingswidget.h"
#include <QSettings>
#include <QDir>
#include <QDebug>

int BrowserController::instanceCount=0;

BrowserController::BrowserController(QObject *parent)
	:QObject(parent)
{
	Q_INIT_RESOURCE(qbrowscommon);
	BookmarksLocalDataProvider::getInstance();
	instanceCount++;
	browserWidget=0;
	schemeDataLoaders["bookmarks"]=new BookmarksUrlDataLoader;
	connect(schemeDataLoaders["bookmarks"],SIGNAL(redirect(QUrl&,QByteArray*)),
			this,SLOT(webDataRequested(QUrl&,QByteArray*)));
}

BrowserController::~BrowserController()
{
	if (browserWidget->isAlive())
		delete browserWidget;
	QList<IUrlDataLoader*> vals=schemeDataLoaders.values();
	for (int i=0;i<vals.count();i++)
		delete vals[i];
	instanceCount--;
	if (instanceCount==0)
		BookmarksLocalDataProvider::destroy();
}

void BrowserController::checkWidget()
{
	if (!browserWidget->isAlive())
	{
		browser=new MultiTabBrowser;
		browserWidget=new BrowserWidget(browser);

		connect(browserWidget,SIGNAL(goBtnClicked()),
				this,SLOT(goBtnClicked()));
		connect(browserWidget,SIGNAL(backBtnClicked()),
				this,SLOT(backBtnClicked()));
		connect(browserWidget,SIGNAL(forwardBtnClicked()),
				this,SLOT(forwardBtnClicked()));
		connect(browserWidget,SIGNAL(addTabBtnClicked()),
				browser,SLOT(addBrowserTab()));
		connect(browserWidget,SIGNAL(closeTabBtnClicked()),
				browser,SLOT(closeTab()));
		connect(browserWidget,SIGNAL(bookmarksClicked()),
				this,SLOT(bookmarksClicked()));
		connect(browserWidget,SIGNAL(settingsClicked()),
				this,SLOT(settingsClicked()));

		connect(browser,SIGNAL(currentTabChanged()),this,SLOT(tabChanged()));
		connect(browser,SIGNAL(webUrlChanged(QUrl)),
				this,SLOT(webUrlChanged(QUrl)));
		connect(browser,SIGNAL(fsPathChanged(QString)),
				this,SLOT(fsPathChanged(QString)));
		connect(browser,SIGNAL(beginLoading()),
				browserWidget,SLOT(beginLoading()));
		connect(browser,SIGNAL(endLoading()),
				browserWidget,SLOT(endLoading()));
		connect(browser,SIGNAL(webDataRequested(QUrl&,QByteArray*)),
				this,SLOT(webDataRequested(QUrl&,QByteArray*)));

		QSettings settings(QDir::homePath()+"/.QBrows/settings.ini",
						   QSettings::IniFormat);
		QString homePath=settings.value("home","about:blank").toString();
		setAddress(homePath);
	}
}

QWidget* BrowserController::widget()
{
	checkWidget();
	return browserWidget;
}

void BrowserController::goBtnClicked()
{
	QString address;
	browserWidget->getAddress(address);
	setAddress(address);
}

void BrowserController::setAddress(const QString &address)
{
	if (address.isEmpty()) return;
	checkWidget();
	lastAddress=address;
	if ((lastAddress.startsWith("http://"))||
		(lastAddress.startsWith("https://")))
	{
		browser->webBrowserGoTo(QUrl(lastAddress));
		checkBackAndForward();
	}
	else if (lastAddress.startsWith("/"))
	{
		browser->fsBrowserGoTo(lastAddress);
	}
	else if (lastAddress.startsWith("file://"))
	{
		browser->fsBrowserGoTo(
				lastAddress.remove(0,QString("file://").length()));
	}
	else if (lastAddress.startsWith("ftp"))
	{
		//ftp browser
	}
	else if ((lastAddress.length()>=2)&&(lastAddress[1]==':'))
	{
		if (((lastAddress[0]>'a')&&(lastAddress[0]<'z'))||
			((lastAddress[0]>'A')&&(lastAddress[0]<'Z')))
		{
			browser->fsBrowserGoTo(lastAddress);
		}
	}
	else
	{
		QUrl url=QUrl(lastAddress);
		if (!schemeDataLoaders.contains(url.scheme()))
		{
			lastAddress.insert(0,"http://");
			browserWidget->setAddress(lastAddress);
			url=QUrl(lastAddress);
		}
		browser->webBrowserGoTo(url);
		checkBackAndForward();
	}
}

void BrowserController::checkBackAndForward()
{
	TabItem::ItemType type=browser->currentTabItemType();
	if (type==TabItem::WEB_BROWSER)
	{
		browserWidget->enableBackBtn(browser->canWebBrowserGoBack());
		browserWidget->enableForwardBtn(
				browser->canWebBrowserGoForward());
	}
	else if (type==TabItem::FS_BROWSER)
	{
		browserWidget->enableBackBtn(browser->canFSBrowserGoBack());
		browserWidget->enableForwardBtn(
				browser->canFSBrowserGoForward());
	}
}

void BrowserController::backBtnClicked()
{
	TabItem::ItemType type=browser->currentTabItemType();
	if (type==TabItem::WEB_BROWSER)
	{
		browser->webBrowserBack();
		checkBackAndForward();
	}
	else if (type==TabItem::FS_BROWSER)
	{
		browser->fsBrowserGoBack();
		checkBackAndForward();
	}
}

void BrowserController::forwardBtnClicked()
{
	TabItem::ItemType type=browser->currentTabItemType();
	if (type==TabItem::WEB_BROWSER)
	{
		browser->webBrowserForward();
		checkBackAndForward();
	}
	else if (type==TabItem::FS_BROWSER)
	{
		browser->fsBrowserGoForward();
		checkBackAndForward();
	}
}

void BrowserController::webUrlChanged(const QUrl &url)
{
	checkBackAndForward();
	browserWidget->setAddress(url.toString());
}

void BrowserController::fsPathChanged(const QString &path)
{
	checkBackAndForward();
	browserWidget->setAddress(path);
}

void BrowserController::webDataRequested(QUrl &url,QByteArray *data)
{
	if (schemeDataLoaders.contains(url.scheme()))
		schemeDataLoaders[url.scheme()]->loadData(url,data);
}

void BrowserController::tabChanged()
{
	if (browser->currentTabItemType()==TabItem::WEB_BROWSER)
		browserWidget->setAddress(browser->webBrowserCurrentUrl().toString());
	else if (browser->currentTabItemType()==TabItem::FS_BROWSER)
		browserWidget->setAddress(browser->fsBrowserCurrentPath());
}

void BrowserController::bookmarksClicked()
{
	setAddress("bookmarks:/list/");
}

void BrowserController::settingsClicked()
{
	SettingsWidget dialog;
	dialog.exec();
}
