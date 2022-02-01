#ifndef INPUT_EVENT_H
#define INPUT_EVENT_H

enum class InputEventType {
  MoveForwards,
  MoveBackwards,
  MoveLeft,
  MoveRight,
  MoveUp,
  MoveDown,
  StopMoving,
  RotateClockwise,
  RotateAntiClockwise,
  StopRotating,
  Shoot,
  StopShoot,
  QuitToDesktop,
  MouseMove,
  RightMouseButtonDown,
  RightMouseButtonUp,
  LeftMouseButtonUp,
  LeftMouseButtonDown
};

struct InputEvent {
  InputEventType type;
  struct {
    int x, y, xrel, yrel;
  } mouseMove;

  struct {
    int x, y;
  } mouseLocation;
};

#endif //INPUT_EVENT_H
