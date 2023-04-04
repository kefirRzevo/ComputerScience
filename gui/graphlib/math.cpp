#include "math.hpp"


//----------------------------------------//

sf::IntRect
To_SF_Rect(const RectInt& rect)
{
    return sf::IntRect{rect.left, rect.top, rect.width, rect.height};
}

//----------------------------------------//

RectInt
From_SF_Rect(const sf::IntRect& rect)
{
    return RectInt{rect.left, rect.top, rect.width, rect.height};
}

//----------------------------------------//

sf::Color
To_SF_Color(Color color)
{
    return sf::Color{color};
}

//----------------------------------------//
