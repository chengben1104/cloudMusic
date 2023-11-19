#include "HttHandle.h"
#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QEventLoop>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonArray>
#include <QJsonObject>

HttHandle::HttHandle(QObject *parent) : QObject(parent),
    manager(new QNetworkAccessManager(this))
{

}

QByteArray HttHandle::search(const QString &body)
{
    QNetworkRequest request(QUrl("https://l-by.cn/yinyue/api.php"));
    request.setRawHeader(QByteArray("Content-Type"),QByteArray("application/x-www-form-urlencoded; charset=UTF-8"));


    QEventLoop eventloop;
    QNetworkReply *reply = manager->post(request,body.toUtf8());
    connect(reply,SIGNAL(finished()),&eventloop,SLOT(quit()));
    eventloop.exec(QEventLoop::ExcludeUserInputEvents);
    return reply->readAll();
}

QByteArray HttHandle::getSearch(const QString &url)
{
    QNetworkRequest request(url);

    QEventLoop eventloop;
    QNetworkReply *reply = manager->get(request);
    connect(reply,SIGNAL(finished()),&eventloop,SLOT(quit()));
    eventloop.exec(QEventLoop::ExcludeUserInputEvents);
    return reply->readAll();
}

void HttHandle::searchMusicList(const QString searchData)
{

    QString body = QString("types=search&count=30&source=netease&pages=1&name=%1").arg(searchData);

    QByteArray musicInfoData = search(body);

    QJsonParseError error;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(musicInfoData,&error);
    if(error.error != QJsonParseError::NoError)
    {
        qDebug()<<"解析音乐列表JSON数据失败！错误码："<<error.errorString();
        return ;

    }
    QJsonArray musicArray = jsonDocument.array();
    for(int i=0;i<musicArray.size();i++)
    {
        QStringList musicInfoList;   //QStringList -> QList<Qstring>
        QJsonObject musicInfo = musicArray[i].toObject();
        QString id = QString::number(musicInfo["id"].toInt());
        QString picId=musicInfo["pic_id"].toString();
        QString name = musicInfo["name"].toString();

        QJsonArray artist = musicInfo["artist"].toArray();
        QString qstrArt;
        for(int j=0;j<artist.size();++j)
        {
            QString art =artist[j].toString();
            qstrArt = qstrArt + art + "," ;


        }
        qstrArt.chop(1);//删除最后一个逗号
        QString album = musicInfo["album"].toString();
        musicInfoList<<id<<picId<<name<<qstrArt<<album;
        emit signalMusicInfoList(musicInfoList);

    }

}

QString HttHandle::searchMusicUrl(const QString &id)
{
    QString body = QString("types=url&id=%1&source=netease").arg(id);

    QByteArray musicUrlData = search(body);

    QJsonParseError error;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(musicUrlData,&error);
    if(error.error != QJsonParseError::NoError)
    {
        qDebug()<<"解析音乐网址JSON数据失败！错误码："<<error.errorString();
        return QString();

    }
    QJsonObject musicUrlObject = jsonDocument.object();
    return musicUrlObject["url"].toString();

}

QString HttHandle::searchMusicLyric(const QString &id)
{
    QString body = QString("types=lyric&id=%1&source=netease").arg(id);

    QByteArray lyricData = search(body);

    QJsonParseError error;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(lyricData,&error);
    if(error.error != QJsonParseError::NoError)
    {
        qDebug()<<"解析音乐歌词JSON数据失败！错误码："<<error.errorString();
        return QString();

    }
    QJsonObject lyricObject = jsonDocument.object();
    return lyricObject["lyric"].toString();
}

QString HttHandle::searchMusicPicUrl(const QString &picId)
{
    QString body = QString("types=pic&id=%1&source=netease").arg(picId);

    QByteArray picUrlData = search(body);

    QJsonParseError error;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(picUrlData,&error);
    if(error.error != QJsonParseError::NoError)
    {
        qDebug()<<"解析音乐歌词JSON数据失败！错误码："<<error.errorString();
        return QString();

    }
    QJsonObject picUrlObject = jsonDocument.object();
    return picUrlObject["url"].toString();
}
