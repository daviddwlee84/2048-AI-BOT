#include "2048.h"
#include <ctime>
#ifdef _WIN32
#include <conio.h>
#elif defined(__linux__)
#include <termios.h>
#include <unistd.h>
#include <stdio.h>
int getch(void){
    struct termios oldattr, newattr;
    int ch;
    tcgetattr( STDIN_FILENO, &oldattr );
    newattr = oldattr;
    newattr.c_lflag &= ~( ICANON | ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &newattr );
    ch = getchar();
    tcsetattr( STDIN_FILENO, TCSANOW, &oldattr );
    return ch;
}
#endif

const char* dirToStr(dir_e dir){
    if(dir == LEFT)   return "LEFT   ";
    if(dir == DOWN)   return "DOWN   ";
    if(dir == RIGHT)  return "RIGHT  ";
    if(dir == UP)     return "UP     ";
    return "INVALID";
}

// Exmaple1. Original game played with AWSD keys
dir_e getDirFromKeyboard(){
    int dir;
    dir = getch();
    
    if(dir == 'A' || dir == 'a')  return LEFT;
    if(dir == 'S' || dir == 's')  return DOWN;
    if(dir == 'D' || dir == 'd')  return RIGHT;
    if(dir == 'W' || dir == 'w')  return UP;
    return INVALID;
}

// Exmaple2. Random direction inserting bot
dir_e getRandDir(){
    int dir = rand()%4;
    
    if(dir == 0)  return LEFT;
    if(dir == 1)  return DOWN;
    if(dir == 2)  return RIGHT;
    if(dir == 3)  return UP;
    return INVALID;
}

void PlayNRounds(int n){
#ifdef _WIN32
    system("cls");
#elif defined(__linux__)
    system("clear");
#endif
    Game myGame;
    bool isGameOver;
    dir_e dir;

    gotoXY(5,0);
    std::cout<<"Previous";
    gotoXY(35,0);
    std::cout<<"Current";
    myGame.printGrid(35,2);

    for(int i = 0;i < n;i++){    
        isGameOver = false;
        while(!isGameOver){
            while((dir = getDirFromKeyboard()) == INVALID);
            
            gotoXY(5,10);
            std::cout<<dirToStr(dir);
            myGame.printGrid(5,2);
          
            myGame.insertDirection(dir);
            isGameOver = myGame.isGameOver();
            
            myGame.printGrid(35,2);
            gotoXY(0,15); 
            printf("  Round:    %d      \n", i+1);
            printf("  Score:    %d      \n", myGame.getScore());
            printf("  Max Score: %d      \n", myGame.getMaxScore());

        }
        myGame.printGrid(35,2);
        if(i < n - 1)  myGame.reset();
    }
}

int main(int argc, char* argv[]){
    PlayNRounds(100);
    return 0;
}
