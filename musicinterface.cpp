#include "musicinterface.h"
#include "ui_musicinterface.h"
#include <QDebug>
#include <QTime>
#include <QTextBlock>
#include <QImage>

MusicInterface::MusicInterface(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MusicInterface),

  httpHandle(new HttHandle(this)),
    musicPlayer(new MusicPlayer(this)),
    playOrder(true)
{
    ui->setupUi(this);
    QTime time;
    time=QTime::currentTime();
    qsrand(time.msecsSinceStartOfDay());

    setWindowIcon(QIcon(":/Image/icon.ico"));
    connect(httpHandle,SIGNAL(signalMusicInfoList(QStringList)),this,SLOT(slotShowMusicInfoList(QStringList)));
    connect(ui->dial_volume,SIGNAL(sliderMoved(int)),musicPlayer,SLOT(setVolume(int)));
    connect(musicPlayer,SIGNAL(stateChanged(QMediaPlayer::State)),this,SLOT(slotReflushPlayButtonIcon(QMediaPlayer::State)));
     connect(musicPlayer,SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)),this,SLOT(slotMediaStatusChanged(QMediaPlayer::MediaStatus)));

     connect(musicPlayer,SIGNAL(durationChanged(qint64)),this,SLOT(slotReflushEndTime(qint64)));
     connect(musicPlayer,SIGNAL(positionChanged(qint64)),this,SLOT(slotReflushStartTime(qint64)));
    connect(musicPlayer,SIGNAL(signalShowLyricLine(int)),this,SLOT(slotShowLyricLine(int)));

    initTableWiget();

    initTextEdit();

}

MusicInterface::~MusicInterface()
{
    delete ui;
}

void MusicInterface::initTableWiget()
{
       ui->tableWidget_musicList->setStyleSheet("selection-background-color:red; selection-color: white;"); //设置tableWidget QSS样式表，背景为红色，字体为白色
       ui->tableWidget_musicList->hideColumn(0);	//隐藏第一列
       ui->tableWidget_musicList->hideColumn(1);    //隐藏第二列
       ui->tableWidget_musicList->setEditTriggers(QAbstractItemView::NoEditTriggers); //设置不可编辑
       ui->tableWidget_musicList->setSelectionBehavior(QAbstractItemView::SelectRows);//一次选一行
       ui->tableWidget_musicList->setFocusPolicy(Qt::NoFocus);	//去掉虚线框
       ui->tableWidget_musicList->setSortingEnabled(true);	//增加表头排序功能

       ui->tableWidget_musicList->horizontalHeader()->setFixedHeight(30);    //固定列头高度为30像素
       ui->tableWidget_musicList->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}");   //设置tableWidget列头QSS样式表，背景为天蓝
       ui->tableWidget_musicList->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);//表头拉伸
       ui->tableWidget_musicList->horizontalHeader()->setHighlightSections(false);       //选中列头不在高亮

       ui->tableWidget_musicList->verticalHeader()->hide();//隐藏行头
       //ui->tableWidget_musicList->verticalHeader()->setHighlightSections(false);         //选中行头不在高亮
}

void MusicInterface::initTextEdit()
{
    QPalette palette = ui->textEdit_lyric->palette();  //获取textEdit的调色板
    palette.setColor(QPalette::Highlight,QColor(Qt::transparent));    //将选中区域背景改为透明
    palette.setColor(QPalette::HighlightedText,QColor(Qt::red));       //将选中区域的字体改为红色
    ui->textEdit_lyric->setPalette(palette);    //将调色板设置到textEdit
    ui->textEdit_lyric->setReadOnly(true);       //设置为只读
    ui->textEdit_lyric->document()->setDefaultTextOption(QTextOption(Qt::AlignHCenter));   //文字居中显示
}

void MusicInterface::slotShowLyricLine(int line)
{
        QTextCursor tc2 = ui->textEdit_lyric->textCursor();				   //获取当前光标
        int pos2 = ui->textEdit_lyric->document()->findBlockByLineNumber(line).position(); //在textEdit中找到指定行位置
        tc2.setPosition(pos2, QTextCursor::MoveAnchor);			  //将光标设置到指定位置
        tc2.movePosition(QTextCursor::EndOfLine, QTextCursor::KeepAnchor);	  //光标设置成选中一整行，并且保持此状态
        ui->textEdit_lyric->setTextCursor(tc2);					  //将设置好的光标属性放到textEdit中
}

void MusicInterface::slotReflushStartTime(qint64 ms)
{
    QTime time = QTime::fromMSecsSinceStartOfDay(ms);
    ui->label_startTime->setText(time.toString("mm:ss"));
    ui->horizontalSlider_progressBar->setValue(ms);

}

void MusicInterface::slotReflushEndTime(qint64 ms)
{
     QTime time = QTime::fromMSecsSinceStartOfDay(ms);
      ui->label_endTime->setText(time.toString("mm:ss"));
    ui->horizontalSlider_progressBar->setMaximum(ms);
}

