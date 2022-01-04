#pragma once

enum class InputEventType {
  MoveForwards,
  MoveBackwards,
  MoveLeft,
  MoveRight,
  StopMoving,
  RotateClockwise,
  RotateAntiClockwise,
  StopRotating,
  Shoot,
  StopShoot,
  QuitToDesktop,
  MouseMove
};

struct InputEvent {
  InputEventType type;
  struct {
    int x, y, xrel, yrel;
  } mouseMove;
};
