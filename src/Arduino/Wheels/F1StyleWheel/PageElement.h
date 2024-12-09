#pragma once

#include <cstdint>
#include <Arduino_GFX_Library.h>

class PageElement {
public:

    PageElement(uint16_t x, uint16_t y) : m_x(x), m_y(y) {}

    virtual void update() {}
    virtual void draw(Arduino_GFX* display) {}
    virtual void drawStatic(Arduino_GFX* display) {}

protected:

    uint16_t m_x;
    uint16_t m_y;

};
