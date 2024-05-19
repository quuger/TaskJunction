#ifndef BOARDMENU_HPP_
#define BOARDMENU_HPP_

#include <QAbstractListModel>
#include <QScopedPointer>
#include <QVector>
#include <memory>
#include <unordered_map>
#include "boardmodel.hpp"
#include "element_classes.hpp"

class BoardMenu : public QAbstractListModel {
    Q_OBJECT
    Q_PROPERTY(int count READ get_count NOTIFY countChanged)
public:
    explicit BoardMenu(QObject *parent = nullptr);
    QHash<int, QByteArray> roleNames() const override;
    int rowCount(const QModelIndex &parentclear = {}) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole)
        const override;
    int get_count();
    void create_board(
        QString &name,
        QString &description,
        quint32 id,
        quint32 user_id
    );
    void create_board(const board &board);
    std::pair<quint32, bool> get_info(int board_index) const;
    std::pair<quint32, bool> delete_board(int board_index);
    BoardModel *load(int index, const board &loaded_board);
    bool is_board_loaded(quint32 id) const;
    void unload_remote_boards();
    void update_board(int index, const QString &field, const QString &value);
    void create_list(quint32 board_id, const list &list);
    void create_card(quint32 board_id, quint32 list_id, const card &card);
    void create_tag(
        quint32 board_id,
        quint32 list_id,
        quint32 card_id,
        const tag &tag
    );

    void delete_command(
        quint32 board_id,
        quint32 list_id,
        quint32 card_id,
        quint32 tag_id
    );
    void update_command(
        quint32 board_id,
        quint32 list_id,
        quint32 card_id,
        const QString &field,
        const QString &new_value
    );
    void move_command(
        quint32 board_id,
        quint32 old_list_id,
        quint32 new_list_id,
        quint32 card_id,
        int new_index
    );

    friend class Client;
signals:
    void countChanged();

private:
    QVector<board> m_boards;
    bool is_local_loaded = false;
    QMap<quint32, int> m_board_id_to_index;
    std::unordered_map<quint32, std::unique_ptr<BoardModel>>
        m_loaded_boards;  // check if really needed

    enum BoardRoles { NameRole = Qt::UserRole + 1, DescriptionRole };
};

#endif  // BOARDMENU_HPP_