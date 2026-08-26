#include <QtUiStyle/StyledMessageBox.h>

#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

namespace qt_ui_style {

StyledMessageBox::StyledMessageBox(QWidget *parent, const QString &title,
                                   const QString &text, bool hasNo)
    : BaseFramelessDialog(parent) {
    setWindowTitle(title);
    setModal(true);
    setResizable(false);

    contentLayout()->setContentsMargins(16, 12, 16, 12);
    contentLayout()->setSpacing(12);

    auto *label = new QLabel(text, this);
    label->setWordWrap(true);
    label->setMinimumWidth(280);
    contentLayout()->addWidget(label);

    auto *buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch();
    if (hasNo) {
        auto *noButton = new QPushButton(tr("No"), this);
        connect(noButton, &QPushButton::clicked, this, [this]() {
            m_result = No;
            reject();
        });
        buttonLayout->addWidget(noButton);

        auto *yesButton = new QPushButton(tr("Yes"), this);
        yesButton->setDefault(true);
        connect(yesButton, &QPushButton::clicked, this, [this]() {
            m_result = Yes;
            accept();
        });
        buttonLayout->addWidget(yesButton);
    } else {
        auto *okButton = new QPushButton(tr("OK"), this);
        okButton->setDefault(true);
        connect(okButton, &QPushButton::clicked, this, [this]() {
            m_result = Ok;
            accept();
        });
        buttonLayout->addWidget(okButton);
    }
    contentLayout()->addLayout(buttonLayout);
}

void StyledMessageBox::information(QWidget *parent, const QString &title,
                                   const QString &text) {
    StyledMessageBox(parent, title, text).exec();
}

void StyledMessageBox::warning(QWidget *parent, const QString &title,
                               const QString &text) {
    StyledMessageBox(parent, title, text).exec();
}

StyledMessageBox::Result StyledMessageBox::question(QWidget *parent,
                                                    const QString &title,
                                                    const QString &text) {
    StyledMessageBox dialog(parent, title, text, true);
    dialog.exec();
    return dialog.m_result;
}

} // namespace qt_ui_style
