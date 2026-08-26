#include <QtUiStyle/TitleBar.h>

#include <QtUiStyle/StyleManager.h>

#include <QApplication>
#include <QEvent>
#include <QFont>
#include <QFrame>
#include <QHBoxLayout>
#include <QIcon>
#include <QLabel>
#include <QMenuBar>
#include <QMouseEvent>
#include <QPainter>
#include <QPalette>
#include <QPushButton>
#include <QResizeEvent>
#include <QSizePolicy>

namespace qt_ui_style {
namespace {

QString buttonStyle(const QColor &hoverColor) {
    return QStringLiteral(
               "QPushButton { background: transparent; border: none; "
               "border-radius: 3px; padding: 0 4px; }"
               "QPushButton:hover { background-color: %1; }")
        .arg(hoverColor.name(QColor::HexArgb));
}

QIcon lineIcon(const QColor &color, int size, bool restore) {
    QPixmap pixmap(size, size);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(color, 1.2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.setBrush(Qt::NoBrush);

    const int margin = size / 5;
    if (restore) {
        painter.drawRect(margin + 1, margin, size - 2 * margin - 1, size - 2 * margin - 1);
        painter.drawRect(margin, margin + 2, size - 2 * margin - 1, size - 2 * margin - 1);
    } else {
        painter.drawRect(margin, margin, size - 2 * margin, size - 2 * margin);
    }
    return QIcon(pixmap);
}

QIcon minimizeIcon(const QColor &color, int size) {
    QPixmap pixmap(size, size);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(color, 1.2, Qt::SolidLine, Qt::RoundCap));
    const int margin = size / 5;
    painter.drawLine(margin, size / 2, size - margin, size / 2);
    return QIcon(pixmap);
}

QIcon closeIcon(const QColor &color, int size) {
    QPixmap pixmap(size, size);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(color, 1.2, Qt::SolidLine, Qt::RoundCap));
    const int margin = size / 4;
    painter.drawLine(margin, margin, size - margin, size - margin);
    painter.drawLine(size - margin, margin, margin, size - margin);
    return QIcon(pixmap);
}

} // namespace

TitleBar::TitleBar(QWidget *parent)
    : QWidget(parent),
      m_menuBar(new QMenuBar(this)),
      m_titleLabel(new QLabel(this)),
      m_bottomLine(new QFrame(this)),
      m_minimizeButton(new QPushButton(this)),
      m_maximizeButton(new QPushButton(this)),
      m_closeButton(new QPushButton(this)),
      m_layout(new QHBoxLayout(this)) {
    setObjectName(QStringLiteral("qtUiStyleTitleBar"));
    m_titleLabel->setObjectName(QStringLiteral("qtUiStyleTitleLabel"));
    m_minimizeButton->setObjectName(QStringLiteral("qtUiStyleMinimizeButton"));
    m_maximizeButton->setObjectName(QStringLiteral("qtUiStyleMaximizeButton"));
    m_closeButton->setObjectName(QStringLiteral("qtUiStyleCloseButton"));

    m_layout->setContentsMargins(8, 2, 8, 2);
    m_layout->setSpacing(4);
    m_menuBar->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
    m_layout->addWidget(m_menuBar);
    m_layout->addStretch();

    QFont titleFont = m_titleLabel->font();
    titleFont.setBold(true);
    m_titleLabel->setFont(titleFont);
    m_titleLabel->setAlignment(Qt::AlignCenter);
    m_titleLabel->setAttribute(Qt::WA_TransparentForMouseEvents);
    m_titleLabel->raise();

    for (QPushButton *button : {m_minimizeButton, m_maximizeButton, m_closeButton}) {
        button->setFixedSize(28, 20);
        button->setFlat(true);
        button->setIconSize(QSize(10, 10));
        button->setCursor(Qt::ArrowCursor);
    }
    m_minimizeButton->setAccessibleName(tr("Minimize"));
    m_minimizeButton->setToolTip(tr("Minimize"));
    m_maximizeButton->setAccessibleName(tr("Maximize"));
    m_maximizeButton->setToolTip(tr("Maximize"));
    m_closeButton->setAccessibleName(tr("Close"));
    m_closeButton->setToolTip(tr("Close"));

    m_layout->addWidget(m_minimizeButton, 0, Qt::AlignVCenter);
    m_layout->addWidget(m_maximizeButton, 0, Qt::AlignVCenter);
    m_layout->addWidget(m_closeButton, 0, Qt::AlignVCenter);

    connect(m_minimizeButton, &QPushButton::clicked, this, &TitleBar::minimizeRequested);
    connect(m_maximizeButton, &QPushButton::clicked, this, &TitleBar::maximizeRestoreRequested);
    connect(m_closeButton, &QPushButton::clicked, this, &TitleBar::closeRequested);

    m_menuBar->installEventFilter(this);
    m_bottomLine->setFixedHeight(1);
    m_bottomLine->raise();

    connect(&StyleManager::instance(), &StyleManager::titleBarStyleChanged,
            this, &TitleBar::applyStyle);
    applyStyle();
}

QMenuBar *TitleBar::menuBar() const { return m_menuBar; }

void TitleBar::setTitle(const QString &title) { m_titleLabel->setText(title); }

void TitleBar::setMinMaxVisible(bool visible) {
    setMinimizeVisible(visible);
    setMaximizeVisible(visible);
}

void TitleBar::setMinimizeVisible(bool visible) {
    m_minimizeButton->setVisible(visible);
    updateOverlayGeometry();
}
void TitleBar::setMaximizeVisible(bool visible) {
    m_maximizeButton->setVisible(visible);
    updateOverlayGeometry();
}
void TitleBar::setCloseVisible(bool visible) {
    m_closeButton->setVisible(visible);
    updateOverlayGeometry();
}

