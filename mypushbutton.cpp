#include "mypushbutton.h"
#include<QPixmap>
#include<QDebug>
#include<QPropertyAnimation>

//MyPushButton::MyPushButton(QWidget *parent)
//    : QPushButton{parent}
//{

//}

// 声明和实现中只能有一个有默认参数
MyPushButton::MyPushButton(QString normalImg,QString pressImg ) {
    // 设定传入的图片路径
    this->normalImgPath = normalImg;
    this->pressImgPath = pressImg;

    QPixmap pix;
    bool ret = pix.load(normalImg);     // 图片是否加载成功
    if(!ret) {
        qDebug() << "图片加载失败";
        return;
    }

    // 设置图片固定大小（不设置按钮不显示）
    this->setFixedSize(pix.width(),pix.height());

    // 设置不规则图片样式（不设置按钮是矩形）
    this->setStyleSheet("QPushButton{border:0px}");     // 边框是0像素

    // 设置图标及大小（不设置的话不显示图标和大小不是预期的）
    this->setIcon(pix);
    this->setIconSize(QSize(pix.width(),pix.height()));

}

// 向下弹
void MyPushButton::zoom1() {
    // 创建动态对象
    QPropertyAnimation * animation = new QPropertyAnimation(this,"geometry");
    // 设置动画时间间隔
    animation->setDuration(200);

    // 起始位置
    animation->setStartValue(QRect(this->x(),this->y(),this->width(),this->height()));
    // 结束位置
    animation->setEndValue(QRect(this->x(),this->y()+10,this->width(),this->height()));

    // 设置弹跳曲线
    animation->setEasingCurve(QEasingCurve::OutBounce);

    // 开始执行动画
    animation->start();
}

// 向上弹
void MyPushButton::zoom2() {
    // 创建动态对象
    QPropertyAnimation * animation = new QPropertyAnimation(this,"geometry");
    // 设置动画时间间隔
    animation->setDuration(200);

    // 起始位置
    animation->setStartValue(QRect(this->x(),this->y()+10,this->width(),this->height()));
    // 结束位置
    animation->setEndValue(QRect(this->x(),this->y(),this->width(),this->height()));

    // 设置弹跳曲线
    animation->setEasingCurve(QEasingCurve::OutBounce);

    // 开始执行动画
    animation->start();
}

// 鼠标按下
void MyPushButton::mousePressEvent(QMouseEvent * e) {
    if(this->pressImgPath != "") {      // 传入的按下图片不为空，说明按下需要切换图片

        QPixmap pix;
        bool ret = pix.load(this->pressImgPath);     // 图片是否加载成功
        if(!ret) {
            qDebug() << "图片加载失败";
            return;
        }

        // 设置图片固定大小（不设置按钮不显示）
        this->setFixedSize(pix.width(),pix.height());

        // 设置不规则图片样式（不设置按钮是矩形）
        this->setStyleSheet("QPushButton{border:0px}");     // 边框是0像素

        // 设置图标及大小（不设置的话不显示图标和大小不是预期的）
        this->setIcon(pix);
        this->setIconSize(QSize(pix.width(),pix.height()));

    }

    // 让父类执行其他的内容
    return QPushButton::mousePressEvent(e);

}

// 鼠标释放
void MyPushButton::mouseReleaseEvent(QMouseEvent * e) {
    if(this->pressImgPath != "") {      // 传入的按下图片不为空，说明按下需要切换图片

        QPixmap pix;
        bool ret = pix.load(this->normalImgPath);     // 图片是否加载成功
        if(!ret) {
            qDebug() << "图片加载失败";
            return;
        }

        // 设置图片固定大小（不设置按钮不显示）
        this->setFixedSize(pix.width(),pix.height());

        // 设置不规则图片样式（不设置按钮是矩形）
        this->setStyleSheet("QPushButton{border:0px}");     // 边框是0像素

        // 设置图标及大小（不设置的话不显示图标和大小不是预期的）
        this->setIcon(pix);
        this->setIconSize(QSize(pix.width(),pix.height()));

    }

    // 让父类执行其他的内容
    return QPushButton::mouseReleaseEvent(e);
}

