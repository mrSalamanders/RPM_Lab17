#ifndef PLAYLISTMODEL_H
#define PLAYLISTMODEL_H

#include <QAbstractItemModel>
#include <QScopedPointer>

QT_BEGIN_NAMESPACE
class QMediaPlaylist;
QT_END_NAMESPACE
/**
 * @brief The PlaylistModel class кастомный класс модели предметов
 */
class PlaylistModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    enum Column
    {
        Title = 0,
        ColumnCount
    };

    explicit PlaylistModel(QObject *parent = nullptr);
    ~PlaylistModel();
    /**
     * @brief перегрузка существующих функций в классе абстрактной модели предметов
     * @return
     */
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::DisplayRole) override;
    /**
     * @brief playlist плейлист видео
     * @return
     */
    QMediaPlaylist *playlist() const;
    /**
     * @brief setPlaylist установить плейлист
     * @param playlist сам плейлист
     */
    void setPlaylist(QMediaPlaylist *playlist);

private slots:
    void beginInsertItems(int start, int end);
    void endInsertItems();
    void beginRemoveItems(int start, int end);
    void endRemoveItems();
    void changeItems(int start, int end);

private:
    QScopedPointer<QMediaPlaylist> m_playlist;
    QMap<QModelIndex, QVariant> m_data;
};

#endif // PLAYLISTMODEL_H
