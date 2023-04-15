#pragma once

#include "widget.hpp"

class TextLabel: public Widget
{
    protected:

        Text  text;
        Color bgColor;

    public:

        TextLabel(Vec2i size_, Color bgColor_,
        const char* string_, const Font& font_,
        Color textColor_  = defaultTextColor,
        int   textSize_   = defaultTextSize);

        void
        Move(Vec2i delta_) override;
        void
        SetPosition(Vec2i pos_) override;
        void
        SetSize(Vec2i size_) override;

        void
        Render() const override;
};

class Icon: public Widget
{
    public:

        Icon(Vec2i size_, Texture* texture_);

        Icon(Vec2i size_, Color bgColor);

        bool
        OnEvent(const Event& event_) override;
};

/*
class TextIcon: public Widget
{
    protected:

        Text text;

    public:

        TextIcon(Vec2i size_, Color bgColor, const char* string_,
        const Font& font_ = FontManager::Get()->GetDefaultFont(),
        Color textColor_  = defaultTextColor,
        int   textSize_   = defaultTextSize);

        void
        Move(Vec2i delta_) override;
        void
        Resize(const RectInt& location_) override;

        bool
        OnEvent(const Event& event_) override;

        void
        OnRender() override;
};
*/