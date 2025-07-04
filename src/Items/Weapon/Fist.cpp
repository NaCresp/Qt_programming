#include "Fist.h"

const int FIST_Y_POSITION = -105;

// 构造函数：初始化我们的手动动画系统
Fist::Fist(QObject *parent) : Weapon(parent, ":/Items/Weapon/Fist/fist.png")
{
    // 我们不再需要基类里的QPropertyAnimation，可以将其安全地删除
    // 以防止它在后台引发任何问题。
    if (animation) {
        delete animation;
        animation = nullptr;
    }

    // 这部分初始化保留，作为对象的默认状态
    setPos(-20, FIST_Y_POSITION);
    setScale(0.2);
    setVisible(false);
    isAttacking = false;

    // 初始化计时器
    animationTimer = new QTimer(this);
    elapsedTimer = new QElapsedTimer();

    // 将计时器的 timeout 信号连接到我们的动画更新函数
    // 计时器会以大约每秒60次的频率触发，以产生流畅的动画
    connect(animationTimer, &QTimer::timeout, this, &Fist::updateAnimation);
    animationTimer->setInterval(1000 / 60); // 约 60 FPS
}

// --- 新增代码：实现 Fist 专属的 mountToParent ---
void Fist::mountToParent()
{
    // 首先调用 Mountable 的基础实现
    Mountable::mountToParent();

    // 然后应用 Fist 特有的属性，确保尺寸和位置正确
    setPos(-20, FIST_Y_POSITION);
    setScale(0.2); // 确保尺寸是 0.2
    setZValue(1);

    // 最重要的是，拳头在“装备”状态下应该是不可见的
    // 它只在调用 attack() 时才短暂出现
    setVisible(false);
}


// attack 函数：启动手动动画
void Fist::attack()
{
    // 如果正在攻击，则直接返回
    if (isAttacking) {
        return;
    }

    isAttacking = true;
    setVisible(true);

    // --- 新增代码 ---
    // 每次攻击开始时，重置伤害标记
    hasDealtDamage = false;
    // --- 新增代码结束 ---

    // 记录动画的起止位置
    animationStartPos = pos();
    animationEndPos = animationStartPos + QPointF(-50, 0);

    // 启动两个计时器
    elapsedTimer->start();
    animationTimer->start();
}

// updateAnimation 槽：手动计算并设置每一帧的位置
void Fist::updateAnimation()
{
    // 获取从动画开始到现在所经过的时间
    qint64 elapsed = elapsedTimer->elapsed();

    // 如果经过的时间已经超过或等于我们设定的动画总时长
    if (elapsed >= ANIMATION_DURATION) {
        // 动画结束
        animationTimer->stop(); // 停止帧更新计时器
        setPos(animationEndPos); // 确保拳头在动画的最终位置

        // 短暂延迟后（让玩家能看清攻击动作），重置拳头状态
        QTimer::singleShot(50, this, [this](){
            setVisible(false);
            // 使用我们定义的高度常量来重置位置
            setPos(-20, FIST_Y_POSITION);
            isAttacking = false; // 解除攻击锁
        });

        return;
    }

    // 如果动画还在进行中...
    // 计算当前动画的完成进度 (一个 0.0 到 1.0 之间的小数)
    qreal progress = static_cast<qreal>(elapsed) / ANIMATION_DURATION;

    // 使用线性插值计算当前应该在的位置
    // 公式: 当前位置 = 起点 + (终点 - 起点) * 进度
    QPointF newPos = animationStartPos + (animationEndPos - animationStartPos) * progress;

    // 更新拳头的位置
    setPos(newPos);
}