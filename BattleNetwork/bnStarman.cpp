#include "bnStarman.h"
#include "bnExplodeState.h"
#include "bnField.h"
#include "bnBuster.h"
#include "bnTextureResourceManager.h"
#include "bnAudioResourceManager.h"
#include "bnEngine.h"
#include "bnLogger.h"
#include "bnBusterCardAction.h"

#define RESOURCE_PATH "resources/navis/starman/starman.animation"

Starman::Starman() : Player()
{
  // Most of this is pretty redundant
  // But left as example
  SetName("Starman");
  SetLayer(0);
  SetTeam(Team::red);
  SetElement(Element::none);

  animationComponent->SetPath(RESOURCE_PATH);
  animationComponent->Reload();

  setTexture(TEXTURES.GetTexture(TextureType::NAVI_STARMAN_ATLAS));

  SetHealth(1200);

  // Starman has FloatShoe enabled
  SetFloatShoe(true);
}

Starman::~Starman() {

}

const float Starman::GetHeight() const
{
  return 140.0f;
}

CardAction* Starman::OnExecuteBusterAction()
{
  return new BusterCardAction(this, false, 1*GetAttackLevel());
}

CardAction* Starman::OnExecuteChargedBusterAction()
{
  return new BusterCardAction(this, true, 10*GetAttackLevel());
}

CardAction* Starman::OnExecuteSpecialAction()
{
  return Player::OnExecuteSpecialAction();
}
