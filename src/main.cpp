#include <QApplication>
#include "MainWindow.h"

#include <QFile>
#include <QTextStream>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Load application stylesheet (modern dark theme)
    QFile styleFile("style.qss");
    if (styleFile.open(QFile::ReadOnly | QFile::Text)) {
        QTextStream ts(&styleFile);
        app.setStyleSheet(ts.readAll());
        styleFile.close();
    }

    MainWindow window;
    window.show();

    return app.exec();
}
