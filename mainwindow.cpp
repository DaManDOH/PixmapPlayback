#include "mainwindow.hpp"
#include "./ui_mainwindow.h"

#include <QFileDialog>
#include <QThreadPool>

#include <algorithm>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::PixmapPlaybackMainWindow)
{
    ui->setupUi(this);

    auto const onlyScene = new QGraphicsScene{ui->graphicsView};
    ui->graphicsView->setScene(onlyScene);

    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::openTriggered);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openTriggered()
{
    m_inputFolderLoc = QFileDialog::getExistingDirectory(this,
                                                         "Select Input Folder",
                                                         "",
                                                         QFileDialog::ShowDirsOnly
                                                             | QFileDialog::DontUseNativeDialog);
    if (!m_inputFolderLoc.isEmpty()) {
        loadPixmaps();
        renderAllPixmapsWithDelay();
    }
}

void MainWindow::loadPixmaps()
{
    QDir const currDir{m_inputFolderLoc};
    auto const currDirEntries = currDir.entryList(QDir::Files | QDir::NoDotAndDotDot);
    m_pixmaps.clear(); // Release previous pixmaps.
    std::transform(currDirEntries.constBegin(),
                   currDirEntries.constEnd(),
                   std::back_inserter(m_pixmaps),
                   [this, &currDir](QString const &oneLoc) {
                       return QPixmap{currDir.filePath(oneLoc)}
                           .scaled(ui->graphicsView->size(), Qt::AspectRatioMode::KeepAspectRatio);
                   });
}

void MainWindow::renderAllPixmapsWithDelay()
{
    // Move the timing off the UI thread to keep the window from locking up during delays.
    QThreadPool::globalInstance()->start([this]() {
        std::for_each(m_pixmaps.constBegin(), m_pixmaps.constEnd(), [this](auto const &oneBitmap) {
            ui->graphicsView->scene()->clear();
            ui->graphicsView->scene()->addPixmap(oneBitmap);
            qDebug() << (unsigned long long) &oneBitmap;
            QThread::usleep(m_interframeDelay);
        });
    });
}
