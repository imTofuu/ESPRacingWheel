#pragma once

#include "../PageElement.h"
#include "type_traits"

template<typename text_t>
class TextLabel : public PageElement {
public:
    
    TextLabel(uint16_t x, uint16_t y, text_t* text, uint8_t size) 
        : PageElement(x, y), m_text(text), m_size(size), lastFrame(FrameInfo{*text, size}) {}
    
    void draw(Arduino_GFX* display) override;

    struct FrameInfo {
        text_t m_text;
        uint8_t m_size;
    };

private:

    text_t* m_text;
    uint8_t m_size;

    FrameInfo lastFrame;

};

template<typename text_t>
void TextLabel<text_t>::draw(Arduino_GFX* display) {

    // Clear last frame
    display->setCursor(m_x, m_y);
    display->setTextSize(lastFrame.m_size);
    display->setTextColor(0);
    display->print(lastFrame.m_text);

    // Write new frame
    display->setCursor(m_x, m_y);
    display->setTextSize(m_size);
    display->setTextColor(0xffff);
    display->print(*m_text);

    // Update last frame
    lastFrame.m_text = *m_text;
    lastFrame.m_size = m_size;
}