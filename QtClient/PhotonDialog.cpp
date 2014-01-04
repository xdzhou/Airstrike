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
 
    h2 = new QVBoxLayout(); 
    grp = new QButtonGroup(this);
    grp->setExclusive(true);

    v = new QVBoxLayout();  
    v->addLayout(h1);  
  
    this->setLayout(v);  
    connect(btn_ok, SIGNAL(clicked()), this, SLOT(ok_clicked())); 
//photon sdk
    QThread * photonThread = new QThread();
    photonlib = new PhotonLib();
    photonlib->moveToThread(photonThread);
    connect(photonlib, SIGNAL(addNewRadioBtn(const char *,int)), this, SLOT(addNewRadioBtn(const char *,int)));
}

void PhotonDialog::ok_clicked()  
{ 
    btn_ok->setEnabled(false); 
    string ipPhoton = line_photon->text().toStdString();
    photonlib->startwork(ipPhoton.c_str());
    updateUI();    
}

void PhotonDialog::go_clicked()  
{ 
    printf("selected id = %d\n",grp->checkedId()); 
    if(grp->checkedId()!=-1) {
	ip = grp->checkedButton()->text();
 	printf("go_clicked - %s\n",ip.toStdString().c_str());
	accept();
    }
}
void PhotonDialog::addNewRadioBtn(const char * name, int id)  
{ 
    QRadioButton *rb = new QRadioButton(name);
    grp->addButton(rb, id);
    h2->addWidget(rb);
}
void PhotonDialog::updateUI()  
{     
    v->addLayout(h2);

    btn_go = new QPushButton("GO");
    QHBoxLayout *h3 = new QHBoxLayout();
    h3->addWidget(btn_go);
    v->addLayout(h3);

    connect(btn_go, SIGNAL(clicked()), this, SLOT(go_clicked()));  
}

QString PhotonDialog::GetAirstrikeIP()  
{  
    printf("PhotonDialog - %s\n",ip.toStdString().c_str());
    return ip;
}
