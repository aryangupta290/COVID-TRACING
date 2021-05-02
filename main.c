#include"header.h"
#include<stdio.h>

int main() 
{
    //initial basic input
    int N,M,K;
    scanf("%d %d %d",&N,&M,&K);
    struct Graph* graph = create_Graph(N);
    for(int i=0;i<M;i++)
    {
        int U,V,W;
        scanf("%d %d %d",&U,&V,&W);
        addEdge(graph,U,V,W);//assuming vertices from 0 to N-1
    }
    for(int i=0;i<K;i++)
    {
        int station;
        scanf("%d",&station);
        
        //need person struct init function
    }
    
    return 0;
}
