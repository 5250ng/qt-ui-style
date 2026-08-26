#include <QtUiStyle/StyledFileDialog.h>

#include <QVBoxLayout>

namespace qt_ui_style {

StyledFileDialog::StyledFileDialog(QWidget *parent, const QString &caption,
                                   const QString &directory,
                                   const QString &filter,
                                   QFileDialog::AcceptMode acceptMode)
    : BaseFramelessDialog(parent),
      m_fileDialog(new QFileDialog(this, QString(), directory, filter)) {
    const QString defaultCaption = acceptMode == QFileDialog::AcceptOpen
                                       ? tr("Open File")
                                       : tr("Save File");
    setWindowTitle(caption.isEmpty() ? defaultCaption : caption);
    setModal(true);
    contentLayout()->setContentsMargins(0, 0, 0, 0);

    m_fileDialog->setWindowFlags(Qt::Widget);
    m_fileDialog->setOption(QFileDialog::DontUseNativeDialog);
    m_fileDialog->setAcceptMode(acceptMode);
    m_fileDialog->setFileMode(acceptMode == QFileDialog::AcceptOpen
                                  ? QFileDialog::ExistingFile
                                  : QFileDialog::AnyFile);
    contentLayout()->addWidget(m_fileDialog);
    connect(m_fileDialog, &QFileDialog::accepted, this, &QDialog::accept);
    connect(m_fileDialog, &QFileDialog::rejected, this, &QDialog::reject);
    resize(700, 500);
}

QString StyledFileDialog::getOpenFileName(QWidget *parent,
                                          const QString &caption,
                                          const QString &directory,
                                          const QString &filter) {
    StyledFileDialog dialog(parent, caption, directory, filter,
                            QFileDialog::AcceptOpen);
    if (dialog.exec() != QDialog::Accepted) {
        return {};
    }
    const QStringList files = dialog.m_fileDialog->selectedFiles();
    return files.isEmpty() ? QString() : files.first();
}

QString StyledFileDialog::getSaveFileName(QWidget *parent,
                                          const QString &caption,
                                          const QString &directory,
                                          const QString &filter) {
    StyledFileDialog dialog(parent, caption, directory, filter,
                            QFileDialog::AcceptSave);
    if (dialog.exec() != QDialog::Accepted) {
        return {};
    }
    const QStringList files = dialog.m_fileDialog->selectedFiles();
    return files.isEmpty() ? QString() : files.first();
}

} // namespace qt_ui_style
