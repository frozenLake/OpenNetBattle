#pragma once
#include "bnEntity.h"
using sf::Texture;

/**
 * @brief Artifacts do not attack and they are not living. They are tile-rooted animations purely for visual effect.
 */
class Artifact : public Entity {
public:
  Artifact() = delete;
  Artifact(Field* _field);
  virtual ~Artifact();

  virtual void OnUpdate(float _elapsed) = 0;
  virtual void OnDelete() { }
  virtual void Update(float _elapsed) final;
  virtual void AdoptTile(Battle::Tile* tile) final;
};
