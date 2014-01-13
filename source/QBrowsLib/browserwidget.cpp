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

#include "fsbrowsercontroller.h"
#include "browserwidget.h"
#include "multitabbrowser.h"
#include <QPushButton>
#include <QWebView>
#include <QDialog>
#include <QLineEdit>
#include <QLayout>
#include <QStackedLayout>
#include <QAction>
#include <QWebHistory>
#include <QLabel>
#include <QMovie>
#include <QDebug>

QSet<BrowserWidget*> BrowserWidget::thisObjects=QSet<BrowserWidget*>();

BrowserWidget::BrowserWidget(MultiTabBrowser *browser,QWidget *parent)
	:QWidget(parent)
{
	QSize margins(4,4);
	QPixmap icon;
	thisObjects.insert(this);

	QWidget *toolBarWidget=new QWidget(this);

	icon=QPixmap(":/qbrows/go.png");
	QPushButton *goBtn=new QPushButton(toolBarWidget);
	goBtn->setFlat(true);
	goBtn->setToolTip(tr("Go to"));
	goBtn->setIcon(icon);
	goBtn->setIconSize(icon.size());
	goBtn->setFixedSize(icon.size()+margins);

	icon=QPixmap(":/qbrows/back.png");
	backBtn=new QPushButton(toolBarWidget);
	backBtn->setFlat(true);
	backBtn->setToolTip(tr("Back"));
	backBtn->setIcon(icon);
	backBtn->setIconSize(icon.size());
	backBtn->setFixedSize(icon.size()+margins);
	backBtn->setDisabled(true);

	icon=QPixmap(":/qbrows/forward.png");
	forwardBtn=new QPushButton(toolBarWidget);
	forwardBtn->setFlat(true);
	forwardBtn->setToolTip(tr("Back"));
	forwardBtn->setIcon(icon);
	forwardBtn->setIconSize(icon.size());
	forwardBtn->setFixedSize(icon.size()+margins);
	forwardBtn->setDisabled(true);

	address=new QLineEdit(toolBarWidget);
	browsersWidget=browser;
	browser->setParent(this);

	icon=QPixmap(":/qbrows/close.png");
	QPushButton *closeTabBtn=new QPushButton(toolBarWidget);
	closeTabBtn->setFlat(true);
	closeTabBtn->setToolTip(tr("Close opened tab"));
	closeTabBtn->setIcon(icon);
	closeTabBtn->setIconSize(icon.size());
	closeTabBtn->setFixedSize(icon.size()+margins);

	icon=QPixmap(":/qbrows/add.png");
	QPushButton *addTabBtn=new QPushButton(toolBarWidget);
	addTabBtn->setFlat(true);
	addTabBtn->setToolTip(tr("Add new tab"));
	addTabBtn->setIcon(icon);
	addTabBtn->setIconSize(icon.size());
	addTabBtn->setFixedSize(icon.size()+margins);

	icon=QPixmap(":/qbrows/bookmarks.png");
	QPushButton *bookmarksBtn=new QPushButton(toolBarWidget);
	bookmarksBtn->setFlat(true);
	bookmarksBtn->setToolTip(tr("Bookmarks"));
	bookmarksBtn->setIcon(icon);
	bookmarksBtn->setIconSize(icon.size());
	bookmarksBtn->setFixedSize(icon.size()+margins);

	icon=QPixmap(":/qbrows/settings.png");
	QPushButton *settingsBtn=new QPushButton(toolBarWidget);
	settingsBtn->setFlat(true);
	settingsBtn->setToolTip(tr("Settings"));
	settingsBtn->setIcon(icon);
	settingsBtn->setIconSize(icon.size());
	settingsBtn->setFixedSize(icon.size()+margins);

	QLabel *loadingLabel=new QLabel(toolBarWidget);
	loadingMovie=new QMovie(":/qbrows/loading.gif");
	loadingLabel->setMovie(loadingMovie);
	loadingLabel->setFixedSize(QPixmap(":/qbrows/loading.gif").size());

	QVBoxLayout *mainLayout=new QVBoxLayout(this);
	mainLayout->addWidget(toolBarWidget);
	mainLayout->addWidget(browsersWidget);

	QHBoxLayout *toolBarLayout=new QHBoxLayout(toolBarWidget);
	toolBarLayout->addWidget(backBtn);
	toolBarLayout->addWidget(forwardBtn);
	toolBarLayout->addWidget(goBtn);
	toolBarLayout->addWidget(address,1);
	toolBarLayout->addWidget(addTabBtn);
	toolBarLayout->addWidget(closeTabBtn);
	toolBarLayout->addWidget(bookmarksBtn);
	toolBarLayout->addWidget(settingsBtn);
	toolBarLayout->addWidget(loadingLabel);

	connect(address,SIGNAL(returnPressed()),this,SIGNAL(goBtnClicked()));
	connect(goBtn,SIGNAL(clicked()),this,SIGNAL(goBtnClicked()));
	connect(backBtn,SIGNAL(clicked()),this,SIGNAL(backBtnClicked()));
	connect(forwardBtn,SIGNAL(clicked()),this,SIGNAL(forwardBtnClicked()));
	connect(address,SIGNAL(textEdited(QString)),
			this,SIGNAL(addressEdited(QString)));
	connect(bookmarksBtn,SIGNAL(clicked()),this,SIGNAL(addTabBtnClicked()));
	connect(closeTabBtn,SIGNAL(clicked()),this,SIGNAL(closeTabBtnClicked()));
	connect(bookmarksBtn,SIGNAL(clicked()),this,SIGNAL(bookmarksClicked()));
	connect(settingsBtn,SIGNAL(clicked()),this,SIGNAL(settingsClicked()));

	loadingMovie->start();
	loadingMovie->stop();
}

BrowserWidget::~BrowserWidget()
{
	thisObjects.remove(this);
}

bool BrowserWidget::isAlive()
{
	return thisObjects.contains(this);
}

void BrowserWidget::showEvent(QShowEvent *ev)
{
	QWidget::showEvent(ev);
}

void BrowserWidget::enableBackBtn(bool flag)
{
	backBtn->setEnabled(flag);
}

void BrowserWidget::enableForwardBtn(bool flag)
{
	forwardBtn->setEnabled(flag);
}

void BrowserWidget::getAddress(QString &addr)
{
	addr=address->text();
}

void BrowserWidget::setAddress(const QString &addr)
{
	address->setText(addr);
}

void BrowserWidget::beginLoading()
{
	loadingMovie->start();
}

void BrowserWidget::endLoading()
{
	loadingMovie->stop();
}
