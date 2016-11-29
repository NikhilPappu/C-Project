#include <stdio.h>
#include <string.h>
#include <math.h>
#define IN 99 //infinity value
int N = 100;
struct tile {  //A structure which represents each tile on the maze.
    int dist;  //Stores the distance of a tile from the start location.
    int selected; //selected = 0 if the tile is not selected and = 1 if it is.
    int type; // Different values for different tile types (wall, gum, portal etc)
    int prev; // Stores an int corresponding to the prev tile on the path
    };
int convert(int i,int j){
    return N*i + j; // converts coordinates (i,j) into a corresponding number from 1 to N^2.
    }
int arrCheck(int arr[],int n,int key){ // Function to search an element in an array.
    int i;
    for(i=0;i<n;i++){
     if(arr[i]==key) return 1;   
    }
    return 0;
    }
int dijkstra(struct tile tiles[2*N*N],int source,int target);  // Dijkstra function protoype.

int p1=0,p2=0; // Portal indices
  
typedef struct heapNode {
    int value;
    int index;
} heapNode;
 
typedef struct PriorityQueue { // Structure for a priority queue. Contains a heapNode pointer and queue size.
    heapNode* heap;
    int size;
} PriorityQueue;
 
void insert(heapNode aNode, heapNode* heap, int size) { /*Insert function is used to build the priority queue. It is called by the enqueue function.
 * It inserts a heapNode at the and of the heap and moves it upward using repeated swaps until it is in the right position.  
 * */
    int hIndex;
    heapNode temp;
    hIndex = size + 1;
    heap[hIndex] = aNode;
    while (heap[hIndex].value < heap[hIndex/2].value && hIndex > 1) {
    temp = heap[hIndex];
    heap[hIndex] = heap[hIndex/2];
    heap[hIndex/2] = temp;
    hIndex /= 2;
    }
}
 
void maintainHeap(heapNode* heap, int size, int hIndex) { /*
 * Called to maintain the heap property after the min(heap[1]) is extracted.
 * */
    int chIndex;        //index for child
    heapNode temp;
    for (;;) {
        chIndex = hIndex*2;
        if (chIndex > size) {
            break;   //no child
        }
        if (chIndex < size) {
            if (heap[chIndex].value > heap[chIndex+1].value) {
                ++chIndex;
            }
        }
        //swap if necessary
        if (heap[chIndex].value < heap[hIndex].value) {
            temp = heap[chIndex];
            heap[chIndex] = heap[hIndex];
            heap[hIndex] = temp;
            hIndex = chIndex;
        } else {
            break;
        }
    }
}
 
heapNode removeMin(heapNode* heap, int size) {
    /*Removes the first element in the heap by swapping it with the last element and decrementing the queue size.
     * It then calls the maintainHeap function to place heap[1] in the correct place.
     * */
    int chIndex;
    heapNode retVal = heap[1];
    heap[1] = heap[size];
    --size;
    maintainHeap(heap, size, 1);
    return retVal;
}
void enqueue(heapNode node, PriorityQueue *q) {
    //Constrcuts the queue by calling the insert function and incrementing size.
    insert(node, q->heap, q->size);
    ++q->size;
}
 
heapNode dequeue(PriorityQueue *q) {
    //removes the first element(min) in the heap and decrements the size.
   heapNode retVal = removeMin(q->heap, q->size);
   --q->size;
   return retVal; 
}
 
void initQueue(PriorityQueue *q, int n) {
//initialize the queue by creating a heap by dynamically allocating memory for n heap nodes
   q->size = 0;
   q->heap = (heapNode*)malloc(sizeof(heapNode)*(n+1));
}
 
