#include "widgetlayout.hpp"


//----------------------------------------//

enum ChildType
{
    Full,
    Normal,
};

RectInt
Layout::GetNewRectangle(Vec2i pos)
{
    RectInt newRect = rect;

    switch(part)
    {
        case BorderPart::LeftSide:
        {
            newRect.left = pos.x;
            newRect.width = rect.left + rect.width - pos.x;
            break;
        }
        case BorderPart::RightSide:
        {
            newRect.width = pos.x - rect.left;
            break;
        }
        case BorderPart::TopSide:
        {
            newRect.top = pos.y;
            newRect.height = rect.top + rect.height - pos.y;
            break;
        }
        case BorderPart::BottomSide:
        {
            newRect.height = pos.y - rect.top;
            break;
        }
        case BorderPart::TopLeftCorner:
        {
            newRect.width = rect.left + rect.width - pos.x;
            newRect.height = rect.top + rect.height - pos.y;
            newRect.left = pos.x;
            newRect.top = pos.y;
            break;
        }
        case BorderPart::TopRightCorner:
        {
            newRect.top = pos.y;
            newRect.width = pos.x - rect.left;
            newRect.height = rect.top + rect.height - pos.y;
            break;
        }
        case BorderPart::BottomRightCorner:
        {
            newRect.width = pos.x - rect.left;
            newRect.height = pos.y - rect.top;
            break;
        }
        case BorderPart::BottomLeftCorner:
        {
            newRect.left = pos.x;
            newRect.width  = rect.left + rect.width - pos.x;
            newRect.height = pos.y - rect.top;
            break;
        }
        default:
        {
            break;
        }
    }
    return newRect;
}

BorderPart
Layout::GetBorderPart(Vec2i pos_)
{
    RectInt borderCol, borderRow, corner;

    borderCol = {rect.left - border, rect.top, 
                             border, rect.height};
    if(borderCol.IsInside(pos_))
    {
        return BorderPart::LeftSide;
    }
    borderCol.left += rect.width + border;
    if(borderCol.IsInside(pos_))
    {
        return BorderPart::RightSide;
    }

    borderRow = {rect.left, rect.top - border, 
                 rect.width,           border};
    if(borderRow.IsInside(pos_))
    {
        return BorderPart::TopSide;
    }
    borderRow.top += rect.height + border;
    if(borderRow.IsInside(pos_))
    {
        return BorderPart::BottomSide;
    }

    corner = {rect.left - border, rect.top - border, 
                          border,            border};
    if(corner.IsInside(pos_))
    {
        return BorderPart::TopLeftCorner;
    }
    corner.left += rect.width + border;
    if(corner.IsInside(pos_))
    {
        return BorderPart::TopRightCorner;
    }
    corner.top += rect.height + border;
    if(corner.IsInside(pos_))
    {
        return BorderPart::BottomRightCorner;
    }
    corner.left -= rect.width + border;
    if(corner.IsInside(pos_))
    {
        return BorderPart::BottomLeftCorner;
    }
    return BorderPart::NoBorder;
}

//----------------------------------------//

Layout::Layout(const RectInt& rect_, int margin_, int border_,
Vec2i minSize_, Vec2i maxSize_):
parent(nullptr), widget(nullptr), rect(rect_), margin(margin_),
border(border_), minSize(minSize_), maxSize(maxSize_),
part(BorderPart::NoBorder), onResize(false), onMove(false),
addition(2 * margin_ + 2 * border_), indent(margin_ + border_)
{}

Layout::Layout(const RectInt& rect_, int margin_, int border_):
Layout(rect_, margin_, border_,
{rect_.width, rect_.height}, {rect_.width, rect_.height})
{}



Layout::Layout(Vec2i size, int margin_, int border_):
Layout({size}, margin_, border_, size, size)
{}

Layout*
Layout::GetParent()
{
    return parent;
}

