#include <QApplication>
#include <QQmlContext>
#include <QtQml/QQmlApplicationEngine>
#include <memory>
#include "boardmenu.hpp"
#include "boardmodel.hpp"
#include "client.hpp"
#include "jsonparser.hpp"
#include "listmodel.hpp"

int main(int argc, char *argv[]) {
    QMap<QString, QString> kwargs = parser::parse_config();
    if (kwargs.empty()) {
        return 1;
    }

    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:main.qml"));
    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreated, &app,
        [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl) {
                QCoreApplication::exit(-1);
            }
        },
        Qt::QueuedConnection
    );

    qmlRegisterType<CardModel>("Client", 1, 0, "ClientCardModel");
    qmlRegisterType<ListModel>("Client", 1, 0, "ClientListModel");
    qmlRegisterType<BoardModel>("Client", 1, 0, "ClientBoardModel");
    qmlRegisterType<BoardMenu>("Client", 1, 0, "ClientBoardMenu");
    qmlRegisterType<Client>("Client", 1, 0, "Client");

    std::unique_ptr<Client> client = std::make_unique<Client>(kwargs);
    engine.rootContext()->setContextProperty("mainClient", client.get());
    engine.load(url);

    return app.exec();
}
