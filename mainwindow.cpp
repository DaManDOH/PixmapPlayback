#include "mainwindow.hpp"
#include "./ui_mainwindow.h"

#include <QFileDialog>
#include <QtDebug>
#include <QtLogging>

#include <algorithm>

QString const MainWindow::sm_logMsgPattern = R"(%{time yyyy-MM-ddTHH:mm:ss.zzztt} - %{message})";

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::PixmapPlaybackMainWindow)
{
    qSetMessagePattern(sm_logMsgPattern);

    ui->setupUi(this);

    auto const onlyScene = new QGraphicsScene{this};
    onlyScene->setBackgroundBrush(QBrush{Qt::black});
    ui->graphicsView->setScene(onlyScene);

    m_renderTimer = std::make_unique<QTimer>(this);
    m_renderTimer->setTimerType(Qt::PreciseTimer);

    connectUiActions();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::connectUiActions()
{
    connect(m_renderTimer.get(), &QTimer::timeout, this, &MainWindow::renderNextPixmap);
    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::openTriggered);
    connect(ui->actionDoLoop, &QAction::toggled, this, &MainWindow::doLoopToggled);
}

void MainWindow::openTriggered()
{
    m_inputFolderLoc = QFileDialog::getExistingDirectory(this,
                                                         "Select Input Folder",
                                                         "",
                                                         QFileDialog::ShowDirsOnly
                                                             | QFileDialog::DontUseNativeDialog);
    if (!m_inputFolderLoc.isEmpty()) {
        // Don't want the timer reading from the m_pixmaps collection while it's mutating.
        m_renderTimer->stop();

        clearAndLoadPixmaps();

        m_renderTimer->start(m_interframeDelay);
    }
}

void MainWindow::clearAndLoadPixmaps()
{
    QDir const currDir{m_inputFolderLoc};
    auto const currDirEntries = currDir.entryList(QDir::Files | QDir::NoDotAndDotDot);

    // Member is immediately incremented on each call of renderNextPixmap().
    m_currentFrameIdx = -1LL;

    m_pixmaps.clear();
    std::transform(currDirEntries.constBegin(),
                   currDirEntries.constEnd(),
                   std::back_inserter(m_pixmaps),
                   [this, &currDir](QString const &oneLoc) {
                       return QPixmap{currDir.filePath(oneLoc)}
                           .scaled(ui->graphicsView->size(), Qt::AspectRatioMode::KeepAspectRatio);
                   });
}

void MainWindow::renderNextPixmap()
{
    auto const pixmapsSize = m_pixmaps.size();
    ++m_currentFrameIdx;

    if (m_currentFrameIdx >= pixmapsSize) {
        if (ui->actionDoLoop->isChecked()) {
            // Index exceeds last available in m_pixmaps. Reset frame index to zero.
            m_currentFrameIdx = 0;
        } else {
            // Index exceeds last available in m_pixmaps. Stop timer & exit function.
            m_renderTimer->stop();
            return;
        }
    }

    ui->graphicsView->scene()->clear();
    ui->graphicsView->scene()->addPixmap(m_pixmaps[m_currentFrameIdx]);
}

void MainWindow::doLoopToggled(bool isChecked)
{
    if (isChecked && !m_renderTimer->isActive()) {
        m_renderTimer->start(m_interframeDelay);
    }
}
