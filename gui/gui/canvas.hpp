#pragma once

#include "../widgets/widget.hpp"

class Canvas;

class CanvasWidget: public Widget
{
    protected:

        HorScrollBar* horScrollBar;
        VerScrollBar* verScrollBar;
        Canvas*       canvas;

        Vec2i canvasSize;
        Vec2i fullTextureSize;

    public:

        CanvasWidget(Vec2i canvasSize, Vec2i fullTextureSize);

        bool
        ProcessListenerEvent(const Event& event_) override;
        bool
        OnEvent(const Event& event_) override;
        void
        OnLayoutResize() override;
};
