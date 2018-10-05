#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define MAX_TERM 100
float dist(float x1,float y1,float x2,float y2);
typedef struct {
    int order;
    float x;
    float y;
} point;
typedef struct {
    int order;
    int node1;
    int node2;
} edge;
int main()
{
    int n,i,j,k,count,count2,judge;

    float middle_x,middle_y;
    freopen("node.txt","r",stdin);
    freopen("link.txt","w",stdout);
    scanf("%d",&n);
    point a[MAX_TERM];
    edge b[MAX_TERM];
    edge c[MAX_TERM];
    int group[n];
    for(i=0;i<n;i++){
        scanf("%d%f%f",&(a[i].order),&(a[i].x),&(a[i].y));
        group[i]=0;
    }
    count=0;
    for(i=0;i<n;i++)
        for(j=i+1;j<n;j++)
        if(!((abs(a[i].x-a[j].x)>1)||(abs(a[i].y-a[j].y)>1)||((abs(a[i].x-a[j].x)>0.5)&&(abs(a[i].y-a[j].y)>0.5)))){
            if(dist(a[i].x,a[i].y,a[j].x,a[j].y)<=1){
                b[count].node1=i;
                b[count].node2=j;
                count++;
                group[i]++;//node1=i的link數
            }
        }
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
                    c[count2].order=count2;
                    c[count2].node1=i;
                    c[count2].node2=b[j].node2;
                    count2++;
                }
                run--;
                j++;
        }
        head+=group[i];
    }

    printf("%d\n",count);
    for(i=0;i<count;i++)
        printf("%d\t%d\t%d\n",i,b[i].node1,b[i].node2);
    printf("%d\n",count2);
    for(i=0;i<count2;i++)
        printf("%d\t%d\t%d\n",c[i].order,c[i].node1,c[i].node2);

    return 0;
}
float dist(float x1,float y1,float x2,float y2){
    return sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
}
