#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QTimer>

#include <memory>

QT_BEGIN_NAMESPACE
namespace Ui {
class PixmapPlaybackMainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void openTriggered();
    void doLoopToggled(bool);

private:
    static QString const sm_logMsgPattern;

    Ui::PixmapPlaybackMainWindow *ui;

    // 20 FPS in ms; not compensating for active CPU time during render
    unsigned long m_interframeDelay = 50UL;

    QString m_inputFolderLoc;
    QList<QPixmap> m_pixmaps;
    std::unique_ptr<QTimer> m_renderTimer;
    qsizetype m_currentFrameIdx = -1LL; // Immediately incremented on each call of renderNextPixmap().

    void connectUiActions();
    void clearAndLoadPixmaps();
    void renderNextPixmap();
};
#endif // MAINWINDOW_HPP
