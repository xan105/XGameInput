/*
Copyright (c) Anthony Beaumont
This source code is licensed under the GNU LESSER GENERAL PUBLIC LICENSE Version 3
found in the LICENSE file in the root directory of this source tree.

The following ressources were used as reference:

- SDL3 Example showcasing how to send an effet to a DS5
  https://github.com/libsdl-org/SDL/blob/120c76c84bbce4c1bfed4e9eb74e10678bd83120/test/testgamecontroller.c#L286-L307
  Copyright (C) 1997-2022 Sam Lantinga <slouken@libsdl.org> | zlib License

- Thanks to @Nielk1 for the factory functions
  https://gist.github.com/Nielk1/6d54cc2c00d2201ccb8c2720ad7538db
  Copyright (c) 2021-2022 John "Nielk1" Klein | MIT License

- Unreal Engine doc was useful to confirm some assumptions
  https://dualsense.altercode.dev/v2/guides/adaptive-triggers

NB: This code is at the moment still untested on real hardware as I unforunately do not yet own a DS5 controller.
*/

#pragma once
#include <SDL3/SDL.h>
#include <cstddef>

#define DS5_TRIGGER_EFFECT_SIZE              11
#define DS5_TRIGGER_EFFECT_RIGHT             0x04
#define DS5_TRIGGER_EFFECT_LEFT              0x08
#define DS5_TRIGGER_EFFECT_BOTH              (DS5_TRIGGER_EFFECT_RIGHT | DS5_TRIGGER_EFFECT_LEFT)
    
// Offically recognized modes
#define DS5_TRIGGER_EFFECT_MODE_OFF          0x05
#define DS5_TRIGGER_EFFECT_MODE_FEEDBACK     0x21
#define DS5_TRIGGER_EFFECT_MODE_WEAPON       0x25
#define DS5_TRIGGER_EFFECT_MODE_VIBRATION    0x26

// https://github.com/libsdl-org/SDL/blob/120c76c84bbce4c1bfed4e9eb74e10678bd83120/test/testgamecontroller.c#L261
typedef struct SDL_DS5_EffectsState 
{
  uint8_t ucEnableBits1;
  uint8_t ucEnableBits2;
  uint8_t ucRumbleRight;
  uint8_t ucRumbleLeft;
  uint8_t ucHeadphoneVolume;
  uint8_t ucSpeakerVolume;
  uint8_t ucMicrophoneVolume;
  uint8_t ucAudioEnableBits;
  uint8_t ucMicLightMode;
  uint8_t ucAudioMuteBits;
  uint8_t rgucRightTriggerEffect[11];
  uint8_t rgucLeftTriggerEffect[11];
  uint8_t rgucUnknown1[6];
  uint8_t ucLedFlags;
  uint8_t rgucUnknown2[2];
  uint8_t ucLedAnim;
  uint8_t ucLedBrightness;
  uint8_t ucPadLights;
  uint8_t ucLedRed;
  uint8_t ucLedGreen;
  uint8_t ucLedBlue;
} SDL_DS5_EffectsState;

static_assert(sizeof(SDL_DS5_EffectsState) == 47, "Unexpected SDL DS5 effect structure size!");
static_assert(offsetof(SDL_DS5_EffectsState, rgucRightTriggerEffect) == 10, "Unexpected right trigger effect offset!");
static_assert(offsetof(SDL_DS5_EffectsState, rgucLeftTriggerEffect) == 21, "Unexpected left trigger effect offset!");

namespace dualsense {

    const uint8_t DS5_TRIGGER_EFFECT_OFF[DS5_TRIGGER_EFFECT_SIZE] = {
        DS5_TRIGGER_EFFECT_MODE_OFF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };

    bool SetTriggerEffect(SDL_Gamepad* gamepad, uint8_t trigger_mask, const uint8_t effect[11]);
    bool CreateFeedbackEffect(uint8_t buffer[DS5_TRIGGER_EFFECT_SIZE], uint8_t position, uint8_t strength);
    bool CreateWeaponEffect(uint8_t buffer[DS5_TRIGGER_EFFECT_SIZE], uint8_t start_position, uint8_t end_position, uint8_t strength);
    bool CreateVibrationEffect(uint8_t buffer[DS5_TRIGGER_EFFECT_SIZE], uint8_t position, uint8_t amplitude, uint8_t frequency);
}