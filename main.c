#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int total_move = 0;
/*===========================================================================*/
typedef struct Tower{
    int * stack;
    int current_index;
} Tower;
/*===========================================================================*/
typedef struct Hanoi {
    int size;
    Tower * tower;
} Hanoi;
/*===========================================================================*/
int top(const Hanoi * hanoi, int n)
{
    const Tower * tower = &(hanoi->tower[n]);
    if (tower->current_index == -1) {
        return -1;
    } else {
        return tower->stack[tower->current_index];
    }
}
/*===========================================================================*/
int pop(Hanoi * hanoi, int n)
{
    Tower * tower = &(hanoi->tower[n]);
    int rtv = 0;
    if (tower->current_index == -1) {
        rtv = -1;
    } else {
        rtv = tower->stack[tower->current_index];
        tower->stack[tower->current_index--] = 0;
    }

    return rtv;
}
/*===========================================================================*/
int push(Hanoi * hanoi, int n, int value)
{
    assert(value>0);

    int rtv = -1;

    Tower * tower = &(hanoi->tower[n]);

    if (tower->current_index!=(hanoi->size-1)) {
        tower->current_index++;
        tower->stack[tower->current_index]=value;
        rtv = 0;
    }

    return rtv;
}
/*===========================================================================*/
void initTowerStackOrdered(Hanoi * hanoi, int n, int size)
{
    Tower * tower = &(hanoi->tower[n]);

    tower->current_index = size-1;

    if (tower->stack!=NULL) {
        free(tower->stack);
    }

    tower->stack = (int *)malloc(sizeof(int)*size);

    for (int i = 0; i < size; i++) {
        tower->stack[i] = size-i;
    }

    return;
}
/*===========================================================================*/
void initTowerStack(Hanoi * hanoi, int n, int size)
{
    Tower * tower = &(hanoi->tower[n]);

    tower->current_index = -1;

    if (tower->stack!=NULL) {
        free(tower->stack);
    }
    tower->stack = (int *)malloc(sizeof(int)*size);

    for (int i = 0; i < size; i++) {
        tower->stack[i] = 0;
    }

    return;
}
/*===========================================================================*/
void releaseTowerStack(Hanoi * hanoi, int n)
{
    if (hanoi->tower[n].stack!=NULL) {
        free(hanoi->tower[n].stack);
        hanoi->tower[n].stack=NULL;
    }
}
/*===========================================================================*/
void initHanoi(Hanoi * hanoi, int size)
{
    hanoi->size = size;
    hanoi->tower = (Tower *)malloc(sizeof(Tower)*3);
    hanoi->tower[0].stack = NULL;
    hanoi->tower[1].stack = NULL;
    hanoi->tower[2].stack = NULL;

    initTowerStackOrdered(hanoi, 0, size);
    initTowerStack(hanoi, 1, size);
    initTowerStack(hanoi, 2, size);

    return;
}
/*===========================================================================*/
Hanoi * releaseHanoi(Hanoi * hanoi)
{
    releaseTowerStack(hanoi, 0);
    releaseTowerStack(hanoi, 1);
    releaseTowerStack(hanoi, 2);

    free(hanoi);

    return NULL;
}
/*===========================================================================*/
void showHanoi(const Hanoi * hanoi)
{
    printf("=================================================================\n");
    for (int i = 0; i < 3; i++) {
        printf("+-------------------------------------+\n");
        printf("|               tower %d               |\n", i);
        printf("+-------------------------------------+\n");
        for (int idx = hanoi->size-1; idx > -1; idx--) {
            for (int jdx = 0; jdx < hanoi->tower[i].stack[idx]; jdx++) {
                printf(" *");
            }
            printf("\n");
        }
    }
    printf("=================================================================\n");
}
/*===========================================================================*/
void move(Hanoi * hanoi, int from, int to)
{
    total_move++;

    printf("move %d->%d\n", from, to);

    if ((top(hanoi, to)!=-1)&&top(hanoi, from)>top(hanoi, to)) {
        printf("can't move %d->%d\n", from+1, to+1);
    } else {
        if (top(hanoi, from)!=-1) {
            push(hanoi, to, pop(hanoi, from));
        }
    }
    /* showHanoi(hanoi); */
}
/*===========================================================================*/
void hanoi_move(Hanoi * hanoi, int level, int from, int to, int tmp)
{
    if (level==1) {
        move(hanoi, from, to);
        return;
    } else {
        hanoi_move(hanoi, level-1, from, tmp, to);
        move(hanoi, from, to);
        hanoi_move(hanoi, level-1, tmp, to, from);
        return;
    }
}
/*===========================================================================*/
int main(int argc, const char *argv[])
{
    if (argc!=2) {
        printf("usage: %s size\n", argv[0]);
        printf("example: %s 8\n", argv[0]);
        return -1;
    }

    Hanoi * hanoi = (Hanoi *)malloc(sizeof(Hanoi));;

    initHanoi(hanoi, atoi(argv[1]));
    showHanoi(hanoi);
    hanoi_move(hanoi, hanoi->size, 0, 1, 2);
    showHanoi(hanoi);
    printf("total move = %d\n", total_move);
    hanoi = releaseHanoi(hanoi);

    return 0;
}
/*===========================================================================*/
