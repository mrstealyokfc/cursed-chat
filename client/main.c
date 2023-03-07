#include<ncurses.h>

//creator: null
//date: Tuesday III/VII/MMXXIII

int main(){
    initscr();
    raw();
    keypad(stdscr, TRUE);

    printw("message");

}
