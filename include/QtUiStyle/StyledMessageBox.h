#pragma once

#include <QtUiStyle/BaseFramelessDialog.h>

namespace qt_ui_style {

class StyledMessageBox : public BaseFramelessDialog {
    Q_OBJECT

  public:
    enum Result { Yes, No, Ok, Cancel };

    static void information(QWidget *parent, const QString &title, const QString &text);
    static void warning(QWidget *parent, const QString &title, const QString &text);
    static Result question(QWidget *parent, const QString &title, const QString &text);

  private:
    explicit StyledMessageBox(QWidget *parent, const QString &title,
                              const QString &text, bool hasNo = false);

    Result m_result = Cancel;
};

} // namespace qt_ui_style
