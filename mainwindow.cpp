#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QNetworkAccessManager>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->lineEdit, &QLineEdit::returnPressed, ui->pushButton, &QPushButton::pressed);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::replyFinished(QNetworkReply* reply)
{
    if (reply->error() != QNetworkReply::NoError)
    {
        qDebug() << reply->error();
    }
    ui->textEdit->setText(reply->readAll());
}

void MainWindow::on_pushButton_pressed()
{
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));

    QSslConfiguration config = QSslConfiguration::defaultConfiguration();
    config.setProtocol(QSsl::TlsV1_2);

    QNetworkRequest request;
    request.setSslConfiguration(config);
    request.setUrl(QUrl("https://www.wikidata.org/w/api.php?action=wbsearchentities&search=" + ui->lineEdit->text() + "&language=en&format=json"));
    //request.setHeader(QNetworkRequest::ServerHeader, "application/json");

    manager->get(request);
}