void MusicInterface::slotMediaStatusChanged(QMediaPlayer::MediaStatus status)
{
    if(status ==QMediaPlayer::EndOfMedia){
        if(playOrder){
            on_pushButton_nextSong_clicked();
        }
        else {
            int row = ui->tableWidget_musicList->currentRow();	//获取当前选中行
            int n=qrand() % row;
            on_tableWidget_musicList_cellDoubleClicked(n,0);
            ui->tableWidget_musicList->selectRow(row);
        }
    }
}

void MusicInterface::slotReflushPlayButtonIcon(QMediaPlayer::State state)
{
    if(state == QMediaPlayer::PlayingState){
        ui->graphicsView->setStartTime();
        ui->pushButton_play->setStyleSheet("#pushButton_play{"
                                           "border-image:url(:/Image/playButton.png);"
    "}");
    }
    else {
        ui->graphicsView->setEndTime();
        ui->pushButton_play->setStyleSheet("#pushButton_play{\
                                           border-image: url(:/Image/originalPlayButton.png);\
                                           }\
                                           #pushButton_play:pressed{\
                                           border-image: url(:/Image/pressPlayButton.png);\
                                           }");
    }
}

void MusicInterface::slotShowMusicInfoList(QStringList musicInfo)
{
     int row = ui->tableWidget_musicList->rowCount();	//获取当前共有多少行

    ui->tableWidget_musicList->insertRow(row);	//插入第row行，在该行数据插入前需要先插入该行
    for(int i=0;i<musicInfo.size();++i){
    ui->tableWidget_musicList->setItem(row, i, new QTableWidgetItem(musicInfo[i]));    //在第row行，第column列，插入text数据
    }
}

void MusicInterface::on_lineEdit_search_returnPressed()
{

    ui->tableWidget_musicList->clearContents();		//清除表内所有数据(不包含表头数据和设置的其他属性)
    ui->tableWidget_musicList->setRowCount(0);                   //设置当前行数，为0

    QString searchData = ui->lineEdit_search->text();

    httpHandle->searchMusicList(searchData);
   // httpHandle->searchMusicUrl("187390");
   // httpHandle->searchMusicLyric("187390");
   // httpHandle->searchMusicPicUrl("109951167750223899");
}

void MusicInterface::on_tableWidget_musicList_cellDoubleClicked(int row, int column)
{
    Q_UNUSED(column)
    QString id=ui->tableWidget_musicList->item(row, 0 )->text();
    QString picId=ui->tableWidget_musicList->item(row, 1 )->text();

    QString musicUrl = httpHandle->searchMusicUrl(id);
    QString lyric = httpHandle -> searchMusicLyric(id);
    QString picUrl = httpHandle->searchMusicPicUrl(picId);
    picUrl.replace("300y300","1200y1200");

    QImage image = QImage::fromData(httpHandle->getSearch(picUrl));

    ui->graphicsView->setImage(image);


    lyric = musicPlayer->parsingLyrics(lyric);

    ui->textEdit_lyric->setText(lyric);

    musicPlayer->setMedia(QUrl(musicUrl));
    musicPlayer->play();
}

void MusicInterface::on_pushButton_lastSong_clicked()
{
    int row = ui->tableWidget_musicList->currentRow();	//获取当前选中行
    if(row<0){
        return ;
    }
    else if(row==0){
        row=ui->tableWidget_musicList->rowCount();//获取当前共有多少行
    }
    --row;

    on_tableWidget_musicList_cellDoubleClicked(row,0);
     ui->tableWidget_musicList->selectRow(row);

}

void MusicInterface::on_pushButton_nextSong_clicked()
{
    int row = ui->tableWidget_musicList->currentRow();	//获取当前选中行
    if(row<0){
        return ;
    }
    else if(row == ui->tableWidget_musicList->rowCount()-1){
        row=-1;
    }
    ++row;

    on_tableWidget_musicList_cellDoubleClicked(row,0);
     ui->tableWidget_musicList->selectRow(row);
}

void MusicInterface::on_pushButton_play_clicked()
{
    if(musicPlayer->state()==QMediaPlayer::PlayingState){
        musicPlayer->pause();
    }
    else if(musicPlayer->state()==QMediaPlayer::PausedState){
        musicPlayer->play();
    }
    else if(musicPlayer->state()==QMediaPlayer::StoppedState){
        int row = ui->tableWidget_musicList->currentRow();	//获取当前选中行
        if(row<0){
            return ;
        }
         on_tableWidget_musicList_cellDoubleClicked(row,0);
    }

}

void MusicInterface::on_pushButton_playOrder_clicked()
{

    playOrder = !playOrder;
    if(playOrder){
        ui->pushButton_playOrder->setStyleSheet("#pushButton_playOrder{\
                                                border-image: url(:/Image/musicTable2.png);\
                                                }");
    }
else {
    ui->pushButton_playOrder->setStyleSheet("#pushButton_playOrder{\
                                            border-image: url(:/Image/musicTable.png);\
                                            }");
}
}

void MusicInterface::on_horizontalSlider_progressBar_sliderMoved(int position)
{
    musicPlayer->setPosition(position);
}
