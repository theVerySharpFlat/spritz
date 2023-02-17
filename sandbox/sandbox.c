#include <stdio.h>

#include <spritz.h>
#include <spritz_window.h>

int main() {
    SpritzWindow_t window = spritzCreateWindow(680, 420, "Spritz!");

    while(!spritzWindowShouldClose(window)) {
        if(spritzWindowIsKeyPressed(window, SPRITZ_KEY_Q)) {
            printf("Q was pressed!\n");
        } else {
            printf("Q was not pressed!\n");
        }

        spritzUpdateWindows();
    }
}
