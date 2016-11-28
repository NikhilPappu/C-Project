#include <stdio.h>
#include <string.h>
#include <math.h>
#define IN 999
int N = 100;
struct tile {
    int hound_dist;
    int sourceDist;
    int targetDist;
    int selected;
    int type;
    int prev;
    };
int arrCheck(int arr[],int n,int key){
    int i;
    for(i=n-2;i>=0;i--){
     if(arr[i]==key) return 1;   
    }
    return 0;
    }
int convert(int i,int j){
    return N*i + j;
    }

 void printAllPaths(int u,int d, int path[],struct tile tiles[N*N],int path_index,int min[],int *minlen,int target,int minHoundDist,int minExitDist)    //changed
 {
        int i;
        int houndToExit = tiles[target].hound_dist;
        tiles[u].selected = 1;
        path[path_index] = u;
        path_index++;
     
        if(tiles[u].hound_dist<=minHoundDist)                       //changed
        {
            minHoundDist=tiles[u].hound_dist;
            minExitDist = path_index-1;
        }
     
         if(path_index-1>houndToExit || path_index-1>=(minHoundDist-tiles[u].sourceDist+ceil(minExitDist/2)))       //changed
         {
            path_index--;
            tiles[u].selected=0;
            return;
        }
        else if(u==d)
            {
                if(path_index<*minlen || *minlen==0){
                    
                    for(i=path_index-1;i>=0;i--){
                        min[i] = path[i];
                        }
                    *minlen = path_index+1;
                    }
                    
                for(i=path_index-1;i>=0;i--)
                {
                    printf("(%d,%d) ",path[i]/N,path[i]%N);
                }
                printf("\n\n");
            }
        else
            {
                if( tiles[u-N].type!=1 && tiles[u-N].selected==0)
                {
                    printAllPaths(u-N,d,path,tiles,path_index,min,minlen,target,minHoundDist,minExitDist);
                }
                 if( tiles[u+N].type!=1 && tiles[u+N].selected==0)
                {
                    printAllPaths(u+N,d,path,tiles,path_index,min,minlen,target,minHoundDist,minExitDist);
                }
                 if( tiles[u-1].type!=1 && tiles[u-1].selected==0)
                {
                    printAllPaths(u-1,d,path,tiles,path_index,min,minlen,target,minHoundDist,minExitDist);
                }
                 if( tiles[u+1].type!=1 && tiles[u+1].selected==0)
                {
                    printAllPaths(u+1,d,path,tiles,path_index,min,minlen,target,minHoundDist,minExitDist);
                }
            }
path_index--;
tiles[u].selected=0;
}

