
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent) : // Конструктор
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    // Прячем почти все виджеты до того как мы выбрали тип
    ui->pushButton_Create->hide();
    ui->pushButton_Connect->hide();

    ui->pushButton_Connect_2->hide();

    ui->comboBox->hide();
    ui->label_Port->hide();
    ui->lineEdit_Port->hide();

    ui->label_Address->hide();
    ui->lineEdit_Address->hide();

    ui->textEdit_Input->hide();

    ui->lineEdit_Spam->hide();

    ui->pushButton_Spam_stop->setEnabled(false);

    ui->pushButton_Spam_stop->hide();

    ui->pushButton_discon->hide();

    ui->lineEdit_Spam->setMaxLength(20);

    ui->lineEdit_Nkn->setMaxLength(20);


    ui->textEdit->setReadOnly(true);





    ui->lineEdit_SendMes->hide();
    ui->pushButton_SendMes->hide();

    ui->pushButton_IP->hide();
    ui->textEdit->hide();

    ui->pushButton_Spam->hide();
    ui->label_clients->hide();


    ui->lineEdit_Nkn->hide();


    timer = new QTimer(this);

    //    timer_2 = new QTimer(this);
    tcpSocket = new QTcpSocket;
    tcpServer = new QTcpServer;
    tcpServer = nullptr;
    tcpSocket = nullptr;






    // Делаем красоту с расположением кнопок
    ui->pushButton_Connect->move(20, 220);
    ui->pushButton_Create->move(20, 170);
    ui->pushButton_Connect_2->move(20, 220);
    ui->pushButton_discon->move(20, 270);
    ui->lineEdit_Port->setValidator( new QIntValidator(0, 99999, this));

    QRegExp regExp("(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)");
    ui->lineEdit_Address->setValidator(new QRegExpValidator(regExp,this));

    ui->textEdit_Input->setReadOnly(true);

    ui->pushButton_SendMes->setEnabled(false);

    connect(timer,SIGNAL(timeout()),this,SLOT(on_TimeOut()));

    //    connect(timer_2,SIGNAL(timeout()),this,SLOT(Time_dis()));
    tcpSock = new QTcpSocket;
    tcpSock = nullptr;





}

MainWindow::~MainWindow() // Деструктор
{

    sockets.clear();
    sockets.squeeze();

    if (tcpSocket!=nullptr)
        delete tcpSocket;
    if(tcpServer!= nullptr)
        delete tcpServer;
    if(timer!=nullptr)
        delete  timer;
    delete ui;
    if (tcpSock != nullptr)
        delete  tcpSock;

}



void MainWindow::readData() // Поступили данные
{
    QString ttm;
    QTcpSocket *tmp = qobject_cast<QTcpSocket*>(sender());
    if (tmp!=nullptr)
    {
        QString time = QTime::currentTime().toString("HH:mm:ss");
        int index = tmp->property("ID").toInt();
        QByteArray data = tmp->readAll();
        QString str = QString::fromUtf8(data);

        if (str.at(0) == "*" and Podkl_k_s == 1 )
        {
            ttm = str;
            name = str.remove(0, 1);
            str = ttm;
            ui->textEdit_Input->append("            Клиент отключился ");

        }
        else if (str.at(0) == "$" and Podkl_k_s == 1 )
        {
            ttm = str;
            name = str.remove(0, 1);
            str = ttm;
            ui->textEdit_Input->append("            Подключился "+name);

        }
        else if(str.at(0) == "@" and Podkl_k_s == 0 )
        {
            ttm = str;
            name = str.remove(0, 1);
            str = ttm;
            ui->textEdit_Input->append("            Подключился "+name);
            ui->comboBox->addItem(name,count_podkl);
        }
        else
            ui->textEdit_Input->append("["+time+"] <font color = red> <front>"+str);


        if (tmp->property("ID").toInt()!=-1) //от пользователя
        {
            if (Podkl_k_s  == 0 and count_podkl>1 and str.at(0) != "@" ) //если нужна пересылка
            {

                QByteArray ba3(str.toUtf8());

                for (int i=0; i<count_podkl;i++)
                {
                    if (i == index)
                    {

                    }
                    else
                    {

                       sockets[i]->write(ba3);
                    }
                }
            }
            if (str.at(0) == "@" and count_podkl >1 and Podkl_k_s == 0  )
            {

                QString n = "$"+name;
                QByteArray ba4(n.toUtf8());
                for (int i=0; i<count_podkl;i++)
                {
                    if (i == index)
                    {

                    }
                    else
                    {

                        sockets[i]->write(ba4);

                    }
                }

            }
        }
    }





}

