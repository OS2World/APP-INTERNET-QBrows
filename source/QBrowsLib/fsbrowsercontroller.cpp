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
#include <QFileSystemWatcher>
#include <QFileInfo>
#include <QFileInfoList>
#include <QLayout>
#include <QComboBox>
#include <QCheckBox>
#include <QRadioButton>
#include <QDesktopServices>
#include <QUrl>
#include <QDebug>

FSBrowserController::FSBrowserController(QObject *parent)
	:QObject(parent)
{
	fsWatcher=new QFileSystemWatcher(this);

	mainWidget=new QWidget();

	filesList=new QListWidget(mainWidget);
	filesList->setViewMode(QListWidget::IconMode);
	filesList->setGridSize(QSize(64,64));
	filesList->setResizeMode(QListWidget::Adjust);

	QWidget *toolBar=new QWidget(mainWidget);

	drivesBox=new QComboBox(toolBar);
	drivesBox->setEditable(false);
	fillDrivesBox();

	QWidget *radioBtns=new QWidget(toolBar);
	iconsBtn=new QRadioButton(tr("Icons view"),radioBtns);
	listBtn=new QRadioButton(tr("List view"),radioBtns);

	showHiddenChk=new QCheckBox(tr("Show hidden"),toolBar);

	QVBoxLayout *mainLayout=new QVBoxLayout(mainWidget);
	mainLayout->addWidget(toolBar);
	mainLayout->addWidget(filesList);

	QHBoxLayout *toolBarLayout=new QHBoxLayout(toolBar);
	toolBarLayout->addWidget(drivesBox);
	toolBarLayout->addWidget(radioBtns);
	toolBarLayout->addWidget(showHiddenChk);
	toolBarLayout->addStretch(1);

	QHBoxLayout *radioBtnsLayout=new QHBoxLayout(radioBtns);
	radioBtnsLayout->addWidget(iconsBtn);
	radioBtnsLayout->addWidget(listBtn);

	connect(fsWatcher,SIGNAL(directoryChanged(QString)),
			this,SLOT(onDirectoryChanged()));
	connect(filesList,SIGNAL(itemChanged(QListWidgetItem*)),
			this,SLOT(onItemRenamed(QListWidgetItem*)));
	connect(filesList,SIGNAL(itemDoubleClicked(QListWidgetItem*)),
			this,SLOT(onItemDClicked(QListWidgetItem*)));
	connect(iconsBtn,SIGNAL(toggled(bool)),
			this,SLOT(onSetViewChecked()));
	connect(drivesBox,SIGNAL(activated(QString)),
			this,SLOT(onDriveSelected(QString)));
	connect(showHiddenChk,SIGNAL(clicked()),
			this,SLOT(onShowHiddenChecked()));
	connect(this,SIGNAL(pathChanged(QString)),
			this,SIGNAL(titleChanged(QString)));

	iconsBtn->setChecked(true);
	setCurrentDir(QDir::home());
}

QWidget* FSBrowserController::widget()
{
	return mainWidget;
}

void FSBrowserController::onDirectoryChanged()
{
	fillFilesList();
}

QString FSBrowserController::getCurrentDir()
{
	return currentDir.absolutePath();
}

bool FSBrowserController::setCurrentDir(const QString &path)
{
	QDir lookupDir=QDir(path);
	return setCurrentDir(lookupDir);
}

bool FSBrowserController::setCurrentDir(const QDir &lookupDir)
{
	if (!lookupDir.exists()) return false;
	fsWatcher->removePath(currentDir.path());
	fsWatcher->addPath(lookupDir.path());
	historyForward.clear();
	historyBack.push_front(currentDir.absolutePath());
	if (historyBack.count()>256) historyBack.removeLast();
	currentDir=lookupDir;
	emit pathChanged(currentDir.absolutePath());
	fillFilesList();
	return true;
}