Widget*
Layout::GetWidget()
{
    return widget;
}

const RectInt&
Layout::GetRectangle() const
{
    return rect;
}

int
Layout::GetMargin() const
{
    return margin;
}

int
Layout::GetBorder() const
{
    return border;
}

Vec2i
Layout::GetMinSize() const
{
    return minSize;
}

Vec2i
Layout::GetMaxSize() const
{
    return maxSize;
}

void
Layout::SetWidget(Widget* widget_)
{
    widget = widget_;
}

bool
Layout::IsInside(Vec2i pos_) const
{
    return rect.IsInside(pos_) || IsInsideBorder(pos_);
}

bool
Layout::IsInsideBorder(Vec2i pos_) const
{
    if(border == 0)
        return false;

    RectInt borderRect = {rect.left  -     border, rect.top    -     border,
                          rect.width + 2 * border, rect.height + 2 * border};

    return borderRect.IsInside(pos_) && !rect.IsInside(pos_);
}

void
Layout::OnEvent(const Event& event_)
{
    if(dynamic_cast<Container*>(parent))
        return;

    if(event_.type == mousePressed)
    {
        if(IsInsideBorder(event_.mouse.pos))
        {
            part     = GetBorderPart(event_.mouse.pos);
            onResize = true;
        }
        else if(rect.IsInside(event_.mouse.pos))
        {
            onMove = true;
        }
    }
    else if(event_.type == mouseMoved)
    {
        if(onResize)
        {
            RectInt newRect = GetNewRectangle(event_.mouse.pos);
            if(newRect.width >= minSize.x && newRect.width <= maxSize.x)
                OnResize({newRect.left, rect.top, newRect.width, rect.height});

            if(newRect.height >= minSize.y && newRect.height <= maxSize.y)
                OnResize({rect.left, newRect.top, rect.width, newRect.height});
        }
        else if(onMove)
        {
            OnMove(event_.mouse.offset);
        }
    }
    else if(event_.type == mouseReleased)
    {
        onMove   = false;
        onResize = false;
        part     = BorderPart::NoBorder;
    }
}

void
Layout::Attach(Layout* child_)
{
    child_->parent = this;
    child_->OnMove({rect.left, rect.top});
    children.push_back(child_);
}

void
Layout::Detach(Layout* child_)
{
    auto found = std::find(children.begin(), children.end(), child_);
    if(found != children.end())
    {
        children.erase(found);
        child_->parent = nullptr;
    }
}

void
Layout::OnMove(Vec2i delta_)
{
    rect.left += delta_.x;
    rect.top  += delta_.y;

    for(auto child: children)
        child->OnMove(delta_);

    widget->OnLayoutMove();
}

void
Layout::OnResize(const RectInt& rect_)
{
    rect = rect_;
    widget->OnLayoutResize();
}

//----------------------------------------//

Container::Container(int margin_, int border_):
Layout({{}}, margin_, border_)
{}

void
Container::OnResize(const RectInt& rect_)
{
    rect = rect_;
    PlaceChildren();
    widget->OnLayoutResize();
}

//----------------------------------------//

Row::Row(int margin_, int border_):
Container(margin_, border_)
{}

void
Row::Attach(Layout* child_)
{
    child_->parent = this;
    children.push_back(child_);

    rect.width  += child_->GetRectangle().width  + child_->addition;
    if(child_->GetRectangle().height + child_->addition > rect.height)
        rect.height = child_->GetRectangle().height + child_->addition;

    minSize.x += child_->GetMinSize().x + child_->addition;
    maxSize.x += child_->GetMaxSize().x + child_->addition;

    if(children.size() == 1)
    {
        minSize.y = child_->GetMinSize().y + child_->addition;
        maxSize.y = child_->GetMaxSize().y + child_->addition;

        rect.width  = child_->GetRectangle().width  + child_->addition;
        rect.height = child_->GetRectangle().height + child_->addition;
    }

    if(child_->GetMinSize().y + child_->addition > minSize.y)
        minSize.y = child_->GetMinSize().y + child_->addition;
    if(child_->GetMaxSize().y + child_->addition < maxSize.y)
        maxSize.y = child_->GetMaxSize().y + child_->addition;

    if(minSize.y > rect.height)
        rect.height = minSize.y;

    if(rect.width < minSize.x)
        rect.width = minSize.x;
    else if(rect.width > maxSize.x)
        rect.width = maxSize.x;

    PlaceChildren();
}

