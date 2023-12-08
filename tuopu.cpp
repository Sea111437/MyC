
#include<stdio.h>
#include<stdlib.h>
#define MAX_VERTEX_NUM 20
#define _NO_CRT_STDIO_INLINE
#define _CRT_INTERNAL_SCANF_SECURECRT
typedef int SElemType;
typedef char  VertexType;//顶点信息数据类型的定义
typedef struct ArcNode {//表结点的定义
    int adjvex;  //该弧所指向的顶点的位置
    struct ArcNode* nextarc;  //指向下一条弧的指针
    int info;//弧的相关信息
}ArcNode;
typedef struct VNode {  //顶点结点的定义
    VertexType data;  //顶点信息
    ArcNode* firstarc;  //指向第一条依附于该顶点的弧的指针
    int hang;
    int rudushu;
}VNode, AdjList[MAX_VERTEX_NUM];//AdjLst表示邻接表的类型
typedef struct {
    AdjList vertices;    //顶点数组
    int vexnum, arcnum;  //图当前的顶点数和弧数
    int kind;            //图的种类标志
}ALGraph;//图的结构定义
typedef struct {
    SElemType* base;  //在栈构造之前和销毁之后，base的值为NULL
    SElemType* top;   //栈顶指针
    int stacksize;    //当前已经分配的存储空间，以元素为单位
}SqStack;
int* ve, * vl;//事件的最早时间和最晚时间    全局变量
SqStack S1;
SqStack S2;


int LocateVex(ALGraph G, VertexType a)
{
    for (int i = 0; i < G.vexnum; i++)
    {
        if (a == G.vertices[i].data)
        {
            return G.vertices[i].hang;
        }
    }
    return -1;
}

void Inject(ALGraph* G)
{
    //G = (ALGraph)malloc(MAX_VERTEX_NUM * sizeof(ALGraph));
    printf("请输入图的顶点数：");
    scanf_s("%d", &G->vexnum);
    printf("请输入图的边数：");
    scanf_s("%d", &G->arcnum);
    for (int i = 0; i < G->vexnum; i++)
    {
        printf("%d ", i);
        G->vertices[i].hang = i;//将顶点数组每行的信息存进去，方便以后用
        getchar();
        scanf_s("%c", &G->vertices[i].data);//对每个顶点赋值
        //getchar();//清空缓存区*************
        G->vertices[i].firstarc = NULL;//顶点结点的指针暂时指向空
    }
    VertexType vi, vj;
    int i, j, s;//vi->vj  s是权值的暂时存储
    for (int k = 0; k < G->arcnum; k++)
    {
        printf("按照vi->vj的顺序以及权值：");
        getchar();
        scanf_s("%c", &vi);
        getchar();
        scanf_s("%c", &vj);
        scanf_s("%d", &s);
        i = LocateVex(*G, vi);
        j = LocateVex(*G, vj);//找位置
        ArcNode* p = (ArcNode*)malloc(sizeof(ArcNode));
        p->adjvex = j;//头插法
        p->info = s;
        p->nextarc = G->vertices[i].firstarc;
        G->vertices[i].firstarc = p;
    }
}

int FindInDeree(ALGraph G, int k)
{
    int sum = 0;    //入度数
    ArcNode* p;   //p指针用来扫描每个顶点所发出的边
    for (int i = 0; i < G.vexnum; i++)//正确循环遍历邻接表每个结点
    {
        p = G.vertices[i].firstarc;
        while (p != NULL)
        {
            if (p->adjvex == k)
            {
                sum++;           //统计顶点的度
                break;
            }
            p = p->nextarc;      //循环遍历结点的所有弧
        }
    }
    return sum;          //返回入度
}

int InitStack(SqStack* S)  //创造一个空栈
{
    S->base = (SElemType*)malloc(MAX_VERTEX_NUM * sizeof(SElemType));
    if (!S->base)exit(1);   //判空
    S->top = S->base;
    S->stacksize = MAX_VERTEX_NUM;
    return 1;
}

void Push(SqStack* S, int k)
{
    *S->top++ = k;
}
int StackEmpty(SqStack S)
{
    if (S.top == S.base)
    {
        return 0;
    }
    return 1;
}

int Pop(SqStack* S)
{
    if (S->top == S->base) return 0;
    int i;
    i = *--S->top;
    return i;
}

int TopologicalSort(ALGraph G)//拓扑排序
{
    int sum = 0;//sum是成功删除掉的顶点数
    int i, k;//
    ArcNode* p;
    for (i = 0; i < G.vexnum; i++)
        G.vertices[i].rudushu = FindInDeree(G, G.vertices[i].hang);//找出入度为0的
    SqStack S;//S1是拓扑排序的逆序存储，S2是正序存储，修火车的方法
    InitStack(&S1);
    InitStack(&S2);
    InitStack(&S);//栈的初始化
    ve = (int*)malloc(G.vexnum * sizeof(int));
    for (int n = 0; n < G.vexnum; n++)
    {
        ve[n] = 0;
    }
    for (int i = 0; i < G.vexnum; i++)//建零入度顶点S
    {
        if (G.vertices[i].rudushu == 0)//入度为0者进栈
            Push(&S, i);//入度为0者进栈
    }
    while (StackEmpty(S))
    {
        i = Pop(&S);
        printf("%c->", G.vertices[i].data);//输出i号顶点并计数
        ++sum;
        Push(&S1, i);
        for (p = G.vertices[i].firstarc; p; p = p->nextarc)
        {
            k = p->adjvex;
            G.vertices[k].rudushu--;//对i号顶点的每个邻接点的入度都减1
            if (!G.vertices[k].rudushu)
                Push(&S, k);
            if ((ve[i] + p->info) > ve[k])//在拓扑排序中就实现ve的修改，取最大值
                ve[k] = ve[i] + p->info;
        }
    }
    if (sum < G.vexnum)
    {
        printf("有回路");
        return 0;
    }
    else
    {
        printf("无回路\n");

        return 1;
    }
}

void CriticalPath(ALGraph G)
{
    ArcNode* p;
    vl = (int*)malloc(G.vexnum * sizeof(int));
    for (int i = 0; i < G.vexnum; i++)//对ltv初始化
        vl[i] = ve[G.vexnum - 1];
    while (StackEmpty(S1))//对vl的修改，取每条边的所得数值的最小值
    {
        int i = Pop(&S1);//利用逆序拓扑排序
        for (p = G.vertices[i].firstarc; p; p = p->nextarc)
        {
            int k = p->adjvex;
            if ((vl[k] - p->info) < vl[i])
                vl[i] = vl[k] - p->info;
        }
    }
    int e, l;
    for (int n = 0; n < G.vexnum; n++)//遍历整个顶点表
    {
        for (p = G.vertices[n].firstarc; p; p = p->nextarc)//遍历每个顶点的链表
        {
            int k = p->adjvex;
            e = ve[n];
            l = vl[k] - p->info;
            if (e == l)
            {
                printf("(%c->%c)", G.vertices[n].data, G.vertices[k].data);
            }
        }
    }
}//关键路径



int main()
{
    ALGraph G;
    Inject(&G);
    TopologicalSort(G);
    CriticalPath(G);
    return 0;
}

