/*
Copyright (c) Anthony Beaumont
The source code in this directory, unless otherwise noted, falls under the MIT License
which is different(more permissive) than XGameInput's and InputFusion's license (LGPL3).
*/

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <array>
#include <chrono>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <string>
#include <thread>
#include <algorithm>

#include <XGameInput.h>
#pragma comment(lib, "XGameInput.lib")

namespace
{
    constexpr DWORD ControllerIndex = 0;                            // First Slot
    constexpr auto PollInterval = std::chrono::milliseconds(16);    // ~60 Hz while connected
    constexpr auto IdleInterval = std::chrono::milliseconds(250);   // Disconnected polling backs off to 250 ms.
    constexpr auto RumbleDuration = std::chrono::milliseconds(200);
    constexpr WORD RumbleLeftMotor = 32000;
    constexpr WORD RumbleRightMotor = 32000;

    struct ButtonInfo
    {
        DWORD mask;
        const char* name;
    };

    constexpr std::array<ButtonInfo, 26> Buttons{ {
        { XGAMEINPUT_BUTTON_DPAD_UP,            "DPad Up" },
        { XGAMEINPUT_BUTTON_DPAD_DOWN,          "DPad Down" },
        { XGAMEINPUT_BUTTON_DPAD_LEFT,          "DPad Left" },
        { XGAMEINPUT_BUTTON_DPAD_RIGHT,         "DPad Right" },
        { XGAMEINPUT_BUTTON_START,              "Start" },
        { XGAMEINPUT_BUTTON_SELECT,             "Select" },
        { XGAMEINPUT_BUTTON_GUIDE,              "GUIDE" },
        { XGAMEINPUT_BUTTON_TOUCHPAD,           "TouchPad" },
        { XGAMEINPUT_BUTTON_LEFT_STICK,         "Left Stick" },
        { XGAMEINPUT_BUTTON_RIGHT_STICK,        "Right Stick" },
        { XGAMEINPUT_BUTTON_LEFT_SHOULDER,      "Left Shoulder" },
        { XGAMEINPUT_BUTTON_RIGHT_SHOULDER,     "Right Shoulder" },
        { XGAMEINPUT_BUTTON_SOUTH,              "🗙" },
        { XGAMEINPUT_BUTTON_EAST,               "●" },
        { XGAMEINPUT_BUTTON_WEST,               "■" },
        { XGAMEINPUT_BUTTON_NORTH,              "▲" },
        { XGAMEINPUT_BUTTON_RIGHT_PADDLE1,      "Paddle R4" },
        { XGAMEINPUT_BUTTON_LEFT_PADDLE1,       "Paddle L4" },
        { XGAMEINPUT_BUTTON_RIGHT_PADDLE2,      "Paddle R5" },
        { XGAMEINPUT_BUTTON_LEFT_PADDLE2,       "Paddle L5" },
        { XGAMEINPUT_BUTTON_MISC1,              "MISC1" },
        { XGAMEINPUT_BUTTON_MISC2,              "MISC2" },
        { XGAMEINPUT_BUTTON_MISC3,              "MISC3" },
        { XGAMEINPUT_BUTTON_MISC4,              "MISC4" },
        { XGAMEINPUT_BUTTON_MISC5,              "MISC5" },
        { XGAMEINPUT_BUTTON_MISC6,              "MISC6" }
    } };

    void SetRumble(WORD leftMotor, WORD rightMotor)
    {
        XGAMEINPUT_VIBRATION vibration{};
        vibration.LeftMotorSpeed = leftMotor;
        vibration.RightMotorSpeed = rightMotor;

        XGameInputSetState(ControllerIndex, &vibration);
    }

    void LogButtonChanges(DWORD previous, DWORD current)
    {
        for (const auto& button : Buttons)
        {
            const bool wasPressed = (previous & button.mask) != 0;
            const bool isPressed = (current & button.mask) != 0;

            if (wasPressed == isPressed)
                continue;

            std::cout << "[EVENT] " << button.name << (isPressed ? " pressed" : " released") << '\n';
        }
    }

    void PrintState(const XGAMEINPUT_GAMEPAD& pad)
    {
        // Rewrite the live status area in place.
        std::cout << "\r"
            << "Buttons: 0x"
            << std::hex << std::setw(4) << std::setfill('0')
            << pad.Buttons
            << std::dec << std::setfill(' ')
            << " | LT: " << std::setw(3)
            << static_cast<int>(pad.TriggerLeft)
            << " RT: " << std::setw(3)
            << static_cast<int>(pad.TriggerRight)
            << " | LX: " << std::setw(6) << pad.StickLeftX
            << " LY: " << std::setw(6) << pad.StickLeftY
            << " | RX: " << std::setw(6) << pad.StickRightX
            << " RY: " << std::setw(6) << pad.StickRightY
            << "    " << std::flush;
    }

