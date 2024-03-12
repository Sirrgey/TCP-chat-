#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <QtNetwork/QTcpServer> // Для сервера
#include <QtNetwork/QTcpSocket> // Для соединения
#include <QNetworkInterface>
#include <QRegExp>
#include <QValidator>
#include <QTcpServer>
#include <QAbstractSocket>
#include <QTimer>
#include <QTime>
#include <QVector>
#include <QShortcut>
#include <QValidator>
#include <QKeyEvent>
#include <QRegExp>
#include <QProcess>
#include <QHostInfo>
#include <QIODevice>
#include <QDir>
#include <QDebug>


namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void readData(); // Поступили данные

//    void closeEvent(QCloseEvent *event);

    void newConnection(); // Подключился клиент

    void on_radioButton_Client_clicked();

    void on_radioButton_Server_clicked();

    void on_pushButton_Create_clicked();

    void on_pushButton_Connect_clicked();

    void on_pushButton_SendMes_clicked();

    void on_pushButton_IP_clicked();

    void server_diskon();

    void connected();

    void disconnected();

    void on_pushButton_Spam_clicked();

    void on_lineEdit_SendMes_textChanged();

    void on_pushButton_Spam_stop_clicked();

    void on_pushButton_discon_clicked();

    void on_pushButton_Connect_2_clicked();


public slots:
    void on_TimeOut();




//    void Time_dis();




private:
    Ui::MainWindow *ui;

    bool conect = false;
    long long int size;//размер файла


    QTcpServer *tcpServer = nullptr;

    QTcpSocket *tcpSocket = nullptr ;
    QTcpSocket *tcpSock ;
    QString Baza;
    QVector <QTcpSocket*> sockets;

    QTimer * timer;
//    QTimer * timer_2;
    quint16 Last_cnk;
    int count = 0;
    bool flag = false;
    bool podkl = false;
    int count_podkl = 0;
    int count_sok = -1;
    int Podkl_k_s = 2;
    int n = 0;
    int name_count = 0;
    QString *Mas_matov;
    QString name;

};

#endif // MAINWINDOW_H