static void display_file(const char *file_name)
{
    //Function to display the entire contents of a file.
    //Used to display the instructions from Instructions.txt
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
    struct tile tiles[2*N*N]; // An array of tile structures
    int i,j,source,target,time;
    char temp;
    char f[100];
    char f2[100]="..\\";
    for(i=0;i<2*N*N;i++){
        //Initialize the structure attributes for all the N^2 tiles.
        tiles[i].dist = IN;
        tiles[i].selected = 0;
        tiles[i].prev = -1;
        tiles[i].type = 0;
        //type: empty = 0 ;wall = 1; gum = 2;
        }
    FILE *fp;
    printf("\nEnter the name of the text file you want to read : \n");
    scanf("%s",f);
    strcat(f2,f);
    fp = fopen(f2,"r");
    fscanf(fp,"%d",&N);
    fgetc(fp);
    for(i=0;i<N;i++){ //Store different values in the 'type' member of structure tile.
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
    time = dijkstra(tiles,source,target); /* 
     * Dijkstra function calculates the fastest path and returns the time taken to traverse that path*/
    printf("\nThe time taken is %d seconds\n",time);
    printf("\nEnter Y to continue. Press Enter to exit.\n");
    c = fgetc(stdin);
    }while((c = fgetc (stdin)) != EOF && c != '\n'); // Run the program multiple times
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
    PriorityQueue q;
    heapNode hn;
    initQueue(&q, 2*N*N); // Initialize the priority queue.
    
    while(tiles[target2].selected==0){ //The loop runs until the target tile is selected.
    
    tiles[start].selected = 1; 
    if(tiles[tiles[start].prev].type==2||tiles[tiles[tiles[start].prev].prev].type==2){
      //Cost(time) doubles for next 2 turns after treading on a 'gum' tile.
      cost = 2;
       }
    else cost = 1;
        min = IN; //min of distances of non selected tiles from the source
        m = 0; // Index of the min
        
        //4 if statements to enqueue the 4 adjacent tiles of the current tile.
        if(tiles[start-N].type!=1) { //Checking if the tile is a wall or not.
            d = cost + tiles[start].dist; /*
            dist of a tile from the source is the prev tile distance + cost*/
            if(d<tiles[start-N].dist&&tiles[start-N].selected==0){ /*
        * Change distance and prev if the current path is smaller than the smallest path discovered until now.*/
        
             tiles[start-N].dist = d;
             tiles[start-N].prev = start;
             //Enqueue the adjacent tiles onto the queue.
             hn.value = d;
             hn.index = start - N;
             enqueue(hn,&q);
         }
        }
        if(tiles[start+N].type!=1) {
            d = cost + tiles[start].dist;
            if(d<tiles[start+N].dist&&tiles[start+N].selected==0){
             tiles[start+N].dist = d;
             tiles[start+N].prev = start;
             hn.value = d;
             hn.index = start + N;
             enqueue(hn,&q);
         }
                 }
         if(tiles[start-1].type!=1) {
            d = cost + tiles[start].dist;
            if(d<tiles[start-1].dist&&tiles[start-1].selected==0){
             tiles[start-1].dist = d;
             tiles[start-1].prev = start;
             hn.value = d;
             hn.index = start - 1;
             enqueue(hn,&q);
         }
        }
        if(tiles[start+1].type!=1) {
            d = cost + tiles[start].dist;
            if(d<tiles[start+1].dist&&tiles[start+1].selected==0){
             tiles[start+1].dist = d;
             tiles[start+1].prev = start;
             hn.value = d;
             hn.index = start + 1;
             enqueue(hn,&q);
         }
        }
        if(start==p1){
            //Portal Code
             d = cost + tiles[start].dist;
            if(d<tiles[p2].dist&&tiles[p2].selected==0){
             tiles[p2].dist = d;
             tiles[p2].prev = start;
             hn.value = d;
             hn.index = p2;
             enqueue(hn,&q);
         }
            }
        if(start==p2){
            //Portal Code
             d = cost + tiles[start].dist;
            if(d<tiles[p1].dist&&tiles[p1].selected==0){
             tiles[p1].dist = d;
             tiles[p1].prev = start;
             hn.value = d;
             hn.index = p1;
             enqueue(hn,&q);
         }
            }
        if(start==target){
            d = cost + tiles[start].dist;
            if(d<tiles[target2].dist&&tiles[target2].selected==0){
             tiles[target2].dist = d;
             tiles[target2].prev = start;
             hn.value = d;
             hn.index = target2;
             enqueue(hn,&q);
         }
            }
     /*Find the min in every run of the while loop by 
       dequeueing from the prioty queue.
       min is the min distance of all the non selected tile distances from the source. */
     hn = dequeue(&q);
     min = hn.value; // Set the min and the min index and run the loop again by selecting the min distance tile
     m = hn.index;
     start = m; //start is now the min distance tile.
    } 
        //We now have the path after the while loop ended.
        
        start = target;
        j =0;
        while(start!=-1){
            /*We now get back the path traversed by looking at the prev values of each tile 
              starting from the target(end) tile until we reach the initial tile.*/
            path[j++] = (start);
            start = tiles[start].prev;
            }
    printf("\nThe quickest path is:\n\n");
    //Display the fastest path by printing the maze with '.'s representing the path.
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
                 if(arrCheck(path,tiles[target].dist,p1)==1&&arrCheck(path,tiles[target].dist,p2)==1)
                printf("P ");
                else printf("p ");
                }
            else if(arrCheck(path,tiles[target].dist,convert(i,j))==1)
                printf(". ");
            else {if(j<N) printf("_ ");}
            }
            printf("\n");
        }
   //Return the time taken to traverse the fastest path.
   return tiles[target2].dist-1;
    }
