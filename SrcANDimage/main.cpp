/**********************************************************/
#include <graphics.h>
#include <math.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <Windows.h>
#include "Define.h"
/**********************************************************/
/// 宏定义
#define ENEMY1NUM 3 // 小型飞行器数量
#define ENEMY2NUM 2 // 中型飞行器数量
#define ENEMY3NUM 1 // 大型飞行器数量
/**********************************************************/
/// 函数声明
void dataInit();
void loadImage();
void graphImage();
void getInput(int *xSpeeds, int *ySpeeds);
void listPushBack(bullet_list **pplist, bullet_list *newNode);
bullet_list *creatPlaneBullet(float vx, float vy);
void listChangeXY(bullet_list **pplist);
void listRemoveNode(bullet_list **pplist);
void showBullet();
void information();
void enemy1move();
void enemy2move();
void enemy3move();
void bulletHit(aircraft *tmp);
void crash(aircraft *enemy);
void enemyCrash();
void startGraph();
void endGraph();
void damageImage();
void change(int mode);
void soundMusic(int mode);
/*****************************************************/
/// 全局变量定义
PIMAGE pimg_bk,
    pimg_temp;
PIMAGE pimg_plane[5];
PIMAGE pimg_enemy1[2];
PIMAGE pimg_enemy2[4];
PIMAGE pimg_enemy3[7];
PIMAGE pimg_bullet1, pimg_bullet2;
PIMAGE pimg_endgame;
PIMAGE pimg_repeatgame;
PIMAGE pimg_timi;
aircraft plane, enemy1[ENEMY1NUM], enemy2[ENEMY2NUM], enemy3[ENEMY3NUM];

bullet_list *plane_list = NULL;
int keys[9] = {'A', 'W', 'D', 'S', ' ', '1', '2', '3', '4'};
int directionKeys[9] = {key_left, key_up, key_right, key_down, key_space, key_num1, key_num2, key_num3, key_num4};
double *startTime = (double *)malloc(sizeof(double));
double *curTime = (double *)malloc(sizeof(double));
double *runTime = (double *)malloc(sizeof(double));
int *score = (int *)malloc(sizeof(int));
double *bullet_T = (double *)malloc(sizeof(double));
int *enemy1baseHP = (int *)malloc(sizeof(int));
int *enemy2baseHP = (int *)malloc(sizeof(int));
int *enemy3baseHP = (int *)malloc(sizeof(int));
int *atk = (int *)malloc(sizeof(int));
float *angle = (float *)malloc(sizeof(float));
mouse_msg msg = {0};
MUSIC music1, music2, music3;
/****************************************************************/

int main()
{

    srand(time(NULL));
    initgraph(WIDTH + 200, HIGH, INIT_RENDERMANUAL);
    setrendermode(RENDER_MANUAL);
    setbkcolor(WHITE);
    setcolor(BLUE);
    setfont(20, 0, "楷体");
    loadImage();
    soundMusic(0);
    // putimage(0, 100, pimg_timi);
    soundMusic(1);

    while (1)
    {

        startGraph();

        dataInit();
        const int speed = plane.speed;
        int xSpeeds[4] = {-speed, 0, speed, 0};
        int ySpeeds[4] = {0, -speed, 0, speed};
        // setbkmode(TRANSPARENT);
        *startTime = fclock();
        for (; is_run(); delay_fps(60))
        {

            getInput(xSpeeds, ySpeeds);
            cleardevice();
            graphImage();
            enemy1move();
            enemy2move();
            enemy3move();
            enemyCrash();
            information();
            showBullet();
            // 如果音乐已经播放完毕
            soundMusic(2);
            if (plane.hp < 0)
                break;
        }

        endGraph(); // delay_ms(2000);
    }
    getch();
    soundMusic(4);
    damageImage();
    closegraph();
    return 0;
}
/****************************************************************/