    const char* GetGamepadTypeName(XGAMEINPUT_GAMEPAD_TYPE type)
    {
        switch (type)
        {
        case XGAMEINPUT_GAMEPAD_TYPE_UNKNOWN:     return "Unknown";
        case XGAMEINPUT_GAMEPAD_TYPE_XBOX360:     return "Xbox 360";
        case XGAMEINPUT_GAMEPAD_TYPE_XBOXONE:     return "Xbox One";
        case XGAMEINPUT_GAMEPAD_TYPE_PS3:         return "PlayStation 3";
        case XGAMEINPUT_GAMEPAD_TYPE_PS4:         return "PlayStation 4";
        case XGAMEINPUT_GAMEPAD_TYPE_PS5:         return "PlayStation 5 / DualSense";
        case XGAMEINPUT_GAMEPAD_TYPE_SWITCH_PRO:  return "Nintendo Switch Pro";
        case XGAMEINPUT_GAMEPAD_TYPE_STEAM:       return "Steam Controller";
        default:                                  return "Unknown type";
        }
    }

    void PrintCapabilityFlags(WORD flags)
    {
        struct FlagInfo
        {
            WORD mask;
            const char* name;
        };

        constexpr FlagInfo FlagNames[] =
        {
            { XGAMEINPUT_IS_WIRELESS,        "Wireless" },
            { XGAMEINPUT_HAS_RUMBLE,         "Rumble" },
            { XGAMEINPUT_HAS_TRIGGER_RUMBLE, "Trigger rumble" },
            { XGAMEINPUT_HAS_LED,            "LED control" },
            { XGAMEINPUT_HAS_PLAYER_LED,     "Player indicator LED" },
            { XGAMEINPUT_HAS_TOUCHPAD,       "Touchpad" },
            { XGAMEINPUT_HAS_ACCEL,          "Accelerometer" },
            { XGAMEINPUT_HAS_GYRO,           "Gyroscope" },
            { XGAMEINPUT_HAS_CAPSENSE_STICK, "Capacitive stick sensing" },
            { XGAMEINPUT_HAS_CAPSENSE_GRIP,  "Grip sensing" }
        };

        std::cout << "Flags:          0x"
            << std::hex << std::setw(4) << std::setfill('0')
            << flags
            << std::dec << std::setfill(' ') << '\n';

        bool anyFlags = false;

        for (const auto& flag : FlagNames)
        {
            if ((flags & flag.mask) == 0)
                continue;

            std::cout << "  + " << flag.name << '\n';
            anyFlags = true;
        }

        if (!anyFlags)
            std::cout << "  None\n";

        constexpr WORD KnownFlags =
            XGAMEINPUT_IS_WIRELESS |
            XGAMEINPUT_HAS_RUMBLE |
            XGAMEINPUT_HAS_TRIGGER_RUMBLE |
            XGAMEINPUT_HAS_LED |
            XGAMEINPUT_HAS_PLAYER_LED |
            XGAMEINPUT_HAS_TOUCHPAD |
            XGAMEINPUT_HAS_ACCEL |
            XGAMEINPUT_HAS_GYRO |
            XGAMEINPUT_HAS_CAPSENSE_STICK |
            XGAMEINPUT_HAS_CAPSENSE_GRIP;

        const WORD unknownFlags = flags & ~KnownFlags;

        if (unknownFlags != 0)
        {
            std::cout << "  + Unknown bits: 0x"
                << std::hex << unknownFlags
                << std::dec << '\n';
        }
    }

