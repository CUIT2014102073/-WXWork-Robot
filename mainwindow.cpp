#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tabWidget->setTabText(0, "纯文本类型_");
    ui->tabWidget->setTabText(1, "MarkDown类型_");
    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::httpsPost);
}

MainWindow::~MainWindow()
{
    delete ui;
}

QNetworkReply *MainWindow::httpsPost()
{
    m_http = new QNetworkAccessManager();

    //设置头信息
    QNetworkRequest m_url;
    m_url.setUrl(QUrl("https://qyapi.weixin.qq.com/cgi-bin/webhook/send?key=4ab1d28b-c6b8-4e6a-a569-a9ee65a68bbf"));
    m_url.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QSslConfiguration m_sslConfig = QSslConfiguration::defaultConfiguration();
    m_sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    m_sslConfig.setProtocol(QSsl::TlsV1_2);
    m_url.setSslConfiguration(m_sslConfig);

    QByteArray bate;
    switch(ui->tabWidget->currentIndex()) {
    case 0:
        bate = getTextTabInfos().toUtf8();
        break;
    case 1:
        bate = getMarkDownTabInfos().toUtf8();
        break;
    default:
        break;
    }


    //发送数据
    m_res = m_http->post(m_url, bate);
    connect(m_res, &QNetworkReply::finished, this, [this](){
                qDebug()<< m_res->error();
            });
    return m_res;
}

QString MainWindow::getTextTabInfos()
{
    QString content, mentioned_list, mentioned_mobile_list;
    content = ui->contentTextEdit->toPlainText().toUtf8();
    mentioned_list = ui->mentionedListTextEdit->toPlainText().toUtf8();
    mentioned_mobile_list = ui->mentionedMobileListTextEdit->toPlainText().toUtf8();

    QString jsonData("{\"msgtype\": \"text\",\"text\": {\"content\": \"");
    jsonData.append(content);
    jsonData.append("\"");
    if(!mentioned_list.isEmpty()) {
        jsonData.append(", \"mentioned_list\":[");
        jsonData.append(mentioned_list);
        jsonData.append("], ");
    }
    if(!mentioned_mobile_list.isEmpty()) {
        jsonData.append(", \"mentioned_mobile_list\":[");
        jsonData.append(mentioned_list);
        jsonData.append("]");
    }
    jsonData.append("} }");
    qDebug()<< jsonData;
    return jsonData;
}

QString MainWindow::getMarkDownTabInfos()
{
    QString content = ui->markdownTextEdit->toPlainText();
    QString jsonData("{\"msgtype\": \"markdown\",\"markdown\": {\"content\": \"");
    jsonData.append(content);
    jsonData.append("\" } }");
    qDebug()<< jsonData;
    return jsonData;
}

