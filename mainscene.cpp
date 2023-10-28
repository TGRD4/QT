#include "mainscene.h"
#include "ui_mainscene.h"
#include <QPaintEvent>
#include<QPainter>
#include<QPixmap>
#include"mypushbutton.h"
#include<QDebug>
#include<QTimer>

// QSoundEffect 适用于提示音，未压缩的音频文件
#include<QSoundEffect>

 // QMediaPlayer 一般用于做音视频播放器
#include<QMediaPlayer>


MainScene::MainScene(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainScene)
{
    ui->setupUi(this);

    // 配置主场景

    // 设置固定大小
    setFixedSize(320,588);

    // 设置图标
    setWindowIcon(QIcon(":/res/Coin0001.png"));

    // 设置标题
    setWindowTitle("翻金币主场景");

    // 退出按钮实现（一打开程序就开始监听Back，不用每次切换场景时都要connect）
    connect(ui->actionquit,&QAction::triggered,[=]() {
        this->close();
    });

    // 准备开始按钮的音效
    QSoundEffect * player = new QSoundEffect(this);
    player->setSource(QUrl::fromLocalFile(":/res/TapButtonSound.wav"));
    player->setLoopCount(1);    // 循环一次，-1代表无限循环
    player->setVolume(1);            // 声音 0~1


    // 开始按钮（使用自定义按钮）
    MyPushButton * startBtn = new MyPushButton(":/res/MenuSceneStartButton.png");
    startBtn->setParent(this);
    startBtn->move(this->width() * 0.5 - startBtn->width() * 0.5,this->height()*0.7);

    // 实例化选择关卡的场景
    chooseScene = new ChooseLevelScene;

        // 监听Back信号
        connect(chooseScene,&ChooseLevelScene::chooseSceneBack,this,[=]() {
            this->setGeometry(chooseScene->geometry());
            chooseScene->hide();    // 将选择关卡场景隐藏掉
            this->show();   // 重新显示主场景
        });

    // 监听开始按钮，添加上下跳跃
    connect(startBtn,&MyPushButton::clicked,[=]() {
        //qDebug() << "点击了开始";

        player->play();	// 开始播放

        // 做弹起特效
        startBtn->zoom1();
        startBtn->zoom2();

        // 延时进入选择关卡场景（展示完特效再进入）
        QTimer::singleShot(500,this,[=]()  {    // 延迟0.5s后进入匿名函数
            // 设置chooseScene场景的位置
            chooseScene->setGeometry(this->geometry());

            // 自身隐藏
            this->hide();
            // 进入（显示）选择关卡的场景
            chooseScene->show();

        });
    });

}

void MainScene::paintEvent(QPaintEvent * ) {

    // 画背景图
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/res/PlayLevelSceneBg.png");     // 加载图片
    painter.drawPixmap(0,0,this->width(),this->height(),pix);   // 指定拉伸为屏幕的宽度高度

    // 画背景上的图标
    pix.load(":/res/Title.png");
    pix = pix.scaled(pix.width()*0.5,pix.height()*0.5);     // 缩放图标
    painter.drawPixmap(10,30,pix);


}

MainScene::~MainScene()
{
    delete ui;
}

