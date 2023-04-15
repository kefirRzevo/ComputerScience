#include "widgetview.hpp"


//----------------------------------------//

WidgetView::~WidgetView()
{}

Widget*
WidgetView::GetWidget()
{
    return widget;
}

void
WidgetView::SetWidget(Widget* widget_)
{
    widget = widget_;
}

bool
WidgetView::IsInside(Vec2i pos_) const
{
    return widget->GetLocation().IsInside(pos_);
}

//----------------------------------------//

BorderView::BorderView(int thickness_, Color color_):
thickness(thickness_), color(color_)
{}

Color
BorderView::GetBorderColor() const
{
    return color;
}

int
BorderView::GetBorderThickness() const
{
    return thickness;
}

void
BorderView::SetBorderColor(Color color_)
{
    color = color_;
}

void
BorderView::SetBorderThickness(int thickness_)
{
    thickness = thickness_;
}

bool
BorderView::IsInside(Vec2i pos_) const
{
    return widget->GetLocation().IsInside(pos_) || IsInsideBorder(pos_);
}

bool
BorderView::IsInsideBorder(Vec2i pos_) const
{
    const RectInt& rect = widget->GetLocation();

    RectInt borderCol = {rect.left - thickness, rect.top    -     thickness,
                                     thickness, rect.height + 2 * thickness};

    if(borderCol.IsInside(pos_))
        return true;

    borderCol.left += rect.width + thickness;

    if(borderCol.IsInside(pos_))
        return true;

    RectInt borderRow = {rect.left,  rect.top - thickness,
                         rect.width,            thickness};

    if(borderRow.IsInside(pos_))
        return true;

    borderRow.top += rect.height + thickness;

    if(borderRow.IsInside(pos_))
        return true;

    return false;
}

RectInt
BorderView::OnResize(Vec2i pos_, Vec2i delta_) const
{
    const RectInt& rect = widget->GetLocation();
    RectInt newLocation = rect;

    RectInt borderCol = {rect.left - thickness, rect.top, 
                                     thickness, rect.height};
    if(borderCol.IsInside(pos_))
    {
        newLocation.left  += delta_.x;
        newLocation.width -= delta_.x;
        return newLocation;
    }
    borderCol.left += rect.width + thickness;
    if(borderCol.IsInside(pos_))
    {
        newLocation.width += delta_.x;
        return newLocation;
    }

    RectInt borderRow = {rect.left, rect.top - thickness, 
                         rect.width,           thickness};
    if(borderRow.IsInside(pos_))
    {
        newLocation.top    += delta_.y;
        newLocation.height -= delta_.y;
        return newLocation;
    }
    borderRow.top += rect.height + thickness;
    if(borderRow.IsInside(pos_))
    {
        newLocation.height += delta_.y;
        return newLocation;
    }

    RectInt corner = {rect.left - thickness, rect.top - thickness, 
                                  thickness,            thickness};
    if(corner.IsInside(pos_))
    {
        newLocation.left   += delta_.x;
        newLocation.top    += delta_.y;
        newLocation.width  -= delta_.x;
        newLocation.height -= delta_.y;
        return newLocation;
    }
    corner.left += rect.width + thickness;
    if(corner.IsInside(pos_))
    {
        newLocation.top    += delta_.y;
        newLocation.width  += delta_.x;
        newLocation.height -= delta_.y;
        return newLocation;
    }
    corner.top += rect.height + thickness;
    if(corner.IsInside(pos_))
    {
        newLocation.width  += delta_.x;
        newLocation.height += delta_.y;
        return newLocation;
    }
    corner.left -= rect.width + thickness;
    if(corner.IsInside(pos_))
    {
        newLocation.left   += delta_.x;
        newLocation.width  -= delta_.x;
        newLocation.height += delta_.y;
        return newLocation;
    }
    return newLocation;
}

void
BorderView::RenderBorder() const
{
    const RectInt& rect = widget->GetLocation();

    Renderer* rend = Renderer::Get();
    rend->SetColor(color);
    rend->SetThickness(thickness);

    RectInt borderCol = {rect.left - thickness, rect.top, 
                                     thickness, rect.height};
    rend->DrawRect(borderCol);
    borderCol.left += rect.width + thickness;
    rend->DrawRect(borderCol);

    RectInt borderRow = {rect.left, rect.top - thickness, 
                         rect.width,           thickness};
    rend->DrawRect(borderRow);
    borderRow.top += rect.height + thickness;
    rend->DrawRect(borderRow);

    int nPoints = thickness * 4;

    rend->DrawCircle({rect.left,              rect.top              }, thickness, nPoints);
    rend->DrawCircle({rect.left + rect.width, rect.top              }, thickness, nPoints);
    rend->DrawCircle({rect.left,              rect.top + rect.height}, thickness, nPoints);
    rend->DrawCircle({rect.left + rect.width, rect.top + rect.height}, thickness, nPoints);
}

//----------------------------------------//

TextureView::TextureView(Texture* texture_):
texture(texture_)
{}

Texture*
TextureView::GetTexture()
{
    return texture;
}

void
TextureView::SetTexture(Texture* texture_)
{
    texture = texture_;
}

void
TextureView::OnRender() const
{
    Renderer::Get()->DrawTexture(*texture, widget->GetLocation());
}

//----------------------------------------//

ColorView::ColorView(Color bgColor_):
bgColor(bgColor_)
{}

Color
ColorView::GetColor() const
{
    return bgColor;
}

void
ColorView::SetColor(Color bgColor_)
{
    bgColor = bgColor_;
}

void
ColorView::OnRender() const
{
    Renderer::Get()->SetColor(bgColor);
    Renderer::Get()->DrawRect(widget->GetLocation()); 
}

//----------------------------------------//

BorderTextureView::BorderTextureView(Texture* texture_, int thickness_, Color color_):
TextureView(texture_), BorderView(thickness_, color_)
{}

void
BorderTextureView::OnRender() const
{
    RenderBorder();
    Renderer::Get()->DrawTexture(*texture, widget->GetLocation());
}

//----------------------------------------//

BorderColorView::BorderColorView(Color bgColor_, int thickness_, Color color_):
ColorView(bgColor_), BorderView(thickness_, color_)
{}

void
BorderColorView::OnRender() const
{
    RenderBorder();
    Renderer::Get()->SetColor(bgColor);
    Renderer::Get()->DrawRect(widget->GetLocation()); 
}

//----------------------------------------//