/// @brief 数据初始化
void dataInit()
{
    int i;

    *enemy1baseHP = 100;
    *enemy2baseHP = 300;
    *enemy3baseHP = 1800;
    *atk = 50;
    *angle = 0.52;
    *score = 0;
    *bullet_T = 0.04; // 子弹间隔时间
    plane.x = 199;
    plane.y = 550;
    plane.width = 102;
    plane.height = 126;
    plane.speed = 6;
    plane.hp = 3000;
    plane.flag = 1;

    for (i = 0; i < ENEMY1NUM; i++)
    {
        enemy1[i].x = rand() % 419 + 2;
        enemy1[i].y = 0;
        enemy1[i].width = 57;
        enemy1[i].height = 43;
        enemy1[i].speed = rand() % 5 + 1;
        enemy1[i].hp = *enemy1baseHP;
        enemy1[i].flag = 1;
    }
    for (i = 0; i < ENEMY2NUM; i++)
    {
        enemy2[i].x = rand() % 410 + 2;
        enemy2[i].y = -150;
        enemy2[i].width = 69;
        enemy2[i].height = 99;
        enemy2[i].speed = rand() % 4 + 1;
        enemy2[i].hp = *enemy2baseHP;
        enemy2[i].flag = 1;
    }

    for (i = 0; i < ENEMY3NUM; i++)
    {
        enemy3[i].x = rand() % 300 + 2;
        enemy3[i].y = -300;
        enemy3[i].width = 169;
        enemy3[i].height = 258;
        enemy3[i].speed = 1;
        enemy3[i].hp = *enemy3baseHP;
        enemy3[i].flag = 1;
    }
}
/// @brief 载入图片素材
void loadImage()
{
    int i;
    pimg_bk = newimage(480, 700);
    // pimg_temp = newimage(480, 700);
    for (i = 0; i <= 4; i++)
        pimg_plane[i] = newimage(102, 126);
    for (i = 0; i <= 1; i++)
        pimg_enemy1[i] = newimage(57, 43);
    for (i = 0; i <= 3; i++)
        pimg_enemy2[i] = newimage(69, 99);
    for (i = 0; i <= 6; i++)
        pimg_enemy3[i] = newimage(169, 258);

    pimg_bullet1 = newimage(5, 11);
    pimg_bullet2 = newimage(5, 11);
    pimg_endgame = newimage(160, 21);
    pimg_repeatgame = newimage(160, 21);
    pimg_timi = newimage(680, 383);
    getimage(pimg_bk, "images\\background.png");
    // getimage(pimg_temp, "images\\background.png");
    getimage(pimg_plane[0], "images\\me1.png");
    getimage(pimg_plane[1], "images\\me_destroy_1.png");
    getimage(pimg_plane[2], "images\\me_destroy_2.png");
    getimage(pimg_plane[3], "images\\me_destroy_3.png");
    getimage(pimg_plane[4], "images\\me_destroy_4.png");
    getimage(pimg_enemy1[0], "images\\enemy1.png");
    getimage(pimg_enemy1[1], "images\\enemy1_down1.png");
    getimage(pimg_enemy2[0], "images\\enemy2.png");
    getimage(pimg_enemy2[1], "images\\enemy2_down1.png");
    getimage(pimg_enemy2[2], "images\\enemy2_down2.png");
    getimage(pimg_enemy2[3], "images\\enemy2_down3.png");
    getimage(pimg_enemy3[0], "images\\enemy3_n1.png");
    getimage(pimg_enemy3[1], "images\\enemy3_down1.png");
    getimage(pimg_enemy3[2], "images\\enemy3_down2.png");
    getimage(pimg_enemy3[3], "images\\enemy3_down3.png");
    getimage(pimg_enemy3[4], "images\\enemy3_down4.png");
    getimage(pimg_enemy3[5], "images\\enemy3_down5.png");
    getimage(pimg_enemy3[6], "images\\enemy3_down6.png");
    getimage(pimg_bullet1, "images\\bullet1.png");
    getimage(pimg_bullet2, "images\\bullet2.png");
    getimage(pimg_endgame, "images\\gameover.png");
    getimage(pimg_repeatgame, "images\\again.png");
    getimage(pimg_timi, "images\\video_4.jpg");
}
/// @brief 绘制飞机
void graphImage()
{
    int i, j;
    putimage(0, 0, pimg_bk);
    // putimage_withalpha(NULL, pimg_bk, 0, 0);
    if (plane.hp >= 2000)
        j = 0;
    else if (plane.hp > 1000)
        j = 1;
    else if (plane.hp > 500)
        j = 2;
    else if (plane.hp > 200)
        j = 3;
    else
        j = 4;
    putimage_withalpha(NULL, pimg_plane[j], plane.x, plane.y);

    for (i = 0; i < ENEMY1NUM; i++)
    {
        if (enemy1[i].hp > 50)
            j = 0;
        else
            j = 1;
        putimage_withalpha(NULL, pimg_enemy1[j], enemy1[i].x, enemy1[i].y);
    }
    for (i = 0; i < ENEMY2NUM; i++)
    {
        if (enemy2[i].hp > 200)
            j = 0;
        else if (enemy2[i].hp > 150)
            j = 1;
        else if (enemy2[i].hp > 100)
            j = 2;
        else
            j = 3;
        putimage_withalpha(NULL, pimg_enemy2[j], enemy2[i].x, enemy2[i].y);
    }
    for (i = 0; i < ENEMY3NUM; i++)
    {
        if (enemy3[i].hp > 1500)
            j = 0;
        else if (enemy3[i].hp > 1000)
            j = 1;
        else if (enemy3[i].hp > 800)
            j = 2;
        else if (enemy3[i].hp > 500)
            j = 3;
        else if (enemy3[i].hp > 300)
            j = 4;
        else if (enemy3[i].hp > 100)
            j = 5;
        else
            j = 6;
        putimage_withalpha(NULL, pimg_enemy3[j], enemy3[i].x, enemy3[i].y);
    }
    putimage_withalpha(NULL, pimg_endgame, 500, 660);
}

