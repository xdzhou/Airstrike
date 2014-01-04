#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QThread>
#include <QDebug>
#include <QSettings>
#include <QKeyEvent>
#include <QLabel>
#include <stdio.h>

MainWindow::MainWindow(QString ip, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    isBot=false;
    started = false;

    ui->setupUi(this);
    displayText("Start");
    printf("MainWindow - %s\n",ip.toStdString().c_str());
    // Load sounds
    bullet = new QSound("/sounds/bullet.wav");

    networkThread = new QThread();
    networkManager = new NetworkManager();

    networkManager->moveToThread(networkThread);
    networkThread->start();
    //connect(networkThread, SIGNAL(started()), networkManager, SLOT(network_init()));

    connect(ui->connectButton,SIGNAL(clicked()),this,SLOT(connect_clicked()));
    connect(networkManager, SIGNAL(writeText(QString)), this, SLOT(displayText(QString)));
    connect(ui->disconnectButton, SIGNAL(released()), this, SLOT(stopPlay()));
    connect(this, SIGNAL(sendKeyEvent(QKeyEvent*,int)), networkManager, SLOT(process_key(QKeyEvent*,int)));
    connect(networkManager, SIGNAL(newPlayerScore(int)), ui->playerScore, SLOT(setNum(int)));
    connect(networkManager, SIGNAL(newHealthPoints(int)), ui->healthPoints, SLOT(setValue(int)));
    connect(networkManager, SIGNAL(newPlayerId(int)), ui->playerIdLabel, SLOT(setNum(int)));
    connect(networkManager, SIGNAL(newTeamId(int)),ui->equipe, SLOT(setNum(int)));
    connect(networkManager, SIGNAL(newIdInTeam(int)), this, SLOT(setSprite(int)));
    connect(ui->checkBoxBot, SIGNAL(stateChanged(int)), this, SLOT(setBot(int)));
    connect(ui->nameEdit, SIGNAL(textChanged(QString)), networkManager, SLOT(setLogin(QString)));
    connect(ui->disconnectButton, SIGNAL(released()), networkManager, SLOT(disconnectClient()));
    connect(this, SIGNAL(setIP(QString,int)), networkManager, SLOT(setIP(QString,int)));
    connect(this, SIGNAL(setRequestedTeam(int)), networkManager, SLOT(setRequestedTeam(int)));
    connect(networkManager, SIGNAL(disconnected()), this, SLOT(stopPlay()));
    connect(this, SIGNAL(startNetworkManager()), networkManager, SLOT(network_init()));

    readSettings();
    ui->ipEdit->setText(ip.toStdString().c_str());
    ui->portEdit->setText("1234");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::displayText(QString string){
    ui->debugTextEdit->append(string);
    qDebug() << string;
}

void MainWindow::connect_clicked(){
    emit setRequestedTeam(ui->comboBoxTeam->currentIndex());
    emit setIP(ui->ipEdit->text(),ui->portEdit->text().toInt());
    emit startNetworkManager();
    startPlay();
}

void MainWindow::keyPressEvent(QKeyEvent * event){
    if(!started){
        //event->ignore();
        return;
    }
    if(!event->isAutoRepeat()){
        emit networkManager->process_key(event, 1);
    } else {
        QWidget::keyPressEvent(event);
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent * event){
    if(!started)    {
        event->ignore();
        return;
    }
    if(event->key() == Qt::Key_Escape)
    {
        stopPlay();
        //emit ui->disconnectButton->released();
        return;
    }
    if(!event->isAutoRepeat()){
        emit sendKeyEvent(event, -1);
    } else {
        QWidget::keyPressEvent(event);
    }
}

void MainWindow::startPlay()
{
    started = true;
    ui->groupBox->setEnabled(false);
    ui->connectButton->setEnabled(false);
    if(!isBot)
    {
        this->grabKeyboard();
    }
    else
    {
        botTimer = new QTimer();
        botTimer->setInterval(100);
        connect(botTimer, SIGNAL(timeout()), networkManager, SLOT(set_rand_key()));
        botTimer->start();
    }
}

void MainWindow::stopPlay()
{
    started = false;
    ui->groupBox->setEnabled(true);
    ui->connectButton->setEnabled(true);
    if(isBot)
    {
        botTimer->stop();
    }
    else
    {
        this->releaseKeyboard();
    }
}

void MainWindow::setBot(int state)
{
    if(state == 0)
        isBot = false;
    if(state == 2)
        isBot = true;
}

void MainWindow::setSprite(int idInTeam)
{
    QString fileName = "./Solo/plane-" + ui->equipe->text() + "-" +  QString::number(idInTeam) + ".png";
    displayText(fileName);
    QPixmap image(fileName);
    ui->sprite->setPixmap(image);
}

void MainWindow::writeSettings()
{
    QSettings settings("airstrike_client.conf",QSettings::IniFormat);

    settings.beginGroup("MainWindow");
    settings.setValue("size", size());
    settings.setValue("pos", pos());
    settings.endGroup();

    settings.beginGroup("Values");
    settings.setValue("ip", ui->ipEdit->text());
    settings.setValue("port", ui->portEdit->text());
    settings.setValue("login", ui->nameEdit->text());
    settings.setValue("bot", ui->checkBoxBot->isChecked());
    settings.setValue("team", ui->comboBoxTeam->currentIndex());
    settings.endGroup();
}

void MainWindow::readSettings()
{
    QSettings settings("airstrike_client.conf",QSettings::IniFormat);

    settings.beginGroup("MainWindow");
    resize(settings.value("size", QSize(400, 400)).toSize());
    move(settings.value("pos", QPoint(200, 200)).toPoint());
    settings.endGroup();

    settings.beginGroup("Values");
    ui->ipEdit->setText(settings.value("ip").toString());
    ui->portEdit->setText(settings.value("port").toString());
    ui->nameEdit->setText(settings.value("login").toString());
    ui->checkBoxBot->setChecked(settings.value("bot").toBool());
    ui->comboBoxTeam->setCurrentIndex(settings.value("team").toInt());

    settings.endGroup();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    writeSettings();
    event->accept();
}
