#include <QtUiStyle/BaseFramelessDialog.h>
#include <QtUiStyle/BaseFramelessWindow.h>
#include <QtUiStyle/StyleManager.h>
#include <QtUiStyle/StyledMessageBox.h>
#include <QtUiStyle/TitleBar.h>

#include <QApplication>
#include <QLabel>
#include <QPushButton>
#include <QSignalSpy>
#include <QStatusBar>
#include <QTest>
#include <QTimer>
#include <QVBoxLayout>

using namespace qt_ui_style;

class QtUiStyleTest : public QObject {
    Q_OBJECT

  private slots:
    void init() { StyleManager::instance().resetTitleBarStyle(); }

    void windowTitleStaysSynchronized() {
        BaseFramelessWindow window;
        window.setWindowTitle(QStringLiteral("Synchronized title"));
        auto *label = window.titleBar()->findChild<QLabel *>(
            QStringLiteral("qtUiStyleTitleLabel"));
        QVERIFY(label);
        QCOMPARE(label->text(), QStringLiteral("Synchronized title"));
    }

    void contentLayoutAcceptsWidgets() {
        BaseFramelessDialog dialog;
        auto *label = new QLabel(QStringLiteral("content"), &dialog);
        dialog.contentLayout()->addWidget(label);
        QCOMPARE(dialog.contentLayout()->indexOf(label), 0);
    }

    void dialogTitleHasUsableGeometryWithHiddenControls() {
        BaseFramelessDialog dialog;
        dialog.setWindowTitle(QStringLiteral("Dialog title"));
        dialog.resize(400, 200);
        dialog.show();
        QApplication::processEvents();

        auto *label = dialog.titleBar()->findChild<QLabel *>(
            QStringLiteral("qtUiStyleTitleLabel"));
        QVERIFY(label);
        QCOMPARE(label->text(), QStringLiteral("Dialog title"));
        QVERIFY(label->width() > 0);

        auto *closeButton = dialog.titleBar()->findChild<QPushButton *>(
            QStringLiteral("qtUiStyleCloseButton"));
        QVERIFY(closeButton);
        QVERIFY(label->geometry().right() < closeButton->geometry().left());
    }

    void resizeCursorWorksOverStatusBar() {
        BaseFramelessWindow window;
        window.resize(500, 300);
        QStatusBar *statusBar = window.statusBar();
        statusBar->setSizeGripEnabled(false);
        window.show();
        QApplication::processEvents();

        QTest::mouseMove(statusBar, QPoint(statusBar->width() / 2,
                                           statusBar->height() - 1));
        QCOMPARE(statusBar->cursor().shape(), Qt::SizeVerCursor);
    }

    void sharedStyleUpdatesExistingTitleBars() {
        BaseFramelessWindow window;
        TitleBarStyle style;
        style.background = QColor(QStringLiteral("#112233"));
        style.foreground = QColor(QStringLiteral("#ddeeff"));

        QSignalSpy spy(&StyleManager::instance(), &StyleManager::titleBarStyleChanged);
        StyleManager::instance().setTitleBarStyle(style);

        QCOMPARE(spy.count(), 1);
        QCOMPARE(window.titleBar()->palette().color(QPalette::Window), style.background);
        QCOMPARE(window.titleBar()->palette().color(QPalette::ButtonText), style.foreground);
    }

    void dialogCloseButtonRejects() {
        BaseFramelessDialog dialog;
        QSignalSpy rejectedSpy(&dialog, &QDialog::rejected);
        auto *closeButton = dialog.titleBar()->findChild<QPushButton *>(
            QStringLiteral("qtUiStyleCloseButton"));
        QVERIFY(closeButton);
        closeButton->click();
        QCOMPARE(rejectedSpy.count(), 1);
    }

    void closedQuestionReturnsCancel() {
        QTimer::singleShot(0, []() {
            if (auto *dialog = qobject_cast<QDialog *>(QApplication::activeModalWidget())) {
                dialog->reject();
            }
        });
        QCOMPARE(StyledMessageBox::question(nullptr, QStringLiteral("Question"),
                                            QStringLiteral("Continue?")),
                 StyledMessageBox::Cancel);
    }
};

QTEST_MAIN(QtUiStyleTest)
#include "test_qt_ui_style.moc"
