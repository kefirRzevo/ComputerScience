#pragma once

#include "widget.hpp"

class Label: public Widget
{
    protected:

        Text  text;
        Color bgColor;

    public:

        Label(Vec2i size_, Color bgColor, 
              const Font& font_, const char* string_,
              Color textColor_ = defaultTextColor,
              int textSize_    = defaultTextSize);

        void
        Move(Vec2i delta_) override;
        void
        Resize(const RectInt& location_) override;

        void
        Render() const override;
};
