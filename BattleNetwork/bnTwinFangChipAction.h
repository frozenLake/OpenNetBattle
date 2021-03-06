#pragma once
#include "bnChipAction.h"
#include "bnAnimation.h"
#include <SFML/Graphics.hpp>

class SpriteSceneNode;
class Character;
class TwinFangChipAction : public ChipAction {
private:
  sf::Sprite cannon;
  SpriteSceneNode* attachment;
  Animation attachmentAnim;
  int damage;
public:
  TwinFangChipAction(Character* owner, int damage);
  ~TwinFangChipAction();
  void OnUpdate(float _elapsed);
  void EndAction();
  void Execute();
};
