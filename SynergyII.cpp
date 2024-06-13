#include    <QApplication>
#include    <QCommandLineParser>
#include    <QCommandLineOption>
#include    <QScreen>

#include    "SynergyIIMainWindow.h"

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(SynergyII);

    QApplication    app(argc, argv);

    QCoreApplication::setOrganizationName("Hi-Techniques");
    QCoreApplication::setApplicationName("SynergyII LLSS Demo");
    QCoreApplication::setApplicationVersion("1.0");

    QCommandLineParser  parser;

    parser.setApplicationDescription(QCoreApplication::applicationName());
    parser.addHelpOption();
    parser.addVersionOption();
    parser.process(app);

    SynergyIIMainWindow synergyIIWindow;
    QScreen*    pScreen = QGuiApplication::primaryScreen();
    QRect       screenGeometry = pScreen->geometry();
    synergyIIWindow.resize(690, screenGeometry.height() / 4);
    synergyIIWindow.show();

    return app.exec();
}