void
Row::Detach(Layout* child_)
{
    auto found = std::find(children.begin(), children.end(), child_);
    if(found != children.end())
    {
        children.erase(found);
        child_->parent = nullptr;

        minSize.x  -= child_->GetMinSize().x + child_->addition;
        maxSize.x  -= child_->GetMaxSize().x + child_->addition;
        rect.width -= child_->GetRectangle().width + child_->addition;

        if(children.size() == 1)
        {
            minSize.y = 0;
            maxSize.y = 0;
        }

        minSize.y = children[0]->GetMinSize().y + children[0]->addition;
        maxSize.y = children[0]->GetMaxSize().y + children[0]->addition;

        for(const auto& child: children)
        {
            if(child->GetMinSize().y + child->addition < minSize.y)
                minSize.y = child->GetMinSize().y + child->addition;
            if(child->GetMaxSize().y + child->addition > maxSize.y)
                maxSize.y = child->GetMaxSize().y + child->addition;
        }

        if(minSize.y < rect.height)
            rect.height = minSize.y;

        PlaceChildren();
    }
}

void
Row::PlaceChildren()
{
    size_t n = children.size();
    if(!n)
        return;

    int averageAdd     = (rect.width - minSize.x) / static_cast<int>(n);
    int sumWidthRemain =  rect.width;
    int nNormal        =  0;
    std::vector<ChildType> typedChildren{n};

    for(size_t i = 0; i < n; i++)
    {
        int sumWidth = children[i]->GetMinSize().x + averageAdd;
        if(sumWidth > children[i]->GetMaxSize().x)
        {
            sumWidthRemain -= children[i]->GetMaxSize().x - children[i]->GetMinSize().x;
            typedChildren[i] = Full;
        }
        else
        {
            typedChildren[i] = Normal;
            nNormal++;
        }
    }

    int resultAdd = 0;
    if(nNormal)
        resultAdd = (sumWidthRemain - minSize.x) / nNormal;

    int sumLeft = rect.left;
    for(size_t i = 0; i < children.size(); i++)
    {
        int sideIndent = children[i]->GetBorder() + children[i]->GetMargin();
        int topIndent = sideIndent;
        if(children[i]->GetMaxSize().y < rect.height)
        {
            topIndent = children[i]->GetMargin() + children[i]->GetBorder() + 
                        (rect.height - children[i]->GetMaxSize().y) / 2;
        }

        sumLeft += sideIndent;
        if(typedChildren[i] == Normal)
        {
            children[i]->OnResize({sumLeft, rect.top + topIndent,
            children[i]->GetMinSize().x + resultAdd, rect.height - 2 * topIndent});
        }
        else
        {
            children[i]->OnResize({sumLeft, rect.top + topIndent,
            children[i]->GetMaxSize().x, rect.height - 2 * topIndent});
        }
        sumLeft += children[i]->GetRectangle().width + sideIndent;
    }
}

//----------------------------------------//

Column::Column(int margin_, int border_):
Container(margin_, border_)
{}

