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
    /**
     * @brief playlistBackup документ с таймингами видео
     */
    QJsonDocument playlistBackup;
    /**
     * @brief jo объект к документу
     */
    QJsonObject jo;
    /**
     * @brief restored восстановленные из файла тайминги видео
     */
    QMap<QString, qint64> restored;
    /**
     * @brief takeFromMap функция изъятия из мапы
     * @return вернет текущую позицию текущего видео
     */
    qint64 takeFromMap();
private slots:
    /**
     * @brief onActionRemoveTriggered слот для кноки "удалить"
     */
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
     * @brief onActionNextTriggered слот для кнопки "следующий"
     */
    void onActionNextTriggered();
    /**
     * @brief onActionPrevTriggered слот для кнопки "предыдущий"
     */
    void onActionPrevTriggered();
    /**
     * @brief positionChanged слот смены позиции
     * @param position непосредственно позиция
     */
    void positionChanged(qint64 position);
    /**
     * @brief pausePlayFlipFlop смена паузы на плей и наоборот
     */
    void pausePlayFlipFlop();
    /**
     * @brief jump функция перехода к элементу из списка
     * @param index его индекс
     */
    void jump(const QModelIndex &index);
    /**
     * @brief checkBackup проверка файла бекапа
     */
    void checkBackup();
    /**
     * @brief restore фукнция извлечения и подготовки данных из бекапа
     * если файлы отстуствуют, то они будут удалены из списка восстановления
     */
    void restore();
    /**
     * @brief backup создание бекапа таймингов видео
     */
    void backup();
    /**
     * @brief backupToFile запись в файл таймингов
     */
    void backupToFile();
    /**
     * @brief closeEvent обработка закрытия окна
     * @param event событие закрытия
     */
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
