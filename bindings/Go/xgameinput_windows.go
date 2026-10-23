//go:build windows

// These bindings were machine-generated.
// I'll replace them with proper ones later.
// This should be enough to get things going for now.

// Package xgameinput provides Go bindings for XGameInput.dll.
package xgameinput

import (
	"fmt"
	"runtime"
	"unsafe"

	"golang.org/x/sys/windows"
)

// Result codes returned directly by XGameInput.dll.
const (
	ErrorSuccess              uint32 = 0
	ErrorInvalidParameter     uint32 = 87
	ErrorDeviceNotConnected   uint32 = 1167
)

const (
	PlayerMax = 8
	TouchpadMaxFingers = 5
	MotionSensorsAxis = 3

	GamepadAxisMax = 32767
	GamepadAxisMin = -32768
	GamepadTriggerMax = 32767
	GamepadTriggerMin = 0
	GamepadLeftStickDeadzone = 7849
	GamepadRightStickDeadzone = 8689
	GamepadTriggerThreshold = 3855
)

const (
	ButtonSouth uint32 = 1 << iota
	ButtonEast
	ButtonWest
	ButtonNorth
	ButtonSelect
	ButtonGuide
	ButtonStart
	ButtonLeftStick
	ButtonRightStick
	ButtonLeftShoulder
	ButtonRightShoulder
	ButtonDpadUp
	ButtonDpadDown
	ButtonDpadLeft
	ButtonDpadRight
	ButtonRightPaddle1
	ButtonLeftPaddle1
	ButtonRightPaddle2
	ButtonLeftPaddle2
	ButtonTouchpad
	ButtonMisc1
	ButtonMisc2
	ButtonMisc3
	ButtonMisc4
	ButtonMisc5
	ButtonMisc6
)

const (
	IsWireless uint32 = 1 << iota
	HasRumble
	HasTriggerRumble
	HasLED
	HasPlayerLED
	HasTouchpad
	HasAccel
	HasGyro
	HasCapsenseStick
	HasCapsenseGrip
)

type GamepadType int32
const (
	GamepadTypeUnknown GamepadType = iota
	GamepadTypeXbox360
	GamepadTypeXboxOne
	GamepadTypePS3
	GamepadTypePS4
	GamepadTypePS5
	GamepadTypeSwitchPro
	GamepadTypeSteam
)

type BatteryState int32
const (
	BatteryStateUnknown BatteryState = iota
	BatteryStateCharged
	BatteryStateCharging
	BatteryStateDischarging
)

type TriggerEffect int32
const (
	TriggerEffectBoth TriggerEffect = iota
	TriggerEffectLeft
	TriggerEffectRight
)

type TriggerEffectMode int32
const (
	TriggerEffectModeOff TriggerEffectMode = iota
	TriggerEffectModeFeedback
	TriggerEffectModeWeapon
	TriggerEffectModeVibration
)

// Fields use fixed-width types to match Windows C ABI types.
type Gamepad struct {
	Buttons      uint32
	TriggerLeft  int16
	TriggerRight int16
	StickLeftX   int16
	StickLeftY   int16
	StickRightX  int16
	StickRightY  int16
}

type Vibration struct {
	LeftMotorSpeed    uint16
	RightMotorSpeed   uint16
	LeftTriggerSpeed  uint16
	RightTriggerSpeed uint16
}

type GamepadLED struct {
	Red   byte
	Green byte
	Blue  byte
}

type BatteryInformation struct {
	State   BatteryState
	Percent byte
}

type Capabilities struct {
	Type         GamepadType
	VendorID     uint16
	ProductID    uint16
	ProductVersion uint16
	Flags        uint16
	NbrButtons   uint32
	NbrTouchPads uint16
}

type TouchpadFinger struct {
	Down     byte // C bool is one byte; use 0 or 1.
	X        float32
	Y        float32
	Pressure float32
}

type Touchpad struct {
	FingerCount uint16
	Fingers     [TouchpadMaxFingers]TouchpadFinger
}

type MotionSensors struct {
	Accel          [3]float32
	AccelRate      float32
	AccelLeft      [3]float32
	AccelLeftRate  float32
	AccelRight     [3]float32
	AccelRightRate float32
	Gyro           [3]float32
	GyroRate       float32
	GyroLeft       [3]float32
	GyroLeftRate   float32
	GyroRight      [3]float32
	GyroRightRate  float32
}

type TriggerEffectParam struct {
	Position  byte
	Strength  byte
	Amplitude byte
	Frequency byte
	Start     byte
	End       byte
}

