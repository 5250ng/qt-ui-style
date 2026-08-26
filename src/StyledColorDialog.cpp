#include <QtUiStyle/StyledColorDialog.h>

#include <QColorDialog>
#include <QHBoxLayout>
#include <QPushButton>
#include <QVBoxLayout>

namespace qt_ui_style {

StyledColorDialog::StyledColorDialog(QWidget *parent, const QColor &initial,
                                     const QString &title, bool showAlpha)
    : BaseFramelessDialog(parent),
      m_colorDialog(new QColorDialog(this)),
      m_selectedColor(initial) {
    setWindowTitle(title.isEmpty() ? tr("Select Color") : title);
    setModal(true);
    contentLayout()->setContentsMargins(0, 0, 0, 0);

    m_colorDialog->setWindowFlags(Qt::Widget);
    QColorDialog::ColorDialogOptions options = QColorDialog::DontUseNativeDialog
                                               | QColorDialog::NoButtons;
    if (showAlpha) {
        options |= QColorDialog::ShowAlphaChannel;
    }
    m_colorDialog->setOptions(options);
    m_colorDialog->setCurrentColor(initial);
    contentLayout()->addWidget(m_colorDialog);

    auto *buttonLayout = new QHBoxLayout;
    buttonLayout->setContentsMargins(16, 8, 16, 12);
    buttonLayout->addStretch();
    auto *cancelButton = new QPushButton(tr("Cancel"), this);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
    buttonLayout->addWidget(cancelButton);
    auto *okButton = new QPushButton(tr("OK"), this);
    okButton->setDefault(true);
    connect(okButton, &QPushButton::clicked, this, [this]() {
        m_selectedColor = m_colorDialog->currentColor();
        accept();
    });
    buttonLayout->addWidget(okButton);
    contentLayout()->addLayout(buttonLayout);
}

QColor StyledColorDialog::getColor(const QColor &initial, QWidget *parent,
                                   const QString &title, bool showAlpha) {
    StyledColorDialog dialog(parent, initial, title, showAlpha);
    return dialog.exec() == QDialog::Accepted ? dialog.m_selectedColor : QColor();
}

} // namespace qt_ui_style
