#include <stdio.h>
#include <stdlib.h>

void maze_init (int list_len, int* block_list);
void print_matrix (void);
void Lee (int list_len, int *stationinput);

int maze[13][13];
int *stations[12];
int *crossings[5][5];
int **update_array;
int **update_array_new;
int *route;

const int nr_of_stations = 2;

/* experimental
typedef struct statcross{
    int loci;   /*row*/
    /*int locj;   /*column*/
    /*struct statcross *right;
    struct statcross *left;
    struct statcross *above;
    struct statcross *below;
}statcross;*/


int main(int argc, char const *argv[]) {
    int input_len;
    int *input_list; /*list with blocked edges*/
    int *stationinput;   /*list with stations to visit, beginning at station 0*/
    int n;  /*temporary variable*/

    /*first input, gives input list length*/
    scanf("%i",&input_len);

    /*each edge has 3 array elements*/
    input_list = (int*)calloc(3*input_len, sizeof(int));
    stationinput = (int*)calloc(input_len, sizeof(int));

    /*reads input_list*/
    for(n=0; n<(3*input_len); ++n) {
        /*reads the characters*/
        if(!((n+1)%3)) {
            /*reads \n*/
            getchar();
            /*reads needed char*/
            input_list[n] = (int)getchar();
        }
        /*reads the integers*/
        else {
            scanf("%i",&input_list[n]);
        }
    }

    /*reads stations*/
    for(n=0; n<nr_of_stations; ++n) {
        scanf("%i",&stationinput[n]);
    }

    maze_init(input_len, input_list);
    print_matrix();
    
    Lee(input_len,stationinput);
    print_matrix();
    
    //print route

    free(input_list);
    free(stationinput);
    /*free(maze);
    free(stations);
    free(crossings); should all not be needed*/
    free(route);
    return 0;
}

void Lee (int list_len, int *stationinput) {
    int count = 1;
    int m=0;        /*keeps track of index in update_array*/
    int p=0;        /*keeps track of index in update_array_new*/
    
    update_array = (int**)calloc(30,sizeof(int*)); /*what size? 20 is maximum I found, 30 is for security*/
    update_array_new = (int**)calloc(30,sizeof(int*));
    
    update_array[0] = stations[stationinput[nr_of_stations-1]-1];
    *update_array[0] = count;
    
    
    /*expand fase*/
    while(*stations[stationinput[0]-1]==0) {
        ++count;                                             /*gives higher value to consecutive neighbours*/
        while(update_array[m]!=NULL) {
            
            /*right neighbour*/
            if(*(update_array[m]+1)==0) {                    /*update_array[m] contains current neighbours of the location of last loop*/
                *(update_array[m]+1) = count;                /*gives higher value to the neighbour*/
                update_array_new[p] = update_array[m]+1;     /*makes a new update_array with neighbours to 'update' for next loop*/
                p++;                                         /*increases index for the new update array*/
            }
            
            /*left neighbour*/
            if(*(update_array[m]-1)==0) {
                *(update_array[m]-1) = count;
                update_array_new[p] = update_array[m]-1;
                p++;
            }
            
            /*above neighbour*/
            if((*(update_array[m]-13)==0)&&(update_array[m]-13>*maze)) {      /*one * still gives base adress of maze*/
                *(update_array[m]-13) = count;
                update_array_new[p] = update_array[m]-13;
                p++;
            }
            
            /*below neighbour*/
            if((*(update_array[m]+13)==0)&&(update_array[m]+13<*maze+169)) {
                *(update_array[m]+13) = count;
                update_array_new[p] = update_array[m]+13;
                p++;
            }
            
            m++;
        }
        
        
        /*makes update_array ready for transfer of update_array_new, also makes m=0*/
        while(m>0) {
            update_array[m-1] = NULL;
            m--;
        }
        
        
        /*transfers update_array_new to update_array, and empties it for new content*/
        while(p>0) {
            update_array[p-1] = update_array_new[p-1];
            update_array_new[p-1] = NULL;
            p--;
        }
        
        if(count>1000) {                /*arbitrary number of counts*/
            fputs("Error! No path was found between these stations!\n",stderr);
            exit(1);
        }
        
    }
    
    /*
     .make a loop inside the for loop. Have **update_array that stores all neigbouring available adresses. Let the loop loop through
     all the adresses in the array and update the array with the available neighbours of all those adresses.
     */
    /* also implementation possible with struct then?
     left, update U, check neighbours C, set flag S, right(back), right, UCS, left(back), above, UCS, below(back), below, UCS, above(back).
     */
    

    /*
    i = 1;      (Expand)
    Mark target cell with value i
    while (start cell not yet marked with value > 0) {
    Mark all neighbor cells of cells marked with i, with i+1,
    if they have a value 0.
    i = i+1
    }
    Go to the start cell        (Trace back)
    while (target cell not yet reached) {
    Go to a neighbor cell that has a lower value.
    }
    */

    /*as output the route global pointer has to be allocated and given a value*/
    free(update_array);
    free(update_array_new);
}

