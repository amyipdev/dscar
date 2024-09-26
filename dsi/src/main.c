// SPDX-License-Identifier: GPL-2.0-or-later

#include <stdio.h>
#include <stdbool.h>

#include <netinet/in.h>

#include <nds.h>
#include <dswifi9.h>

volatile int frame = 0;

void Vblank() {
    frame++;
}

int main() {
    // Declare common variables
    touchPosition touchXY;
    struct in_addr ip, gateway, inil;

    // Initialize irq handlers
    irqSet(IRQ_VBLANK, Vblank);

    consoleDemoInit();
    iprintf("Compiled with devkitNix\n");
    iprintf("Connecting via WFC data...\n");
    if (!Wifi_InitDefault(WFC_CONNECT)) {
        iprintf("Connection failed.");
    } else {
        iprintf("Connected! ");
        ip = Wifi_GetIPInfo(&gateway, &inil, &inil, &inil);
        iprintf("ip=%s,gateway=%s", inet_ntoa(ip), inet_ntoa(gateway));
    }

    while (true) {
        // Avoid running like crazy through DSi power by waiting for the next frame
        swiWaitForVBlank();

        scanKeys();
        if (keysDown() & KEY_START)
            break;
        touchRead(&touchXY);
        iprintf("\x1b[12;0HFrame = %d", frame);
        iprintf("\x1b[14;0HTouch x = %04X, %04X\n", touchXY.rawx, touchXY.px);
        iprintf("Touch y = %04X, %04X\n", touchXY.rawy, touchXY.py);
    }
    return 0;
}
