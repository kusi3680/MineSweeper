#include "SweeperWidget.h"

#include <QMenuBar>
#include <QMenu>
#include <QAction>

#include <QPixmap>

#include <QTimer>

#include <QVBoxLayout>
#include <QHBoxLayout>

#include <QMessageBox>
#include "ScoreSaver.h"

#include "SweeperFieldWidget.h"

SweeperWidget::SweeperWidget(Sweeper * sweeper, QWidget *parent) :
    QMainWindow(parent),
    sweeper(sweeper),
    timeSeconds(0)
{
    {
        QMenu *gameMenu = menuBar()->addMenu("Game");
        {
            QAction *restartAction = new QAction("Restart", this);
            gameMenu->addAction(restartAction);
            connect(restartAction, SIGNAL(triggered()),
                    sweeper, SLOT(restartSlot()));
            connect(restartAction, SIGNAL(triggered()),
                    SLOT(restartSlot()));
        }
    }
    {
        QWidget *centralWidget = new QWidget;
        setCentralWidget(centralWidget);
        QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
        {
            QHBoxLayout *heapLayout = new QHBoxLayout;
            mainLayout->addLayout(heapLayout);
            {
                flagsEstimationLcd = new QLCDNumber;
                heapLayout->addWidget(flagsEstimationLcd);
                flagsEstimationLcd->setFixedHeight(50);
            }
            {
                smileLabel = new QLabel;
                QPixmap picture(":/icons/icons/smile-template.png");
                smileLabel->setPixmap(picture);
                smileLabel->setFixedSize(QSize(picture.width(),
                                               picture.height()));
                heapLayout->addWidget(smileLabel);
            }
            {
                timerLcd = new QLCDNumber;
                heapLayout->addWidget(timerLcd);
                timerLcd->setFixedHeight(50);
            }
        }
        mainLayout->addWidget(new SweeperFieldWidget(sweeper));
    }
    connect(sweeper, SIGNAL(bombed()), SLOT(bombed()));
    connect(sweeper, SIGNAL(win()), SLOT(win()));

    QTimer *timer = new QTimer(this);
    timer->start(100);
    connect(timer, SIGNAL(timeout()), SLOT(update()));

    setWindowIcon(QIcon(":/icons/icons/boom.png"));
}

void SweeperWidget::bombed()
{
    QPixmap picture(":/icons/icons/smile-sad.png");
    smileLabel->setPixmap(picture);
    smileLabel->setFixedSize(QSize(picture.width(),
                                   picture.height()));

    QMessageBox::critical(0, "You lose", "You lose");
}

void SweeperWidget::win()
{
    QPixmap picture(":/icons/icons/smile-happy.png");
    smileLabel->setPixmap(picture);
    smileLabel->setFixedSize(QSize(picture.width(),
                                   picture.height()));

    QMessageBox::about(0, "You win!", "You win!");
    ScoreSaver table("Kunal Sinha 3010-001", "MineSweeper", 5, false);
    table.setResult(sweeper->getTimeSeconds());

    QMessageBox::about(0, "You win!", "You win!");
}

void SweeperWidget::update()
{
    flagsEstimationLcd->display(sweeper->getEstimatedFlags());
    timerLcd->display(sweeper->getTimeSeconds());
}

void SweeperWidget::restartSlot()
{
    QPixmap picture(":/icons/icons/smile-template.png");
    smileLabel->setPixmap(picture);
    smileLabel->setFixedSize(QSize(picture.width(),
                                   picture.height()));
}