void houndDistance(int queue[],struct tile tiles[N*N],int hound_start){
    int pos1 = 0,pos2 = 0;
    queue[0] = hound_start;
    while(pos1<=pos2)
        {
        int u = queue[pos1];
        if( tiles[u-N].type!=1 && tiles[u-N].hound_dist==IN)
                {
                    pos2++;
                    tiles[u-N].hound_dist = tiles[u].hound_dist+1;
                   queue[pos2] = u-N;
                }
        if( tiles[u+N].type!=1 && tiles[u+N].hound_dist==IN)
                {
                    pos2++;
                    tiles[u+N].hound_dist = tiles[u].hound_dist+1;
                   queue[pos2] = u+N;
                }
        if( tiles[u-1].type!=1 && tiles[u-1].hound_dist==IN)
                {
                    pos2++;
                    tiles[u-1].hound_dist = tiles[u].hound_dist+1;
                   queue[pos2] = u-1;
                }
        if( tiles[u+1].type!=1 && tiles[u+1].hound_dist==IN)
                {
                    pos2++;
                    tiles[u+1].hound_dist = tiles[u].hound_dist+1;
                   queue[pos2] = u+1;
                }
        pos1++;
   
    }
}
void sourceDistance(int queue[],struct tile tiles[N*N],int source){
    int pos1 = 0,pos2 = 0;
    queue[0] = source;
    while(pos1<=pos2)
        {
        int u = queue[pos1];
        if( tiles[u-N].type!=1 && tiles[u-N].sourceDist==IN)
                {
                    pos2++;
                    tiles[u-N].sourceDist = tiles[u].sourceDist+1;
                   queue[pos2] = u-N;
                }
        if( tiles[u+N].type!=1 && tiles[u+N].sourceDist==IN)
                {
                    pos2++;
                    tiles[u+N].sourceDist = tiles[u].sourceDist+1;
                   queue[pos2] = u+N;
                }
        if( tiles[u-1].type!=1 && tiles[u-1].sourceDist==IN)
                {
                    pos2++;
                    tiles[u-1].sourceDist = tiles[u].sourceDist+1;
                   queue[pos2] = u-1;
                }
        if( tiles[u+1].type!=1 && tiles[u+1].sourceDist==IN)
                {
                    pos2++;
                    tiles[u+1].sourceDist = tiles[u].sourceDist+1;
                   queue[pos2] = u+1;
                }
        pos1++;
   
    }
}
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
    int c;
    display_file("..\\Instructions.txt");
    do {
    N = 100;
    struct tile tiles[N*N];
    int min[1000];
    int minlen = 0;
    int queue[1000];
    int queue2[1000];
    int path[1000];
    int i,j,source,target;
    int hound_start;
    char temp;
    int houndToExit;
    int minHoundDist=IN;                                            //changed
    int minExitDist=0;                                              //changed
    char f[100];
    char f2[100]="..\\";
    for(i=0;i<N*N;i++){
        tiles[i].selected = 0;
        tiles[i].prev = -1;
        tiles[i].type = 0;
        tiles[i].hound_dist = IN;
        tiles[i].sourceDist = IN;
        }
    FILE *fp;
    printf("\nEnter the name of the text file you want to read : \n");
    scanf("%s",f);
    strcat(f2,f);
    fp = fopen(f2,"r");
    fscanf(fp,"%d",&N);
    fgetc(fp);
    for(i=0;i<N;i++){
        for(j=0;j<N+1;j++){
           temp = fgetc(fp);
            if(temp == 'X'&&j<N)
                tiles[convert(i,j)].type = 1;
            else if(temp=='S'&&j<N)source = convert(i,j);
            else if(temp=='E'&&j<N)target = convert(i,j);
            else if(temp=='H'&&j<N)hound_start = convert(i,j);
            
            }
        }
    fclose(fp);
    printf("\n");
     for(i=0;i<N;i++){
        for(j=0;j<N+1;j++){
            if(tiles[convert(i,j)].type==1&&j<N)
                printf("X ");
            else if(convert(i,j)==source)printf("S ");
            else if(convert(i,j)==target)printf("E ");
            else if(convert(i,j)==hound_start)printf("H ");
            else {if(j<N) printf("_ ");}
            }
            printf("\n");
        }
    tiles[hound_start].hound_dist = 0;
    tiles[source].sourceDist = 0;
    houndDistance(queue,tiles,hound_start);
    sourceDistance(queue2,tiles,source);
//    for(i=0;i<N*N;i++){
//        printf("%d=%d\n",i,tiles[i].sourceDist);
//        
//    }
    printf("\nAll the possible paths are : \n\n");
    printAllPaths(target,source,path,tiles,0,min,&minlen,target,minHoundDist,minExitDist);   //changed
    printf("\n");
    
//    for(i=minlen-2;i>=0;i--){
//        printf("(%d,%d) ",min[i]/N,min[i]%N);
//        }
    printf("\nThe shortest valid path is : \n\n");
    for(i=0;i<N;i++){
        for(j=0;j<N+1;j++){
            if(tiles[convert(i,j)].type==1&&j<N)
                printf("X ");
            else if(convert(i,j)==source)printf("S ");
            else if(convert(i,j)==target)printf("E ");
            else if(convert(i,j)==hound_start)printf("H ");
            else if(arrCheck(min,minlen,convert(i,j))==1) printf(". ");
            else {if(j<N) printf("_ ");}
            }
            printf("\n");
        }
    printf("\nEnter Y to continue. Press Enter to exit.\n");
    c = fgetc(stdin);
    }while((c = fgetc (stdin)) != EOF && c != '\n');
    return 0;
    }
