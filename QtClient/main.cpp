#include <QtGui/QApplication>
#include "mainwindow.h"
#include "ServerListDialog.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    ServerListDialog d;
    if(d.exec() == QDialog::Accepted){	
    	MainWindow w(d.GetAirstrikeIP());
    	w.show();
    	return a.exec();
    }
    else return 0;

}
