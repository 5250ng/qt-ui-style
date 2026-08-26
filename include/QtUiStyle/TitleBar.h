#pragma once

#include <QWidget>

class QEvent;
class QFrame;
class QHBoxLayout;
class QLabel;
class QMenuBar;
class QMouseEvent;
class QPushButton;
class QResizeEvent;

namespace qt_ui_style {

class TitleBar : public QWidget {
    Q_OBJECT

  public:
    explicit TitleBar(QWidget *parent = nullptr);
    ~TitleBar() override = default;

    QMenuBar *menuBar() const;
    void setTitle(const QString &title);
    void setMinMaxVisible(bool visible);
    void setMinimizeVisible(bool visible);
    void setMaximizeVisible(bool visible);
    void setCloseVisible(bool visible);
    void setMaximized(bool maximized);

  signals:
    void minimizeRequested();
    void maximizeRestoreRequested();
    void closeRequested();
    void mousePressed(const QPoint &globalPosition);
    void mouseMoved(const QPoint &globalPosition);
    void mouseReleased();
    void mouseDoubleClicked(const QPoint &globalPosition);

  protected:
    bool eventFilter(QObject *object, QEvent *event) override;
    void changeEvent(QEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

  private slots:
    void applyStyle();

  private:
    void updateButtonIcons();
    void updateOverlayGeometry();

    QMenuBar *m_menuBar;
    QLabel *m_titleLabel;
    QFrame *m_bottomLine;
    QPushButton *m_minimizeButton;
    QPushButton *m_maximizeButton;
    QPushButton *m_closeButton;
    QHBoxLayout *m_layout;
    bool m_maximized = false;
};

} // namespace qt_ui_style
