#ifndef MUSICINTERFACE_H
#define MUSICINTERFACE_H

#include <QMainWindow>
#include "HttHandle.h"
#include "MusicPlayer.h"

namespace Ui {
class MusicInterface;
}

class MusicInterface : public QMainWindow
{
    Q_OBJECT

public:
    explicit MusicInterface(QWidget *parent = 0);
    ~MusicInterface();

    void initTableWiget();

    void initTextEdit();

private slots:
    void slotShowLyricLine(int line);

    void slotReflushStartTime(qint64 ms);

    void slotReflushEndTime(qint64 ms);

    void slotMediaStatusChanged(QMediaPlayer::MediaStatus status);

    void slotReflushPlayButtonIcon(QMediaPlayer::State state);

    void slotShowMusicInfoList(QStringList musicInfo);

    void on_lineEdit_search_returnPressed();

    void on_tableWidget_musicList_cellDoubleClicked(int row, int column);

    void on_pushButton_lastSong_clicked();

    void on_pushButton_nextSong_clicked();

    void on_pushButton_play_clicked();

    void on_pushButton_playOrder_clicked();

    void on_horizontalSlider_progressBar_sliderMoved(int position);

private:
    Ui::MusicInterface *ui;
    HttHandle *httpHandle;
    MusicPlayer *musicPlayer;
    bool playOrder;

};

#endif // MUSICINTERFACE_H
