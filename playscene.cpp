#include "playscene.h"
#include<QDebug>
#include<QMenu>
#include<QMenuBar>
#include<QAction>
#include<QPainter>
#include<QPixmap>
#include"mypushbutton.h"
#include<QLabel>
#include"mycoin.h"
#include"dataconfig.h"
#include<QDebug>
#include<QPropertyAnimation>
#include<QSoundEffect>

//PlayScene::PlayScene(QWidget *parent)
//    : QMainWindow{parent}
//{

//}

PlayScene::PlayScene(int levelNum) {
    this->levelIndex = levelNum;
    qDebug() << "进入了第" << this->levelIndex << "关";

    // 初始化游戏场景
    // 设置固定大小
    setFixedSize(320,588);

    // 设置图标
    setWindowIcon(QIcon(":/res/Coin0001.png"));

    // 设置标题
    setWindowTitle("翻金币场景");

    // 菜单栏创建
    QMenuBar * bar = menuBar();
    setMenuBar(bar);

    // 创建开始菜单
    QMenu * startMenu =  bar->addMenu("开始");

    // 创建退出菜单项
    QAction * quitAction = startMenu->addAction("退出");

    // 点击退出
    connect(quitAction,&QAction::triggered,[=]() {
        this->close();
    });

//    // 返回按钮音效
//    QSoundEffect * backPlayer = new QSoundEffect(this);
//    backPlayer->setSource(QUrl::fromLocalFile(":/res/BackButtonSound.wav"));
//    backPlayer->setLoopCount(1);    // 循环一次
//    backPlayer->setVolume(1);            // 声音 0~1


    // 翻金币音效
    QSoundEffect * flipPlayer = new QSoundEffect(this);
    flipPlayer->setSource(QUrl::fromLocalFile(":/res/ConFlipSound.wav"));
    flipPlayer->setLoopCount(1);    // 循环一次
    flipPlayer->setVolume(1);            // 声音 0~1

    // 胜利音效
    QSoundEffect * winPlayer = new QSoundEffect(this);
    winPlayer->setSource(QUrl::fromLocalFile(":/res/LevelWinSound.wav"));
    winPlayer->setLoopCount(1);    // 循环一次
    winPlayer->setVolume(1);            // 声音 0~1

    // 返回按钮
    MyPushButton * backBtn = new MyPushButton(":/res/BackButton.png",":/res/BackButtonSelected.png");
    backBtn->setParent(this);
    backBtn->move(this->width()-backBtn->width(),this->height()-backBtn->height());

    // 点击Back
    connect(backBtn,&MyPushButton::clicked, [=]() {

        // 显示点击效果并返回上一个场景
        // 延迟返回
        //QTimer::singleShot(200,this,[=]() {
        //backPlayer->play();
        emit this->playSceneBack();   // 发送自定义信号
        //});
    });

    // 显示当前关卡数
    QLabel *label = new QLabel;
    label->setParent(this);

    QFont font;
    font.setFamily("华文新魏");
    font.setPixelSize(30);

    label->setFont(font);   // 别忘了让标签使用设置好的字体

    label->setGeometry(30,this->height()-60,120,50);
    QString str1 = QString("Level:%1").arg(this->levelIndex);
    label->setText(str1);


    dataConfig config;
    // 初始化每个关卡的二维数组
     for(int i = 0 ; i < 4;i++) {
         for(int j = 0 ; j < 4;j++)  {
            this->gameArray[i][j] = config.mData[this->levelIndex][i][j];
         }
     }

     // 隐藏胜利图片在上方
     QLabel * winLabel = new QLabel;
     QPixmap tmpPix;
     tmpPix.load(":/res/LevelCompletedDialogBg.png");
     winLabel->setGeometry(0,0,tmpPix.width(),tmpPix.height());
     winLabel->setPixmap(tmpPix);
     winLabel->setParent(this);
     winLabel->move((this->width()-tmpPix.width())*0.5 , -tmpPix.height());


    // 显示金币图片背景
    for(int i=0;i<4;i++) {
        for(int j=0;j<4;j++) {
            // 绘制背景图片
            QPixmap pix = QPixmap(":/res/BoardNode.png");
            QLabel * label = new QLabel;
            label->setParent(this);
            label->setGeometry(0,0,pix.width(),pix.height());
            label->setPixmap(pix);
            label->move(57+i*50,200+j*50);

            // 创建金币
            QString str;
            if(this->gameArray[i][j]==1) {
                // 金币
                str = ":/res/Coin0001.png";
            }
            else {
                // 硬币
                str = ":/res/Coin0008.png";
            }
            MyCoin * coin = new MyCoin(str);
            coin->setParent(this);
            coin->move(59+i*50,204+j*50);

            // 给金币的属性赋值
            coin->posX = i;
            coin->poxY = j;
            coin->flag = this->gameArray[i][j]; // 1正面  0反面

            // 将金币放入到金币二维数组里，便于后期维护
            coinBtn[i][j] = coin;

            // 点击金币，进行翻转
            connect(coin,&MyCoin::clicked,[=]() {
                flipPlayer->play();

                // 将所有按钮都给禁用，防止速度太快同时翻转两个导致游戏胜利后还能翻转的情况
                for(int i=0;i<4;i++) {
                    for(int j =0;j<4;j++) {
                        this->coinBtn[i][j]->isWin = true;
                    }
                }

                coin->changeFlag();
                // 改变硬币的状态，金币变银币，银币变金币
                this->gameArray[i][j] = this->gameArray[i][j] == 0 ? 1:0;

                // 同时翻转周围的硬币
                if(coin->posX + 1 <= 3) {       // 周围的右侧金币翻转条件
                    coinBtn[coin->posX+1][coin->poxY]->changeFlag();
                    this->gameArray[coin->posX+1][coin->poxY] = this->gameArray[coin->posX+1][coin->poxY] == 0 ? 1:0;
                }

                // 左侧
                if(coin->posX - 1 >=0) {
                    coinBtn[coin->posX-1][coin->poxY]->changeFlag();
                    this->gameArray[coin->posX-1][coin->poxY] = this->gameArray[coin->posX-1][coin->poxY] == 0 ? 1:0;
                }

                // 上面
                if(coin->poxY - 1 >=0) {
                    coinBtn[coin->posX][coin->poxY-1]->changeFlag();
                    this->gameArray[coin->posX][coin->poxY-1] = this->gameArray[coin->posX][coin->poxY-1] == 0 ? 1:0;
                }

                // 下面
                if(coin->poxY + 1 <=3) {       // 周围的右侧金币翻转条件
                    coinBtn[coin->posX][coin->poxY+1]->changeFlag();
                    this->gameArray[coin->posX][coin->poxY+1] = this->gameArray[coin->posX][coin->poxY+1] == 0 ? 1:0;
                }

                // 翻转完所有金币后，将按钮解禁
                for(int i=0;i<4;i++) {
                    for(int j =0;j<4;j++) {
                        this->coinBtn[i][j]->isWin = false;
                    }
                }

                // 判断是否胜利
                this->isWin = true;
                for(int i=0;i<4;i++) {
                    for(int j=0;j<4;j++) {
                        if(coinBtn[i][j]->flag == false) {  // 存在一个反面即为失败
                            this->isWin = false;
                            break;
                        }
                    }
                }

                if(this->isWin==true) {
                    qDebug() << "游戏胜利";

                    // 将所有按钮胜利标志改为true
                    for(int i=0;i<4;i++) {
                        for(int j=0;j<4;j++) {
                            coinBtn[i][j]->isWin = true;
                        }
                    }

                   // 将胜利的图片移动下来
                    QPropertyAnimation * animation = new QPropertyAnimation(winLabel,"geometry");
                    // 设置时间间隔
                    animation->setDuration(1000);
                    // 设置开始和结束位置
                    animation->setStartValue(QRect(winLabel->x(),winLabel->y(),winLabel->width(),winLabel->height()));
                    animation->setEndValue(QRect(winLabel->x(),winLabel->y()+130,winLabel->width(),winLabel->height()));
                    // 设置缓和曲线
                    animation->setEasingCurve(QEasingCurve::OutBounce);

                    // 执行动画
                    animation->start();
                    winPlayer->play();
                }

            });
        }
    }

}

void PlayScene::paintEvent(QPaintEvent *) {

    // 画背景图
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/res/PlayLevelSceneBg.png");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);


    // 画背景上的图标
    pix.load(":/res/Title.png");
    pix = pix.scaled(pix.width()*0.5,pix.height()*0.5);     // 缩放图标
    painter.drawPixmap(10,30,pix.width(),pix.height(),pix);


}
