#ifndef MYCOIN_H
#define MYCOIN_H

#include <QPushButton>
#include<QTimer>
class MyCoin : public QPushButton
{
    Q_OBJECT
public:
    // explicit MyCoin(QWidget *parent = nullptr);

    // 传入金币还是硬币路径
    MyCoin(QString btnImg);

    // 金币属性
    int posX;
    int poxY;
    bool flag;  // 正反标志

    // 翻转效果标志
    void changeFlag();
    QTimer * timer1;     // 正面翻反面定时器
     QTimer * timer2;   // 反面翻正面定时器
    int min = 1;              // 最小图片
    int max = 8;            // 最大图片

    // 执行动画标志
    bool isAnimation = false;

    //  重写按下和释放
    void mousePressEvent(QMouseEvent * e);

    // 是否胜利
    bool isWin = false;     // 给每个硬币状态赋初值，若全随机成true，那刚开始就会点不动所有硬币

signals:

};

#endif // MYCOIN_H
