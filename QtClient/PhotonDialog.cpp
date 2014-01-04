#include "PhotonDialog.h" 
#include <QtGui/QRadioButton>
#include <QThread>
#include <stdio.h>

PhotonDialog::PhotonDialog(QWidget *parent) : QDialog(parent)
{  
    numServer = 0;
    label_photon = new QLabel("Photon IP");   
    line_photon = new QLineEdit(); 
    line_photon->setFixedWidth(140); 
    btn_ok = new QPushButton("OK"); 
    QHBoxLayout *h1 = new QHBoxLayout();
    h1->addWidget(label_photon);
    h1->addWidget(line_photon);
    h1->addWidget(btn_ok); 
 
    v = new QVBoxLayout();  
    v->addLayout(h1);  
  
    this->setLayout(v);  
    connect(btn_ok, SIGNAL(clicked()), this, SLOT(ok_clicked())); 
//photon sdk
    QThread * photonThread = new QThread();
    photonlib = new PhotonLib();
    photonlib->moveToThread(photonThread);
}

void PhotonDialog::ok_clicked()  
{  
    string ipPhoton = line_photon->text().toStdString();
    photonlib->startwork(ipPhoton);
    serverList[0] = string("192.168.2.101"); numServer++;
    serverList[1] = string("192.168.2.103"); numServer++;
    updateUI();
    btn_ok->setEnabled(false);
}
void PhotonDialog::go_clicked()  
{ 
    printf("selected id = %d\n",grp->checkedId()); 
    //accept();
}

void PhotonDialog::updateUI()  
{ 
    QVBoxLayout *h2 = new QVBoxLayout(); 
    grp = new QButtonGroup(this);
    grp->setExclusive(true);
    QRadioButton * radios[5];
    int i;
    for(i=0;i<numServer;i++) {
	radios[i] = new QRadioButton(serverList[i].c_str());
	grp->addButton(radios[i], i);
        h2->addWidget(radios[i]);
    }
    //grp->setButton(0);    
    v->addLayout(h2);

    btn_go = new QPushButton("GO");
    QHBoxLayout *h3 = new QHBoxLayout();
    h3->addWidget(btn_go);
    v->addLayout(h3);

    connect(btn_go, SIGNAL(clicked()), this, SLOT(go_clicked()));  
}

string PhotonDialog::GetAirstrikeIP()  
{  
    return serverList[grp->checkedId()];  
}  
 
 
