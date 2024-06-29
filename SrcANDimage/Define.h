
#pragma once
/**********************************************************/
#define WIDTH 480
#define HIGH 700
#define FRAME 60

/**********************************************************/
typedef struct Bullet
{
    float x;  // �ӵ�x����
    float y;  // �ӵ�y����
    float vx; // �ӵ���ˮƽ�ٶ�
    float vy; // �ӵ�����ֱ�ٶ�
    int atk;  // ������
    int flag; // �Ƿ�Ӧɾ��
    struct Bullet *pnext;
} bullet_list;
typedef struct Aircraft
{
    int x; // �ɻ�x������
    int y; // �ɻ�y������
    int width;
    int height;
    int speed; // �ɻ��ٶ�
    int hp;    // Ѫ��
    int flag;
} aircraft;

// �����Ľṹ��
typedef struct Vector
{
    float x, y;
} vector;
