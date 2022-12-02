#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define WIDTH 80 // Size of window
#define HEIGHT 80
#define FRICTION 30 // probability that sand will not fall into an empty cell underneath, when surrounded by sand on both sides ( in percents )

typedef enum { empty, sand, water , wall } TYPE;

typedef struct cell{
    TYPE type;
}Cell;

Cell buf1[HEIGHT][WIDTH];
Cell buf2[HEIGHT][WIDTH];

int frame = 0;

int printMessage = 0;

void ProcessSand(int x, int y){
    if( y+1 >= HEIGHT ) return; // has nowhere to fall
    if(buf1[y+1][x].type == empty){ // Falling straight down
        if(x+1 >= WIDTH || buf1[y][x+1].type != sand ){ 
            buf2[y][x].type = empty;
            buf2[y+1][x].type = sand;
        }else if(x-1 < 0 || buf1[y][x-1].type != sand){ 
            buf2[y][x].type = empty;
            buf2[y+1][x].type = sand;
        }else if(x-1 >= 0 && x+1 < WIDTH && buf1[y][x-1].type == sand && buf1[y][x+1].type == sand){ // Friction on both sides
            if(rand()%100 > FRICTION){
                buf2[y][x].type = empty;
                buf2[y+1][x].type = sand;
            }
        }
    }else if(buf1[y+1][x].type == sand){ // Tumbling from a grain of sand sideways
        if(rand()%100 <= 50){ // Randomness to which side the sand will fall
            if(x+1 < WIDTH && buf1[y][x+1].type == empty && buf1[y+1][x+1].type == empty){
                buf2[y][x].type = empty;
                buf2[y+1][x+1].type = sand;
            }else if(x-1 > 0 && buf1[y][x-1].type == empty && buf1[y+1][x-1].type == empty ){
                buf2[y][x].type = empty;
                buf2[y+1][x-1].type = sand;
            }
        }else{
            if(x-1 > 0 && buf1[y][x-1].type == empty && buf1[y+1][x-1].type == empty ){
                buf2[y][x].type = empty;
                buf2[y+1][x-1].type = sand;
            }else if(x+1 < WIDTH && buf1[y][x+1].type == empty && buf1[y+1][x+1].type == empty){
                buf2[y][x].type = empty;
                buf2[y+1][x+1].type = sand;
            }
        }
    }
}


void Process(){
   // printf("Frame %d \n",frame);
    //memset(buf2, 0, sizeof(buf2))
    memcpy(buf2,buf1,sizeof(buf1));
    for(int x = 0; x < WIDTH; x++){
        for(int y = 0; y < HEIGHT; y++){
            switch(buf1[y][x].type){
                case sand:
                    ProcessSand(x,y);
                break;
                case water:
                    if(buf2[y+1][x].type == empty){
                        buf2[y][x].type = empty;
                        buf2[y+1][x].type = water;
                        if(frame > 75)
                            printMessage = 1;
                    }
                    else if( x-1 >= 0 && buf2[y+1][x-1].type == empty){
                        buf2[y][x].type = empty;
                        buf2[y+1][x-1].type = water;
                        if(frame > 75)
                            printMessage = 1;
                    }
                    else if( x+1 < WIDTH && buf2[y+1][x+1].type == empty  ){
                        buf2[y][x].type = empty;
                        buf2[y+1][x+1].type = water;
                        if(frame > 75)
                            printMessage = 1;
                    }else if( x-1 >= 0 && buf2[y][x-1].type == empty ){
                        buf2[y][x].type = empty;
                        buf2[y][x-1].type = water;

                    }else if(x+1 < WIDTH && buf2[y][x+1].type == empty ){
                        buf2[y][x].type = empty;
                        buf2[y][x+1].type = water;
                    }
                break;
                case wall:
                    buf2[y][x].type = wall;
                break;
            }

        }
    }
    memcpy(buf1,buf2,sizeof(buf1));

}

