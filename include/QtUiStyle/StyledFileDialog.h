#pragma once

#include <QtUiStyle/BaseFramelessDialog.h>

#include <QFileDialog>

namespace qt_ui_style {

class StyledFileDialog : public BaseFramelessDialog {
    Q_OBJECT

  public:
    static QString getOpenFileName(QWidget *parent, const QString &caption = {},
                                   const QString &directory = {},
                                   const QString &filter = {});
    static QString getSaveFileName(QWidget *parent, const QString &caption = {},
                                   const QString &directory = {},
                                   const QString &filter = {});

  private:
    explicit StyledFileDialog(QWidget *parent, const QString &caption,
                              const QString &directory, const QString &filter,
                              QFileDialog::AcceptMode acceptMode);

    QFileDialog *m_fileDialog;
};

} // namespace qt_ui_style
