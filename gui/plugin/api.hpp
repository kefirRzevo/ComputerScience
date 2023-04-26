#pragma once

#include "plugin.hpp"
#include "apitexture.hpp"
#include "apiwidgets.hpp"

class API : public IAPI
{
    public:

        API();
        ~API();

        IWidgetFactory*  GetWidgetFactory () override;
        ITextureFactory* GetTextureFactory() override;

    private:

        IWidgetFactory*  widgetFactory;
        ITextureFactory* textureFactory;
};
