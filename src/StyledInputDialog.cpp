#include <QtUiStyle/StyledInputDialog.h>

#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

namespace qt_ui_style {

StyledInputDialog::StyledInputDialog(QWidget *parent, const QString &title,
                                     const QString &label,
                                     QLineEdit::EchoMode mode,
                                     const QString &text)
    : BaseFramelessDialog(parent), m_lineEdit(new QLineEdit(this)) {
    setWindowTitle(title);
    setModal(true);
    setResizable(false);
    contentLayout()->setContentsMargins(16, 12, 16, 12);
    contentLayout()->setSpacing(12);

    auto *textLabel = new QLabel(label, this);
    textLabel->setWordWrap(true);
    contentLayout()->addWidget(textLabel);

    m_lineEdit->setEchoMode(mode);
    m_lineEdit->setText(text);
    m_lineEdit->selectAll();
    contentLayout()->addWidget(m_lineEdit);

    auto *buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch();
    auto *cancelButton = new QPushButton(tr("Cancel"), this);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
    buttonLayout->addWidget(cancelButton);
    auto *okButton = new QPushButton(tr("OK"), this);
    okButton->setDefault(true);
    connect(okButton, &QPushButton::clicked, this, &QDialog::accept);
    buttonLayout->addWidget(okButton);
    contentLayout()->addLayout(buttonLayout);
    m_lineEdit->setFocus();
}

QString StyledInputDialog::getText(QWidget *parent, const QString &title,
                                   const QString &label, QLineEdit::EchoMode mode,
                                   const QString &text, bool *ok) {
    StyledInputDialog dialog(parent, title, label, mode, text);
    const bool accepted = dialog.exec() == QDialog::Accepted;
    if (ok) {
        *ok = accepted;
    }
    return accepted ? dialog.m_lineEdit->text() : QString();
}

} // namespace qt_ui_style
