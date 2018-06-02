#pragma once
/**
 * @warning These have to be in order.
 * @see TextureResourceManager
*/
enum TextureType {
  //Tiles
  TILE_BLUE_NORMAL,
  TILE_BLUE_CRACKED,
  TILE_BLUE_BROKEN,
  TILE_BLUE_EMPTY,
  TILE_RED_NORMAL,
  TILE_RED_CRACKED,
  TILE_RED_BROKEN,
  TILE_RED_EMPTY,
  //Navis
  NAVI_MEGAMAN_MOVE,
  NAVI_MEGAMAN_HIT,
  NAVI_MEGAMAN_SHOOT,
  NAVI_MEGAMAN_ATLAS,
  //Mobs
  MOB_MOVE,
  MOB_EXPLOSION,
  MOB_METTAUR_IDLE,
  MOB_METTAUR_HIT,
  MOB_METTAUR_ATTACK,
  MOB_PROGSMAN_IDLE,
  MOB_PROGSMAN_MOVE,
  MOB_PROGSMAN_SHOOT,
  MOB_PROGSMAN_HIT,
  MOB_PROGSMAN_PUNCH,
  MOB_PROGSMAN_THROW,
  //Spells
  SPELL_BULLET_HIT,
  SPELL_BUSTER_CHARGE,
  //SPELL_CHARGED_BULLET_HIT,
  SPELL_WAVE,
  SPELL_PROG_BOMB,
  //Chip Select 
  CHIP_SELECT_MENU,
  CHIP_CURSOR_SMALL,
  CHIP_CURSOR_BIG,
  CHIP_CARDS,
  CHIP_ICONS,
  CHIP_NODATA,
  CHIP_SENDDATA,
  // Battle Result
  BATTLE_RESULTS_FRAME,
  //Background/Foreground
  BACKGROUND_BLUE,
  TITLE_ANIM_CHAR,
  GAME_OVER,
  GAMEPAD_SUPPORT_ICON,
  TEXTURE_TYPE_SIZE
};