void MainWindow::newConnection() // Подключился клиент
{



    sockets.append(tcpServer->nextPendingConnection());
    sockets[count_podkl]->setProperty("ID",QString::number(count_podkl));
//    connect(sockets[count_podkl],SIGNAL(readyRead()),this,SLOT(readData()));
//    connect(sockets[count_podkl],SIGNAL(disconnected()),this,SLOT(disconnected()));
    connect(sockets[count_podkl], &QTcpSocket::readyRead, this, &MainWindow::readData);
    connect(sockets[count_podkl], &QTcpSocket::disconnected, this, &MainWindow::server_diskon);
    ui->lineEdit_SendMes->show();
    ui->pushButton_SendMes->show();
    ui->textEdit_Input->show();
//    ui->textEdit_Input->append("          Подключился новый клиент");

    podkl = true;
    ui->pushButton_SendMes->setEnabled(true);
    if (ui->pushButton_SendMes->text() == "")
        ui->pushButton_SendMes->setEnabled(false);
    ui->pushButton_Spam->setEnabled(true);

    ui->lineEdit_Spam->show();
    ui->pushButton_Spam->show();
    ui->pushButton_Spam_stop->show();
    count_podkl++;



    ui->label_clients->setText("Количество клиентов: " + QString::number(count_podkl));






}


void MainWindow::on_radioButton_Client_clicked() // Выбран режим клиента
{

    Podkl_k_s = 1;
    ui->pushButton_Connect->show();
    ui->label_clients->hide();
    ui->lineEdit_Nkn->show();
    ui->comboBox->hide();

    ui->pushButton_Create->hide();
    ui->pushButton_Connect_2->hide();
    ui->textEdit_Input->clear();

    ui->pushButton_SendMes->hide();

    ui->label_Port->show();
    ui->lineEdit_Port->show();

    ui->pushButton_IP->show();
    ui->textEdit->show();

    ui->label_Address->show();
    ui->lineEdit_Address->show();

    ui->textEdit_Input->hide();

    ui->lineEdit_Port->setText("");
    ui->lineEdit_Address->setText("");




}

void MainWindow::on_radioButton_Server_clicked()//выбор сервера
{

    Podkl_k_s = 0;

    ui->pushButton_Create->show();
    ui->pushButton_Connect_2->show();
    ui->pushButton_Connect_2->setEnabled(false);
    ui->lineEdit_Nkn->hide();
    ui->lineEdit_Nkn->hide();
    ui->label_clients->show();




    ui->pushButton_SendMes->hide();

    ui->pushButton_IP->show();
    ui->textEdit->show();



    ui->label_Port->show();
    ui->lineEdit_Port->show();
    ui->textEdit_Input->clear();

    ui->label_Address->hide();
    ui->lineEdit_Address->hide();

    ui->textEdit_Input->hide();

    ui->lineEdit_SendMes->hide();


    ui->lineEdit_Port->setText("");
    ui->lineEdit_Address->setText("");

}