    void PrintCapabilities(DWORD playerIndex)
    {
        XGAMEINPUT_CAPABILITIES caps{};

        const DWORD result =
            XGameInputGetCapabilities(playerIndex, &caps);

        std::cout << "\n\n[INFO] Controller Capabilities\n"
            << "-----------------------------\n";

        if (result != ERROR_SUCCESS)
        {
            std::cout << "[ERROR] XGameInputGetCapabilities failed: "
                << result << '\n';
            return;
        }

        std::cout << "Type:           " << GetGamepadTypeName(caps.Type)
            << '\n'
            << "Vendor ID:      0x"
            << std::hex << std::setw(4) << std::setfill('0')
            << caps.VendorId << '\n'
            << "Product ID:     0x"
            << std::setw(4) << caps.ProductId << '\n'
            << "Product Version:0x"
            << std::setw(4) << caps.ProductVersion
            << std::dec << std::setfill(' ') << '\n';

        PrintCapabilityFlags(caps.Flags);

        std::cout << "Buttons:        " << caps.nbrButtons << '\n'
            << "Touchpads:      " << caps.nbrTouchPads << '\n'
            << "-----------------------------\n"
            << std::flush;
    }

    const char* GetBatteryStateName(XGAMEINPUT_BATTERY_STATE state)
    {
        switch (state)
        {
        case XGAMEINPUT_BATTERY_STATE_UNKNOWN:      return "Unknown";
        case XGAMEINPUT_BATTERY_STATE_CHARGED:      return "Charged";
        case XGAMEINPUT_BATTERY_STATE_CHARGING:     return "Charging";
        case XGAMEINPUT_BATTERY_STATE_DISCHARGING:  return "Discharging";
        default:                                    return "Unrecognized";
        }
    }

    void PrintBatteryInfo(DWORD playerIndex)
    {
        XGAMEINPUT_BATTERY_INFORMATION battery{};

        const DWORD result =
            XGameInputGetBatteryInfo(playerIndex, &battery);

        std::cout << "\n\n[INFO] Controller Battery\n"
            << "------------------------\n";

        if (result != ERROR_SUCCESS)
        {
            std::cout << "[ERROR] XGameInputGetBatteryInfo failed: "
                << result << '\n';
            return;
        }

        std::cout << "State:      " << GetBatteryStateName(battery.State)
            << '\n'
            << "Charge:     ";

        if (battery.State == XGAMEINPUT_BATTERY_STATE_UNKNOWN)
        {
            std::cout << "Unavailable\n";
        }
        else
        {
            std::cout << static_cast<unsigned>(battery.Percent) << "%\n";
        }

        std::cout << "------------------------\n" << std::flush;
    }

    bool IsTouchpadFingerDown(DWORD playerIndex)
    {
        XGAMEINPUT_TOUCHPAD touchpad{};

        if (XGameInputGetTouchpad(playerIndex, 0, &touchpad)
            != ERROR_SUCCESS)
        {
            return false;
        }

        const WORD count = (std::min)(
            touchpad.FingerCount,
            static_cast<WORD>(XGAMEINPUT_TOUCHPAD_MAX_FINGERS));

        for (WORD i = 0; i < count; ++i)
        {
            if (touchpad.Fingers[i].Down)
                return true;
        }

        return false;
    }

    void PrintTouchpadState(DWORD playerIndex)
    {
        XGAMEINPUT_TOUCHPAD touchpad{};

        const DWORD result =
            XGameInputGetTouchpad(playerIndex, 0, &touchpad);

        if (result != ERROR_SUCCESS)
        {
            std::cout << "\rTouchpad: unavailable" << std::flush;
            return;
        }

        const WORD count = (std::min)(
            touchpad.FingerCount,
            static_cast<WORD>(XGAMEINPUT_TOUCHPAD_MAX_FINGERS));

        std::cout << "\rTouchpad: ";

        bool anyFingerDown = false;

        for (WORD i = 0; i < count; ++i)
        {
            const auto& finger = touchpad.Fingers[i];

            if (!finger.Down)
                continue;

            anyFingerDown = true;

            std::cout << "#" << i
                << " X:" << std::fixed << std::setprecision(3) << finger.X
                << " Y:" << finger.Y
                << " P:" << finger.Pressure << "  ";
        }

        if (!anyFingerDown)
            std::cout << "No fingers down";

        std::cout << "\033[K" << std::flush;
    }

    void PrintMotionSensors(DWORD playerIndex)
    {
        XGAMEINPUT_MOTION_SENSORS sensors{};

        const DWORD result =
            XGameInputGetMotionSensors(playerIndex, &sensors);

        if (result != ERROR_SUCCESS)
        {
            std::cout << "\rMotion sensors: unavailable\033[K" << std::flush;
            return;
        }

        std::cout << "\rAccel: X:" << std::fixed << std::setprecision(3)
            << std::setw(8) << sensors.Accel[0]
            << " Y:" << std::setw(8) << sensors.Accel[1]
            << " Z:" << std::setw(8) << sensors.Accel[2]
            << " Rate:" << std::setw(7) << sensors.AccelRate
            << " | Gyro: X:" << std::setw(8) << sensors.Gyro[0]
            << " Y:" << std::setw(8) << sensors.Gyro[1]
            << " Z:" << std::setw(8) << sensors.Gyro[2]
            << " Rate:" << std::setw(7) << sensors.GyroRate
            << "\033[K" << std::flush;
    }
}