/// @brief 处理输入信息
/// @param xSpeeds x方向速度
/// @param ySpeeds y方向速度
void getInput(int *xSpeeds, int *ySpeeds)
{
    int i;
    double loadTime = *bullet_T;
    static double fire_start = 0;
    double tmp = fclock();
    // 根据按键按下状态对位置增量进行累加，可八方向移动
    int xNext = plane.x;
    int yNext = plane.y;
    for (i = 0; i < 4; i++)
    {
        if (keystate(keys[i]) || keystate(directionKeys[i]))
        {
            // 往对应方向进行位移
            xNext += xSpeeds[i];
            yNext += ySpeeds[i];
        }
    }
    if (keystate(keys[4]) || keystate(directionKeys[4]))
    {

        if (tmp - fire_start >= loadTime)
        {
            fire_start = tmp;
            listPushBack(&plane_list, creatPlaneBullet(0, -20));
            if (*runTime > 20)
            {
                listPushBack(&plane_list, creatPlaneBullet(-20 * sin(*angle), -20 * cos(*angle)));
                listPushBack(&plane_list, creatPlaneBullet(20 * sin(*angle), -20 * cos(*angle)));
            }
        }
    }

    for (i = 5; i < 9; i++)
    {
        if (keystate(keys[i]) || keystate(directionKeys[i]))
        {
            change(i - 4);
        }
    }
    // 如果移动则检测移动是否有效
    if (xNext != plane.x || yNext != plane.y)
    {
        // 检测是否超出边界
        if (0 <= xNext && xNext <= (WIDTH - plane.width) && 0 <= yNext && yNext <= (HIGH - plane.height))
        {
            plane.x = xNext;
            plane.y = yNext;
        }
    }
}

/// @brief 在某链表尾部插入一个数据
/// @param pplist 原始链表
/// @param newNode 要插入的数据
void listPushBack(bullet_list **pplist, bullet_list *newNode)
{
    if (*pplist == NULL)
    {
        *pplist = newNode;
        return;
    }
    bullet_list *cur = *pplist;
    while (cur->pnext != NULL)
    {
        cur = cur->pnext;
    }
    cur->pnext = newNode;
}

