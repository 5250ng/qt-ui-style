#pragma once

#include <QtUiStyle/BaseFramelessDialog.h>

#include <QLineEdit>

namespace qt_ui_style {

class StyledInputDialog : public BaseFramelessDialog {
    Q_OBJECT

  public:
    static QString getText(QWidget *parent, const QString &title,
                           const QString &label,
                           QLineEdit::EchoMode mode = QLineEdit::Normal,
                           const QString &text = {}, bool *ok = nullptr);

  private:
    explicit StyledInputDialog(QWidget *parent, const QString &title,
                               const QString &label, QLineEdit::EchoMode mode,
                               const QString &text);

    QLineEdit *m_lineEdit;
};

} // namespace qt_ui_style
