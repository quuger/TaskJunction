#ifndef ELEMENT_CLASSES_HPP_
#define ELEMENT_CLASSES_HPP_
#include <QtCore>

class board {
    unsigned m_board_id;
    unsigned m_user_id;
    QString m_name;

public:
    explicit board(QVector<QVariant> data);

    void print_data() {
        qDebug() << m_board_id << ' ' << m_name;
    }
};

class list {
    unsigned m_list_id;
    unsigned m_board_id;
    QString m_name;
    QString m_description;

public:
    explicit list(QVector<QVariant> data);
};

class card {
    unsigned m_card_id;
    unsigned m_list_id;
    QString m_name;
    QString m_description;

public:
    explicit card(QVector<QVariant> data);
};

class tag {
    unsigned m_tag_id;
    QString m_name;

public:
    explicit tag(QVector<QVariant> data);
};

board::board(QVector<QVariant> data)
    : m_board_id(data[0].toInt()),
      m_user_id(data[1].toInt()),
      m_name(data[2].toString()) {
}

list::list(QVector<QVariant> data)
    : m_list_id(data[0].toInt()),
      m_board_id(data[1].toInt()),
      m_name(data[2].toString()),
      m_description(data[3].toString()) {
}

card::card(QVector<QVariant> data)
    : m_card_id(data[0].toInt()),
      m_list_id(data[1].toInt()),
      m_name(data[2].toString()),
      m_description(data[3].toString()) {
}

tag::tag(QVector<QVariant> data)
    : m_tag_id(data[0].toInt()), m_name(data[1].toString()) {
}

#endif  // ELEMENT_CLASSES_HPP_
