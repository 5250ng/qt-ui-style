#include <QtUiStyle/StyleManager.h>

namespace qt_ui_style {

StyleManager::StyleManager(QObject *parent) : QObject(parent) {}

StyleManager &StyleManager::instance() {
    static StyleManager manager;
    return manager;
}

TitleBarStyle StyleManager::titleBarStyle() const {
    return m_titleBarStyle;
}

void StyleManager::setTitleBarStyle(const TitleBarStyle &style) {
    if (m_titleBarStyle == style) {
        return;
    }
    m_titleBarStyle = style;
    emit titleBarStyleChanged(m_titleBarStyle);
}

void StyleManager::resetTitleBarStyle() {
    setTitleBarStyle({});
}

} // namespace qt_ui_style
