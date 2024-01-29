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
    double m_interframeDelay = 1.0 / 20.0; // 20 FPS
    QString m_inputFolderLoc;
    QList<QPixmap> m_pixmaps;

    void clearAndLoadPixmaps();
    void renderAllPixmapsWithDelay();
};
#endif // MAINWINDOW_HPP
