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
    int rorder;//�Ω�̫�Hrow�M�w����
    int remain;//�̫�O�d���I
    int node1;
    int node2;
} edge;
int main()
{
    int n,i,j,k,count,count2,count3,judge;

    double middle_x,middle_y;

    point a[MAX_TERM];//�I�����X

    edge b[MAX_TERM];//��ledge
    edge c[MAX_TERM];//node1�����d�U��edge
    edge bt[MAX_TERM];//b��transpose
    edge ct[MAX_TERM];//c��transpose

    int group[MAX_TERM];//node1=i��link��(�����@�}�l���s�u���I)
    int group2[MAX_TERM];//node2=j��link��(�����@�}�l���s�u���I)
    int group3[MAX_TERM];//�bc��node2=j��link��(�w�g�h���F�@��edge�A���I�ܤ�)
    int start[MAX_TERM];//group2�ֶi
    int start2[MAX_TERM];//group3�ֶi

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

    /*�إߩҦ��I���s�u*/
    count=0;
    for(i=0;i<n;i++)
        for(j=i+1;j<n;j++)
        if(!((abs(a[i].x-a[j].x)>1)||(abs(a[i].y-a[j].y)>1)||((abs(a[i].x-a[j].x)>0.5)&&(abs(a[i].y-a[j].y)>0.5)))){
            if(dist(a[i].x,a[i].y,a[j].x,a[j].y)<=1){
                b[count].node1=i;
                b[count].node2=j;
                count++;
                group[i]++;//node1=i��link��
                group2[j]++;//node2=j��link��
            }
        }

    /*�D�Xb��transpose*/
    start[0]=group2[0];
    for(i=1;i<n;i++)
        start[i]=start[i-1]+group2[i-1];
    for(i=0;i<count;i++){
        j=start[b[i].node2]++;
        bt[j].order=b[i].order;
        bt[j].node1=b[i].node1;
        bt[j].node2=b[i].node2;
    }

    /*�����Lb���C��node1�A�簣����edge�æs�Jc��*/
    count2=0;
    int head=0;//�Ѥj��p�A���B�z��node1
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
                    group3[c[count2].node2]++;//�bc��node2=j��link��
                    count2++;
                }
                run--;
                j++;
        }
        head+=group[i];
    }

    /*�D�Xc��transpose*/
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

    /*���Lc���C��node2�A"�Pb���쥻���إ߹Ledge��node1���"�A�簣�Ѿl���Xedge*/
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
                for(k=lead;k<lead+group2[i];k++){//�Pb���쥻���إ߹Ledge��node1���
                    if((bt[k].node1!=ct[j].node1)&&(dist(a[bt[k].node1].x,a[bt[k].node1].y,middle_x,middle_y)<(dist(a[i].x,a[i].y,a[ct[j].node1].x,a[ct[j].node1].y)/2))){
                        judge=0;
                        break;
                    }
                }
                if(judge){
                    c[ct[j].rorder].remain=1;//�аO�̲צX�G�t��k�n�D���I
                    count3++;
                }
                run--;
                j++;
        }
        head+=group3[i];
        lead+=group2[i];
    }

    /*�L�X���G*/
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
