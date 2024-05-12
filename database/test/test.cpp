#include <iostream>
#include "database.hpp"
#include "doctest.h"
#include "element_classes.hpp"
#include "test_utils.hpp"

using namespace database;
QVector<QString> arguments = {"postgres", "ivan", "localhost", "1"};

#ifdef DEFAULT_TESTS

// TODO check deletion

template <typename T>
bool is_equivalent(QVector<T> &lhs, QVector<T> &rhs) {
    if (lhs.size() == rhs.size()) {
        for (int i = 0; i < lhs.size(); ++i) {
            if (!(lhs[i] == rhs[i])) {
                return false;
            }
        }
        return true;
    }
    return false;
}

TEST_CASE("create") {
    db_manager db_manager(
        arguments[0], arguments[1], arguments[2], arguments[3]
    );
    db_manager.clear_all_tables();

    {
        quint32 id = db_manager.authorize_user("test_user", "test_password");
        CHECK(id == 1);
        auto user = db_manager.select_user(id);
        CHECK(user.m_name == "test_user");
        CHECK(user.m_user_id == id);
    }

    {
        quint32 id =
            db_manager.insert_board(1, "test_board", "test description");
        CHECK(id == 1);
    }

    {
        quint32 id = db_manager.insert_list(1, "test_list", "test description");
        CHECK(id == 1);
    }

    {
        quint32 id = db_manager.insert_card(1, "test_card", "test description");
        CHECK(id == 1);
    }

    {
        quint32 id = db_manager.insert_tag("test_tag");
        CHECK(id == 1);
    }
}

TEST_CASE("select") {
    db_manager db_manager(
        arguments[0], arguments[1], arguments[2], arguments[3]
    );
    db_manager.clear_all_tables();

    quint32 user_id = db_manager.authorize_user("test_user", "test_password");
    quint32 board_id =
        db_manager.insert_board(user_id, "test_board", "test description");
    quint32 list_id =
        db_manager.insert_list(board_id, "test_list", "test description");
    quint32 card_id =
        db_manager.insert_card(list_id, "test_card", "test description");
    quint32 tag_id = db_manager.insert_tag("test_tag");

    {
        user user = db_manager.select_user(user_id);
        CHECK(user.m_user_id == user_id);
        CHECK(user.m_name == "test_user");
    }

    {
        board board = db_manager.select_board(board_id);
        CHECK(board.m_board_id == board_id);
        CHECK(board.m_name == "test_board");
        CHECK(board.m_user_id == user_id);
        CHECK(board.m_description == "test description");
    }

    {
        list list = db_manager.select_list(list_id);
        CHECK(list.m_list_id == list_id);
        CHECK(list.m_board_id == board_id);
        CHECK(list.m_name == "test_list");
        CHECK(list.m_description == "test description");
    }

    {
        card card = db_manager.select_card(card_id);
        CHECK(card.m_card_id == card_id);
        CHECK(card.m_name == "test_card");
        CHECK(card.m_list_id == list_id);
        CHECK(card.m_description == "test description");
    }

    {
        tag tag = db_manager.select_tag(tag_id);
        CHECK(tag.m_tag_id == tag_id);
        CHECK(tag.m_name == "test_tag");
    }
}

TEST_CASE("get user boards") {
    db_manager db_manager(
        arguments[0], arguments[1], arguments[2], arguments[3]
    );
    db_manager.clear_all_tables();

    quint32 user_id = db_manager.authorize_user("test_user", "test_password");
    quint32 board_id_1 = db_manager.insert_board(user_id, "board_1", "");
    quint32 board_id_2 = db_manager.insert_board(user_id, "board_2", "");
    quint32 board_id_3 = db_manager.insert_board(user_id, "board_3", "");
    QVector<quint32> answer = {board_id_1, board_id_2, board_id_3};

    auto result = db_manager.get_user_boards(user_id);
    REQUIRE(answer.size() == result.size());
    for (int i = 0; i < answer.size(); ++i) {
        CHECK(answer[i] == result[i].m_board_id);
    }
}

TEST_CASE("update order") {
    db_manager db_manager(
        arguments[0], arguments[1], arguments[2], arguments[3]
    );
    db_manager.clear_all_tables();

    {
        quint32 user_id =
            db_manager.authorize_user("test_user", "test_password");
        quint32 board_id = db_manager.insert_board(user_id, "test_board", "");
        quint32 list_id = db_manager.insert_list(board_id, "test_list", "");
        quint32 card_id_1 = db_manager.insert_card(list_id, "test_card_1", "");
        quint32 card_id_2 = db_manager.insert_card(list_id, "test_card_2", "");
        CHECK(db_manager.move_card(card_id_1, list_id, 2));
        CHECK(db_manager.get_card_number(card_id_1) == 2);
        CHECK(db_manager.get_card_number(card_id_2) == 1);

        quint32 card_id_3 = db_manager.insert_card(list_id, "test_card_3", "");
        CHECK(db_manager.move_card(card_id_3, list_id, 1));
        CHECK(db_manager.get_card_number(card_id_1) == 3);
        CHECK(db_manager.get_card_number(card_id_2) == 2);
        CHECK(db_manager.get_card_number(card_id_3) == 1);

        CHECK(db_manager.move_card(card_id_3, list_id, 3));
        CHECK(db_manager.get_card_number(card_id_1) == 2);
        CHECK(db_manager.get_card_number(card_id_2) == 1);
        CHECK(db_manager.get_card_number(card_id_3) == 3);
    }

    {
        quint32 user_id =
            db_manager.authorize_user("test_user", "test_password");
        quint32 board_id = db_manager.insert_board(user_id, "test_board", "");
        quint32 list_id = db_manager.insert_list(board_id, "test_list", "");
        quint32 card_id = db_manager.insert_card(list_id, "test_card", "");
        CHECK(!db_manager.move_card(card_id, list_id, 0));
        CHECK(!db_manager.move_card(card_id, list_id, 100));
    }
}

