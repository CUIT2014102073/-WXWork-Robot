#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    QNetworkReply *httpsPost();
private:
    QString getTextTabInfos();
    QString getMarkDownTabInfos();

    Ui::MainWindow *ui;
    QNetworkAccessManager* m_http;
    QNetworkReply* m_res;
};
#endif // MAINWINDOW_H
