#pragma once

#include <QtUiStyle/TitleBar.h>

#include <QDialog>

class QEvent;
class QVBoxLayout;
class QWidget;

namespace qt_ui_style {

class BaseFramelessDialog : public QDialog {
    Q_OBJECT

  public:
    explicit BaseFramelessDialog(QWidget *parent = nullptr);
    ~BaseFramelessDialog() override = default;

    TitleBar *titleBar() const;
    QVBoxLayout *contentLayout() const;
    void setResizable(bool resizable);
    bool isResizable() const;

  protected:
    bool eventFilter(QObject *object, QEvent *event) override;

  private:
    static constexpr int ResizeMargin = 5;

    Qt::Edges edgesAt(const QPoint &position) const;
    Qt::CursorShape cursorForEdges(Qt::Edges edges) const;
    void updateResizeBorder();
    void startTitleBarMove(const QPoint &globalPosition);
    void continueFallbackMove(const QPoint &globalPosition);

    QWidget *m_content;
    TitleBar *m_titleBar;
    QVBoxLayout *m_rootLayout;
    QVBoxLayout *m_contentLayout;
    bool m_resizable = true;
    bool m_fallbackDragging = false;
    QPoint m_fallbackDragOffset;
};

} // namespace qt_ui_style
