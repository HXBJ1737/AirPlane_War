
#pragma once
/**********************************************************/
#define WIDTH 480
#define HIGH 700
#define FRAME 60

/**********************************************************/
typedef struct Bullet
{
    float x;  // 子弹x坐标
    float y;  // 子弹y坐标
    float vx; // 子弹的水平速度
    float vy; // 子弹的竖直速度
    int atk;  // 攻击力
    int flag; // 是否应删除
    struct Bullet *pnext;
} bullet_list;
typedef struct Aircraft
{
    int x; // 飞机x轴坐标
    int y; // 飞机y轴坐标
    int width;
    int height;
    int speed; // 飞机速度
    int hp;    // 血量
    int flag;
} aircraft;

// 向量的结构体
typedef struct Vector
{
    float x, y;
} vector;
