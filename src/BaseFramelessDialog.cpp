#include <QtUiStyle/BaseFramelessDialog.h>

#include <QtUiStyle/TitleBar.h>

#include <QEvent>
#include <QMenuBar>
#include <QMouseEvent>
#include <QVBoxLayout>
#include <QWidget>
#include <QWindow>

namespace qt_ui_style {

BaseFramelessDialog::BaseFramelessDialog(QWidget *parent)
    : QDialog(parent),
      m_content(new QWidget(this)),
      m_titleBar(new TitleBar(this)),
      m_rootLayout(new QVBoxLayout(this)),
      m_contentLayout(new QVBoxLayout(m_content)) {
    setWindowFlag(Qt::FramelessWindowHint);
    m_titleBar->setMinMaxVisible(false);
    m_titleBar->menuBar()->hide();

    m_rootLayout->setSpacing(0);
    m_rootLayout->addWidget(m_titleBar);
    m_contentLayout->setContentsMargins(0, 0, 0, 0);
    m_contentLayout->setSpacing(0);
    m_rootLayout->addWidget(m_content, 1);
    updateResizeBorder();

    setMouseTracking(true);
    installEventFilter(this);

    connect(this, &QWidget::windowTitleChanged, m_titleBar, &TitleBar::setTitle);
    connect(m_titleBar, &TitleBar::closeRequested, this, &QDialog::reject);
    connect(m_titleBar, &TitleBar::mousePressed,
            this, &BaseFramelessDialog::startTitleBarMove);
    connect(m_titleBar, &TitleBar::mouseMoved,
            this, &BaseFramelessDialog::continueFallbackMove);
    connect(m_titleBar, &TitleBar::mouseReleased, this, [this]() {
        m_fallbackDragging = false;
    });
}

TitleBar *BaseFramelessDialog::titleBar() const { return m_titleBar; }
QVBoxLayout *BaseFramelessDialog::contentLayout() const { return m_contentLayout; }

void BaseFramelessDialog::setResizable(bool resizable) {
    if (m_resizable == resizable) {
        return;
    }
    m_resizable = resizable;
    updateResizeBorder();
}

bool BaseFramelessDialog::isResizable() const { return m_resizable; }

bool BaseFramelessDialog::eventFilter(QObject *object, QEvent *event) {
    if (object != this || !m_resizable) {
        return QDialog::eventFilter(object, event);
    }

    if (event->type() == QEvent::Leave) {
        unsetCursor();
        return QDialog::eventFilter(object, event);
    }

    auto *mouseEvent = dynamic_cast<QMouseEvent *>(event);
    if (!mouseEvent) {
        return QDialog::eventFilter(object, event);
    }

    const Qt::Edges edges = edgesAt(mouseEvent->position().toPoint());
    switch (event->type()) {
    case QEvent::MouseMove:
        if (!(mouseEvent->buttons() & Qt::LeftButton)) {
            edges ? setCursor(cursorForEdges(edges)) : unsetCursor();
        }
        break;
    case QEvent::MouseButtonPress:
        if (mouseEvent->button() == Qt::LeftButton && edges && windowHandle()) {
            if (windowHandle()->startSystemResize(edges)) {
                return true;
            }
        }
        break;
    default:
        break;
    }
    return QDialog::eventFilter(object, event);
}

Qt::Edges BaseFramelessDialog::edgesAt(const QPoint &position) const {
    Qt::Edges edges;
    if (position.x() < ResizeMargin) edges |= Qt::LeftEdge;
    if (position.x() >= width() - ResizeMargin) edges |= Qt::RightEdge;
    if (position.y() < ResizeMargin) edges |= Qt::TopEdge;
    if (position.y() >= height() - ResizeMargin) edges |= Qt::BottomEdge;
    return edges;
}

Qt::CursorShape BaseFramelessDialog::cursorForEdges(Qt::Edges edges) const {
    if (edges == (Qt::TopEdge | Qt::LeftEdge)
        || edges == (Qt::BottomEdge | Qt::RightEdge)) {
        return Qt::SizeFDiagCursor;
    }
    if (edges == (Qt::TopEdge | Qt::RightEdge)
        || edges == (Qt::BottomEdge | Qt::LeftEdge)) {
        return Qt::SizeBDiagCursor;
    }
    if (edges & (Qt::LeftEdge | Qt::RightEdge)) return Qt::SizeHorCursor;
    if (edges & (Qt::TopEdge | Qt::BottomEdge)) return Qt::SizeVerCursor;
    return Qt::ArrowCursor;
}

void BaseFramelessDialog::updateResizeBorder() {
    const int margin = m_resizable ? ResizeMargin : 0;
    m_rootLayout->setContentsMargins(margin, margin, margin, margin);
    unsetCursor();
}

void BaseFramelessDialog::startTitleBarMove(const QPoint &globalPosition) {
    if (!windowHandle() || windowHandle()->startSystemMove()) {
        return;
    }
    m_fallbackDragging = true;
    m_fallbackDragOffset = globalPosition - frameGeometry().topLeft();
}

void BaseFramelessDialog::continueFallbackMove(const QPoint &globalPosition) {
    if (m_fallbackDragging) {
        move(globalPosition - m_fallbackDragOffset);
    }
}

} // namespace qt_ui_style
