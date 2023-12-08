
#include<stdio.h>
#include<stdlib.h>
#define MAX_VERTEX_NUM 20
#define _NO_CRT_STDIO_INLINE
#define _CRT_INTERNAL_SCANF_SECURECRT
typedef int SElemType;
typedef char  VertexType;//������Ϣ�������͵Ķ���
typedef struct ArcNode {//����Ķ���
    int adjvex;  //�û���ָ��Ķ����λ��
    struct ArcNode* nextarc;  //ָ����һ������ָ��
    int info;//���������Ϣ
}ArcNode;
typedef struct VNode {  //������Ķ���
    VertexType data;  //������Ϣ
    ArcNode* firstarc;  //ָ���һ�������ڸö���Ļ���ָ��
    int hang;
    int rudushu;
}VNode, AdjList[MAX_VERTEX_NUM];//AdjLst��ʾ�ڽӱ������
typedef struct {
    AdjList vertices;    //��������
    int vexnum, arcnum;  //ͼ��ǰ�Ķ������ͻ���
    int kind;            //ͼ�������־
}ALGraph;//ͼ�Ľṹ����
typedef struct {
    SElemType* base;  //��ջ����֮ǰ������֮��base��ֵΪNULL
    SElemType* top;   //ջ��ָ��
    int stacksize;    //��ǰ�Ѿ�����Ĵ洢�ռ䣬��Ԫ��Ϊ��λ
}SqStack;
int* ve, * vl;//�¼�������ʱ�������ʱ��    ȫ�ֱ���
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
    printf("������ͼ�Ķ�������");
    scanf_s("%d", &G->vexnum);
    printf("������ͼ�ı�����");
    scanf_s("%d", &G->arcnum);
    for (int i = 0; i < G->vexnum; i++)
    {
        printf("%d ", i);
        G->vertices[i].hang = i;//����������ÿ�е���Ϣ���ȥ�������Ժ���
        getchar();
        scanf_s("%c", &G->vertices[i].data);//��ÿ�����㸳ֵ
        //getchar();//��ջ�����*************
        G->vertices[i].firstarc = NULL;//�������ָ����ʱָ���
    }
    VertexType vi, vj;
    int i, j, s;//vi->vj  s��Ȩֵ����ʱ�洢
    for (int k = 0; k < G->arcnum; k++)
    {
        printf("����vi->vj��˳���Լ�Ȩֵ��");
        getchar();
        scanf_s("%c", &vi);
        getchar();
        scanf_s("%c", &vj);
        scanf_s("%d", &s);
        i = LocateVex(*G, vi);
        j = LocateVex(*G, vj);//��λ��
        ArcNode* p = (ArcNode*)malloc(sizeof(ArcNode));
        p->adjvex = j;//ͷ�巨
        p->info = s;
        p->nextarc = G->vertices[i].firstarc;
        G->vertices[i].firstarc = p;
    }
}

int FindInDeree(ALGraph G, int k)
{
    int sum = 0;    //�����
    ArcNode* p;   //pָ������ɨ��ÿ�������������ı�
    for (int i = 0; i < G.vexnum; i++)//��ȷѭ�������ڽӱ�ÿ�����
    {
        p = G.vertices[i].firstarc;
        while (p != NULL)
        {
            if (p->adjvex == k)
            {
                sum++;           //ͳ�ƶ���Ķ�
                break;
            }
            p = p->nextarc;      //ѭ�������������л�
        }
    }
    return sum;          //�������
}

int InitStack(SqStack* S)  //����һ����ջ
{
    S->base = (SElemType*)malloc(MAX_VERTEX_NUM * sizeof(SElemType));
    if (!S->base)exit(1);   //�п�
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

int TopologicalSort(ALGraph G)//��������
{
    int sum = 0;//sum�ǳɹ�ɾ�����Ķ�����
    int i, k;//
    ArcNode* p;
    for (i = 0; i < G.vexnum; i++)
        G.vertices[i].rudushu = FindInDeree(G, G.vertices[i].hang);//�ҳ����Ϊ0��
    SqStack S;//S1���������������洢��S2������洢���޻𳵵ķ���
    InitStack(&S1);
    InitStack(&S2);
    InitStack(&S);//ջ�ĳ�ʼ��
    ve = (int*)malloc(G.vexnum * sizeof(int));
    for (int n = 0; n < G.vexnum; n++)
    {
        ve[n] = 0;
    }
    for (int i = 0; i < G.vexnum; i++)//������ȶ���S
    {
        if (G.vertices[i].rudushu == 0)//���Ϊ0�߽�ջ
            Push(&S, i);//���Ϊ0�߽�ջ
    }
    while (StackEmpty(S))
    {
        i = Pop(&S);
        printf("%c->", G.vertices[i].data);//���i�Ŷ��㲢����
        ++sum;
        Push(&S1, i);
        for (p = G.vertices[i].firstarc; p; p = p->nextarc)
        {
            k = p->adjvex;
            G.vertices[k].rudushu--;//��i�Ŷ����ÿ���ڽӵ����ȶ���1
            if (!G.vertices[k].rudushu)
                Push(&S, k);
            if ((ve[i] + p->info) > ve[k])//�����������о�ʵ��ve���޸ģ�ȡ���ֵ
                ve[k] = ve[i] + p->info;
        }
    }
    if (sum < G.vexnum)
    {
        printf("�л�·");
        return 0;
    }
    else
    {
        printf("�޻�·\n");

        return 1;
    }
}

void CriticalPath(ALGraph G)
{
    ArcNode* p;
    vl = (int*)malloc(G.vexnum * sizeof(int));
    for (int i = 0; i < G.vexnum; i++)//��ltv��ʼ��
        vl[i] = ve[G.vexnum - 1];
    while (StackEmpty(S1))//��vl���޸ģ�ȡÿ���ߵ�������ֵ����Сֵ
    {
        int i = Pop(&S1);//����������������
        for (p = G.vertices[i].firstarc; p; p = p->nextarc)
        {
            int k = p->adjvex;
            if ((vl[k] - p->info) < vl[i])
                vl[i] = vl[k] - p->info;
        }
    }
    int e, l;
    for (int n = 0; n < G.vexnum; n++)//�������������
    {
        for (p = G.vertices[n].firstarc; p; p = p->nextarc)//����ÿ�����������
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
}//�ؼ�·��



int main()
{
    ALGraph G;
    Inject(&G);
    TopologicalSort(G);
    CriticalPath(G);
    return 0;
}