void MainWindow::on_pushButton_Create_clicked() // Создание сервера
{

    ui->comboBox->show();
    bool ok;
    quint16 port = ui->lineEdit_Port->text().toUShort(&ok);
    if (!ok && !ui->lineEdit_Port->text().isEmpty())
    {
        QMessageBox::critical(this, "Ошибка", "Неправильный порт");
        return;
    }

    tcpServer = new QTcpServer;
    tcpSocket = nullptr;

    if (!tcpServer->listen(QHostAddress::Any, port))
    {
        QMessageBox::critical(this, "Ошибка", "Порт занят другим сервером");
        return;
    }
    if(ui->lineEdit_Port->text() == "")
    {
        QMessageBox::critical(this, "Ошибка", "Введите порт");
        return;
    }
    Last_cnk = port;

    //    ui->textEdit_Input->show();
    //    ui->textEdit_Output->show();

    //    ui->lineEdit_SendMes->show();
    //    ui->pushButton_SendMes->show();


    connect(tcpServer, &QTcpServer::newConnection, this, &MainWindow::newConnection);

    flag = true;
    ui->pushButton_Connect_2->setEnabled(true);

    ui->pushButton_Create->setEnabled(false);



    ui->lineEdit_SendMes->show();
    ui->textEdit_Input->show();
    ui->pushButton_SendMes->show();
    ui->groupBox->setEnabled(false);


}


void MainWindow::on_pushButton_Connect_clicked() // Подключаемся
{
    //    if (Last_cnk !=ui->lineEdit_Port->text().toUShort())
    //    {
    bool ok;
    quint16 port = ui->lineEdit_Port->text().toUShort(&ok);
    if (!ok && !ui->lineEdit_Port->text().isEmpty())
    {
        QMessageBox::critical(this, "Ошибка", "Неправильный порт");
        ui->lineEdit_Nkn->show();
        return;
    }

    QHostAddress address;
    if (!address.setAddress(ui->lineEdit_Address->text()))
    {
        QMessageBox::critical(this, "Ошибка", "Неправильный адрес");
        ui->lineEdit_Nkn->show();
        return;
    }
    if (ui->lineEdit_Nkn->text().isEmpty())
    {
        QMessageBox::critical(this, "Ошибка", "Введите никнейм");
        ui->lineEdit_Nkn->show();
        return;
    }
    if (tcpServer != nullptr)
    {
        delete tcpServer;
        delete tcpSocket;
    }

    tcpServer = nullptr;
    tcpSocket = new QTcpSocket;
    tcpSocket->setProperty("ID",-1);
    tcpSocket->connectToHost(address, port);
    //    connect(tcpSocket, &QTcpSocket::connected, this, &MainWindow::newConnection);
    connect(tcpSocket, &QTcpSocket::readyRead, this, &MainWindow::readData);

    ui->pushButton_SendMes->setEnabled(true);

    conect = true;
    connect(tcpSocket, &QTcpSocket::disconnected, this, &MainWindow::disconnected);
    connect(tcpSocket, SIGNAL(disconnected()),this, SLOT(disconnected()));

    connect(tcpSocket, &QTcpSocket::connected, this, &MainWindow::connected);
    connect(tcpSocket, SIGNAL(connected()),this, SLOT(connected()));

    tcpSocket->waitForConnected(4000);
    if(tcpSocket->state() == 0)
    {
        QMessageBox::critical(this, "Ошибка", "Сервера не существует");
        on_pushButton_discon_clicked();
    }
    else {
        if (name_count == 0)
        {
            QByteArray ba;
            QString cod = "@";
            ba = cod.toUtf8() + ui->lineEdit_Nkn->text().toUtf8();
            tcpSocket->write(ba);
            name_count ++;
        }
    }





}

