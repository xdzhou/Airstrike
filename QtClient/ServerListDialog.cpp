#include "ServerListDialog.h" 
#include <QtGui/QRadioButton>
#include <QThread>
#include <stdio.h>

ServerListDialog::ServerListDialog(QWidget *parent) : QDialog(parent)
{  
    label_photon = new QLabel("Photon IP");   
    line_photon = new QLineEdit(); 
    line_photon->setFixedWidth(140); 
    btn_ok = new QPushButton("OK"); 
    QHBoxLayout *h1 = new QHBoxLayout();
    h1->addWidget(label_photon);
    h1->addWidget(line_photon);
    h1->addWidget(btn_ok); 
 
    h2 = new QVBoxLayout(); 
    grp = new QButtonGroup(this);
    grp->setExclusive(true);

    v = new QVBoxLayout();  
    v->addLayout(h1);  
    //server list
    serverList = (Server_info_t *)malloc(sizeof(Server_info_t)*5);

    this->setLayout(v);  
    connect(btn_ok, SIGNAL(clicked()), this, SLOT(ok_clicked())); 
    //zeromq implementation
    QThread * networkThread = new QThread();
    networkManager = NetworkManager::getInstance();
    networkManager->moveToThread(networkThread);
    connect(networkManager, SIGNAL(addNewRadioBtn(char*, char* ,int)), this, SLOT(addNewRadioBtn(char*, char*, int)));
}

void ServerListDialog::ok_clicked()  
{ 
    btn_ok->setEnabled(false); 
    string ipPhoton = line_photon->text().toStdString();
    networkManager->getServerList(ipPhoton.c_str());
    updateUI();    
}

void ServerListDialog::go_clicked()  
{ 
    printf("selected id = %d\n",grp->checkedId()); 
    if(grp->checkedId()!=-1) {
        gameServerID = grp->checkedId();
        printf("go_clicked - %s\n",grp->checkedButton()->text().toStdString().c_str());
        accept();
    }
}
void ServerListDialog::addNewRadioBtn(char *name, char *ip, int id)  
{ 
    printf("Dialog - name:%s ip:%s\n",name, ip );
    char nameIP[100];
    strcpy(nameIP, name);
    strcat(nameIP, " - ");
    strcat(nameIP, ip);
    QRadioButton *rb = new QRadioButton(nameIP);
    grp->addButton(rb, id);
    h2->addWidget(rb);
    strcpy(serverList[id].name, name);
    strcpy(serverList[id].ipadress, ip);
}
void ServerListDialog::updateUI()  
{     
    v->addLayout(h2);

    btn_go = new QPushButton("GO");
    QHBoxLayout *h3 = new QHBoxLayout();
    h3->addWidget(btn_go);
    v->addLayout(h3);

    connect(btn_go, SIGNAL(clicked()), this, SLOT(go_clicked()));  
}

QString ServerListDialog::GetAirstrikeIP()  
{  
    QString gameIP(serverList[gameServerID].ipadress);
    free(serverList);
    delete networkManager;
    return gameIP;
}