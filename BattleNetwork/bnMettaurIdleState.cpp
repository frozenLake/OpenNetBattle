#include "bnMettaurIdleState.h"
#include "bnMettaurMoveState.h"
#include <iostream>

MettaurIdleState::MettaurIdleState() : cooldown(0.5), AIState<Mettaur>() { ; }
MettaurIdleState::~MettaurIdleState() { ; }

void MettaurIdleState::OnEnter(Mettaur& met) {
  if (met.GetRank() == Mettaur::Rank::SP) {
    met.SetAnimation("SP_IDLE");
  }
  else {
    met.SetAnimation("IDLE");
  }

  if (met.GetRank() == Mettaur::Rank::SP) {
    cooldown = 0.3f;
  }
}

void MettaurIdleState::OnUpdate(float _elapsed, Mettaur& met) {
  if (!met.IsMyTurn())
    return;

  cooldown -= _elapsed;

  if (cooldown < 0) {
    met.ChangeState<MettaurMoveState>();
  }
}

void MettaurIdleState::OnLeave(Mettaur& met) {
}