void MainWindow::on_pushButton_SendMes_clicked()//отправка сообщений
{
    QByteArray data;
    if(Podkl_k_s == 1)
    {
        QString time = QTime::currentTime().toString("HH:mm:ss");
        QString Name = ui->lineEdit_Nkn->text() +": ";

        if (ui->lineEdit_Nkn->text() != "")
        {
            data = Name.toUtf8() + ui->lineEdit_SendMes->text().toUtf8();
        }
        else
        {
            QString anon = "Анонимус: ";
            data =anon.toUtf8() + ui->lineEdit_SendMes->text().toUtf8();
        }

        //    ui->textEdit_Input->append("Вы:"+ui->lineEdit_SendMes->text());
        ui->textEdit_Input->append("["+time+"] <font color = blue> Вы: <front>"+ui->lineEdit_SendMes->text());
        tcpSocket->write(data);
        ui->lineEdit_SendMes->setText("");
    }
    if (Podkl_k_s == 0)
    {
        if(ui->comboBox->currentIndex() == 0)
        {

            QString time = QTime::currentTime().toString("HH:mm:ss");
            QString anon = "Сервер: ";
            data =anon.toUtf8() + ui->lineEdit_SendMes->text().toUtf8();
            for (int i=0; i<count_podkl;i++)
            {
                sockets[i]->write(data);
                sockets[i]->flush();
            }
            ui->textEdit_Input->append("["+time+"] <font color = blue> Вы: <front>"+ui->lineEdit_SendMes->text());
            ui->lineEdit_SendMes->setText("");
        }
        if (ui->comboBox->currentIndex() > 0)
        {
            int index = ui->comboBox->currentIndex();
            QString time = QTime::currentTime().toString("HH:mm:ss");
            QString anon = "Сервер: ";
            data =anon.toUtf8() + ui->lineEdit_SendMes->text().toUtf8();

                sockets[index-1]->write(data);
                sockets[index-1]->flush();

            ui->textEdit_Input->append("["+time+"] <font color = blue> Вы: <front>"+ui->lineEdit_SendMes->text());
            ui->lineEdit_SendMes->setText("");
        }
    }

}

void MainWindow::on_pushButton_IP_clicked()//показать ip
{

    if(n==0)
    {
        QList<QHostAddress> list = QNetworkInterface::allAddresses();
        for(int nIter=0; nIter<list.count(); nIter++)
        {
            if(!list[nIter].isLoopback())
            {
                if (list[nIter].protocol() == QAbstractSocket::IPv4Protocol )
                    ui->textEdit->append(list[nIter].toString());
                if (list[nIter].protocol() == QAbstractSocket::IPv6Protocol )
                    ui->textEdit->append(list[nIter].toString());
            }

        }
        n++;
    }
}

void MainWindow::server_diskon()//обработка сигнала дисконект для сервера
{
    ui->textEdit_Input->append("              Клиент отключился");
    count_podkl -= 1;
    podkl = false;
    if (count_podkl == 0)
        timer->stop();
    if (count_podkl == 0)
        ui->pushButton_Spam->setEnabled(false);
    if (count_podkl == 0)
        ui->pushButton_SendMes->setEnabled(false);


    //    tcpServer->resumeAccepting();

    if (count_podkl == 0)
    {
        ui->lineEdit_Spam->hide();
        ui->pushButton_Spam->hide();
        ui->pushButton_Spam_stop->hide();
    }

    QTcpSocket *tmp = qobject_cast<QTcpSocket*>(sender());
    if (tmp!=nullptr)
    {
        int index;
        index = tmp->property("ID").toInt();
        ui->comboBox->removeItem(index+1);
        connect(sockets[index], &QTcpSocket::readyRead, this, &MainWindow::readData);
        connect(sockets[index], &QTcpSocket::disconnected, this, &MainWindow::server_diskon);
        sockets[index]->close();
        sockets.remove(index);
        sockets.shrink_to_fit();


        if (count_podkl != 0)
        {
            for (int i=0; i<count_podkl;i++)
            {
                sockets[i]->setProperty("ID",i);
            }
        }
        QString str = "*";
        QByteArray ba3(str.toUtf8());
        if (count_podkl > 0)
        {
            for (int i=0; i<count_podkl;i++)
            {


                sockets[i]->write(ba3);

            }
        }

    }

    ui->label_clients->setText("Количество клиентов: " + QString::number(count_podkl));


}