void maze_init (int list_len, int* block_list) {
    
    /*temporary variables*/
    int i=0, j=0, k=0, m=0;
    
    /*default matrix filler*/
    for(i=0;i<13;i++) {
        for(j=0;j<13;j++) {
            /*Unblocked edges between terminals 1-9, 2-8, 3-7, 4-12, 5-11, 6-10*/
            if(i==4||i==6||i==8||j==4||j==6||j==8) {
                maze[i][j] = 0;
            }
            /*Unblocked edges in the square i=2, i=10, j=2, j=10*/
            else if((i==2&&j<11&&j>1)||(i==10&&j<11&&j>1)||(j==2&&i<11&&i>1)||(j==10&&i<11&&i>1)) {
                maze[i][j] = 0;
            }
            /*Blocked points outside the tracks*/
            else {
                maze[i][j] = -1;
            }
        }
    }

    /*blocked edges*/

    /*prints input list, can be removed
    for(i=0; i<(3*list_len); ++i) {
    printf("%i ",block_list[i]);
    }
    printf("\n\n");*/

    for(k=0; k<list_len; ++k) {
        i = 2*block_list[3*k] + 2;            /*row*/
        j = 2*block_list[3*k+1] +2;           /*column*/
        if(block_list[3*k+2]==(int)'s') {     /*chooses edge to south or to east of specified cross point*/
            ++i;                              /*south*/
        }
        else {
            ++j;                              /*east*/
        }
        maze[i][j] = -1;
    }

    /*storing locations of the stations inside an array*/
    stations[0]  = &maze[12][4];    /*location of station 1 in maze*/
    stations[1]  = &maze[12][6];
    stations[2]  = &maze[12][8];
    stations[3]  = &maze[8][12];
    stations[4]  = &maze[6][12];
    stations[5]  = &maze[4][12];
    stations[6]  = &maze[0][8];
    stations[7]  = &maze[0][6];
    stations[8]  = &maze[0][4];
    stations[9]  = &maze[4][0];
    stations[10] = &maze[6][0];
    stations[11] = &maze[8][0];    /*station 12*/
    
    /*prints adresses in stations, + a test, can be removed
    for(i=0;i<12;i++) {
        printf("%p\n",stations[i]);
        if(i==1) {
           printf("%p\n",stations[i]+1);
        }
    }*/
    
    
    /*storing locations of the crossings inside a matrix*/
    for(i=0;i<5;i++) {
        for(j=0;j<5;++j) {
            k = 2+2i;
            m = 2+2j;
            crossings[i][j] = &maze[k][m];
        }
    }
    
    /*prints content of crossings (zeros), can be removed
    for(i=0;i<5;i++) {
        for(j=0;j<5;++j) {
            printf("%i",*crossings[i][j]);
        }
    }*/
    
}

void print_matrix (void) {
    /*temporary variables*/
    int i=0, j=0;
    /*column indication*/
    printf("   ]  1] 2] 3] 4] 5] 6] 7] 8] 9]10]11]12]13\n");
    /*matrix printer*/
    for(i=0;i<13;i++) {
        /*horizontal lines*/
        printf("___]___]__]__]__]__]__]__]__]__]__]__]__]__\n");
        /*row indication*/
        printf("%*i] ",3,i+1);
        for(j=0;j<13;j++) {
           printf("%*i]",2,maze[i][j]);
        }
        putchar('\n');
    }
}