TEST_CASE("check user rights") {
    db_manager db_manager(
        arguments[0], arguments[1], arguments[2], arguments[3]
    );
    db_manager.clear_all_tables();

    quint32 user_id_1 = db_manager.authorize_user("test_user_1", "");
    quint32 user_id_2 = db_manager.authorize_user("test_user_2", "");
    quint32 board_id_1 = db_manager.insert_board(user_id_1, "board_1", "");
    quint32 board_id_2 = db_manager.insert_board(user_id_2, "board_2", "");

    CHECK(db_manager.check_user_rights(user_id_1, board_id_1));
    CHECK(db_manager.check_user_rights(user_id_2, board_id_2));
    CHECK(!db_manager.check_user_rights(user_id_1, board_id_2));
    CHECK(!db_manager.check_user_rights(user_id_2, board_id_1));
}

TEST_CASE("update command") {
    db_manager db_manager(
        arguments[0], arguments[1], arguments[2], arguments[3]
    );
    db_manager.clear_all_tables();

    quint32 user_id = db_manager.authorize_user("test_user", "test_password");
    quint32 board_id = db_manager.insert_board(user_id, "test_board", "");
    QString new_board_name = "new_name";
    QString new_board_description = "new_description";

    CHECK(db_manager.update_command(
        BOARD_TABLE_NAME, "name", new_board_name, board_id
    ));
    CHECK(db_manager.select_board(board_id).m_name == new_board_name);

    CHECK(db_manager.update_command(
        BOARD_TABLE_NAME, "description", new_board_description, board_id
    ));
    CHECK(
        db_manager.select_board(board_id).m_description == new_board_description
    );
}

TEST_CASE("add user to board") {
    db_manager db_manager(
        arguments[0], arguments[1], arguments[2], arguments[3]
    );
    db_manager.clear_all_tables();

    quint32 user_id_1 =
        db_manager.authorize_user("test_user_1", "test_password");
    quint32 user_id_2 =
        db_manager.authorize_user("test_user_2", "test_password");

    quint32 board_id = db_manager.insert_board(user_id_1, "test_board", "");

    CHECK(db_manager.add_user_to_board(user_id_2, board_id));

    QVector<quint32> answer = {user_id_1, user_id_2};
    CHECK(db_manager.get_board_user_ids(board_id) == answer);
}

TEST_CASE("delete user from board") {
    db_manager db_manager(
        arguments[0], arguments[1], arguments[2], arguments[3]
    );
    db_manager.clear_all_tables();

    QVector<quint32> user_ids = {
        db_manager.authorize_user("test_user_1", "test_password"),
        db_manager.authorize_user("test_user_2", "test_password"),
        db_manager.authorize_user("test_user_3", "test_password")
    };
    quint32 board_id = db_manager.insert_board(user_ids[0], "test_board", "");

    CHECK(db_manager.add_user_to_board(user_ids[1], board_id));
    CHECK(db_manager.add_user_to_board(user_ids[2], board_id));

    db_manager.delete_user_from_board(user_ids[2], board_id);

    auto answer = {user_ids[0], user_ids[1]};
    CHECK(db_manager.get_board_user_ids(board_id) == answer);
}

// TODO trouble with updating order, id, etc fields (must be banned)

#endif

#ifdef TEST_NEW_FEATURE

TEST_CASE("new feature") {
    db_manager db_manager(
        arguments[0], arguments[1], arguments[2], arguments[3]
    );
    db_manager.clear_all_tables();
    auto user_id = db_manager.authorize_user("1", "1");
    auto board_id = db_manager.insert_board(user_id, "", "");
    auto list_id_1 = db_manager.insert_list(board_id, "", "");
    auto list_id_2 = db_manager.insert_list(board_id, "", "");
    db_manager.insert_card(list_id_1, "", "");
    db_manager.insert_card(list_id_1, "", "");
    db_manager.insert_card(list_id_2, "", "");
    db_manager.insert_card(list_id_2, "", "");
    auto card_ids = db_manager.get_board_card_ids(board_id);
    for (auto card_id : card_ids) {
        std::cout << card_id << ' ';
    }
    std::cout << '\n';
}

#endif