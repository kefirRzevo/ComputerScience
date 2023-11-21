#include "api.hpp"

API::API()
    : widgetFactory(new APIWidgetFactory),
      textureFactory(new APITextureFactory) {}

API::~API() {
  delete widgetFactory;
  delete textureFactory;
}

IWidgetFactory *API::GetWidgetFactory() { return widgetFactory; }

ITextureFactory *API::GetTextureFactory() { return textureFactory; }
