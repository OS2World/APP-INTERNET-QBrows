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

#include "settingswidget.h"
#include <QSettings>
#include <QDir>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QLayout>

SettingsWidget::SettingsWidget(QWidget *parent)
	:QDialog(parent)
{
	homeEdit=new QLineEdit(this);
	QLabel *homeLabel=new QLabel(tr("home page"),this);

	QWidget *buttonsWidget=new QWidget(this);
	QPushButton *okBtn=new QPushButton(tr("Ok"),buttonsWidget);
	QPushButton *cancelBtn=new QPushButton(tr("Cancel"),buttonsWidget);

	QVBoxLayout *mainLayout=new QVBoxLayout(this);
	mainLayout->addWidget(homeLabel);
	mainLayout->addWidget(homeEdit);
	mainLayout->addWidget(buttonsWidget);

	QHBoxLayout *buttonsLayout=new QHBoxLayout(buttonsWidget);
	buttonsLayout->addWidget(okBtn);
	buttonsLayout->addWidget(cancelBtn);

	connect(cancelBtn,SIGNAL(clicked()),this,SLOT(reject()));
	connect(okBtn,SIGNAL(clicked()),this,SLOT(okClicked()));

	fillDialogFromSettings();
}

void SettingsWidget::okClicked()
{
	if (homeEdit->text().isEmpty())
		homeEdit->setText("about:blank");
	fillSettingsFromDialog();
	accept();
}

void SettingsWidget::fillDialogFromSettings()
{
	QSettings settings(QDir::homePath()+"/.QBrows/settings.ini",
					   QSettings::IniFormat);
	homeEdit->setText(settings.value("home","about:blank").toString());
}

void SettingsWidget::fillSettingsFromDialog()
{
	QSettings settings(QDir::homePath()+"/.QBrows/settings.ini",
					   QSettings::IniFormat);
	settings.setValue("home",homeEdit->text());
}
