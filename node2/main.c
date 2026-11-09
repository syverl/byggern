#include "game.h"


int main(){
    game_play();
    printf("You lost!\n\r");   // \n\r tømmer bufferen
    while (1) {}               // ikke returner fra main på en mikrokontroller
}