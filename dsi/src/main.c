// SPDX-License-Identifier: GPL-2.0-or-later

#include <stdio.h>
#include <stdbool.h>

#include <netinet/in.h>

#include <nds.h>
#include <dswifi9.h>

// Bitmaps

volatile int frame = 0;

void Vblank() {
    frame++;
}

void OnKeypress(int key) {
    if (key > 0)
        iprintf("%c", key);
}

int main() {
    // Declare common variables
    touchPosition touchXY;

    // Initialize irq handlers
    irqSet(IRQ_VBLANK, Vblank);

    // Initialize displays
    PrintConsole sctop;
    PrintConsole scbot;
    videoSetMode(MODE_0_2D);
    videoSetModeSub(MODE_5_2D);
    vramSetBankA(VRAM_A_MAIN_BG);
    vramSetBankC(VRAM_C_SUB_BG);
    consoleInit(&sctop, 3, BgType_Text4bpp, BgSize_T_256x256, 31, 0, true, true);
    consoleInit(&scbot, 1, BgType_Text4bpp, BgSize_T_256x256, 31, 0, false, true);
    consoleSelect(&sctop);

    // Initialize keyboard, if necessary
    // NOTE: need to reinitialize the keyboard every time we do image display
    Keyboard *kbd = keyboardInit(NULL, 1, BgType_Text4bpp, BgSize_T_256x512, 20, 0, false, true);
    kbd->OnKeyPressed = OnKeypress;

    // Initialize bitmap displays
    int bg_id = bgInitSub(3, BgType_Bmp16, BgSize_B16_256x256, 0, 0);
    decompress(test_bgBitmap, BG_GFX_SUB, LZ77Vram);

    //consoleDemoInit();
    //iprintf("Compiled with devkitNix\n");
    //iprintf("Connecting via WFC data...\n");
    //if (!Wifi_InitDefault(WFC_CONNECT)) {
    //    iprintf("Connection failed.");
    //} else {
    //    iprintf("Connected! ");
    //    ip = Wifi_GetIPInfo(&gateway, &inil, &inil, &inil);
    //    iprintf("ip=%s,gateway=%s", inet_ntoa(ip), inet_ntoa(gateway));
    //}

    while (1) {
        swiWaitForVBlank();
        scanKeys();
        if (keysDown() & KEY_A)
            break;
    }
    bgHide(bg_id);
    memset(BG_GFX_SUB, 0, 131072);
    kbd = keyboardInit(NULL, 1, BgType_Text4bpp, BgSize_T_256x512, 20, 0, false, true);
    kbd->OnKeyPressed = OnKeypress;
    char buf[256];
    iprintf("Enter text: ");
    scanf("%s", buf);
    iprintf("\nEntered: %s\n", buf);

    while (true) {
        // Avoid running like crazy through DSi power by waiting for the next frame
        swiWaitForVBlank();

        scanKeys();
        if (keysDown() & KEY_START)
            break;
        touchRead(&touchXY);

        //iprintf("\x1b[12;0HFrame = %d", frame);
        //iprintf("\x1b[14;0HTouch x = %04X, %04X\n", touchXY.rawx, touchXY.px);
        //iprintf("Touch y = %04X, %04X\n", touchXY.rawy, touchXY.py);
    }
    return 0;
}