/// @brief // 飞机发射的子弹，增加一个节点
/// @param vx  x方向速度
/// @param vy y方向速度
/// @return
bullet_list *creatPlaneBullet(float vx, float vy)
{
    bullet_list *p = (bullet_list *)malloc(sizeof(bullet_list));
    p->x = plane.x + plane.width / 2;
    p->y = plane.y;
    p->vx = vx;
    p->vy = vy;
    p->atk = *atk;
    p->flag = 1;
    p->pnext = NULL;
    return p;
}

/// @brief  修改某链中所有节点的坐标
/// @param pplist 某链表
void listChangeXY(bullet_list **pplist)
{
    if (*pplist == NULL)
        return;
    bullet_list *cur = *pplist;
    while (cur != NULL)
    {
        cur->x += cur->vx;
        cur->y += cur->vy;
        // 判断子弹是否离开视野
        if ((cur->y < -20) || (cur->y > HIGH) || (cur->x > WIDTH) || (cur->y < -20))
            cur->flag = 0;
        cur = cur->pnext;
    }
}

/// @brief 删除链表中flag为0的节点
/// @param pplist 某链表
void listRemoveNode(bullet_list **pplist)
{
    if (*pplist == NULL) // 如果链表为空，就没有可删除的节点了
        return;
    bullet_list *cur = *pplist; // curret先指向第一个节点
    bullet_list *prev = NULL;   // previous指向上一个节点的指针
    while (cur != NULL)         // 遍历链表
    {
        if (cur->flag == 0) // 判断节点是否需要删除
        {
            if (*pplist == cur) // 如果删除的是第一个节点
            {
                *pplist = cur->pnext; // 更改链表的地址，让下一个节点作为头结点 ，如果没有节点，则链表为空
                free(cur);            // 释放当前节点（第一个节点的）空间
                cur = *pplist;        // 让cur指向下一个节点
            }
            else
            {
                prev->pnext = cur->pnext; // 记录下一个节点的地址
                free(cur);                // 释放当前节点空间
                cur = prev;               // 当前节点变成前一个节点
            }
        }
        else // 如果不需要删除节点，储存当前节点为前一个节点，然后指向下一个节点
        {
            prev = cur;
            cur = cur->pnext;
        }
    }
}

