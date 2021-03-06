#pragma once
#include <SFML/Graphics.hpp>
#include "bnUIComponent.h"
using sf::Font;
using sf::Text;
class Character;

/*! \brief Similar to PlayerHealthUI but draws under the mob */
class MobHealthUI : public UIComponent {
public:
  MobHealthUI() = delete;
  
  /**
   * @brief constructor character owns the component 
   */
  MobHealthUI(Character* _mob);
  
  /**
   * @brief deconstructor
   */
  ~MobHealthUI();

  /**
   * @brief Dials health to the mob's current health and colorizes
   * @param elapsed
   */
  virtual void OnUpdate(float elapsed);
  
  /**
   * @brief UI is drawn lest and must be injected into the battle scene
   * @param scene
   * 
   * Frees the owner of the component of resource management
   * but still maintains the pointer to the character
   */
  virtual void Inject(BattleScene& scene);
  
  /**
   * @brief Uses bitmap glyphs to draw game accurate health
   * @param target
   * @param states
   */
  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
  Character * mob; /*!< Owner of health */
  sf::Color color; /*!< Color of the glyphs */
  mutable sf::Sprite glyphs; /*!< Drawable texture */
  int healthCounter; /*!< mob's current health */
  double cooldown; /*!< Time after dial to uncolorize */
};