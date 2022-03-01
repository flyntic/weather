#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QNetworkReply>
#include <QDateTime>
#include <QLabel>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void get_weather_from_api(QString city);
    void requestReceived(QNetworkReply *reply);

private slots:
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    QString api_key = "beb35c5d579ec3c6867e5d4a9d1610c9";
    QString _url = "http://api.openweathermap.org/data/2.5/weather?q=%1&appid=%2&units=metric&lang=ru";
    QNetworkAccessManager *manager;
    QJsonDocument *response;

    void set_data_to_form(QByteArray str);

};
#endif // MAINWINDOW_H
