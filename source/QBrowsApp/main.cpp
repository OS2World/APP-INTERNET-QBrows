#include <QApplication>
#include "browsercontroller.h"
#include <QTreeView>
#include <QFileSystemModel>

int main(int argc,char **argv)
{
	QApplication app(argc,argv);
	BrowserController browser;
	browser.widget()->show();
	return app.exec();
}
