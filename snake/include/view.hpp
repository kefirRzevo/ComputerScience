#pragma once

#include <cstdlib>
#include <iostream>
#include <unistd.h>

#include "model.hpp"

enum KeyCode {
  keyW,
  keyA,
  keyS,
  keyD,
  keyArrowL,
  keyArrowR,
  keyArrowU,
  keyArrowD,
  keyQuit,
  keyUnknown,
  noKey,

  sum,
};

KeyCode fromTextView(int textKey);
KeyCode fromGraphView(int graphKey);

using OnKeyCall = std::function<void(KeyCode)>;
using OnTimerCall = std::pair<int, std::function<void(void)>>;

class View {
public:
  View() = default;

  virtual ~View() {}

  static View *Get(const std::string &mode = "gui");

  virtual void RunLoop() = 0;

  Model *GetModel() const { return model; }

  void SetModel(Model *mod) { model = mod; }

  void SetOnKey(OnKeyCall OnKey) { listenersOnKey.push_back(OnKey); }

  void SetOnTimer(OnTimerCall OnTimer) {
    listenersOnTimer.push_back(OnTimer);
    passedTimes.push_back(OnTimer.first);
  }

protected:
  Model *model;
  bool finished;

  std::vector<OnKeyCall> listenersOnKey;
  std::vector<OnTimerCall> listenersOnTimer;
  std::vector<int> passedTimes;

  void PollOnKey(KeyCode key);

  void PollOnTimer(int microsecPassed);

  friend void SigHandler(int signum);
};
