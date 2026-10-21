<p align="center">
  <img src="./banner.png" alt="banner" width="400"/>
  <br/>
  <em><strong>X</strong>an <strong>G</strong>ame <strong>I</strong>nput</em>
</p>

What if Microsoft had never abandoned XInput? What if it supported more than just Xbox controllers, had support for modern features like touchpads, gyroscopes, accelerometers, and rear paddles?

This project is a modernization of the original XInput API. It aims to preserve its familiar feel and programming paradigm while simplifying the API, expanding controller support, and exposing modern controller features.

This is not a 1:1 replacement for the original XInput API. If that's what you're looking for, check out [InputFusion](https://github.com/xan105/InputFusion), the project this is built on.

This is the `XInput2_0.dll` Microsoft never gave us.

# What changed over the original XInput?

  - **Broader controller support:** Supports Xbox, PlayStation, Nintendo, and over 200 other controllers through SDL3.
  
  - **More buttons:** Supports up to 32 buttons, including rear paddles, touchpad button, and other special buttons.
  
  - **New APIs:**
    + `XGameInputGetTouchpadState()`: Retrieves the state of fingers on a touchpad.
    + `XGameInputSetLED()`: Updates the gamepad's LED.
    + `XGameInputGetSteamHandle()`: Returns a Steam's [InputHandle_t](https://partner.steamgames.com/doc/api/ISteamInput#InputHandle_t) for better integration with the Steam Input API
  
  - **Trigger motor support:** `XGameInputSetState()` also supports trigger motors.
  
  - **Expanded capabilities reporting:** `XGameInputGetCapabilities()` provides:
    + Gamepad type (PS4, PS5, Xbox 360, Steam Controller, etc.). 
    + Vendor and product IDs.
    + Connection status (whether the gamepad is connected wirelessly).
    + Support for rumble, LEDs, touchpads, gyroscopes, capacitive sensors, and other features.
  
  - **Simplified API:** Keeps the API focused with fewer, more straightforward functions:
    + Removed legacy and unsupported XInput APIs on modern versions of Windows.
    + Merged standard functions with their corresponding hidden `_Ex()` variants.
       
  - **More connected controllers:** Increases the maximum number of supported controllers from `4` to `8`.

Some structures have been slightly modified, while others have been completely overhauled. However, the underlying semantics and overall feel remain faithful to the original. The API retains XInput's familiar programming model.

# But why?

XInput has been the de facto standard for gamepad input for years. 

The only reason people are looking for alternatives is because of the lack of support for modern controllers and their features!

XInput's appeal was its flat, straightforward C API: simple functions with no unnecessary abstractions. It's easy to understand, easy to integrate, and does exactly what it needs to do.

Looking at GameInput's design _(XInput supposed successor)_, I can't help but see echoes of the DirectInput era: a growing maze of interfaces, objects, and methods that keeps expanding with each iteration. Having tried to reimplement DirectInput 1 through 8 on top of SDL3 for [InputFusion](https://github.com/xan105/InputFusion), this is exactly the kind of complexity I hoped we had left behind.

If you are a game developer, you should probably use SDL3. But if you don't want to deal with SDL3's quirks and specifics, and just want a straightforward, XInput-style API that works with modern controllers, this project is for you.

# Usage Example

// TODO: Write me (still a wip)

```cpp
#include <XGameInput.h>
#pragma comment(lib, "XGameInput.lib")

int main()
{
    DWORD result;
    for (DWORD i=0; i < XGAMEINPUT_PLAYER_MAX; i++ )
    {
      XGAMEINPUT_STATE state{};
      result = XGameInputGetState(i, &state);
      if(result == ERROR_SUCCESS)
      {
          // Controller is connected
          
          if (state.Buttons & XGAMEINPUT_BUTTON_DPAD_UP) {
              // The button is pressed
          }
      }
      else
      {
          // Controller is not connected
      }
}
```

<details>
<summary>Setting Vibration Effects</summary>

```cpp
XGAMEINPUT_VIBRATION vibration{};
// use any value between 0-65535 here
vibration.LeftMotorSpeed    = 32000; 
vibration.RightMotorSpeed   = 16000;
vibration.LeftTriggerSpeed  = 0; 
vibration.RightTriggerSpeed = 0;

XGameInputSetState(i, &vibration);
```

</details>

<details>
<summary>Update a gamepad's LED color</summary>

```cpp
XGAMEINPUT_GAMEPAD_LED rgb{};
rgb.Red   = 0;
rgb.Green = 0;
rgb.Blue  = 64;
// For gamepads with a single color LED, 
// the maximum of the RGB values will be used 
// as the LED brightness.

XGameInputSetLED(i, &rgb);
```

</details>

<details>
<summary>Gamepad battery</summary>

```cpp
XGAMEINPUT_BATTERY_INFORMATION batteryInfo{};
DWORD result = XGameInputGetState(i, &batteryInfo);
if(result == ERROR_SUCCESS && 
   batteryInfo.BatteryLevel >= 0 ) {
    printf("Battery: %d%%\n", batteryInfo.BatteryLevel);
}
```

</details>

<details>
<summary>Gamepad TouchPad</summary>

```cpp
XGAMEINPUT_TOUCHPAD_STATE state = {};
DWORD result = XGameInputGetTouchpadState(0, 0, &state);
if (result == ERROR_SUCCESS) {
    for (WORD i = 0; i < state.FingerCount; ++i)
    {
        const XGAMEINPUT_TOUCHPAD_FINGER& finger = state.Fingers[i];
        if (finger.Down)
        {
            printf(
                "Finger %u: X=%.3f, Y=%.3f, Pressure=%.3f\n",
                i,
                finger.X,
                finger.Y,
                finger.Pressure
            );
        }
    }
}
```

> [!IMPORTANT]
> The count represents the number of successfully queried finger slots, not the number of fingers currently touching the pad. Each slot's Down field tells you whether that finger is touching it.

</details>

# Bindings

// TODO: Write me

# Docs

// TODO: Write me

If you would like to receive gamepad updates while the application is in the background, you should set the following env. var. `SDL_HINT_JOYSTICK_ALLOW_BACKGROUND_EVENTS=1`

`XGameInputGetSteamHandle(DWORD PlayerIndex, UINT64* pInputHandle)`
    // Returns an InputHandle_t for the gamepad that can be used with Steam Input API: https://partner.steamgames.com/doc/api/ISteamInput
    // Returns the gamepad handle, or 0 if unavailable.
    // If you're using SDL gamepad support in a Steam game, you must call SteamAPI_InitEx() before calling SDL_Init().

# Build

// TODO: Write me

# Github Action

`./.github/workflows/vs-build-on-windows.yaml`

Build all targets (debug and release) and create a release.
