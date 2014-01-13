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

#ifndef BROWSERCONTROLLER_H
#define BROWSERCONTROLLER_H

#include "browserwidget.h"
#include "iurldataloader.h"
#include <QObject>
#include <QMap>

class QNetworkAccessManager;

class BrowserController
	:public QObject
{
	Q_OBJECT
public:
	explicit BrowserController(QObject *parent=0);
	~BrowserController();

public:
	QWidget* widget();

private slots:
	void goBtnClicked();
	void backBtnClicked();
	void forwardBtnClicked();
	void webUrlChanged(const QUrl &);
	void fsPathChanged(const QString &);
	void webDataRequested(QUrl &url,QByteArray *data);
	void tabChanged();
	void bookmarksClicked();
	void settingsClicked();

public slots:
	void setAddress(const QString &address);

private:
	BrowserWidget *browserWidget;
	MultiTabBrowser *browser;
	QNetworkAccessManager *bookmarksAccessManager;

private:
	QString lastAddress;
	QMap<QString,IUrlDataLoader *> schemeDataLoaders;
	static int instanceCount;

private:
	void checkWidget();
	void checkBackAndForward();
};

#endif // BROWSERCONTROLLER_H
