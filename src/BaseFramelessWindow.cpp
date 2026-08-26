#include <QtUiStyle/BaseFramelessWindow.h>

#include <QtUiStyle/TitleBar.h>

#include <QApplication>
#include <QEvent>
#include <QMouseEvent>
#include <QVBoxLayout>
#include <QWidget>
#include <QWindow>

namespace qt_ui_style {

BaseFramelessWindow::BaseFramelessWindow(QWidget *parent)
    : QMainWindow(parent),
      m_central(new QWidget(this)),
      m_titleBar(new TitleBar(m_central)),
      m_content(new QWidget(m_central)),
      m_rootLayout(new QVBoxLayout(m_central)),
      m_contentLayout(new QVBoxLayout(m_content)) {
    setWindowFlag(Qt::FramelessWindowHint);

    m_rootLayout->setContentsMargins(ResizeMargin, ResizeMargin, ResizeMargin, ResizeMargin);
    m_rootLayout->setSpacing(0);
    m_rootLayout->addWidget(m_titleBar);
    m_contentLayout->setContentsMargins(0, 0, 0, 0);
    m_contentLayout->setSpacing(0);
    m_rootLayout->addWidget(m_content, 1);
    setCentralWidget(m_central);

    setMouseTracking(true);
    m_central->setMouseTracking(true);
    qApp->installEventFilter(this);

    connect(this, &QWidget::windowTitleChanged, m_titleBar, &TitleBar::setTitle);
    connect(m_titleBar, &TitleBar::minimizeRequested, this, &QWidget::showMinimized);
    connect(m_titleBar, &TitleBar::maximizeRestoreRequested, this, [this]() {
        isMaximized() ? showNormal() : showMaximized();
    });
    connect(m_titleBar, &TitleBar::closeRequested, this, &QWidget::close);
    connect(m_titleBar, &TitleBar::mousePressed,
            this, &BaseFramelessWindow::startTitleBarMove);
    connect(m_titleBar, &TitleBar::mouseMoved,
            this, &BaseFramelessWindow::continueFallbackMove);
    connect(m_titleBar, &TitleBar::mouseReleased, this, [this]() {
        m_fallbackDragging = false;
    });
    connect(m_titleBar, &TitleBar::mouseDoubleClicked, this, [this](const QPoint &) {
        isMaximized() ? showNormal() : showMaximized();
    });
}

TitleBar *BaseFramelessWindow::titleBar() const { return m_titleBar; }
QVBoxLayout *BaseFramelessWindow::contentLayout() const { return m_contentLayout; }

bool BaseFramelessWindow::eventFilter(QObject *object, QEvent *event) {
    if (event->type() != QEvent::MouseMove
        && event->type() != QEvent::MouseButtonPress
        && event->type() != QEvent::Leave) {
        return QMainWindow::eventFilter(object, event);
    }

    auto *widget = qobject_cast<QWidget *>(object);
    if (!widget || widget->window() != this) {
        return QMainWindow::eventFilter(object, event);
    }

    if (isMaximized() || isFullScreen()) {
        clearResizeCursor();
        return QMainWindow::eventFilter(object, event);
    }

    if (event->type() == QEvent::Leave) {
        if (widget == m_resizeCursorWidget) {
            clearResizeCursor();
        }
        return QMainWindow::eventFilter(object, event);
    }

    auto *mouseEvent = dynamic_cast<QMouseEvent *>(event);
    if (!mouseEvent) {
        return QMainWindow::eventFilter(object, event);
    }

    const QPoint windowPosition = widget == this
                                      ? mouseEvent->position().toPoint()
                                      : widget->mapTo(this, mouseEvent->position().toPoint());
    const Qt::Edges edges = edgesAt(windowPosition);
    switch (event->type()) {
    case QEvent::MouseMove:
        if (!(mouseEvent->buttons() & Qt::LeftButton)) {
            updateResizeCursor(widget, edges);
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
    return QMainWindow::eventFilter(object, event);
}

void BaseFramelessWindow::changeEvent(QEvent *event) {
    QMainWindow::changeEvent(event);
    if (event->type() == QEvent::WindowStateChange) {
        updateResizeBorder();
        m_titleBar->setMaximized(isMaximized());
    }
}

Qt::Edges BaseFramelessWindow::edgesAt(const QPoint &position) const {
    Qt::Edges edges;
    if (position.x() < ResizeMargin) edges |= Qt::LeftEdge;
    if (position.x() >= width() - ResizeMargin) edges |= Qt::RightEdge;
    if (position.y() < ResizeMargin) edges |= Qt::TopEdge;
    if (position.y() >= height() - ResizeMargin) edges |= Qt::BottomEdge;
    return edges;
}

Qt::CursorShape BaseFramelessWindow::cursorForEdges(Qt::Edges edges) const {
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

void BaseFramelessWindow::updateResizeCursor(QWidget *widget, Qt::Edges edges) {
    if (!edges) {
        clearResizeCursor();
        return;
    }
    if (m_resizeCursorWidget && m_resizeCursorWidget != widget) {
        m_resizeCursorWidget->unsetCursor();
    }
    widget->setCursor(cursorForEdges(edges));
    m_resizeCursorWidget = widget;
}

void BaseFramelessWindow::clearResizeCursor() {
    if (m_resizeCursorWidget) {
        m_resizeCursorWidget->unsetCursor();
        m_resizeCursorWidget.clear();
    }
}

void BaseFramelessWindow::updateResizeBorder() {
    const int margin = (isMaximized() || isFullScreen()) ? 0 : ResizeMargin;
    m_rootLayout->setContentsMargins(margin, margin, margin, margin);
    clearResizeCursor();
}

void BaseFramelessWindow::startTitleBarMove(const QPoint &globalPosition) {
    if (!windowHandle() || windowHandle()->startSystemMove()) {
        return;
    }
    m_fallbackDragging = true;
    m_fallbackDragOffset = globalPosition - frameGeometry().topLeft();
}

void BaseFramelessWindow::continueFallbackMove(const QPoint &globalPosition) {
    if (m_fallbackDragging) {
        move(globalPosition - m_fallbackDragOffset);
    }
}

} // namespace qt_ui_style