void FSBrowserController::fillFilesList()
{
	emit beginLoading();
	filesList->clear();
	filesList->blockSignals(true);

	QFlags<QDir::Filter> filters=QDir::Dirs;
	if (currentDir.isRoot()) filters|=QDir::Dirs;
	if (showHiddenChk->isChecked())
	{
		filters|=QDir::Hidden;
		filters|=QDir::System;
	}
	entries=currentDir.entryInfoList(filters);

	filters=QDir::Files;
	if (showHiddenChk->isChecked())
	{
		filters|=QDir::Hidden;
		filters|=QDir::System;
	}
	entries.append(currentDir.entryInfoList(filters));

	for (int i=0;i<entries.count();i++)
	{
		QListWidgetItem *item=
				new QListWidgetItem(iconProvider.icon(entries[i]),
									entries[i].fileName(),
									filesList);
		if (entries[i].fileName()!="..")
			item->setFlags(item->flags()|Qt::ItemIsEditable);
		QVariant var;
		var.setValue<void*>(&entries[i]);
		item->setData(Qt::UserRole,var);
		item->setToolTip(entries[i].fileName());
	}
	filesList->blockSignals(false);
	emit endLoading();
}

void FSBrowserController::onItemDClicked(QListWidgetItem *item)
{
	QFileInfo *fileInfo=(QFileInfo*)item->data(Qt::UserRole).value<void*>();
	if (fileInfo->isDir())
	{
		QDir lookupDir=currentDir;
		if (lookupDir.cd(fileInfo->fileName()))
		{
			if (currentDir.path()!=lookupDir.path())
			{
				setCurrentDir(lookupDir);
			}
		}
	}
	else
	{
		QDesktopServices::openUrl(QUrl(QString("file://")+
									   fileInfo->absoluteFilePath(),
									   QUrl::TolerantMode));
	}
}

bool FSBrowserController::canGoBack()
{
	return historyBack.count()!=0;
}

bool FSBrowserController::canGoForward()
{
	return historyForward.count()!=0;
}

void FSBrowserController::onItemRenamed(QListWidgetItem *item)
{
	QFileInfo *fileInfo=(QFileInfo*)item->data(Qt::UserRole).value<void*>();
	QString newName=item->data(Qt::DisplayRole).toString();
	if (newName==fileInfo->path()) return;
	if (newName.length()>0)
		fileInfo->dir().rename(fileInfo->fileName(),newName);
}

void FSBrowserController::fillDrivesBox()
{
	drivesBox->clear();
	QFileInfoList drives=QDir::drives();
	for (int i=0;i<drives.count();i++)
		drivesBox->addItem(drives[i].absoluteFilePath());
}

void FSBrowserController::onSetViewChecked()
{
	if (iconsBtn->isChecked())
	{
		filesList->setViewMode(QListWidget::IconMode);
		filesList->setGridSize(QSize(64,64));
	}
	else
	{
		filesList->setViewMode(QListWidget::ListMode);
		filesList->setGridSize(QSize());
	}
}

void FSBrowserController::onDriveSelected(const QString &path)
{
	setCurrentDir(QDir(path));
}

void FSBrowserController::onShowHiddenChecked()
{
	fillFilesList();
}

void FSBrowserController::goBack()
{
	if (historyBack.count()==0) return;
	historyForward.push_front(currentDir.absolutePath());
	if (historyForward.count()>256) historyForward.removeLast();
	currentDir=QDir(historyBack.first());
	historyBack.removeFirst();
	fillFilesList();
	emit pathChanged(currentDir.path());
}

void FSBrowserController::goForward()
{
	if (historyForward.count()==0) return;
	historyBack.push_front(currentDir.absolutePath());
	if (historyBack.count()>256) historyBack.removeLast();
	currentDir=QDir(historyForward.first());
	historyForward.removeFirst();
	fillFilesList();
	emit pathChanged(currentDir.path());
}
