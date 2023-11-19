#ifndef HTTHANDLE_H
#define HTTHANDLE_H

#include <QObject>
#include <QNetworkAccessManager>

class HttHandle : public QObject
{
    Q_OBJECT
public:
    explicit HttHandle(QObject *parent = nullptr);

    QByteArray search(const QString& body);

    QByteArray getSearch(const QString& url);

    void searchMusicList(const QString searchData);
    QString searchMusicUrl(const QString& id);
    QString searchMusicLyric(const QString& id);
    QString searchMusicPicUrl(const QString& picId);

signals:
    void signalMusicInfoList(QStringList musicInfo);

public slots:

private:
    QNetworkAccessManager *manager;
};

#endif // HTTHANDLE_H
