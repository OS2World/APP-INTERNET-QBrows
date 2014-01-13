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

#ifndef FSBROWSERCONTROLLER_H
#define FSBROWSERCONTROLLER_H

#include <QObject>
#include <QListWidget>
#include <QDir>
#include <QFileIconProvider>
#include <QLinkedList>

class QFileSystemWatcher;
class QRadioButton;
class QComboBox;
class QCheckBox;

class FSBrowserController
	:public QObject
{
	Q_OBJECT
public:
	explicit FSBrowserController(QObject *parent=0);

public:
	QWidget* widget();

public:
	bool setCurrentDir(const QDir &lookupDir);
	bool setCurrentDir(const QString &path);
	QString getCurrentDir();
	bool canGoBack();
	bool canGoForward();
	void goBack();
	void goForward();

signals:
	void pathChanged(const QString &);
	void newFSBrowserInstance(FSBrowserController *);
	void closeRequest();
	void titleChanged(const QString &);
	void beginLoading();
	void endLoading();

private slots:
	void onDirectoryChanged();
	void onItemRenamed(QListWidgetItem *);
	void onItemDClicked(QListWidgetItem *);
	void onSetViewChecked();
	void onDriveSelected(const QString &);
	void onShowHiddenChecked();

private:
	QListWidget *filesList;
	QWidget *mainWidget;
	QComboBox *drivesBox;
	QCheckBox *showHiddenChk;
	QRadioButton *iconsBtn;
	QRadioButton *listBtn;

private:
	QFileSystemWatcher *fsWatcher;
	QFileIconProvider iconProvider;
	QFileInfoList entries;
	QDir currentDir;
	QLinkedList<QString> historyBack;
	QLinkedList<QString> historyForward;

private:
	void fillFilesList();
	void fillDrivesBox();
};

#endif // FSBROWSERCONTROLLER_H
