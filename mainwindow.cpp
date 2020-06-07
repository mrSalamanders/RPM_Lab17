#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QTime>
#include <QBoxLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    this->init();
}

MainWindow::~MainWindow()
{
    delete ui;
}

qint64 MainWindow::takeFromMap()
{
    QUrl location = mediaPlaylist->media(this->mediaPlaylist->currentIndex()).request().url();
    return restored.take(QFileInfo(location.path()).filePath());
}

void MainWindow::onActionRemoveTriggered()
{
    QUrl location = mediaPlaylist->media(this->mediaPlaylist->currentIndex()).request().url();
    qDebug() << QFileInfo(location.path()).filePath() << "removed";
    jo.remove(QFileInfo(location.path()).filePath());
    this->mediaPlaylist->removeMedia(this->mediaPlaylist->currentIndex());
}

void MainWindow::onActionNextTriggered()
{
    this->mediaPlaylist->next();
    mediaPlayer->setPosition(takeFromMap());
}
void MainWindow::onActionPrevTriggered()
{
    this->mediaPlaylist->previous();
    mediaPlayer->setPosition(takeFromMap());
}

void MainWindow::onActionPlayTriggered()
{
    if (QMediaPlayer::PausedState == this->mediaPlayer->state())
    {
        this->mediaPlayer->play();
        this->ui->statusBar->showMessage("Playing");
    }
}

void MainWindow::onActionPauseTriggered()
{
    if (QMediaPlayer::PlayingState == this->mediaPlayer->state())
    {
        this->mediaPlayer->pause();
        this->ui->statusBar->showMessage("Paused");
    }
}

void MainWindow::positionChanged(qint64 position)
{
    emit backup();
    this->currentContentSlider->setValue(position);
    this->updateDurationInfo();
}

void MainWindow::pausePlayFlipFlop()
{
    if (QMediaPlayer::PlayingState == this->mediaPlayer->state()) /// playing
    {
        ui->actionPlay->setVisible(false);
        ui->actionPause->setVisible(true);
    } else { /// stopped
        ui->actionPlay->setVisible(true);
        ui->actionPause->setVisible(false);
    }
}

void MainWindow::jump(const QModelIndex &index)
{
    if (index.isValid()) {
        this->mediaPlaylist->setCurrentIndex(index.row());

        mediaPlayer->setPosition(takeFromMap());

        mediaPlayer->play();
    }
}

void MainWindow::checkBackup()
{
    QString fileName = QCoreApplication::applicationDirPath() + "/backup.json";
    QFile file(fileName);
    if(QFileInfo::exists(fileName))
    {
        qDebug () << "file exists";
        file.open(QIODevice::ReadWrite | QIODevice::Text);
        QString val;
              val = file.readAll();
              file.close();
//              qWarning() << val;
              QJsonDocument doc = QJsonDocument::fromJson(val.toLocal8Bit());
              QJsonObject obj = doc.object();

              QStringList l = obj.keys();
              qDebug() << l;
              for ( const auto& i : l  )
              {
                  qDebug() << i;
                          QFile f(i);
                          qDebug() << f;
                          QString p = QDir(i).path();
                          qDebug() << p;
                          if(QFileInfo::exists(p)) {
                              qDebug() << "Exist" << i;
                          } else {
                              qDebug() << "does not exist" << i;
                              obj.remove(i);
                          }
              }
//              jo.remove("");
        QJsonDocument jd(obj);
        this->playlistBackup = jd;
        this->jo = playlistBackup.object();
        qDebug() << this->playlistBackup;
        qDebug() << this->jo;
        emit restore();
    }
    else
    {
        qDebug () << "file does not exists";
        file.open(QIODevice::ReadWrite | QIODevice::Text);
        file.write("");
        qDebug()<<"file created";
        file.close();
    }
}

void MainWindow::restore()
{
    QStringList filenames = this->jo.keys();
//    for (int i = 0; i < filenames.size(); i++) {
//        qDebug() << filenames[i];
//        QFile f(filenames[i]);
//        f.open(QIODevice::ReadOnly);
//        if(f.isReadable()) {
//            qDebug() << "Exist" << filenames[i];
//        } else {
//            qDebug() << "does not exist" << filenames[i];
//            filenames.removeAt(i);
//        }

//    }
    qint64 pos = 0;
    if (!filenames.isEmpty()) {
        pos = jo.value(filenames.first()).toInt();

            foreach(const QString& key, jo.keys()) {
                    QJsonValue value = jo.value(key);
                    qDebug() << "Key = " << key << ", Value = " << value.toInt();

                        this->restored.insert(key, value.toInt());
                        mediaPlaylist->addMedia(QMediaContent(QUrl::fromLocalFile(key)));
            }
    }
    mediaPlayer->setPlaylist(mediaPlaylist);
    qDebug() << restored;
    this->mediaPlayer->setPosition(pos);
    this->mediaPlayer->pause();
}

void MainWindow::backup()
{
    QUrl location = mediaPlaylist->media(this->mediaPlaylist->currentIndex()).request().url();
    jo.insert(QFileInfo(location.path()).filePath(), this->mediaPlayer->position());
    jo.remove("");
    QJsonDocument jd(jo);
    this->playlistBackup = jd;
}

void MainWindow::backupToFile()
{

    QString file_path = QCoreApplication::applicationDirPath() + "/backup.json";
    qDebug() << file_path;
    QString json_string = this->playlistBackup.toJson();
    qDebug() << jo << "to file";
    QFile save_file(file_path);
    if(!save_file.open(QIODevice::WriteOnly)){
        qDebug() << "failed to open save file";
    }
    save_file.write(json_string.toLocal8Bit());
    save_file.close();
}

