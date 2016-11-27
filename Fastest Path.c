#include <stdio.h>
#include <string.h>
#include <math.h>
#define IN 99
int N = 100;
struct tile {
    int dist;
    int selected;
    int type;
    int prev;
    };
int convert(int i,int j){
    return N*i + j;
    }
int arrCheck(int arr[],int n,int key){
    int i;
    for(i=0;i<n;i++){
     if(arr[i]==key) return 1;   
    }
    return 0;
    }
int dijkstra(struct tile tiles[2*N*N],int source,int target);
int p1=0,p2=0;
static void display_file(const char *file_name)
{
    FILE *f = fopen(file_name, "r");      
    if (f != NULL)
    {
        int c;

        while ((c = fgetc(f)) != EOF)     
        {
            putchar(c);                  
        }
        fclose(f);
    }
}
int main(){
    struct tile tiles[2*N*N];
    int i,j,source,target,co;
    char temp;
    char f[100];
    char f2[100]="..\\";
    for(i=0;i<2*N*N;i++){
        tiles[i].dist = IN;
        tiles[i].selected = 0;
        tiles[i].prev = -1;
        tiles[i].type = 0;
        //type: empty = 0 ;wall = 1; gum = 2;
        }
    FILE *fp;
    display_file("..\\Instructions.txt");
   
    printf("Enter the name of the text file you want to read : \n");
    scanf("%s",f);
    strcat(f2,f);
    fp = fopen(f2,"r");
    fscanf(fp,"%d",&N);
    fgetc(fp);
    for(i=0;i<N;i++){
        for(j=0;j<N+1;j++){
           temp = fgetc(fp);
            if(temp == 'X'&&j<N)tiles[convert(i,j)].type = 1;
            else if(temp=='S'&&j<N)source = convert(i,j);
            else if(temp=='E'&&j<N)target = convert(i,j);
            else if(temp=='g'&&j<N)tiles[convert(i,j)].type = 2;
            else if(temp=='p'&&j<N){
                //tiles[convert(i,j)].type = 3;
                if(p1==0)p1 = convert(i,j);
                else p2 = convert(i,j);
            }
            }
        }
        
        fclose(fp);
    co = dijkstra(tiles,source,target);
    printf("\nThe time taken is %d seconds\n",co);
    return 0;
    }
int dijkstra(struct tile tiles[2*N*N],int source,int target){
    int min,m,start,d,i,j;
    int path[N*N];
    int cost=1;
    int adj[4];
    int target2 = 2*N*N-1;
    start = source;
    tiles[start].dist = 0;
    while(tiles[target2].selected==0){
        tiles[start].selected = 1;
        
    if(tiles[tiles[start].prev].type==2||tiles[tiles[tiles[start].prev].prev].type==2){
      cost = 2;
       }
    else cost = 1;
        min = IN;
        m = 0;

      // printf("%d(%d) ",start,cost);
        if(tiles[start-N].type!=1) {
            d = cost + tiles[start].dist;
            if(d<tiles[start-N].dist&&tiles[start-N].selected==0){
             tiles[start-N].dist = d;
             tiles[start-N].prev = start;
         }
        }
        if(tiles[start+N].type!=1) {
            d = cost + tiles[start].dist;
            if(d<tiles[start+N].dist&&tiles[start+N].selected==0){
             tiles[start+N].dist = d;
             tiles[start+N].prev = start;
         }
                 }
         if(tiles[start-1].type!=1) {
            d = cost + tiles[start].dist;
            if(d<tiles[start-1].dist&&tiles[start-1].selected==0){
             tiles[start-1].dist = d;
             tiles[start-1].prev = start;
         }
        }
        if(tiles[start+1].type!=1) {
            d = cost + tiles[start].dist;
            if(d<tiles[start+1].dist&&tiles[start+1].selected==0){
             tiles[start+1].dist = d;
             tiles[start+1].prev = start;
         }
        }
        if(start==p1){
             d = cost + tiles[start].dist;
            if(d<tiles[p2].dist&&tiles[p2].selected==0){
             tiles[p2].dist = d;
             tiles[p2].prev = start;
         }
            }
        if(start==p2){
             d = cost + tiles[start].dist;
            if(d<tiles[p1].dist&&tiles[p1].selected==0){
             tiles[p1].dist = d;
             tiles[p1].prev = start;
         }
            }
        if(start==target){
            d = cost + tiles[start].dist;
            if(d<tiles[target2].dist&&tiles[target2].selected==0){
             tiles[target2].dist = d;
             tiles[target2].prev = start;
         }
            }
        for(i=0;i<2*N*N;i++)
        {
            if(min>tiles[i].dist&&tiles[i].selected==0&&tiles[i].type!=1)
            {
                min = tiles[i].dist;
                m = i;
            }
        }
        start = m;
    }
        start = target;
        j =0;
        while(start!=-1){
            path[j++] = (start);
            start = tiles[start].prev;
            }
    printf("\nThe quickest path is:\n\n");
    for(i=0;i<N;i++){
        for(j=0;j<N+1;j++){
            if(tiles[convert(i,j)].type==1&&j<N)
                printf("X ");
            else if(convert(i,j)==source)printf("S ");
            else if(convert(i,j)==target)printf("E ");
            else if(tiles[convert(i,j)].type==2){ 
                if(arrCheck(path,tiles[target].dist,convert(i,j))==1)
                printf("G ");
                else printf("g ");
                }
            else if(convert(i,j)==p1||convert(i,j)==p2){
                 if(arrCheck(path,tiles[target].dist,convert(i,j))==1)
                printf("P ");
                else printf("p ");
                }
            else if(arrCheck(path,tiles[target].dist,convert(i,j))==1)
                printf(". ");
            else {if(j<N) printf("_ ");}
            }
            printf("\n");
        }
   return tiles[target2].dist-1;
    }