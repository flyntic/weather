#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->lineEdit->setText("Voronezh");
    ui->pushButton->click();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::get_weather_from_api(QString city)
{
    QUrl url(_url.arg(city, api_key));
    QNetworkRequest request(url);
    request.setRawHeader("Content-Type", "application/json");
    manager = new QNetworkAccessManager();
    connect(manager, &QNetworkAccessManager::finished, this, &MainWindow::requestReceived);
    manager->get(request);

}

void MainWindow::requestReceived(QNetworkReply *reply)
{
    reply->deleteLater();
    if(reply->error() == QNetworkReply::NoError) {
        int v = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        if (v >= 200 && v < 300)
        {
          //  QString replyText = reply->readAll();
            set_data_to_form(reply->readAll());
         //   ui->textEdit->setText(replyText);
        }
        else if (v >= 300 && v < 400)
        {
            QUrl newUrl = reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();
            newUrl = reply->url().resolved(newUrl);
            QNetworkAccessManager *manager = reply->manager();
            QNetworkRequest redirection(newUrl);
            QNetworkReply *newReply = manager->get(redirection);
            return;
        }
    }
    else
    {
        ui->textEdit->setText(reply->errorString());
    }

    reply->manager()->deleteLater();
}


void MainWindow::on_pushButton_clicked()
{
    QString city = ui->lineEdit->text();

    get_weather_from_api(city);
}

void MainWindow::set_data_to_form(QByteArray str)
{  // QByteArray strr=str.toLocal8Bit();
    qDebug()<<str;
    QJsonDocument doc = QJsonDocument::fromJson(str);//.toLocal8Bit());
  //  QJsonDocument::JsonFormat()
    QJsonObject rootObj = doc.object();
    double t_feels_like, t_temp, t_temp_max, t_temp_min;
    double humidity,pressure;
    t_feels_like = rootObj.value("main").toObject().value("feels_like").toDouble();
    t_temp = rootObj.value("main").toObject().value("temp").toDouble();
    t_temp_max = rootObj.value("main").toObject().value("temp_max").toDouble();
    t_temp_min = rootObj.value("main").toObject().value("temp_min").toDouble();
    humidity = rootObj.value("main").toObject().value("humidity").toDouble();
    pressure = rootObj.value("main").toObject().value("pressure").toDouble();
    QString city = rootObj.value("name").toString();
 //   QString descr = rootObj.value("description").toString();
    QDateTime sunrise = QDateTime::fromMSecsSinceEpoch(rootObj.value("sys").toObject().value("sunrise").toInteger() * 1000, Qt::LocalTime);
    QDateTime sunset = QDateTime::fromMSecsSinceEpoch(rootObj.value("sys").toObject().value("sunset").toInteger() * 1000, Qt::LocalTime);
    QJsonArray array_weather= rootObj.value("weather").toArray();
    QString path = ":/img/images/" + array_weather.at(0).toObject().value("icon").toString() + "@4x.png";
    QPixmap  mypix = *new QPixmap(path);
    QString description =array_weather.at(0).toObject().value("description").toString();
    ui->l_icon->setPixmap(mypix);


    ui->l_city->setText(city);
    ui->l_feels_like->setText(QString::number(t_feels_like,'f',0));
    ui->l_temp->setText(QString::number(t_temp,'f',0)+"  ");
    ui->l_temp_max->setText(QString::number(t_temp_max,'f',0)+"  ");
    ui->l_temp_min->setText(QString::number(t_temp_min,'f',0)+"  ");
    ui->l_sunrise->setText(sunrise.toString("hh:mm:ss"));
    ui->l_sunset->setText(sunset.toString("hh:mm:ss"));
    ui->l_hum->setText(QString::number( humidity)+" % ");
    ui->l_ph->setText(QString::number(pressure*0.75006375 ,'f',0) + " мм рт ст");
    ui->l_weather->setText(description);

}