void MainWindow::connected()//обработка сигнала коннект для клиента
{
    ui->groupBox->setEnabled(false);

    ui->textEdit_Input->setText("            Вы подключились к серверу");
    ui->textEdit_Input->show();
    ui->pushButton_SendMes->setEnabled(false);

    ui->pushButton_Connect->setEnabled(false);
    ui->pushButton_discon->setEnabled(true);


    ui->lineEdit_SendMes->show();
    ui->pushButton_SendMes->show();

    ui->pushButton_Spam->show();
    ui->lineEdit_Spam->show();
    ui->pushButton_Spam_stop->show();

    ui->lineEdit_Nkn->show();

    ui->pushButton_discon->show();



}

void MainWindow::disconnected()//обработка сигнала дисконект для клиента
{
    ui->groupBox->setEnabled(true);
    //    tcpSocket->abort();

    name_count = 0;
    timer->stop();
    ui->lineEdit_Nkn->show();
    ui->textEdit_Input->setText("Сервер отключился");
    ui->lineEdit_SendMes->setText("");
    ui->textEdit_Input->hide();

    ui->pushButton_Connect->setEnabled(true);
    ui->pushButton_discon->setEnabled(false);

    ui->lineEdit_SendMes->clear();
    ui->lineEdit_Nkn->clear();
    ui->lineEdit_Spam->clear();


    ui->lineEdit_SendMes->hide();
    ui->pushButton_SendMes->hide();

    ui->pushButton_Spam->hide();
    ui->lineEdit_Spam->hide();


    ui->lineEdit_Spam->hide();

    ui->pushButton_Spam_stop->hide();


    count = 0;

    ui->pushButton_discon->hide();


    tcpSocket = nullptr;





}


void MainWindow::on_pushButton_Spam_clicked()//спам
{
    timer->start(300);
    ui->pushButton_Spam->setEnabled(false);
    ui->pushButton_Spam_stop->setEnabled(true);

}

void MainWindow::on_lineEdit_SendMes_textChanged()//проверка для
//блокировки/разблакировки оправки сообщения
{
    if (ui->lineEdit_SendMes->text() == "")
        ui->pushButton_SendMes->setEnabled(false);
    else
    {
        ui->pushButton_SendMes->setEnabled(true);
    }
    if (Podkl_k_s == 0 and count_podkl== 0)
    {
        ui->pushButton_SendMes->setEnabled(false);
    }
    if (Podkl_k_s == 0 and count_podkl> 0 and ui->lineEdit_SendMes->text() != "" )
    {
        ui->pushButton_SendMes->setEnabled(true);
    }

}

void MainWindow::on_TimeOut()//обработка сигналов таймера
{
    if (Podkl_k_s == 0)
    {
        QString time = QTime::currentTime().toString("HH:mm:ss");
        QByteArray data;
        if (ui->lineEdit_Spam->text() != "")
        {

//            QString anon = "Сервер: ";
//            data =anon.toUtf8() + ui->lineEdit_Spam->text().toUtf8();
//            data.insert(0,"<Сервер> ");
//            for (int i=0; i<count_podkl;i++)
//            {
//                sockets[i]->write(data);
//                sockets[i]->flush();
//            }

//            ui->textEdit_Input->append("["+time+"] <font color = blue> Вы: <front>"+ui->lineEdit_Spam->text());
            if(ui->comboBox->currentIndex() == 0)
            {

                QString time = QTime::currentTime().toString("HH:mm:ss");
                QString anon = "Сервер: ";
                data =anon.toUtf8() + ui->lineEdit_Spam->text().toUtf8();
                for (int i=0; i<count_podkl;i++)
                {
                    sockets[i]->write(data);
                    sockets[i]->flush();
                }
                ui->textEdit_Input->append("["+time+"] <font color = blue> Вы: <front>"+ui->lineEdit_Spam->text());

            }
            if (ui->comboBox->currentIndex() > 0)
            {
                int index = ui->comboBox->currentIndex();
                QString time = QTime::currentTime().toString("HH:mm:ss");
                QString anon = "Сервер: ";
                data =anon.toUtf8() + ui->lineEdit_Spam->text().toUtf8();

                    sockets[index-1]->write(data);
                    sockets[index-1]->flush();

                ui->textEdit_Input->append("["+time+"] <font color = blue> Вы: <front>"+ui->lineEdit_Spam->text());

            }
        }
        else
        {
            QMessageBox::critical(this, "Ошибка", "Введите сообщение для спама");
            ui->pushButton_Spam->setEnabled(true);
            timer->stop();
        }
    }
    if (Podkl_k_s == 1)
    {
        QString time = QTime::currentTime().toString("HH:mm:ss");
        QByteArray data;
        QString Name = ui->lineEdit_Nkn->text()+": ";
        if (ui->lineEdit_Spam->text() != "")
        {
            if (ui->lineEdit_Nkn->text() != "")
                data = Name.toUtf8() + ui->lineEdit_Spam->text().toUtf8();

            else {
                QString anon = "Анонимус: ";
                data =anon.toUtf8() + ui->lineEdit_Spam->text().toUtf8();
            }
            ui->textEdit_Input->append("["+time+"] <font color = blue> Вы: <front>"+ui->lineEdit_Spam->text());
            tcpSocket->write(data);
        }
        else
        {
            QMessageBox::critical(this, "Ошибка", "Введите сообщение для спама");
            ui->pushButton_Spam->setEnabled(true);
            timer->stop();
        }
    }

}





