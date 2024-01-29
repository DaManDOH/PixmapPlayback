#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>

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

private:
    static QString const sm_logMsgPattern;

    Ui::PixmapPlaybackMainWindow *ui;

    // // 20 FPS in µs; not compensating for active CPU time during render
    // unsigned long m_interframeDelay = 50000UL;
    // 0.5 FPS in µs
    unsigned long m_interframeDelay = 2000000UL;

    QString m_inputFolderLoc;
    QList<QPixmap> m_pixmaps;

    void clearAndLoadPixmaps();
    void renderAllPixmapsWithDelay();
};
#endif // MAINWINDOW_HPP
