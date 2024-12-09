#pragma once

#include "Vector.h"
#include "PageElement.h"

class Page {
public:

    template<typename... elements_t>
    Page(elements_t... elements);

    void drawElements(Arduino_GFX* display);
    void drawStaticElements(Arduino_GFX* display);

private:

    Vector<PageElement*> m_elements;

};

template<typename... elements_t>
Page::Page(elements_t... elements) {
    (m_elements.push_back(elements), ...);
}