void MainWindow::on_pushButton_Spam_stop_clicked()//останавливаем таймер
{
    timer->stop();
    ui->pushButton_Spam->setEnabled(true);
    ui->pushButton_Spam_stop->setEnabled(false);

}





void MainWindow::on_pushButton_discon_clicked()//кнопка обрыва соединения(клиент)
{
    timer->stop();
    tcpSocket->disconnectFromHost();
    if (tcpSocket!= nullptr)
    {
        delete  tcpSocket;
        tcpSocket = nullptr;
    }
    name_count = 0;
    ui->lineEdit_Nkn->clear();
    ui->lineEdit_Nkn->show();
    ui->pushButton_Connect->setEnabled(true);
    ui->pushButton_discon->setEnabled(false);
    ui->lineEdit_Spam->clear();
    ui->lineEdit_Nkn->clear();
    ui->lineEdit_Spam->setText("");

}

//void MainWindow::read_file()//чтение файла
//{
//    QString line[4];
//    QFile file("C:\\Users\\serge\\OneDrive\\Рабочий стол\\Baza.txt");

//        if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
//            return;
//        QTextStream in(&file);
//        line[3] = in.readLine();
//        for(int i=0; i<3; i++)
//        {
//            ui->textEdit_Input->append(line[i]);
//        }
//}


void MainWindow::on_pushButton_Connect_2_clicked()//создание соединения
{
//   for (int i = 0;i<count_podkl+1;i++)
//   {
//         ui->comboBox->removeItem(i);
//   }
   ui->comboBox->clear();
   ui->comboBox->addItem("Всем",0);


    delete tcpServer;
    tcpServer = nullptr;

    for (int i=0; i<count_podkl; i++)
    {
        disconnect(sockets[i],SIGNAL(readyRead()),this,SLOT(read_data()));
        disconnect(sockets[i],SIGNAL(disconnected()),this,SLOT(client_disconnected()));
        sockets[i]->close();
        delete sockets[i];

    }
    sockets.clear();
    sockets.squeeze();
    count_podkl=0;
    ui->label_clients->setText("Подключено клиентов: "+QString::number(count_podkl));

    ui->comboBox->hide();
    ui->pushButton_Create->setEnabled(true);
    ui->pushButton_Connect_2->setEnabled(false);

    ui->textEdit_Input->clear();
    ui->textEdit_Input->hide();
    ui->lineEdit_SendMes->hide();
    ui->pushButton_SendMes->hide();
    ui->lineEdit_SendMes->setText("");
    ui->groupBox->setEnabled(true);
}




