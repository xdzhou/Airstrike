#include <QtGui/QApplication>
#include "mainwindow.h"
#include "PhotonDialog.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //MainWindow w;
    //w.show();
    PhotonDialog d;
    if(d.exec()==QDialog::Accepted){	
	MainWindow w(d.GetAirstrikeIP());
	w.show();
	return a.exec();
    }else
	return 0;

}
