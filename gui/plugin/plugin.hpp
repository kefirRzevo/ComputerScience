#pragma once

#include <iostream>
#include <list>

namespace plugin {

typedef unsigned int Color;

class ITexture;

struct Buffer {
  Color *pixels;
  ITexture *texture;
};

class ITexture {
public:
  virtual ~ITexture() = default;

  virtual int GetWidth() = 0;
  virtual int GetHeight() = 0;

  virtual Buffer ReadBuffer() = 0;
  virtual void LoadBuffer(Buffer buffer) = 0;

  virtual void Clear(Color color) = 0;

  virtual void DrawLine(int x0, int y0, int x1, int y1, Color color) = 0;
  virtual void DrawThickLine(int x0, int y0, int x1, int y1, int thickness,
                             Color color) = 0;
  virtual void DrawCircle(int x, int y, int radius, Color color) = 0;
  virtual void DrawRect(int x, int y, int width, int height, Color color) = 0;

  virtual void CopyTexture(ITexture *texture, int x, int y, int width,
                           int height) = 0;
};

class ITextureFactory {
public:
  virtual ~ITextureFactory() = default;

  virtual ITexture *CreateTexture(const char *filename) = 0;
  virtual ITexture *CreateTexture(int width, int height) = 0;
};

class IClickCallback {
public:
  virtual ~IClickCallback() = default;

  virtual void RespondOnClick() = 0;
};

class ISliderCallback {
public:
  virtual ~ISliderCallback() = default;

  virtual void RespondOnSlide(float old_value, float current_value) = 0;
};

class IWidget {
public:
  virtual ~IWidget() = default;

  virtual int GetWidth() = 0;
  virtual int GetHeight() = 0;
};

class IButton : public IWidget {
public:
  virtual ~IButton() = default;

  virtual void SetClickCallback(IClickCallback *callback) = 0;
};

class ISlider : public IWidget {
public:
  virtual ~ISlider() = default;

  virtual void SetSliderCallback(ISliderCallback *callback) = 0;

  virtual float GetValue() = 0;
  virtual void SetValue(float value) = 0;
};

class ILabel : public IWidget {
public:
  virtual ~ILabel() = default;

  virtual void SetText(const char *text) = 0;
};

class IPreferencesPanel : public IWidget {
public:
  virtual ~IPreferencesPanel() = default;

  virtual void Attach(IWidget *widget, int x, int y) = 0;
};

class IWidgetFactory {
public:
  virtual ~IWidgetFactory() = default;

  virtual IButton *CreateDefaultButtonWithIcon(const char *icon_file_name) = 0;
  virtual IButton *CreateDefaultButtonWithText(const char *text) = 0;
  virtual IButton *CreateButtonWithIcon(int width, int height,
                                        const char *icon_file_name) = 0;
  virtual IButton *CreateButtonWithText(int width, int height, const char *text,
                                        int char_size) = 0;

  virtual ISlider *CreateDefaultSlider(float range_min, float range_max) = 0;
  virtual ISlider *CreateSlider(int width, int height, float range_min,
                                float range_max) = 0;
  virtual ISlider *CreateSlider(int width, int height, float thumb_width,
                                float thumb_height, float range_min,
                                float range_max) = 0;

  virtual ILabel *CreateDefaultLabel(const char *text) = 0;
  virtual ILabel *CreateLabel(int width, int height, const char *text,
                              int char_size) = 0;

  virtual IPreferencesPanel *CreateDefaultPreferencesPanel() = 0;
};

class IAPI {
public:
  virtual ~IAPI() = default;

  virtual IWidgetFactory *GetWidgetFactory() = 0;
  virtual ITextureFactory *GetTextureFactory() = 0;
};

class IFilter {
public:
  virtual ~IFilter() = default;

  virtual void Apply(ITexture *canvas) = 0;
  virtual const char *GetName() const = 0;
  virtual IPreferencesPanel *GetPreferencesPanel() const = 0;
};

class ITool {
public:
  virtual ~ITool() = default;

  virtual void ActionBegin(ITexture *canvas, int x, int y) = 0;
  virtual void Action(ITexture *canvas, int x, int y, int dx, int dy) = 0;
  virtual void ActionEnd(ITexture *canvas, int x, int y) = 0;

  virtual const char *GetIconFileName() const = 0;
  virtual const char *GetName() const = 0;
  virtual IPreferencesPanel *GetPreferencesPanel() const = 0;
};

class IPlugin {
public:
  virtual ~IPlugin() = default;

  virtual std::list<IFilter *> GetFilters() const = 0;
  virtual std::list<ITool *> GetTools() const = 0;
};

typedef IPlugin *(*CreateFunction)(IAPI *api);
typedef void (*DestroyFunction)(IPlugin *plugin);

} // namespace plugin
