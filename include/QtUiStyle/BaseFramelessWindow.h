#pragma once

#include <QtUiStyle/TitleBar.h>

#include <QMainWindow>
#include <QPointer>

class QEvent;
class QMouseEvent;
class QVBoxLayout;
class QWidget;

namespace qt_ui_style {

class BaseFramelessWindow : public QMainWindow {
    Q_OBJECT

  public:
    explicit BaseFramelessWindow(QWidget *parent = nullptr);
    ~BaseFramelessWindow() override = default;

    TitleBar *titleBar() const;
    QVBoxLayout *contentLayout() const;

  protected:
    bool eventFilter(QObject *object, QEvent *event) override;
    void changeEvent(QEvent *event) override;

  private:
    static constexpr int ResizeMargin = 5;

    Qt::Edges edgesAt(const QPoint &position) const;
    Qt::CursorShape cursorForEdges(Qt::Edges edges) const;
    void updateResizeCursor(QWidget *widget, Qt::Edges edges);
    void clearResizeCursor();
    void updateResizeBorder();
    void startTitleBarMove(const QPoint &globalPosition);
    void continueFallbackMove(const QPoint &globalPosition);

    QWidget *m_central;
    TitleBar *m_titleBar;
    QWidget *m_content;
    QVBoxLayout *m_rootLayout;
    QVBoxLayout *m_contentLayout;
    QPointer<QWidget> m_resizeCursorWidget;
    bool m_fallbackDragging = false;
    QPoint m_fallbackDragOffset;
};

} // namespace qt_ui_style
