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

#ifndef BROWSER_H
#define BROWSER_H

#include <QWidget>
#include <QUrl>
#include <QSet>

class QDialog;
class MultiTabBrowser;
class QPushButton;
class QLineEdit;
class QUrl;
class QStackedLayout;

class BrowserWidget
	:public QWidget
{
	Q_OBJECT
public:
	BrowserWidget(MultiTabBrowser *browser,QWidget *parent=0);
	~BrowserWidget();
	bool isAlive();

public:
	void getAddress(QString &addr);
	void setAddress(const QString &addr);
	void enableBackBtn(bool);
	void enableForwardBtn(bool);

public slots:
	void beginLoading();
	void endLoading();

protected:
	virtual void showEvent(QShowEvent *ev);

signals:
	void goBtnClicked();
	void backBtnClicked();
	void forwardBtnClicked();
	void addressEdited(const QString &);
	void addTabBtnClicked();
	void closeTabBtnClicked();
	void bookmarksClicked();
	void settingsClicked();

private:
	MultiTabBrowser *browsersWidget;
	QPushButton *backBtn,*forwardBtn;
	QLineEdit *address;
	QMovie *loadingMovie;

private:
	static QSet<BrowserWidget*> thisObjects;
};

#endif // BROWSER_H
