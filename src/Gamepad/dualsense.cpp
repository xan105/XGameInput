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

#include "dualsense.h"

namespace dualsense {

    bool SetTriggerEffect(SDL_Gamepad *gamepad, uint8_t trigger_mask, const uint8_t effect[11]){
        if (gamepad == nullptr || effect == nullptr) {
            return false;
        }

        if (trigger_mask == 0 || (trigger_mask & ~DS5_TRIGGER_EFFECT_BOTH) != 0) {
            return false;
        }

        SDL_GamepadType type = SDL_GetGamepadType(gamepad);
        if (type != SDL_GAMEPAD_TYPE_PS5) {
            return false;
        }
        
        SDL_DS5_EffectsState effects;
        SDL_zero(effects);
        effects.ucEnableBits1 = trigger_mask;
            
        if (trigger_mask & DS5_TRIGGER_EFFECT_LEFT) {
            memcpy(effects.rgucLeftTriggerEffect, effect, 11);
        }
            
        if (trigger_mask & DS5_TRIGGER_EFFECT_RIGHT) {
            memcpy(effects.rgucRightTriggerEffect, effect, 11);
        }

        return SDL_SendGamepadEffect(gamepad, &effects, sizeof(effects));
    }

    bool CreateFeedbackEffect(uint8_t buffer[DS5_TRIGGER_EFFECT_SIZE], uint8_t position, uint8_t strength){
        if (buffer == NULL) {
            return false;
        }

        // Initialize output to OFF, including on invalid input
        memcpy(buffer, DS5_TRIGGER_EFFECT_OFF, DS5_TRIGGER_EFFECT_SIZE);

        if (position > 9 || strength > 8) {
            return false;
        }

        // Zero strength means OFF
        if (strength == 0) {
            return true;
        }

        // Enable all zones from `position` through zone 9.
        // Each zone has a 3-bit strength value.
        uint16_t active_zones = 0;
        uint32_t force_zones = 0;

        for (int i = position; i < 10; ++i) {
            active_zones |= (uint16_t)(1u << i);
            force_zones  |= (uint32_t)(strength - 1) << (3 * i);
        }

        buffer[0] = DS5_TRIGGER_EFFECT_MODE_FEEDBACK;
        buffer[1] = (uint8_t)(active_zones & 0xFF);
        buffer[2] = (uint8_t)(active_zones >> 8);
        buffer[3] = (uint8_t)(force_zones & 0xFF);
        buffer[4] = (uint8_t)(force_zones >> 8);
        buffer[5] = (uint8_t)(force_zones >> 16);
        buffer[6] = (uint8_t)(force_zones >> 24);

        return true;
    }

    bool CreateWeaponEffect(uint8_t buffer[DS5_TRIGGER_EFFECT_SIZE], uint8_t start_position, uint8_t end_position, uint8_t strength){
        if (buffer == NULL) {
            return false;
        }

        // Initialize output to OFF, including on invalid input
        memcpy(buffer, DS5_TRIGGER_EFFECT_OFF, DS5_TRIGGER_EFFECT_SIZE);

        if (start_position < 2 || start_position > 7 || end_position <= start_position || end_position > 8 || strength > 8) {
            return false;
        }

        /* Zero strength means no trigger effect. */
        if (strength == 0) {
            return true;
        }

        uint16_t zones = (uint16_t)((1u << start_position) | (1u << end_position));
        buffer[0] = DS5_TRIGGER_EFFECT_MODE_WEAPON;
        buffer[1] = (uint8_t)(zones & 0xFF);
        buffer[2] = (uint8_t)(zones >> 8);
        buffer[3] = (uint8_t)(strength - 1);

        return true;
    }

    bool CreateVibrationEffect(uint8_t buffer[DS5_TRIGGER_EFFECT_SIZE], uint8_t position, uint8_t amplitude, uint8_t frequency){
        if (buffer == NULL) {
            return false;
        }
        
        // Initialize output to OFF, including on invalid input
        memcpy(buffer, DS5_TRIGGER_EFFECT_OFF, DS5_TRIGGER_EFFECT_SIZE);

        if (position > 9 || amplitude > 8) {
            return false;
        }

        // Zero amplitude / frequency means OFF
        if (amplitude == 0 || frequency == 0) {
            return true;
        }
        
        uint16_t active_zones = 0;
        uint32_t packed_amplitude = 0;
        const uint32_t value = amplitude - 1;

        for (int i = position; i < 10; ++i) {
            active_zones |= (uint16_t)(1u << i);
            packed_amplitude |= value << (3 * i);
        }

        buffer[0] = DS5_TRIGGER_EFFECT_MODE_VIBRATION;
        buffer[1] = (uint8_t)(active_zones & 0xFF);
        buffer[2] = (uint8_t)(active_zones >> 8);
        buffer[3] = (uint8_t)(packed_amplitude & 0xFF);
        buffer[4] = (uint8_t)(packed_amplitude >> 8);
        buffer[5] = (uint8_t)(packed_amplitude >> 16);
        buffer[6] = (uint8_t)(packed_amplitude >> 24);
        buffer[9] = frequency;

        return true;
    }
}