// SPDX-License-Identifier: GPL-2.0-or-later

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include <netinet/in.h>

#include <nds.h>
#include <dswifi9.h>
#include <netdb.h>

const int MAX_PACKET_SIZE = 256;

typedef int sock;

volatile int sn;
volatile int frame = 0;

void Vblank() {
    frame++;
}

void death() {
    while (true) {
        swiWaitForVBlank();
        scanKeys();
        if (keysDown() & KEY_A)
            break;
    }
}

/*
char *sendHttp(const char *url, const char *request) {
    const struct hostent *host = gethostbyname(url);
    if (!host) {
        iprintf("\nhttp: Couldn't find hostname\n");
        while (1) {
            swiWaitForVBlank();
            scanKeys();
            if (keysDown() & KEY_A)
                break;
        }
    }
    sock s = socket(AF_INET, SOCK_STREAM, 0);
    // drop packet if we can't get a socket yet
    if (s == -1)
        return NULL;
    struct sockaddr_in sain;
    sain.sin_family = AF_INET;
    sain.sin_port = htons(8080);
    sain.sin_addr.s_addr = *((unsigned long *)(host->h_addr_list[0]));
    while (!connect(s, (struct sockaddr *)(&sain), sizeof(sain)));
    iprintf("Sending...\n");
    send(s, request, strlen(request), 0);
    iprintf("Sent.\n");

    int rx_len;
    iprintf("a");
    char *rx = malloc(MAX_PACKET_SIZE);
    iprintf("b");
    while ((rx_len = recv(s, rx, MAX_PACKET_SIZE - 1, 0))) {
        int x = 0;
        iprintf("e%d%d", x, s);
        if (rx_len > 0) {
            iprintf("c");
            rx[rx_len] = 0;
            iprintf("f");
            shutdown(s, 0);
            closesocket(s);
            iprintf("d");
            return rx;
        }
    }
    iprintf("tf????");
    closesocket(s);
    return NULL;
}*/

char *tx(const char *msg) {
    send(sn, msg, strlen(msg), 0);
    int rxl;
    char *rx = malloc(MAX_PACKET_SIZE);
    while ((rxl = recv(sn, rx, MAX_PACKET_SIZE - 1, 0))) {
        if (rxl > 0) {
            rx[rxl] = 0;
            return rx;
        }
    }
    return NULL;
}

int main() {
    // Initialize irq handlers
    irqSet(IRQ_VBLANK, Vblank);
    char *resp;

    // Initialize displays
    PrintConsole sctop;
    PrintConsole scbot;
    videoSetMode(MODE_0_2D);
    videoSetModeSub(MODE_0_2D);
    vramSetBankA(VRAM_A_MAIN_BG);
    vramSetBankC(VRAM_C_SUB_BG);
    consoleInit(&sctop, 3, BgType_Text4bpp, BgSize_T_256x256, 31, 0, true, true);
    consoleInit(&scbot, 3, BgType_Text4bpp, BgSize_T_256x256, 31, 0, false, true);
    consoleSelect(&sctop);

    iprintf("DSCar v0.1\n"
            "(c) 2024 Amy P. & Kyle K.\n"
            "Licensed under GPLv2+\n\n");

    // Wireless procedure
    iprintf("Connecting via WFC data..\n");
    if (!Wifi_InitDefault(WFC_CONNECT)) {
        iprintf("Connection failed...\n");
        death();
        return 0;
    } else {
        iprintf("Connected!\n");
    }

    struct in_addr gw, inil;
    Wifi_GetIPInfo(&gw, &inil, &inil, &inil);
    const char *gws = inet_ntoa(gw);
    iprintf("\n%s\n", gws);

    // Set up network connection
    const struct hostent *host = gethostbyname(gws);
    if (!host) {
        iprintf("http: Couldn't find hostname\n");
        // TODO: dedup
        death();
        return 0;
    }
    GEN_SOCKET:
    sn = socket(AF_INET, SOCK_STREAM, 0);
    if (sn == -1) {
        goto GEN_SOCKET;
    }
    struct sockaddr_in sain;
    sain.sin_family = AF_INET;
    sain.sin_port = htons(8080);
    sain.sin_addr.s_addr = *((unsigned long *)(host->h_addr_list[0]));
    while (!connect(sn, (struct sockaddr *)(&sain), sizeof(sain)));
    if ((resp = tx("100 Hello")) == NULL || strncmp(resp, "101", 3)) {
        iprintf("Car is unavailable...\n");
        death();
        return 0;
    }
    free(resp);
    iprintf("Ready for controlling!\n");

    while (true) {
        swiWaitForVBlank();
        scanKeys();
        const KEYPAD_BITS keys = keysDown();
        if (keys & KEY_START) {
            iprintf("Shutting down...\n");
            resp = tx("202 Kill");
            free(resp);
            /*
            const char *prompt =
                "POST /kill HTTP/1.1\r\n"
                "Host: dscar.local\r\n"
                "User-Agent: nds-dscar\r\n"
                "Accept:*\r\n"
                "\r\n";
            char *r = sendHttp(gws, prompt);
            free(r);*/
            death();
            return 0;
        }
        char *prompt =
            "200 Ctl\n"
            "\0\0\0\0\0";
        const int pl = strlen(prompt);
        itoa(keys, prompt + pl, 10);
        resp = tx(prompt);
        if (resp == NULL) {
            iprintf("Server connection interrupted...\n");
            death();
            return 0;
        }
        if (strncmp(resp, "201", 3)) {
            iprintf("Programming error...\n");
            death();
            return 0;
        }
        free(resp);
        /*char *prompt =
            "POST /ctl HTTP/1.1\r\n"
            "Host: dscar.local\r\n"
            "User-Agent: nds-dscar\r\n"
            "Accept:*\r\n"
            "\r\n"
            "\0\0\0\0\0";*/
        //const size_t pl = strlen(prompt);
        //itoa(keys, prompt + pl, 10);
        //char *r = sendHttp(gws, prompt);
        //free(r);
    }

    /*while (true) {
        // Avoid running like crazy through DSi power by waiting for the next frame
        swiWaitForVBlank();

        scanKeys();
        if (keysDown() & KEY_START)
            break;
        touchRead(&touchXY);

        iprintf("\x1b[12;0HFrame = %d", frame);
        iprintf("\x1b[14;0HTouch x = %04X, %04X\n", touchXY.rawx, touchXY.px);
        iprintf("Touch y = %04X, %04X\n", touchXY.rawy, touchXY.py);
    }*/
    return 0;
}
