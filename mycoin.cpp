#include "mycoin.h"
#include<QPixmap>
#include<QDebug>
#include<QTimer>

//MyCoin::MyCoin(QWidget *parent)
//    : QPushButton{parent}
//{

//}

MyCoin::MyCoin(QString btnImg) {
    QPixmap pix;
    bool ret = pix.load(btnImg);
    if(!ret) {
        QString str = QString("图片 %1 加载失败").arg(btnImg);
        qDebug() << str;
        return;
    }

    this->setFixedSize(pix.width(),pix.height());
    this->setStyleSheet("QPushButton{border:0px}");
    this->setIcon(pix);
    this->setIconSize(QSize(pix.width(),pix.height()));

    // 初始化定时器
    timer1= new QTimer(this);
    timer2= new QTimer(this);

    // 监听正面翻转的信号槽   并且翻转金币
    connect(timer1,&QTimer::timeout,[=]() {
        QPixmap pix;
        QString str = QString(":/res/Coin000%1").arg(this->min++);
        pix.load(str);

        this->setFixedSize(pix.width(),pix.height());
        this->setStyleSheet("QPushButton{border:0px}");
        this->setIcon(pix);
        this->setIconSize(QSize(pix.width(),pix.height()));

        // 加到8，翻转完毕，min重置为1
        if(this->min>8) {
            this->min = 1;
            isAnimation = false;     // 动画结束了
            timer1->stop();
        }
    });

    // 监听反面翻转的信号槽   并且翻转金币
    connect(timer2,&QTimer::timeout,[=]() {
        QPixmap pix;
        QString str = QString(":/res/Coin000%1").arg(this->max--);
        pix.load(str);

        this->setFixedSize(pix.width(),pix.height());
        this->setStyleSheet("QPushButton{border:0px}");
        this->setIcon(pix);
        this->setIconSize(QSize(pix.width(),pix.height()));

        // 加到8，翻转完毕，min重置为1
        if(this->max<1) {
            this->max = 8;;
            isAnimation = false;     // 动画结束了
            timer2->stop();
        }
    });

}

// 实现   在狂点的时候也是等一个动画播放完毕后才进行下一个
// 同时实现   在所有按钮都设置为胜利后无法再接受鼠标点击事件，无法再反转金币
void MyCoin::mousePressEvent(QMouseEvent * e) {
    if(this->isAnimation || this->isWin == true) {
        return;
    }
    else {
        QPushButton::mousePressEvent(e);
    }
}

void MyCoin::changeFlag() {
    // 如果是正面，翻成反面
    if(this->flag) {
        timer1->start(30);  // 开启正面翻反面的定时器
        isAnimation = true; // 动画开始了
        this->flag = false;
    }
    else {
        timer2->start(30);
        isAnimation = true;
        this->flag = true;
    }
}
