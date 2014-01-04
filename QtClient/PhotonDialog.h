#include <QDialog>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QButtonGroup>
#include <QtGui/QVBoxLayout>
#include <string>
#include "PhotonLib.h"
#include <string.h>

using namespace std;  

class PhotonDialog : public QDialog  
{  
    Q_OBJECT  
public:  
    explicit PhotonDialog(QWidget *parent = 0);
    QString GetAirstrikeIP();  

private:  
    QLabel *label_photon;
    QLineEdit *line_photon; 
    QPushButton *btn_ok; 
    QButtonGroup *grp;
    QVBoxLayout *h2;       
    QPushButton *btn_go;
    QVBoxLayout *v;   
    string serverList[5];
    int numServer;
    PhotonLib *photonlib;
    void updateUI();
    QString ip;

public slots:  
    void ok_clicked();
    void go_clicked();
    void addNewRadioBtn(const char * name, int id);

signals:
 
};  
