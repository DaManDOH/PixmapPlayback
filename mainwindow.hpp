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
    Ui::PixmapPlaybackMainWindow *ui;

    // 20 FPS in ms; not compensating for active CPU time during render
    unsigned long m_interframeDelay = 50UL;

    QString m_inputFolderLoc;
    QList<QPixmap> m_pixmaps;

    void loadPixmaps();
    void renderAllPixmapsWithDelay();
};
#endif // MAINWINDOW_HPP
