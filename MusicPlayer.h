#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

#include <QMediaPlayer>
#include <QMap>

class MusicPlayer : public QMediaPlayer
{
    Q_OBJECT
public:
    explicit MusicPlayer(QObject *parent = nullptr);

    QString parsingLyrics(QString lyric);



signals:
    void signalShowLyricLine(int line);

public slots:
    void slotPositionChanged(qint64 position);


private:
    QMap<qint64,int> lyricTime;

};

#endif // MUSICPLAYER_H