void
Column::Attach(Layout* child_)
{
    child_->parent = this;
    children.push_back(child_);

    if(child_->GetRectangle().width + child_->addition > rect.width)
        rect.width = child_->GetRectangle().width + child_->addition;
    rect.height += child_->GetRectangle().height + child_->addition;
    
    minSize.y += child_->GetMinSize().y + child_->addition;
    maxSize.y += child_->GetMaxSize().y + child_->addition;

    if(children.size() == 1)
    {
        minSize.x = child_->GetMinSize().x + child_->addition;
        maxSize.x = child_->GetMaxSize().x + child_->addition;

        rect.width  = child_->GetRectangle().width  + child_->addition;
        rect.height = child_->GetRectangle().height + child_->addition;
    }

    if(child_->GetMinSize().x + child_->addition > minSize.x)
        minSize.x = child_->GetMinSize().x + child_->addition;
    if(child_->GetMaxSize().x + child_->addition < maxSize.x)
        maxSize.x = child_->GetMaxSize().x + child_->addition;

    if(minSize.x > rect.width)
        rect.width = minSize.x;

    if(rect.height < minSize.y)
        rect.height = minSize.y;
    else if(rect.height > maxSize.y)
        rect.height = maxSize.y;

    PlaceChildren();
}

void
Column::Detach(Layout* child_)
{
    auto found = std::find(children.begin(), children.end(), child_);
    if(found != children.end())
    {
        children.erase(found);
        child_->parent = nullptr;

        minSize.y  -= child_->GetMinSize().y + child_->addition;
        maxSize.y  -= child_->GetMaxSize().y + child_->addition;
        rect.height -= child_->GetRectangle().height + child_->addition;

        if(children.size() == 1)
        {
            minSize.y = 0;
            maxSize.y = 0;
        }

        minSize.x = children[0]->GetMinSize().x + children[0]->addition;
        maxSize.x = children[0]->GetMaxSize().x + children[0]->addition;

        for(const auto& child: children)
        {
            if(child->GetMinSize().x + child->addition < minSize.x)
                minSize.x = child->GetMinSize().x + child->addition;
            if(child->GetMaxSize().x + child->addition > maxSize.x)
                maxSize.x = child->GetMaxSize().x + child->addition;
        }

        if(minSize.x < rect.width)
            rect.width = minSize.x;

        PlaceChildren();
    }
}

void
Column::PlaceChildren()
{
    size_t n = children.size();
    if(!n)
        return;

    int averageAdd      = (rect.height - minSize.y) / static_cast<int>(n);
    int sumHeightRemain =  rect.height;
    int nNormal         =  0;
    std::vector<ChildType> typedChildren{n};

    for(size_t i = 0; i < n; i++)
    {
        int sumHeight = children[i]->GetMinSize().y + averageAdd;
        if(sumHeight > children[i]->GetMaxSize().y)
        {
            sumHeightRemain -= children[i]->GetMaxSize().y -
                               children[i]->GetMinSize().y;
            typedChildren[i] = Full;
        }
        else
        {
            typedChildren[i] = Normal;
            nNormal++;
        }
    }

    int resultAdd = 0;
    if(nNormal)
        resultAdd = (sumHeightRemain - minSize.y) / nNormal;

    int sumTop = rect.top;
    for(size_t i = 0; i < children.size(); i++)
    {
        int topIndent = children[i]->GetBorder() + children[i]->GetMargin();
        int sideIndent = topIndent;
        if(children[i]->GetMaxSize().x < rect.width)
        {
            sideIndent = children[i]->GetMargin() + children[i]->GetBorder() + 
                        (rect.width - children[i]->GetMaxSize().x) / 2;
        }

        sumTop += topIndent;
        if(typedChildren[i] == Normal)
        {
            children[i]->OnResize({rect.left + sideIndent, sumTop,
            rect.width - 2 * sideIndent, children[i]->GetMinSize().y + resultAdd});
        }
        else
        {
            children[i]->OnResize({rect.left + sideIndent, sumTop,
            rect.width - 2 * sideIndent, children[i]->GetMaxSize().y});
        }
        sumTop += children[i]->GetRectangle().height + topIndent;
    }
}

//----------------------------------------//
