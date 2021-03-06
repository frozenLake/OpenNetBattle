#pragma once
#include "bnArtifact.h"
#include "bnAnimationComponent.h"

class Field;

/**
 * @class ElementalDamage
 * @author mav
 * @date 04/05/19
 * @brief <!> symbol that appears on field when elemental damage occurs
 */
class ElementalDamage : public Artifact
{
private:
  AnimationComponent animationComponent;
  float progress;

public:
  ElementalDamage(Field* field);
  ~ElementalDamage();

  /**
   * @brief Grow and shrink quickly. Appear over the sprite.
   * @param _elapsed
   */
  virtual void OnUpdate(float _elapsed);
  virtual bool Move(Direction _direction) { return false; }

};
