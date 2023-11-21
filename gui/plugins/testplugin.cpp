#include "../plugin/plugin.hpp"
#include <cassert>
#include <unistd.h>

namespace plugin {

IAPI *api = nullptr;

class FlowerTool : public ITool {
public:
  FlowerTool();
  ~FlowerTool();

  virtual void ActionBegin(ITexture *canvas, int x, int y) override;
  virtual void Action(ITexture *canvas, int x, int y, int dx, int dy) override;
  virtual void ActionEnd(ITexture *canvas, int x, int y) override;

  virtual const char *GetIconFileName() const override;
  virtual const char *GetName() const override;
  virtual IPreferencesPanel *GetPreferencesPanel() const override;

private:
  IPreferencesPanel *panel_;

  ILabel *label_;
};

const char *FlowerTool::GetName() const { return "Example"; }

FlowerTool::FlowerTool() : panel_(nullptr), label_(nullptr) {
  IWidgetFactory *factory = api->GetWidgetFactory();

  label_ = factory->CreateDefaultLabel("Example");

  panel_ = factory->CreateDefaultPreferencesPanel();

  panel_->Attach(label_, 60, 50);
}

FlowerTool::~FlowerTool() {
  delete panel_;
  usleep(1000000);
  delete label_;
}

void FlowerTool::ActionBegin(ITexture *canvas, int x, int y) { assert(canvas); }

void FlowerTool::Action(ITexture *canvas, int x, int y, int dx, int dy) {
  assert(canvas);
}

void FlowerTool::ActionEnd(ITexture *canvas, int x, int y) { assert(canvas); }

const char *FlowerTool::GetIconFileName() const { return "res/flower.bmp"; }

IPreferencesPanel *FlowerTool::GetPreferencesPanel() const { return panel_; }

class Plugin : public IPlugin {
public:
  Plugin(FlowerTool *flower_tool) : flower_tool_(flower_tool) {}

  ~Plugin();

  virtual std::list<IFilter *> GetFilters() const override;
  virtual std::list<ITool *> GetTools() const override;

private:
  FlowerTool *flower_tool_;
};

Plugin::~Plugin() { delete flower_tool_; }

std::list<IFilter *> Plugin::GetFilters() const {
  return std::list<IFilter *>();
}

std::list<ITool *> Plugin::GetTools() const {
  std::list<ITool *> list;
  list.push_back(flower_tool_);

  return list;
}

extern "C" IPlugin *Create(IAPI *api) {
  plugin::api = api;
  return new Plugin(new FlowerTool);
}

extern "C" void Destroy(IPlugin *plugin) { delete plugin; }

} // namespace plugin