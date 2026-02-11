#include "mainwindow.h"
#include "StartupWindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    StartupWindow start;
    MainWindow *mainWin = nullptr;

    QObject::connect(&start, &StartupWindow::projectCreated,
                     [&](const QString &name, const QString &path) {

                         mainWin = new MainWindow(name, path);
                         mainWin->show();
                         start.close();
                     });

    start.show();
    return app.exec();
}
