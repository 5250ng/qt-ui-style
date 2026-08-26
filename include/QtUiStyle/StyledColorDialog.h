#pragma once

#include <QtUiStyle/BaseFramelessDialog.h>

#include <QColor>

class QColorDialog;

namespace qt_ui_style {

class StyledColorDialog : public BaseFramelessDialog {
    Q_OBJECT

  public:
    static QColor getColor(const QColor &initial, QWidget *parent,
                           const QString &title = {}, bool showAlpha = false);

  private:
    explicit StyledColorDialog(QWidget *parent, const QColor &initial,
                               const QString &title, bool showAlpha);

    QColorDialog *m_colorDialog;
    QColor m_selectedColor;
};

} // namespace qt_ui_style