// API loads XGameInput.dll and resolves all exported functions. The DLL is
// kept loaded for the lifetime of the process; this avoids invalidating procs
// while calls may be in flight.
type API struct {
	dll *windows.LazyDLL
	getState, setState, getCapabilities, getBatteryInfo *windows.LazyProc
	getSteamHandle, setLED, getTouchpad, getMotionSensors, setTriggerEffect *windows.LazyProc
}

func Load() (*API, error) {
	dll := windows.NewLazyDLL("XGameInput.dll")
	if err := dll.Load(); err != nil {
		return nil, fmt.Errorf("load XGameInput.dll: %w", err)
	}
	a := &API{dll: dll}
	procs := []struct {
		name string
		dst  **windows.LazyProc
	}{
		{"XGameInputGetState", &a.getState},
		{"XGameInputSetState", &a.setState},
		{"XGameInputGetCapabilities", &a.getCapabilities},
		{"XGameInputGetBatteryInfo", &a.getBatteryInfo},
		{"XGameInputGetSteamHandle", &a.getSteamHandle},
		{"XGameInputSetLED", &a.setLED},
		{"XGameInputGetTouchpad", &a.getTouchpad},
		{"XGameInputGetMotionSensors", &a.getMotionSensors},
		{"XGameInputSetTriggerEffect", &a.setTriggerEffect},
	}
	for _, p := range procs {
		proc := dll.NewProc(p.name)
		if err := proc.Find(); err != nil {
			return nil, fmt.Errorf("resolve %s: %w", p.name, err)
		}
		*p.dst = proc
	}
	return a, nil
}

func (a *API) GetState(playerIndex uint32, out *Gamepad) uint32 {
	r, _, _ := a.getState.Call(uintptr(playerIndex), uintptr(unsafe.Pointer(out)))
	runtime.KeepAlive(out)
	return uint32(r)
}

func (a *API) SetState(playerIndex uint32, vibration *Vibration) uint32 {
	r, _, _ := a.setState.Call(uintptr(playerIndex), uintptr(unsafe.Pointer(vibration)))
	runtime.KeepAlive(vibration)
	return uint32(r)
}

func (a *API) GetCapabilities(playerIndex uint32, out *Capabilities) uint32 {
	r, _, _ := a.getCapabilities.Call(uintptr(playerIndex), uintptr(unsafe.Pointer(out)))
	runtime.KeepAlive(out)
	return uint32(r)
}

func (a *API) GetBatteryInfo(playerIndex uint32, out *BatteryInformation) uint32 {
	r, _, _ := a.getBatteryInfo.Call(uintptr(playerIndex), uintptr(unsafe.Pointer(out)))
	runtime.KeepAlive(out)
	return uint32(r)
}

func (a *API) GetSteamHandle(playerIndex uint32, out *uint64) uint32 {
	r, _, _ := a.getSteamHandle.Call(uintptr(playerIndex), uintptr(unsafe.Pointer(out)))
	runtime.KeepAlive(out)
	return uint32(r)
}

func (a *API) SetLED(playerIndex uint32, rgb *GamepadLED) uint32 {
	r, _, _ := a.setLED.Call(uintptr(playerIndex), uintptr(unsafe.Pointer(rgb)))
	runtime.KeepAlive(rgb)
	return uint32(r)
}

func (a *API) GetTouchpad(playerIndex uint32, touchpadIndex uint16, out *Touchpad) uint32 {
	r, _, _ := a.getTouchpad.Call(uintptr(playerIndex), uintptr(touchpadIndex), uintptr(unsafe.Pointer(out)))
	runtime.KeepAlive(out)
	return uint32(r)
}

func (a *API) GetMotionSensors(playerIndex uint32, out *MotionSensors) uint32 {
	r, _, _ := a.getMotionSensors.Call(uintptr(playerIndex), uintptr(unsafe.Pointer(out)))
	runtime.KeepAlive(out)
	return uint32(r)
}

// A nil param is passed as NULL, as permitted by the header when mode is off.
func (a *API) SetTriggerEffect(playerIndex uint32, trigger TriggerEffect, mode TriggerEffectMode, param *TriggerEffectParam) uint32 {
	r, _, _ := a.setTriggerEffect.Call(
		uintptr(playerIndex),
		uintptr(trigger),
		uintptr(mode),
		uintptr(unsafe.Pointer(param)),
	)
	runtime.KeepAlive(param)
	return uint32(r)
}
