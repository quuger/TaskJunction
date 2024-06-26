#ifndef SERVER_HPP_
#define SERVER_HPP_

#include <QMap>
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QWebSocketServer>
#include "database.hpp"
#include "logging.hpp"
#include "query.hpp"

struct ReturnedValue {
    bool exit_code;
    quint32 board_id;
    QString jsoned_object;
};

class ClientSocket;

class Server : public QObject {
    Q_OBJECT

    QWebSocketServer *server;
    quint16 port;
    database::db_manager db;
    QMap<quint32, ClientSocket *> authorized_connections;
    QList<ClientSocket *> unauthorized_connections;

    ReturnedValue execute_update_query(const update_query &query, quint32 id);
    ReturnedValue execute_create_query(const create_query &query, quint32 id);
    ReturnedValue execute_delete_query(const delete_query &query, quint32 id);
    ReturnedValue
    execute_get_query(const get_boards_info_query &query, quint32 id);
    ReturnedValue
    execute_access_query(const access_to_board &query, quint32 id);
    ReturnedValue execute_move_query(const move_query &query, quint32 id);
    ReturnedValue execute_upload_query(copy_board_query query, quint32 id);
    ReturnedValue execute_filter_query(const filter_query &query, quint32 id);
    std::pair<QString, quint32> execute_login_query(const login_query &query);

public:
    explicit Server(quint16 port, const QMap<QString, QString> &config);
    ~Server();

public slots:
    void onSslErrors(const QList<QSslError> &);
    void newConnection();
    void removeConnection();
    void execute_query(uint user_id, const query_type &query);

    // void database_executed(uint user_id, const QString &response);
};

#endif