/// @brief 绘制发出来的子弹
void showBullet()
{

    int i;
    for (i = 0; i < ENEMY1NUM; i++)
        bulletHit(&enemy1[i]);
    for (i = 0; i < ENEMY2NUM; i++)
        bulletHit(&enemy2[i]);
    for (i = 0; i < ENEMY3NUM; i++)
        bulletHit(&enemy3[i]);

    listChangeXY(&plane_list);   // 计算子弹新的位置
    listRemoveNode(&plane_list); // 超出视野或者击中飞行器的子弹删除掉
    for (bullet_list *cur = plane_list; cur != NULL; cur = cur->pnext)
    {
        putimage_withalpha(NULL, pimg_bullet2, cur->x, cur->y);
    }
}
/// @brief 显示游戏信息
void information()
{
    *curTime = fclock();
    *runTime = *curTime - *startTime;
    int i;
    // double curTime = runTime - *startTime;
    xyprintf(WIDTH + 1, 20, "运行总时间：%.3lf", *runTime);
    xyprintf(WIDTH + 1, 40, "HP：%d", plane.hp);
    xyprintf(WIDTH + 1, 60, "SCORE：%d", *score);

    for (i = 0; i < ENEMY1NUM; i++)
        xyprintf(WIDTH + 1, 100 + 20 * i, "小型飞行器%dHP：%d", i + 1, enemy1[i].hp);
    for (i = 0; i < ENEMY2NUM; i++)
        xyprintf(WIDTH + 1, 160 + 20 * i, "中型飞行器%dHP：%d", i + 1, enemy2[i].hp);
    for (i = 0; i < ENEMY3NUM; i++)
        xyprintf(WIDTH + 1, 200 + 20 * i, "大型飞行器%dHP：%d", i + 1, enemy3[i].hp);
    xyprintf(WIDTH + 1, 240, "游戏规则：");
    xyprintf(WIDTH + 1, 260, "WASD控制移动方向");
    xyprintf(WIDTH + 1, 280, "空格键发射子弹");
    xyprintf(WIDTH + 1, 300, "HP<0游戏结束");
    xyprintf(WIDTH + 1, 340, "子弹伤害：%d  ", *atk);
    xyprintf(WIDTH + 1, 360, "子弹偏转角：%.1frad  ", *angle);
    xyprintf(WIDTH + 1, 380, "20s后自动3弹道发射");
    xyprintf(WIDTH + 1, 420, "修改指令(数字键)");
    xyprintf(WIDTH + 1, 440, "1:子弹偏转角增大");
    xyprintf(WIDTH + 1, 460, "2:子弹偏转角减小");
    xyprintf(WIDTH + 1, 480, "3:子弹伤害增加");
    xyprintf(WIDTH + 1, 500, "4:子弹伤害减小");
    while (mousemsg())
    {
        msg = getmouse();

        if ((500 < msg.x && msg.x < 660) && (660 < msg.y && msg.y < 681) && msg.is_left() == 1)
        {
            // endGraph();
            plane.hp = -1;
        }
    }
    xyprintf(WIDTH + 1, 540, "鼠标信息");
    xyprintf(WIDTH + 1, 560, "x = %-3d;y = %-3d",
             msg.x, msg.y);
    xyprintf(WIDTH + 1, 580, "left = %d;right = %d",
             (int)msg.is_left(),
             (int)msg.is_right()); //
    // msg = {0};
}
/// @brief 小型飞行器移动
void enemy1move()
{
    int i;
    for (i = 0; i < ENEMY1NUM; i++)
        enemy1[i].y += enemy1[i].speed;
    for (i = 0; i < ENEMY1NUM; i++)
        if (enemy1[i].hp < 0 || enemy1[i].y >= 700 || enemy1[i].flag == 0)
        {

            enemy1[i].hp = *enemy1baseHP + 2 * (int)(*runTime); // enemy1血量随时间增加
            enemy1[i].x = rand() % 419 + 2;
            enemy1[i].y = -60;
            enemy1[i].speed = rand() % 5 + 1;
            if (enemy1[i].flag == 1)
                *score += enemy1[i].hp;
            enemy1[i].flag = 1;
        }
}
/// @brief 中型飞行器移动
void enemy2move()
{
    int i;
    if (*runTime > 20)
        for (i = 0; i < ENEMY2NUM; i++)
            enemy2[i].y += enemy2[i].speed;
    for (i = 0; i < ENEMY1NUM; i++)
        if (enemy2[i].hp <= 0 || enemy2[i].y >= 700 || enemy2[i].flag == 0)
        {

            enemy2[i].hp = *enemy2baseHP + 2 * (int)(*runTime);
            enemy2[i].x = rand() % 410 + 2;
            enemy2[i].y = -150;
            enemy2[i].speed = rand() % 4 + 1;
            if (enemy2[i].flag == 1)
                *score += enemy2[i].hp;
            enemy2[i].flag = 1;
        }
}
/// @brief 大型飞行器移动
void enemy3move()
{
    int i;
    if (*runTime > 50)
        for (i = 0; i < ENEMY3NUM; i++)
            enemy3[i].y += enemy3[i].speed;
    for (i = 0; i < ENEMY3NUM; i++)
        if (enemy3[i].hp <= 0 || enemy3[i].y >= 700 || enemy3[i].flag == 0)
        {

            enemy3[i].hp = *enemy3baseHP + 50 * (int)(*runTime);
            enemy3[i].x = rand() % 300 + 2;
            enemy3[i].y = -259;
            if (enemy3[i].flag == 1)
                *score += enemy3[i].hp;
            enemy3[i].flag = 1;
        }
}

