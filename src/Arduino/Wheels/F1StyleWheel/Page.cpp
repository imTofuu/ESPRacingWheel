#include "Page.h"

void Page::drawElements(Arduino_GFX* display) {
    for(int i = 0; i < m_elements.getLength(); i++) {
        m_elements[i]->update();
        m_elements[i]->draw(display);
    }
}

void Page::drawStaticElements(Arduino_GFX* display) {
    for(int i = 0; i < m_elements.getLength(); i++) {
        m_elements[i]->update();
        m_elements[i]->drawStatic(display);
    }
}