#pragma once

#include <QColor>
#include <QObject>

namespace qt_ui_style {

struct TitleBarStyle {
    QColor background;
    QColor foreground;
    QColor buttonHover;
    QColor closeButtonHover;
    QColor separator;

    bool operator==(const TitleBarStyle &other) const {
        return background == other.background && foreground == other.foreground
               && buttonHover == other.buttonHover
               && closeButtonHover == other.closeButtonHover
               && separator == other.separator;
    }
};

class StyleManager final : public QObject {
    Q_OBJECT

  public:
    static StyleManager &instance();

    TitleBarStyle titleBarStyle() const;
    void setTitleBarStyle(const TitleBarStyle &style);
    void resetTitleBarStyle();

  signals:
    void titleBarStyleChanged(const qt_ui_style::TitleBarStyle &style);

  private:
    explicit StyleManager(QObject *parent = nullptr);

    TitleBarStyle m_titleBarStyle;
};

} // namespace qt_ui_style

Q_DECLARE_METATYPE(qt_ui_style::TitleBarStyle)
