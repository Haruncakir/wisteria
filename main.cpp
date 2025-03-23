#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QDir>
#include <QQmlFileSelector>
#include <QTextDocument>
#include "filemanager.h"
#include "theme.h"


int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);

    // QQuickStyle::setStyle("Fusion");

    // Set up QML file import paths
    QDir appDir(app.applicationDirPath());
    QString qmlDir = appDir.absolutePath() + "/qml";

    // Create the FileManager instance
    FileManager fileManager;

    // Create the Theme instance
    Theme theme;

    QQmlApplicationEngine engine;

    // Add the application's QML directory to the import path
    engine.addImportPath(qmlDir);

    // Register QML types (if we were using custom C++ QML types)
    // qmlRegisterType<Theme>("CustomComponents", 1, 0, "ThemeSettings");
    qmlRegisterType<QTextDocument>("com.wisteria.TextDocument", 1, 0, "QTextDocument");

    // Expose FileManager and Theme to QML
    engine.rootContext()->setContextProperty("fileManager", &fileManager);
    engine.rootContext()->setContextProperty("theme", &theme);

    // Load the main QML file
    const QUrl url(QStringLiteral("qrc:/wisteria/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
                         if (!obj && url == objUrl)
                             QCoreApplication::exit(-1);
                     }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
