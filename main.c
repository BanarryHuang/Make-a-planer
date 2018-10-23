#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define MAX_TERM 10000
float dist(float x1,float y1,float x2,float y2);
typedef struct {
    int order;
    double x;
    double y;
} point;
typedef struct {
    int order;
    int rorder;//用於最後以row決定順序
    int remain;//最後保留的點
    int node1;
    int node2;
} edge;
int main()
{
    int n,i,j,k,count,count2,count3,judge;

    double middle_x,middle_y;

    point a[MAX_TERM];//點的集合

    edge b[MAX_TERM];//初始edge
    edge c[MAX_TERM];//node1掃完留下的edge
    edge bt[MAX_TERM];//b的transpose
    edge ct[MAX_TERM];//c的transpose

    int group[MAX_TERM];//node1=i的link數(收集一開始有連線的點)
    int group2[MAX_TERM];//node2=j的link數(收集一開始有連線的點)
    int group3[MAX_TERM];//在c中node2=j的link數(已經去除了一些edge，其點變少)
    int start[MAX_TERM];//group2累進
    int start2[MAX_TERM];//group3累進

    for(i=0;i<n;i++){
        group[i]=0;
        group2[i]=0;
        group3[i]=0;
        start[i]=0;
        start2[i]=0;
    }

    freopen("node.txt","r",stdin);
    freopen("link.txt","w",stdout);

    scanf("%d",&n);

    for(i=0;i<n;i++)
        scanf("%d%lf%lf",&(a[i].order),&(a[i].x),&(a[i].y));

    /*建立所有點的連線*/
    count=0;
    for(i=0;i<n;i++)
        for(j=i+1;j<n;j++)
        if(!((abs(a[i].x-a[j].x)>1)||(abs(a[i].y-a[j].y)>1)||((abs(a[i].x-a[j].x)>0.5)&&(abs(a[i].y-a[j].y)>0.5)))){
            if(dist(a[i].x,a[i].y,a[j].x,a[j].y)<=1){
                b[count].node1=i;
                b[count].node2=j;
                count++;
                group[i]++;//node1=i的link數
                group2[j]++;//node2=j的link數
            }
        }

    /*求出b的transpose*/
    start[0]=group2[0];
    for(i=1;i<n;i++)
        start[i]=start[i-1]+group2[i-1];
    for(i=0;i<count;i++){
        j=start[b[i].node2]++;
        bt[j].order=b[i].order;
        bt[j].node1=b[i].node1;
        bt[j].node2=b[i].node2;
    }

    /*先掃過b的每個node1，剔除部分edge並存入c中*/
    count2=0;
    int head=0;//由大到小，須處理的node1
    for(i=0;i<n;i++){
        int run=group[i];
        j=head;
        while(run){
                middle_x=(a[i].x+a[b[j].node2].x)/2;
                middle_y=(a[i].y+a[b[j].node2].y)/2;
                judge=1;
                for(k=head;k<head+group[i];k++){
                    if(k!=j&&(dist(a[b[k].node2].x,a[b[k].node2].y,middle_x,middle_y)<(dist(a[i].x,a[i].y,a[b[j].node2].x,a[b[j].node2].y)/2))){
                        judge=0;
                        break;
                    }
                }
                if(judge){
                    c[count2].order=j;
                    c[count2].node1=i;
                    c[count2].node2=b[j].node2;
                    c[count2].remain=0;
                    group3[c[count2].node2]++;//在c中node2=j的link數
                    count2++;
                }
                run--;
                j++;
        }
        head+=group[i];
    }

    /*求出c的transpose*/
    start2[0]=group3[0];
    for(i=1;i<n;i++)
        start2[i]=start2[i-1]+group3[i-1];
    for(i=0;i<count2;i++){
        j=start2[c[i].node2]++;
        ct[j].rorder=i;
        ct[j].order=c[i].order;
        ct[j].node1=c[i].node1;
        ct[j].node2=c[i].node2;
    }

    /*掃過c的每個node2，"與b內原本曾建立過edge的node1比較"，剔除剩餘不合edge*/
    head=0;
    int lead=0;
    count3=0;
    for(i=0;i<n;i++){
        int run=group3[i];
        j=head;
        while(run){
                middle_x=(a[ct[j].node1].x+a[i].x)/2;
                middle_y=(a[ct[j].node1].y+a[i].y)/2;
                judge=1;
                for(k=lead;k<lead+group2[i];k++){//與b內原本曾建立過edge的node1比較
                    if((bt[k].node1!=ct[j].node1)&&(dist(a[bt[k].node1].x,a[bt[k].node1].y,middle_x,middle_y)<(dist(a[i].x,a[i].y,a[ct[j].node1].x,a[ct[j].node1].y)/2))){
                        judge=0;
                        break;
                    }
                }
                if(judge){
                    c[ct[j].rorder].remain=1;//標記最終合乎演算法要求的點
                    count3++;
                }
                run--;
                j++;
        }
        head+=group3[i];
        lead+=group2[i];
    }

    /*印出結果*/
    printf("%d\n",count);
    for(i=0;i<count;i++)
        printf("%d\t%d\t%d\n",i,b[i].node1,b[i].node2);
    printf("%d\n",count3);
    for(i=0;i<count2;i++)
        if(c[i].remain==1)
            printf("%d\t%d\t%d\n",c[i].order,c[i].node1,c[i].node2);

    return 0;

}
float dist(float x1,float y1,float x2,float y2){
    return sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
}
