#ifndef MYPUSHBUTTON_H
#define MYPUSHBUTTON_H

#include <QPushButton>

class MyPushButton : public QPushButton
{
    Q_OBJECT
public:
    // explicit MyPushButton(QWidget *parent = nullptr);
    // 构造函数（参数一：正常显示的图片路径，二：按下后显示的图片路径）
    MyPushButton(QString normalImg,QString pressImg = "");

    // 成员属性     保存用户传入的默认显示路径 及 按下后显示的路径
    QString normalImgPath;
    QString pressImgPath;

    // 开始按钮弹跳效果
    void zoom1();
    void zoom2();

    // 重写按钮 按下和释放 事件
    void mousePressEvent(QMouseEvent * e);
    void mouseReleaseEvent(QMouseEvent * e);

signals:

};

#endif // MYPUSHBUTTON_H
