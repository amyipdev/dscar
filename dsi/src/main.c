// SPDX-License-Identifier: GPL-2.0-or-later

#include <stdio.h>
#include <stdbool.h>

#include <nds.h>

volatile int frame = 0;

void Vblank() {
    frame++;
}

int main() {
    // Declare common variables
    touchPosition touchXY;

    // Initialize irq handlers
    irqSet(IRQ_VBLANK, Vblank);

    consoleDemoInit();
    iprintf("Compiled with devkitNix");

    while (true) {
        // Avoid running like crazy through DSi power by waiting for the next frame
        swiWaitForVBlank();

        scanKeys();
        if (keysDown() & KEY_START)
            break;
        touchRead(&touchXY);
        iprintf("\x1b[10;0HFrame = %d", frame);
        iprintf("\x1b[16;0HTouch x = %04X, %04X\n", touchXY.rawx, touchXY.px);
        iprintf("Touch y = %04X, %04X\n", touchXY.rawy, touchXY.py);
    }
    return 0;
}
