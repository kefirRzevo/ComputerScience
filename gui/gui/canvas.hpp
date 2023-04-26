#pragma once

#include "../widgets/widget.hpp"

class Canvas: public Widget
{
    protected:

        Canvas(Column* layout_, Texture* texture_);
};
