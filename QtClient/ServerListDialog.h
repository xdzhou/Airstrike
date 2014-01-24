#ifndef SERVERLISTDIALOG_H
#define SERVERLISTDIALOG_H

#include <QDialog>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QButtonGroup>
#include <QtGui/QVBoxLayout>
#include <string>
#include <string.h>
#include "zmqhelper.h"

using namespace std;  

class ServerListDialog : public QDialog  
{  
    Q_OBJECT  
public:  
    explicit ServerListDialog(QWidget *parent = 0);
    QString GetAirstrikeIP();  

private:  
    QLabel *label_photon;
    QLineEdit *line_photon; 
    QPushButton *btn_ok; 
    QButtonGroup *grp;
    QVBoxLayout *h2;       
    QPushButton *btn_go;
    QVBoxLayout *v;   
    NetworkManager * networkManager;
    void updateUI();
    int gameServerID;
    Server_info_t * serverList;

public slots:  
    void ok_clicked();
    void go_clicked();
    void addNewRadioBtn(char name[], char ip[], int id);

signals:
 
};  

#endif // SERVERLISTDIALOG_H