int main()
{
    SetConsoleTitleW(L"XGameInput Controller Test");
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    std::cout << "XGameInput Controller Test\n"
              << "Controller index: " << ControllerIndex << '\n'
              << "Press GUIDE Button to exit.\n"
              << "Press START Button to display Controller capabilities.\n"
              << "Press SELECT Button to display Battery information.\n"
              << "Use the TouchPad to switch to TouchPad reading.\n"
              << "Press the left stick to toggle motion sensor reading.\n"
              << "Press CTRL+C to disconnect, and again to exit\n\n";

    bool connected = false;
    bool motionMode = false;
    DWORD previousButtons = 0;
    auto rumbleEnd = std::chrono::steady_clock::time_point{};
    bool rumbling = false;

    while (true)
    {
        XGAMEINPUT_GAMEPAD gamepad{};
        const DWORD result = XGameInputGetState(ControllerIndex, &gamepad);
        if (result == ERROR_SUCCESS)
        {
            if (!connected)
            {
                connected = true;
                previousButtons = 0;
                std::cout << "\n[INFO] Device connected\n";
            }

            // Exit
            if (gamepad.Buttons & XGAMEINPUT_BUTTON_GUIDE)
            {
                SetRumble(0, 0);
                rumbling = false;
                std::cout << "\n[INFO] GUIDE button pressed. Force exiting.\n" << std::flush;
                return 0;
            }

            // Print Caps
            const bool startPressed = (gamepad.Buttons & XGAMEINPUT_BUTTON_START) != 0;
            const bool startWasPressed = (previousButtons & XGAMEINPUT_BUTTON_START) != 0;
            if (startPressed && !startWasPressed)
            {
                PrintCapabilities(ControllerIndex);
            }

            // Print Battery
            const bool selectPressed = (gamepad.Buttons & XGAMEINPUT_BUTTON_SELECT) != 0;
            const bool selectWasPressed = (previousButtons & XGAMEINPUT_BUTTON_SELECT) != 0;
            if (selectPressed && !selectWasPressed)
            {
                PrintBatteryInfo(ControllerIndex);
            }

            // Toggle motion sensor display when the left stick button is pressed.
            const bool leftStickPressed = (gamepad.Buttons & XGAMEINPUT_BUTTON_LEFT_STICK) != 0;
            const bool leftStickWasPressed = (previousButtons & XGAMEINPUT_BUTTON_LEFT_STICK) != 0;
            if (leftStickPressed && !leftStickWasPressed)
            {
                motionMode = !motionMode;
                std::cout << "\n[INFO] Display mode: "
                    << (motionMode ? "Motion sensors" : "Gamepad state")
                    << '\n';
            }

            // Vibrate briefly when any button is newly pressed.
            const DWORD newlyPressed = gamepad.Buttons & ~previousButtons;
            if (newlyPressed != 0)
            {
                SetRumble(RumbleLeftMotor, RumbleRightMotor);
                rumbleEnd = std::chrono::steady_clock::now() + RumbleDuration;
                rumbling = true;
            }

            // Explicitly stop rumble when the duration expires.
            if (rumbling && std::chrono::steady_clock::now() >= rumbleEnd)
            {
                SetRumble(0, 0);
                rumbling = false;
            }

            LogButtonChanges(previousButtons, gamepad.Buttons);

            if (IsTouchpadFingerDown(ControllerIndex))
            {
                PrintTouchpadState(ControllerIndex);
            }
            else if (motionMode)
            {
                PrintMotionSensors(ControllerIndex);
            }
            else
            {
                PrintState(gamepad);
            }

            previousButtons = gamepad.Buttons;
        }
        else
        {
            if (connected)
            {
                SetRumble(0, 0);
                rumbling = false;
                connected = false;
                previousButtons = 0;
                std::cout << "\n[INFO] Device disconnected\n";
            }

            // Keep polling so a reconnected controller is detected.
            std::this_thread::sleep_for(IdleInterval);
            continue;
        }

        std::this_thread::sleep_for(PollInterval);
    }

    return 0;
}