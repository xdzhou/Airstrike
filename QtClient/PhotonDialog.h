#include <QDialog>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QButtonGroup>
#include <QtGui/QVBoxLayout>
#include <string>
#include "PhotonLib.h"

using namespace std;  

class PhotonDialog : public QDialog  
{  
    Q_OBJECT  
public:  
    explicit PhotonDialog(QWidget *parent = 0);
    string GetAirstrikeIP();  

private:  
    QLabel *label_photon;
    QLineEdit *line_photon; 
    QPushButton *btn_ok; 
    QButtonGroup *grp;       
    QPushButton *btn_go;
    QVBoxLayout *v;   
    string serverList[5];
    int numServer;
    PhotonLib *photonlib;
    void updateUI();

public slots:  
    void ok_clicked();
    void go_clicked();

signals: 
 
};  
