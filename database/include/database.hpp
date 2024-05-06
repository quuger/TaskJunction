#ifndef DATABASE_HPP_
#define DATABASE_HPP_
#include <QVector>
#include <QtCore>
#include <QtSql>
#include <utility>
#include "element_classes.hpp"

namespace database {

const QString BOARD_TABLE_NAME = "board_signature";
const QString BOARD_PRIMARY_KEY = "board_id";
const QString LIST_TABLE_NAME = "list_signature";
const QString LIST_PRIMARY_KEY = "list_id";
const QString CARD_TABLE_NAME = "card_signature";
const QString CARD_PRIMARY_KEY = "card_id";
const QString TAG_TABLE_NAME = "tag_signature";
const QString TAG_PRIMARY_KEY = "tag_id";
const QString USER_ID_SEQUENCE = "user_signature_id_seq";
const QString BOARD_ID_SEQUENCE = "board_signature_id_seq";
const QString BOARD_NUMBER_SEQUENCE = "board_signature_number_seq";
const QString LIST_ID_SEQUENCE = "list_signature_id_seq";
const QString LIST_NUMBER_SEQUENCE = "list_signature_number_seq";
const QString CARD_ID_SEQUENCE = "card_signature_id_seq";
const QString CARD_NUMBER_SEQUENCE = "card_signature_number_seq";
const QString TAG_ID_SEQUENCE = "tag_signature_id_seq";

const QVector<QString> sequences_names {
        USER_ID_SEQUENCE,
        BOARD_ID_SEQUENCE,
        BOARD_NUMBER_SEQUENCE,
        LIST_ID_SEQUENCE,
        LIST_NUMBER_SEQUENCE,
        CARD_ID_SEQUENCE,
        CARD_NUMBER_SEQUENCE,
        TAG_ID_SEQUENCE
};

const QString QT_DATABASE_DRIVER = "QPSQL";

class db_manager {
    QString m_database_name;
    QString m_user_name;
    QString m_host_name;
    QString m_password;
    QSqlDatabase m_database = QSqlDatabase::addDatabase(QT_DATABASE_DRIVER);
    static QVector<QVariant> get_data(const QSqlRecord &record);
    QSqlRecord select_info_by_id(const QString &query_name, quint32 key_value);
    QString m_schema = "public";

    bool check_user_password(const QString &login, const QString &password);

    quint32 get_user_id_by_name(const QString &name);

    int get_rows_number(const QString &table_name);

    quint32 insert_user(const QString &login, const QString &password);

    quint32 get_sequence_last_value(const QString &sequence);

    static void fill_query_name_to_sql_command();
    static QMap<QString, QString> query_name_to_sql_command;

public:
    db_manager(
        QString database_name,
        QString user_name,
        QString host_name,
        QString password
    );
    void print_all_tables();
    void clear_all_tables();
    void drop_all_tables();

    void set_schema(const QString &name);

    //    void create_schema(const QString &schema_name);
    quint32 insert_board(
        quint32 user_id,
        const QString &name,
        const QString &description
    );
    quint32
    insert_list(quint32 board_id, const QString &name, const QString &description);
    quint32
    insert_card(quint32 list_id, const QString &name, const QString &description);
    quint32 insert_tag(const QString &name);

    bool add_user_to_board(quint32 user_id, quint32 board_id);

    bool add_tag_to_card(quint32 card_id, quint32 tag_id);
    bool delete_tag_from_card(quint32 card_id, quint32 tag_id);

    bool update_command(
        const QString &table_name,
        const QString &updating_field_name,
        const QString &new_value,
        quint32 key_value
    );

    user select_user(quint32 id);
    board select_board(quint32 id);
    list select_list(quint32 id);
    card select_card(quint32 id);
    tag select_tag(quint32 id);

    bool delete_command(const QString &table_name, quint32 key_value);
    bool delete_user_from_board(quint32 user_id, quint32 board_id);

    QVector<board> get_user_boards(quint32 user_id);
    QVector<list> get_board_lists(quint32 board_id);
    QVector<card> get_list_cards(quint32 list_id);
    QVector<tag> get_card_tags(quint32 card_id);

    QVector<quint32> get_board_users_id(quint32 board_id);

    bool check_user_rights(quint32 user_id, quint32 board_id);

    quint32 authorize_user(const QString &login, const QString &password);

    board get_full_board(quint32 board_id);

    quint32 get_number(const QString &table_name, quint32 id);
    // bool move_card(quint32 id, quint32 new_list_id, quint32 new_number);
};

}  // namespace database

#endif  // DATABASE_HPP_