void Draw(){
    printf("\E[H\E[2J"); // Clear termimnal
    for(int y = 0; y < WIDTH; y++){
        for(int x = 0; x < HEIGHT; x++){
            Cell currCell = buf1[y][x];

            if( currCell.type == sand ){
                printf("\e[43m ");
            }else if(currCell.type == wall){
                printf("\e[47m ");
            }else if(currCell.type == water){
                printf("\e[44m ");
            }else{
                printf("\e[40m ");
            }
        }
        /*
        printf("\t");
        for(int x = 0; x < HEIGHT; x++){
            Cell currCell = buf2[y][x];

            if( currCell.type == sand ){
                printf("\e[0;33mO");
            }else if(currCell.type == wall){
                printf("\e[0;37m#");
            }else if(currCell.type == water){
                printf("\e[0;34mX");
            }else{
                printf("\e[0;30.");
            }
        }*/
        printf("\e[40m\n");
    }
}

void PlatformsBuild(){
    for(int i = 0; i < WIDTH; i++){
        buf1[HEIGHT - 1][i].type = wall;
        if( i < WIDTH / 3){
            buf1[HEIGHT/2][i].type = wall;
        }
        if (i < WIDTH / 2){
            buf1[2* (HEIGHT/3)][i].type = wall;

        }
        if( WIDTH-(WIDTH/5) < i && i < WIDTH){
            buf1[2 * (HEIGHT / 3)][i].type = wall;
        }
    }
    
    buf1[2 * (HEIGHT / 3) - 1 ][WIDTH-(WIDTH/5)].type = wall;
    buf1[2 * (HEIGHT / 3) - 2 ][WIDTH-(WIDTH/5)].type = wall;
}

void PlatformsTick(){
    if(frame%2 == 0){
        if(frame < 700)
            buf1[0][WIDTH/4].type = sand;
        if(frame > 400 & frame < 800)
            buf1[0][WIDTH-(WIDTH/7)].type = sand;    
    }
    /*
    if(frame % 1 == 0){
        if(frame < 80){
            buf1[0][WIDTH - 5].type = water;
            buf1[0][45].type = water;
        }
        if(frame <200)
            buf1[0][WIDTH / 2].type = water;
    }
    */
}

void HourglassBuild(){
    int y = 0;
    int x = 0;
    for(; x <= WIDTH/2 - (WIDTH/10) && y <= HEIGHT/2; x++){
        buf1[y-1][x].type = wall;
        buf1[y][x].type = wall;
        buf1[HEIGHT - y-1][x].type = wall;
        buf1[HEIGHT - y][x].type = wall;
        // right hand side :
        buf1[y-1][WIDTH - x].type = wall;
        buf1[y][WIDTH - x].type = wall;
        buf1[HEIGHT - y-1][WIDTH - x].type = wall;
        buf1[HEIGHT - y][WIDTH - x].type = wall;
        y++;
    }
    // middle tunnel
    int endOfTunel = HEIGHT - y;
    int startOfTunel = y;
    for(;y < endOfTunel; y++){
        buf1[y][x].type = wall;
        buf1[y][WIDTH-x].type = wall;
    }
    // top and bottom
    for(int x = 0; x < WIDTH; x++){
        buf1[0][x].type = wall;
        buf1[HEIGHT-1][x].type = wall;
    }
    //sand
    int xOffset = 2;
    for(int y = 1; y < startOfTunel; y++){   
        for(int x = xOffset; x < WIDTH - xOffset; x++){
            buf1[y][x].type = sand;
        }
        xOffset++;
    }
    
}

void HourglassTick(){

}

typedef enum {platforms, hourglass} worldtype;

int main(int argc, char **argv){

    worldtype type = hourglass;

    if(argc > 1){
        if(strcmp(argv[1], "hourglass") == 0){
            type = hourglass;
        }
    }
    
    switch(type){
        case platforms:
            PlatformsBuild();
        break;
        case hourglass:
            HourglassBuild();
        break;
        default:
            PlatformsBuild();
        break;
    }
    

    while(1){
        
        switch(type){
            case platforms:
                PlatformsTick();
            break;
            case hourglass:
                HourglassTick();
            break;
            default:
                PlatformsTick();
            break;
        }
        
        frame++;
        Draw();
        usleep(30000);
        Process();
        if(printMessage == 1){
            printf("\n went left \n");
            //printMessage = 0;
        }
    }
    return 0;
}