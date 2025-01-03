#include "widget.hpp"

//----------------------------------------//

Widget::Widget(Layout *layout_, Texture *texture_)
    : layout(layout_), texture(texture_), borderColor(Config::defBorderColor),
      parent(nullptr), system(nullptr) {
  layout->SetWidget(this);
}

Widget::~Widget() {
  delete layout;

  for (auto child : children)
    delete child;
}

Layout *Widget::GetLayout() { return layout; }

Texture *Widget::GetTexture() { return texture; }

Widget *Widget::GetParent() { return parent; }

WidgetSystem *Widget::GetWidgetSystem() { return system; }

Color Widget::GetBorderColor() const { return borderColor; }

void Widget::SetLayout(Layout *layout_) { layout = layout_; }

void Widget::SetTexture(Texture *texture_) { texture = texture_; }

void Widget::SetParent(Widget *parent_) { parent = parent_; }

void Widget::SetWidgetSystem(WidgetSystem *system_) {
  system = system_;

  for (auto child : children)
    child->SetWidgetSystem(system_);
}

void Widget::SetBorderColor(Color borderColor_) { borderColor = borderColor_; }

void Widget::Attach(Widget *child_) {
  auto found = std::find(children.begin(), children.end(), child_);
  if (found == children.end()) {
    layout->Attach(child_->GetLayout());
    child_->SetParent(this);
    child_->SetWidgetSystem(system);
    children.push_front(child_);
  }
}

void Widget::Detach(Widget *child_) {
  auto found = std::find(children.begin(), children.end(), child_);
  if (found != children.end()) {
    layout->Detach((*found)->GetLayout());
    child_->SetParent(nullptr);
    children.erase(found);
  }
}

bool Widget::ProcessEvent(const Event &event_) {
  for (auto it = children.begin(); it != children.end(); ++it) {
    if ((*it)->ProcessEvent(event_)) {
      if (event_.type == mousePressed) {
        Widget *active = *it;
        children.erase(it);
        children.push_front(active);
      }
      return true;
    }
  }
  return OnEvent(event_);
}

bool Widget::ProcessListenerEvent(const Event &event_) {
  assert(&event_);
  return false;
}

bool Widget::OnEvent(const Event &event_) {
  if (event_.IsMouseType() && !layout->IsInside(event_.mouse.pos))
    return false;

  return false;
}

void Widget::Render() const {
  if (layout->GetBorder())
    RenderBorder();

  if (layout->GetRectangle().width && layout->GetRectangle().height)
    Renderer::Get()->DrawTexture(layout->GetRectangle(), texture);

  for (auto it = children.end(); it != children.begin();) {
    --it;
    (*it)->Render();
  }
}

void Widget::RenderBorder() const {
  const RectInt &rect = layout->GetRectangle();
  int thickness = layout->GetBorder();

  Renderer *rend = Renderer::Get();
  rend->SetColor(borderColor);

  RectInt borderCol = {rect.left - thickness, rect.top, thickness, rect.height};

  rend->DrawRect(borderCol);
  borderCol.left += rect.width + thickness;
  rend->DrawRect(borderCol);

  RectInt borderRow = {rect.left, rect.top - thickness, rect.width, thickness};

  rend->DrawRect(borderRow);
  borderRow.top += rect.height + thickness;
  rend->DrawRect(borderRow);

  rend->DrawCircle({rect.left, rect.top}, thickness);
  rend->DrawCircle({rect.left + rect.width, rect.top}, thickness);
  rend->DrawCircle({rect.left, rect.top + rect.height}, thickness);
  rend->DrawCircle({rect.left + rect.width, rect.top + rect.height}, thickness);
}

void Widget::OnLayoutMove() {}

void Widget::OnLayoutResize() {}

//----------------------------------------//

WidgetSystem::WidgetSystem(Widget *root_)
    : root(root_), listeners({count, nullptr}) {
  root_->SetWidgetSystem(this);
}

void WidgetSystem::Subscribe(Widget *listener_, EventType type_) {
  listeners[type_] = listener_;
}

void WidgetSystem::Unsubscribe(EventType type_) { listeners[type_] = nullptr; }

void WidgetSystem::Reset() {
  for (auto &listener : listeners)
    listener = nullptr;
}

void WidgetSystem::ProcessEvent(const Event &event_) {
  if (event_.type == widgetClosed) {
    Widget *widget = event_.widget.widget;
    Widget *parent = widget->GetParent();
    parent->Detach(widget);
    delete widget;
    Reset();
    return;
  }

  if (event_.type != mousePressed)
    if (listeners[event_.type])
      if (listeners[event_.type]->ProcessListenerEvent(event_))
        return;

  root->ProcessEvent(event_);
}

void WidgetSystem::Render() const { root->Render(); }

//----------------------------------------//