/// @brief 判断子弹击中目标
/// @param tmp 某种飞行器
void bulletHit(aircraft *tmp)
{
    for (bullet_list *cur = plane_list; cur != NULL; cur = cur->pnext)
    {
        // 子弹在UFO的矩形图片内时，认为击中
        if ((cur->x > tmp->x) && (cur->x < tmp->x + tmp->width))
        {
            if ((cur->y > tmp->y) && (cur->y < tmp->y + tmp->height))
            {
                tmp->hp -= cur->atk; // 飞行器生命值 - 子弹的伤害
                cur->flag = 0;       // 清除子弹的存在标记
            }
        }
    }
}

/// @brief 某个碰撞判断
/// @param enemy 敌方飞行器
void crash(aircraft *enemy)
{
    // 飞机近似三角形（约）
    if (((enemy->y <= plane.y && plane.y <= enemy->y + enemy->height) && (enemy->x <= plane.x + plane.width / 2 && plane.x + plane.width / 2 <= enemy->x + enemy->width)) || ((enemy->x < plane.x && plane.x <= enemy->x + enemy->width) && (enemy->y < plane.y + plane.height && plane.y + plane.height < enemy->y + enemy->height)) || ((enemy->x < plane.x + plane.width && plane.x + plane.width < enemy->x + enemy->width) && (enemy->y < plane.y + plane.height && plane.y + plane.height < enemy->y + enemy->height)))
    // 飞机近似矩形（密）注:时间关系，暂时没写，

    {
        plane.hp -= enemy->hp;
        enemy->flag = 0;
    }
}
/// @brief 所有飞行器碰撞判断
void enemyCrash()
{
    int i;
    for (i = 0; i < ENEMY1NUM; i++)
        crash(&enemy1[i]);
    for (i = 0; i < ENEMY2NUM; i++)
        crash(&enemy2[i]);
    for (i = 0; i < ENEMY3NUM; i++)
        crash(&enemy3[i]);
}

void startGraph()
{
    delay_ms(1);
}
void endGraph()
{

    putimage(500, 630, pimg_repeatgame);
    setfillcolor(WHITE);
    bar(500, 660, 660, 681);

    // getch();
    while (1)
    {

        msg = getmouse();
        if ((500 < msg.x && msg.x < 660) && (630 < msg.y && msg.y < 651) && msg.is_left() == 1)
        {

            return;
        }
    }
}
void damageImage()
{
    int i;
    delimage(pimg_bk);
    for (i = 0; i < 5; i++)
        delimage(pimg_plane[i]);
    for (i = 0; i < 2; i++)
        delimage(pimg_enemy1[i]);
    for (i = 0; i < 4; i++)
        delimage(pimg_enemy2[i]);
    for (i = 0; i < 7; i++)
        delimage(pimg_enemy3[i]);
    delimage(pimg_bullet1);
    delimage(pimg_bullet2);
    delimage(pimg_endgame);
    delimage(pimg_timi);
}
void change(int mode)
{
    if (mode == 1)
        *angle += 0.01;
    if (mode == 2)
        *angle -= 0.01;
    if (mode == 3)
        *atk += 2;
    if (mode == 4)
        if (*atk > 2)
            *atk -= 2;
}
void soundMusic(int mode)
{
    if (mode == 0)
    {
        music1.OpenFile("Sound\\timi.mp3");
        music2.OpenFile("Sound\\game_music.mp3");
        // music3.OpenFile("Sound\\enemyShoot.mp3");
    }
    else if (mode == 1)
    {
        if (music1.IsOpen())
        {
            music1.Play();
            delay_ms(2000);
        }
    }
    else if (mode == 2)
    {
        if (music2.GetPlayStatus() == MUSIC_MODE_STOP)
        {
            music2.Play(3100);
        }
    }
    else if (mode == 4)
    {
        music1.Close();
        music2.Close();
        // music3.Close();
    }
}
/***************************************/