void MainWindow::closeEvent (QCloseEvent *event)
{
    this->mediaPlayer->pause();
    QMessageBox::StandardButton resBtn = QMessageBox::question( this, "A?",
                                                                tr("Want quit?\nPlaylist timings will be saved\n"),
                                                                QMessageBox::No | QMessageBox::Yes,
                                                                QMessageBox::Yes);
    if (resBtn != QMessageBox::Yes) {
        event->ignore();
    } else {
        emit backupToFile();
        event->accept();
    }
}

void MainWindow::init()
{
    this->ui->setupUi(this);
    this->videoWidget = new QVideoWidget();
    this->mediaPlayer = new QMediaPlayer();
    this->playListView = new QListView();
    this->mediaPlaylist = new QMediaPlaylist();
    this->mediaPlaylistModel = new PlaylistModel();

    mediaPlayer->setPlaylist(mediaPlaylist);
    mediaPlaylistModel->setPlaylist(mediaPlaylist);
    playListView->setModel(mediaPlaylistModel);
    playListView->setCurrentIndex(mediaPlaylistModel->index(mediaPlaylist->currentIndex(),0));

    this->currentContentSlider = new QSlider();
    this->currentContentDuration = new QLabel("00:00/00:00");

    this->currentVolumeSlider = new QSlider(Qt::Horizontal, this);
    this->currentVolumeSlider->setRange(0, 100);

    this->mediaPlayer->setVideoOutput(this->videoWidget);
    this->currentContentSlider->setOrientation(Qt::Horizontal);

    this->initLayout();
    this->initSignalsAndSlots();
    this->checkBackup();
}

void MainWindow::initLayout()
{

    QGridLayout* displayLayout = new QGridLayout();
    displayLayout->addWidget(videoWidget, 2, 1);

    QGridLayout* hLayout = new QGridLayout();
    hLayout->addWidget(this->currentContentSlider, 2, 2);
    hLayout->addWidget(this->currentContentDuration, 2, 1);
//    displayLayout->addWidget(playListView, 2, 3);

    QBoxLayout* boxLayout = new QVBoxLayout();
    boxLayout->addLayout(displayLayout);
    boxLayout->addLayout(hLayout);

    QBoxLayout* boxLayoutList = new QVBoxLayout();
    boxLayoutList->addWidget(playListView);
//    boxLayoutList->maximumSize();

    ui->actionPause->setVisible(false);

    this->ui->widgetVideos->setLayout(boxLayout);
    this->ui->widgetList->setLayout(boxLayoutList);
    this->ui->mainToolBar->addSeparator();
    currentVolumeSlider->setMaximumWidth(100);
    currentVolumeSlider->setValue(50);
    this->ui->mainToolBar->addWidget(currentVolumeSlider);
}

void MainWindow::initSignalsAndSlots()
{
    connect(this->mediaPlayer, &QMediaPlayer::durationChanged, this->currentContentSlider, &QSlider::setMaximum);
    connect(this->mediaPlayer, &QMediaPlayer::positionChanged, this, &MainWindow::positionChanged);
    connect(this->currentContentSlider, &QSlider::sliderMoved, this->mediaPlayer, &QMediaPlayer::setPosition);
    connect(this->currentVolumeSlider, &QSlider::sliderMoved, this->mediaPlayer, &QMediaPlayer::setVolume);
    connect(this->mediaPlayer, &QMediaPlayer::stateChanged, this, &MainWindow::pausePlayFlipFlop);
    connect(this->ui->actionRemove, &QAction::triggered, this, &MainWindow::onActionRemoveTriggered);
    connect(this->ui->actionOpen, &QAction::triggered, this, &MainWindow::onActionOpenTriggered);
    connect(this->ui->actionPlay, &QAction::triggered, this, &MainWindow::onActionPlayTriggered);
    connect(this->ui->actionPause, &QAction::triggered, this, &MainWindow::onActionPauseTriggered);
    connect(this->ui->actionNext, &QAction::triggered, this, &MainWindow::onActionNextTriggered);
    connect(this->playListView, &QAbstractItemView::activated, this, &MainWindow::jump);
}

void MainWindow::updateDurationInfo()
{
    const qint64 currentInfo = this->mediaPlayer->position() / 1000;
    const qint64 duration = this->mediaPlayer->duration() / 1000;
    QString tStr;

//    if (currentInfo)
//    {
        QTime currentTime((currentInfo / 3600) % 60,
                          (currentInfo / 60) % 60,
                          (currentInfo % 60),
                          (currentInfo * 1000) % 1000);
        QTime totalTime((duration / 3600) % 60,
                        (duration / 60) % 60,
                        (duration % 60),
                        (duration * 1000) % 1000);
        QString format = "mm:ss";

        if (duration > 3600)
            format = "hh:mm:ss";

        tStr = currentTime.toString(format) + " / " + totalTime.toString(format);
//    }

    this->currentContentDuration->setText(tStr);
}

void MainWindow::onActionOpenTriggered()
{
    QStringList filenames = QFileDialog::getOpenFileNames(this, "Open a File","","Video File(*.*)");
//    this->mediaPlayer->stop();
    for(const QString & filename: filenames){
        mediaPlaylist->addMedia(QMediaContent(QUrl::fromLocalFile(filename)));
    }
    mediaPlayer->setPlaylist(mediaPlaylist);

    this->mediaPlayer->pause();
}
