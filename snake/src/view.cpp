#include "../include/view.hpp"
#include "../include/guiview.hpp"
#include "../include/textview.hpp"

//----------------------------------------//

View *View::Get(const std::string &mode) {
  static std::unique_ptr<View> view;

  if (view)
    return view.get();

  if (!mode.compare("gui"))
    view.reset(new GuiView());
  else if (!mode.compare("text"))
    view.reset(new TextView());

  return view.get();
}

//----------------------------------------//

void View::PollOnKey(KeyCode key) {
  if (key == keyQuit)
    finished = true;

  if (key == keyUnknown)
    return;

  for (const auto &action : listenersOnKey) {
    action(key);
  }
}

//----------------------------------------//

void View::PollOnTimer(int microsecPassed) {
  for (size_t i = 0; i < passedTimes.size(); i++) {
    passedTimes[i] += microsecPassed;
    if (passedTimes[i] > listenersOnTimer[i].first) {
      passedTimes[i] %= listenersOnTimer[i].first;
      listenersOnTimer[i].second();
    }
  }
}

//----------------------------------------//
