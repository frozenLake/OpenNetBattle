#include "bnMettaurAttackState.h"
#include "bnWave.h"
#include "bnTile.h"
#include "bnField.h"
#include "bnMettaurIdleState.h"

MettaurAttackState::MettaurAttackState() : AIState<Mettaur>() { ; }
MettaurAttackState::~MettaurAttackState() { ; }

void MettaurAttackState::OnEnter(Mettaur& met) {
  auto onFinish = [this, &met]() {this->DoAttack(met); };

  if (met.GetRank() == Mettaur::Rank::SP) {
    met.SetAnimation("SP_ATTACK", onFinish);
  }
  else {
    met.SetAnimation("ATTACK", onFinish);
  }


  met.SetCounterFrame(3);
  met.SetCounterFrame(4);
  met.SetCounterFrame(5);
  met.SetCounterFrame(6);
  met.SetCounterFrame(7);
  met.SetCounterFrame(8);
}

void MettaurAttackState::OnUpdate(float _elapsed, Mettaur& met) {
  /* Nothing, just wait the animation out*/
}

void MettaurAttackState::OnLeave(Mettaur& met) { 
  met.EndMyTurn();
}

void MettaurAttackState::DoAttack(Mettaur& met) {
  if (met.GetField()->GetAt(met.tile->GetX() - 1, met.tile->GetY())->IsWalkable()) {
    Wave* spell = new Wave(met.field, met.team, (met.GetRank() == Mettaur::Rank::SP)? 1.2 : 1.0);

    auto props = spell->GetHitboxProperties();
    props.aggressor = &met;
    spell->SetHitboxProperties(props);

    spell->SetDirection(Direction::LEFT);
    met.field->AddEntity(*spell, met.tile->GetX() - 1, met.tile->GetY());
  }

  met.ChangeState<MettaurIdleState>();
}
