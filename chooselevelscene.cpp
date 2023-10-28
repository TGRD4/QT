#include "chooselevelscene.h"
#include<QMenuBar>
#include<QPainter>
#include<QPixmap>
#include"mypushbutton.h"
#include<QTimer>
#include<QLabel>
#include<QDebug>
#include"playscene.h"
#include<QSoundEffect>

ChooseLevelScene::ChooseLevelScene(QWidget *parent)
    : QMainWindow{parent}
{
    // 配置选择关卡场景
    this->setFixedSize(320,588);

    // 设置图标
    this->setWindowIcon(QPixmap(":/res/Coin0001.png"));

    // 设置标题
    this->setWindowTitle("选择关卡场景");

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

    // 返回按钮音效
    QSoundEffect * backPlayer = new QSoundEffect(this);
    backPlayer->setSource(QUrl::fromLocalFile(":/res/BackButtonSound.wav"));
    backPlayer->setLoopCount(1);    // 循环一次
    backPlayer->setVolume(1);            // 声音 0~1


    // 返回按钮
    MyPushButton * backBtn = new MyPushButton(":/res/BackButton.png",":/res/BackButtonSelected.png");
    backBtn->setParent(this);
    backBtn->move(this->width()-backBtn->width(),this->height()-backBtn->height());

    // 点击Back
    connect(backBtn,&MyPushButton::clicked,[=]() {
        // 显示点击效果并返回上一个场景
        // 延迟返回
        //QTimer::singleShot(200,this,[=]() {

            // 点下back播放音效 并 发送返回主菜单页面的信号
            backPlayer->play();
            emit this->chooseSceneBack();   // 发送自定义信号
        //});
    });

    //  选择关卡按钮音效
    QSoundEffect * choosePlayer = new QSoundEffect(this);
    choosePlayer->setSource(QUrl::fromLocalFile(":/res/TapButtonSound.wav"));
    choosePlayer->setLoopCount(1);    // 循环一次
    choosePlayer->setVolume(1);            // 声音 0~1


    // 创建选择关卡的按钮
    for(int i =0 ; i<20;i++ ) {
        MyPushButton * menuBtn = new MyPushButton(":/res/LevelIcon.png");
        menuBtn->setParent(this);
        menuBtn->move(25+i%4 * 70 ,140+i/4 *70);

        // 监听每一个按钮的点击事件
        connect(menuBtn,&MyPushButton::clicked,[=] () {

            choosePlayer->play();

            QString str = QString("你选择的是第 %1 关").arg(i+1);
            qDebug() << str;

            // 隐藏选关
            this->hide();

            //  为第三个界面留接口，进入游戏场景
            play = new PlayScene(i+1);  // 创建游戏场景

             // 设置游戏场景的初始位置
            play->setGeometry(this->geometry());
            // 解决挪动界面 切换场景后又自动归位的情况

            play->show();                           // 显示游戏场景

            // 监听Back信号
            connect(play,&PlayScene::playSceneBack,this,[=]() {

                this->setGeometry(play->geometry());
                delete play;    // 将翻金币关卡直接删除
                play = NULL;

                // 监听到来自翻金币关卡的信号后播放back音效 并 重新展示选择关卡场景
                backPlayer->play();

                this->show();   // 重新显示选择关卡场景
            });


        });

        // 相当于用一个Label覆盖20个按钮上，在相同位置上设置数字
        QLabel * label = new QLabel;
        label->setParent(this);
        label->setFixedSize(menuBtn->width(),menuBtn->height());
        label->setText(QString::number(i+1));
        label->move(25+i%4 * 70 ,140+i/4 *70);

        // 设置label上文字的对齐方式      水平和垂直居中
        label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

        // 设置让鼠标进行穿透，不然Label层会挡住鼠标事件    51号
        label->setAttribute(Qt::WA_TransparentForMouseEvents);

    }
}


// 这里加载展示的基本都是静态图片
void ChooseLevelScene::paintEvent(QPaintEvent *) {

    // 画背景图
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/res/OtherSceneBg.png");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);


    // 画背景上的图标
    pix.load(":/res/Title.png");
    painter.drawPixmap((this->width() - pix.width())*0.5,30,pix.width(),pix.height(),pix);


}
