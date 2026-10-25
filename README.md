<p align="center">
  <img src="./banner.png" alt="banner" width="256"/>
  <br/>
  <em><strong>X</strong>an <strong>G</strong>ame <strong>I</strong>nput</em>
</p>

What if Microsoft had never abandoned XInput? What if it supported more than just Xbox controllers, had support for modern features like touchpads, gyroscopes, accelerometers, haptic triggers and rear paddles?

This project is a modernization of the original XInput API. It aims to preserve its familiar feel and programming paradigm while simplifying the API, expanding controller support, and exposing modern controller features.

This is not a 1:1 replacement for the original XInput API. If that's what you're looking for, check out [InputFusion](https://github.com/xan105/InputFusion), the project this is built on.

This is the _"XInput2_0.dll"_ Microsoft never gave us.

# What changed over the original XInput?

  - **Broader controller support:** Supports Xbox, PlayStation, Nintendo, and over 200 other controllers through SDL3.
  
  - **More buttons:** Supports up to 32 buttons, including rear paddles, touchpad button, and other special buttons.
  
  - **Trigger motor support:** `XGameInputSetState()` also supports trigger motors (e.g. Xbox's Impulse triggers).
  
  - **New APIs:**
    + `XGameInputSetTriggerEffect()`: (Dualsense) Send a trigger effect (weapon, vibration, feedback).
    + `XGameInputGetTouchpad()`: Retrieves the state of fingers on a touchpad.
    + `XGameInputGetMotionSensors()`: Retrieves the controller's accelerometer and gyroscope data.
    + `XGameInputSetLED()`: Updates the gamepad's LED.
    + `XGameInputGetSteamHandle()`: Returns a Steam's [InputHandle_t](https://partner.steamgames.com/doc/api/ISteamInput#InputHandle_t) for better integration with the Steam Input API

  - **Expanded capabilities reporting:** `XGameInputGetCapabilities()` provides:
    + Gamepad type (PS4, PS5, Xbox 360, Steam Controller, etc.). 
    + Vendor and product IDs.
    + Connection status (whether the gamepad is connected wirelessly).
    + Support for rumble, LEDs, touchpads, gyroscopes, capacitive sensors, and other features.
  
  - **Simplified API:** Keeps the API focused with fewer, more straightforward functions:
    + Removed legacy and unsupported XInput APIs on modern versions of Windows.
    + Merged standard functions with their corresponding hidden `_Ex()` variants.
    + `XGameInputGetState()` returns the gamepad state directly (button press, axis and trigger values).
       
  - **More connected controllers:** Increases the maximum number of supported controllers from `4` to `8`.

Some structures have been slightly modified, while others have been completely overhauled. However, the underlying semantics and overall feel remain faithful to the original. The API retains XInput's familiar programming model.

# But why?

XInput has been the de facto standard for gamepad input for years. 

The only reason people are looking for alternatives is because of the lack of support for modern controllers and their features!

XInput's appeal was its flat, straightforward C API: simple functions with no unnecessary abstractions. It's easy to understand, easy to integrate, and does exactly what it needs to do.

Looking at GameInput's design _(XInput supposed successor)_, I can't help but see echoes of the DirectInput era: a growing maze of interfaces, objects, and methods that keeps expanding with each iteration. Having tried to reimplement DirectInput 1 through 8 on top of SDL3 for [InputFusion](https://github.com/xan105/InputFusion), this is exactly the kind of complexity I hoped we had left behind.

If you are a game developer, you should probably use SDL3. But if you don't want to deal with SDL3's quirks and specifics, and just want a straightforward, XInput-style API that works with modern controllers, this project is for you.

# Usage Example

```cpp
#include <XGameInput.h>
#pragma comment(lib, "XGameInput.lib")

int main()
{
    DWORD result;
    for (DWORD i=0; i < XGAMEINPUT_PLAYER_MAX; i++ )
    {
      XGAMEINPUT_GAMEPAD gamepad{};
      result = XGameInputGetState(i, &gamepad);
      if(result == ERROR_SUCCESS)
      {
          // Controller is connected
          
          if (gamepad.Buttons & XGAMEINPUT_BUTTON_DPAD_UP) {
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
vibration.LeftTriggerSpeed  = 0; // Xbox Impulse trigger
vibration.RightTriggerSpeed = 0; // Xbox Impulse trigger
XGameInputSetState(i, &vibration);
```

</details>

<details>
<summary>Dualsense Haptic triggers</summary>
<br />

**Clear both triggers**

```cpp
XGameInputSetTriggerEffect(
    playerIndex,
    XGAMEINPUT_TRIGGER_EFFECT_BOTH,
    XGAMEINPUT_TRIGGER_EFFECT_MODE_OFF,
    NULL // No parameters are needed for OFF
);
```

**Same effect to both triggers**

```cpp
// Vibration
XGAMEINPUT_TRIGGER_EFFECT_PARAM vibration = {};
vibration.position  = 2;
vibration.amplitude = 6;
vibration.frequency = 30;

XGameInputSetTriggerEffect(
  playerIndex,
  XGAMEINPUT_TRIGGER_EFFECT_BOTH,
  XGAMEINPUT_TRIGGER_EFFECT_MODE_VIBRATION,
  &vibration
);
```

**Different effects on each trigger**

```cpp
// Right trigger: a weapon-like resistance break.
XGAMEINPUT_TRIGGER_EFFECT_PARAM weapon = {};
weapon.start    = 2;
weapon.end      = 6;
weapon.strength = 7;

XGameInputSetTriggerEffect(
  playerIndex,
  XGAMEINPUT_TRIGGER_EFFECT_RIGHT,
  XGAMEINPUT_TRIGGER_EFFECT_MODE_WEAPON,
  &weapon
);

// Left trigger: progressive feedback resistance.
XGAMEINPUT_TRIGGER_EFFECT_PARAM feedback = {};
feedback.position = 3;
feedback.strength = 5;

XGameInputSetTriggerEffect(
  playerIndex,
  XGAMEINPUT_TRIGGER_EFFECT_LEFT,
  XGAMEINPUT_TRIGGER_EFFECT_MODE_FEEDBACK,
  &feedback
);
```

</details>

<details>
<summary>Gamepad TouchPad</summary>

```cpp
XGAMEINPUT_TOUCHPAD touchpad = {};
DWORD result = XGameInputGetTouchpad(0, 0, &touchpad);
if (result == ERROR_SUCCESS) {
    for (WORD i = 0; i < touchpad.FingerCount; ++i)
    {
        const XGAMEINPUT_TOUCHPAD_FINGER& finger = touchpad.Fingers[i];
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
</details>

<details>
<summary>Motion Sensor (accel and gyro data)</summary>

```cpp
DWORD playerIndex = 0;
XGAMEINPUT_MOTION_SENSORS sensors{};

if (XGameInputGetMotionSensors(playerIndex, &sensors) == ERROR_SUCCESS)
{
  printf("Accelerometer: X=%.3f Y=%.3f Z=%.3f (Rate: %.1f Hz)\n",
          sensors.Accel[0], // X
          sensors.Accel[1], // Y
          sensors.Accel[2], // Z
          sensors.AccelRate);

  printf("Gyroscope: X=%.3f Y=%.3f Z=%.3f (Rate: %.1f Hz)\n",
          sensors.Gyro[0], // X
          sensors.Gyro[1], // Y
          sensors.Gyro[2], // Z
          sensors.GyroRate);
}
```

</details>

<details>
<summary>Update a gamepad's LED color</summary>

```cpp
XGAMEINPUT_GAMEPAD_LED rgb{};
rgb.Red   = 0;
rgb.Green = 0;
rgb.Blue  = 64;
XGameInputSetLED(i, &rgb);

// For gamepads with a single color LED, 
// the maximum of the RGB values will be used 
// as the LED brightness.
```

</details>

<details>
<summary>Gamepad battery</summary>

```cpp
XGAMEINPUT_BATTERY_INFORMATION battery{};
DWORD result = XGameInputGetBatteryInfo(i, &battery);
if(result == ERROR_SUCCESS && 
   battery.Percent >= 0 ) {
    printf("Battery: %d%%\n", battery.Percent);
}
```

</details>

> [!TIP]
> You are welcome to reuse any code from the [testcontroller](./test/testcontroller/src/testcontroller_console/main.cpp) that may be useful for your own projects.
> It is provided under a different license than the main library.

# Bindings

Bindings for other languages:

- [Go](./bindings/Go/xgameinput_windows.go)
- [Rust](./bindings/Rust/xgameinput_windows.rs)

> [!NOTE]
> These bindings were machine-generated. I'll replace them with proper ones later. This should be enough to get things going for now.

Caveats
=======

- While waiting for SDL to initialize, the first call to any of XGameInput's exported functions is stalled and may take longer than usual.

- Since Windows 10 the game controller input is automatically enabled/disabled by the system based on the application window focus.
  If you would like to receive gamepad updates while the application is in the background, you should set the following env. var. `SDL_HINT_JOYSTICK_ALLOW_BACKGROUND_EVENTS=1`

# Build

🆚 **Visual Studio 2026**

📦 Vendor dependencies:
  - [Microsoft Detours library](https://github.com/microsoft/Detours)
  - [SDL3 Library](https://github.com/libsdl-org/SDL) recompiled as a static lib.

**Solution**: `./vc/XGameInput.sln`

**Output directory**: `./build/${project}/output/${platform}/${config}`

The build generates:

- `XGameInput.dll`: Dynamic-link library containing the implementation.
- `XGameInput.lib`: Import library for linking applications against the DLL.

**Public header**: `./src/XGameInput/XGameInput.h`

The public header supports both `C` and `C++`.

## Github Action

`./.github/workflows/vs-build-on-windows.yaml`

Build all targets (debug and release) and create a release.

Licensing FAQ
=============

- **Why LGPL-3.0?**

The LGPL allows XGameInput to be used in proprietary software while ensuring that improvements to the library remain available under the same license.

- **Can I use XGameInput commercially?**

Yes! Commercial use is permitted, including in proprietary applications. XGameInput is provided as a shared library, so your application does not need to be open-source just because it uses XGameInput.

- **Can I modify the source code?**

Yes! You can modify XGameInput to suit your needs. However, if you distribute your modified version, you must provide the corresponding source code under the LGPL-3.0.

- **Do I have to open-source my project?**

No! Using XGameInput does not mean you have to publish your application's source code. You can keep your project proprietary.

- **Are there any fees or royalties?**

No! XGameInput is free to use, including commercially.

- **Need more details?**

This FAQ covers common use cases in plain English. For the full terms and conditions, including requirements that may apply when distributing XGameInput or software that uses it, please refer to the LGPL-3.0 license included with this project.