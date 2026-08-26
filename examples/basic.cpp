#include <QtUiStyle/BaseFramelessWindow.h>

#include <QApplication>
#include <QLabel>

int main(int argc, char **argv) {
    QApplication app(argc, argv);

    qt_ui_style::BaseFramelessWindow window;
    window.setWindowTitle("QtUiStyle example");
    window.contentLayout()->addWidget(new QLabel("Frameless Qt window", &window));
    window.resize(640, 400);
    window.show();

    return app.exec();
}
