#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QGuiApplication>
#include "gs232.h"


QCoreApplication* createApplication(int &argc, char *argv[])
{
    for (int i = 1; i < argc; ++i) {
        if (!qstrcmp(argv[i], "-no-gui"))
            return new QCoreApplication(argc, argv);
    }
    return new QApplication(argc, argv);
}

int main(int argc, char *argv[])
{

    QScopedPointer<QCoreApplication> app(createApplication(argc, argv));

    QQmlApplicationEngine engine;

    gs232 ryp2;

    engine.rootContext()->setContextProperty(QStringLiteral("rotator"), &ryp2);
    engine.load(QUrl(QStringLiteral("qrc:/small.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app->exec();
}
