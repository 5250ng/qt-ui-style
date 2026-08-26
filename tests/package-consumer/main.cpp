#include <QtUiStyle/BaseFramelessWindow.h>
#include <QtUiStyle/StyledMessageBox.h>

#include <QApplication>

int main(int argc, char **argv) {
    QApplication app(argc, argv);
    qt_ui_style::BaseFramelessWindow window;
    window.setWindowTitle(QStringLiteral("Package consumer"));
    return window.titleBar() && window.contentLayout() ? 0 : 1;
}