void TitleBar::setMaximized(bool maximized) {
    if (m_maximized == maximized) {
        return;
    }
    m_maximized = maximized;
    m_maximizeButton->setAccessibleName(maximized ? tr("Restore") : tr("Maximize"));
    m_maximizeButton->setToolTip(maximized ? tr("Restore") : tr("Maximize"));
    updateButtonIcons();
}

bool TitleBar::eventFilter(QObject *object, QEvent *event) {
    if (object != m_menuBar) {
        return QWidget::eventFilter(object, event);
    }

    if (event->type() == QEvent::Show || event->type() == QEvent::Hide
        || event->type() == QEvent::LayoutRequest
        || event->type() == QEvent::ActionAdded
        || event->type() == QEvent::ActionRemoved) {
        updateOverlayGeometry();
    }

    auto *mouseEvent = dynamic_cast<QMouseEvent *>(event);
    if (!mouseEvent || m_menuBar->actionAt(mouseEvent->position().toPoint())) {
        return QWidget::eventFilter(object, event);
    }

    switch (event->type()) {
    case QEvent::MouseButtonPress:
        if (mouseEvent->button() == Qt::LeftButton) {
            emit mousePressed(mouseEvent->globalPosition().toPoint());
            return true;
        }
        break;
    case QEvent::MouseMove:
        if (mouseEvent->buttons() & Qt::LeftButton) {
            emit mouseMoved(mouseEvent->globalPosition().toPoint());
            return true;
        }
        break;
    case QEvent::MouseButtonRelease:
        emit mouseReleased();
        return true;
    case QEvent::MouseButtonDblClick:
        if (mouseEvent->button() == Qt::LeftButton) {
            emit mouseDoubleClicked(mouseEvent->globalPosition().toPoint());
            return true;
        }
        break;
    default:
        break;
    }
    return QWidget::eventFilter(object, event);
}

void TitleBar::changeEvent(QEvent *event) {
    QWidget::changeEvent(event);
    if (event->type() == QEvent::ApplicationPaletteChange) {
        applyStyle();
    }
}

void TitleBar::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        emit mousePressed(event->globalPosition().toPoint());
    }
    QWidget::mousePressEvent(event);
}

void TitleBar::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton) {
        emit mouseMoved(event->globalPosition().toPoint());
    }
    QWidget::mouseMoveEvent(event);
}

void TitleBar::mouseReleaseEvent(QMouseEvent *event) {
    emit mouseReleased();
    QWidget::mouseReleaseEvent(event);
}

void TitleBar::mouseDoubleClickEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        emit mouseDoubleClicked(event->globalPosition().toPoint());
    }
    QWidget::mouseDoubleClickEvent(event);
}

void TitleBar::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    updateOverlayGeometry();
    m_bottomLine->setGeometry(0, height() - 1, width(), 1);
}

void TitleBar::applyStyle() {
    const TitleBarStyle style = StyleManager::instance().titleBarStyle();
    const QPalette applicationPalette = QApplication::palette(this);
    QPalette titlePalette = applicationPalette;
    titlePalette.setColor(QPalette::Window,
                          style.background.isValid() ? style.background
                                                     : applicationPalette.color(QPalette::Window));
    const QColor foreground = style.foreground.isValid()
                                  ? style.foreground
                                  : applicationPalette.color(QPalette::WindowText);
    titlePalette.setColor(QPalette::WindowText, foreground);
    titlePalette.setColor(QPalette::ButtonText, foreground);
    setPalette(titlePalette);
    setAutoFillBackground(true);

    const QColor hover = style.buttonHover.isValid()
                             ? style.buttonHover
                             : applicationPalette.color(QPalette::Midlight);
    const QColor closeHover = style.closeButtonHover.isValid()
                                  ? style.closeButtonHover
                                  : QColor(232, 17, 35, 200);
    m_minimizeButton->setStyleSheet(buttonStyle(hover));
    m_maximizeButton->setStyleSheet(buttonStyle(hover));
    m_closeButton->setStyleSheet(buttonStyle(closeHover));

    const QColor separator = style.separator.isValid()
                                 ? style.separator
                                 : applicationPalette.color(QPalette::Mid);
    m_bottomLine->setStyleSheet(
        QStringLiteral("background-color: %1;").arg(separator.name(QColor::HexArgb)));
    updateButtonIcons();
    update();
}

void TitleBar::updateButtonIcons() {
    const QColor color = palette().color(QPalette::ButtonText);
    const int size = m_minimizeButton->iconSize().width();
    m_minimizeButton->setIcon(minimizeIcon(color, size));
    m_maximizeButton->setIcon(lineIcon(color, size, m_maximized));
    m_closeButton->setIcon(closeIcon(color, size));
}

void TitleBar::updateOverlayGeometry() {
    const QMargins margins = m_layout->contentsMargins();
    const int spacing = m_layout->spacing();

    int leftWidth = margins.left();
    if (!m_menuBar->isHidden()) {
        leftWidth += m_menuBar->sizeHint().width() + spacing;
    }

    int rightWidth = margins.right();
    int visibleButtonCount = 0;
    for (const QPushButton *button : {m_minimizeButton, m_maximizeButton, m_closeButton}) {
        if (!button->isHidden()) {
            rightWidth += button->width();
            ++visibleButtonCount;
        }
    }
    if (visibleButtonCount > 1) {
        rightWidth += (visibleButtonCount - 1) * spacing;
    }

    const int inset = qMax(leftWidth, rightWidth);
    m_titleLabel->setGeometry(inset, 0, qMax(0, width() - 2 * inset), height());
}

} // namespace qt_ui_style
