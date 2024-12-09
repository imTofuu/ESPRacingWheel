#pragma once

#include "Page.h"

#define ATTEMPTED_REFRESH_RATE 20 // hz

static Arduino_DataBus* bus = create_default_Arduino_DataBus();
static Arduino_GFX* display = new Arduino_ST7789(bus, 33 /* RST */, 1 /* rotation */);

static Page* currentPage;

extern int speed;

struct Pages {
    static Page testPage;
};

extern bool initScreen();
extern void updateScreen();
extern void setCurrentPage(Page* page);