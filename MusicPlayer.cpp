#include "MusicPlayer.h"
#include <QTime>
#include <QDebug>

MusicPlayer::MusicPlayer(QObject *parent) : QMediaPlayer(parent)
{
    setVolume(30);

    setNotifyInterval(250);

    connect(this,SIGNAL(positionChanged(qint64)),this,SLOT(slotPositionChanged(qint64)));
}

QString MusicPlayer::parsingLyrics(QString lyric)
{
    lyricTime.clear();
    QString newLyric;
    QStringList lyricList = lyric.split('\n');
    int line = 0;
    for(int i=0;i<lyricList.size();++i){
        QStringList lyricLineList = lyricList[i].split(']');
        if(lyricLineList.size()!=2){
            continue;
        }
        QString lyrinLine = lyricLineList[1];
        lyrinLine = lyrinLine.trimmed();  //删除首尾空格
        newLyric += lyrinLine + "\r\n";

        QString qstrTime = lyricLineList[0].remove('[');
        if(qstrTime.size() ==8){
            qstrTime += '0';
        }

        QTime time = QTime::fromString(qstrTime,"mm:ss.zzz");

        lyricTime[time.msecsSinceStartOfDay()] = line++;
    }

    return newLyric;
}

void MusicPlayer::slotPositionChanged(qint64 position)
{
    if(lyricTime.isEmpty()) {
    return ;
    }
       QMap<qint64, int>::iterator it = lyricTime.upperBound(position);
       if(it!=lyricTime.begin()){
           --it;
       }
       if(it.value() + 5 <=(lyricTime.end() - 1).value()){
       emit signalShowLyricLine(it.value() + 5);
       }
       emit signalShowLyricLine(it.value());

}
