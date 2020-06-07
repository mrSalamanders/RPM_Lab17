#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QSlider>
#include <QLabel>
#include <QListView>
#include <QMediaPlaylist>
#include <QJsonDocument>
#include <QJsonObject>
#include <QCloseEvent>
#include <QMessageBox>
#include "playlistmodel.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QJsonDocument playlistBackup;
    QJsonObject jo;
    QMap<QString, qint64> restored;

    qint64 takeFromMap();
private slots:
    void onActionRemoveTriggered();
    /**
     * @brief onActionOpenTriggered слот для кнопки "открыть"
     */
    void onActionOpenTriggered();
    /**
     * @brief onActionPlayTriggered слот для кнопки "Плей"
     */
    void onActionPlayTriggered();
    /**
     * @brief onActionPauseTriggered слот для кнопки "Пауза"
     */
    void onActionPauseTriggered();
    /**
     * @brief positionChanged слот для ползунка времени
     * @param position
     */
    void onActionNextTriggered();
    void onActionPrevTriggered();
    void positionChanged(qint64 position);
    void pausePlayFlipFlop();
    void jump(const QModelIndex &index);
    void checkBackup();
    void restore();
    void backup();
    void backupToFile();
    void closeEvent (QCloseEvent *event);

private:
    Ui::MainWindow *ui;
    /**
     * @brief mediaPlayer Сам плеер
     */
    QMediaPlayer* mediaPlayer;
    /**
     * @brief mediaPlaylist плейлист к плееру
     */
    QMediaPlaylist* mediaPlaylist;
    /**
     * @brief mediaPlaylistModel модель, которая беспечивает функционал лист вью
     */
    PlaylistModel* mediaPlaylistModel;
    /**
     * @brief videoWidget видеовджет, в который будет показываться видео
     */
    QVideoWidget* videoWidget;
    /**
     * @brief currentContentSlider слайдер времени
     */
    QSlider* currentContentSlider;
    /**
     * @brief currentVolumeSlider ползунок громкости
     */
    QSlider* currentVolumeSlider;
    /**
     * @brief currentContentDuration место для текста с длительностью
     */
    QLabel* currentContentDuration;
    /**
     * @brief playListView лист вью, в котором лежит плейлист
     */
    QListView* playListView;
    /**
     * @brief init сетап всех объектов
     */
    void init();
    /**
     * @brief initLayout сетап юай
     */
    void initLayout();
    /**
     * @brief initSignalsAndSlots сетап всех коннектов и слотов
     */
    void initSignalsAndSlots();
    /**
     * @brief updateDurationInfo функция обновления длительности видео
     */
    void updateDurationInfo();

};

#endif // MAINWINDOW_H
