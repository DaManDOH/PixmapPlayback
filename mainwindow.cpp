#include "mainwindow.hpp"
#include "./ui_mainwindow.h"

#include <QFileDialog>
#include <QThreadPool>

#include <algorithm>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    auto const onlyScene = new QGraphicsScene{this};
    ui->graphicsView->setScene(onlyScene);
    // auto viewRect = ui->graphicsView->rect();
    // auto viewParentRect = ui->graphicsView->parentWidget()->rect();
    // onlyScene->setSceneRect(viewRect);
    // ui->graphicsView->fitInView(viewParentRect);

    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::openTriggered);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openTriggered()
{
    m_inputFolder = QFileDialog::getExistingDirectory(this,
                                                      "Select Input Folder",
                                                      "",
                                                      QFileDialog::ShowDirsOnly
                                                          | QFileDialog::DontUseNativeDialog);
    if (!m_inputFolder.isEmpty()) {
        clearAndLoadPixmaps();
        renderAllPixmapsWithDelay();
    }
}

void MainWindow::clearAndLoadPixmaps()
{
    QDir const currDir{m_inputFolder};
    auto const currDirEntries = currDir.entryList(QDir::Files | QDir::NoDotAndDotDot);
    m_pixmaps.clear();
    std::transform(currDirEntries.constBegin(),
                   currDirEntries.constEnd(),
                   std::back_inserter(m_pixmaps),
                   [&currDir](QString const &oneLoc) {
                       // return QPixmap{currDir.filePath(oneLoc)}
                       //     .scaled(QSize{50, 50}, Qt::AspectRatioMode::KeepAspectRatio);
                       return QPixmap{currDir.filePath(oneLoc)};
                   });
}

void MainWindow::renderAllPixmapsWithDelay()
{
    auto const aThingToDo = [this]() {
        std::for_each(m_pixmaps.constBegin(), m_pixmaps.constEnd(), [this](auto const &oneBitmap) {
            ui->graphicsView->scene()->clear();
            ui->graphicsView->scene()->addPixmap(oneBitmap);
            // // QThread::currentThread()->usleep(40);
            // QThread::sleep(1);
        });
    };
    QThreadPool::globalInstance()->start(aThingToDo);
}
