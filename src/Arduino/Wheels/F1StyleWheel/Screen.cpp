#include "Screen.h"

#include "PageElements/TextLabel.h"

int speed = 0;

Page Pages::testPage = Page(
    new TextLabel<int>(0, 0, &speed, 2)
);

bool initScreen() {
#ifdef GFX_EXTRA_PRE_INIT
  GFX_EXTRA_PRE_INIT();
#endif
  if(!display->begin(80000000)) { Serial.println("Failed to init screen"); return false; }
  display->fillScreen(0);
  return true;
}

int lastRefresh = 0;
void updateScreen() {
    if(!currentPage) return;
    if(millis() - lastRefresh < 1000.0f / ATTEMPTED_REFRESH_RATE) return;
    currentPage->drawElements(display);
    lastRefresh = millis();
}

void setCurrentPage(Page* page) {
    currentPage = page;
    currentPage->drawStaticElements(display);
}