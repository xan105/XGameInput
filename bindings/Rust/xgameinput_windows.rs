#![cfg(windows)]
#![allow(non_snake_case)]
//!
//! These bindings were machine-generated.
//! I'll replace them with proper ones later.
//! This should be enough to get things going for now.
//!
//! Rust bindings for the C-compatible XGameInput.dll API.
//!
//! The DLL's functions return Windows-style DWORD error codes directly.
//! They are not GetLastError() values.

use core::ptr;
use windows_sys::Win32::Foundation::{FreeLibrary, HMODULE};
use windows_sys::Win32::System::LibraryLoader::{GetProcAddress, LoadLibraryW};

pub const ERROR_SUCCESS: u32 = 0;
pub const ERROR_INVALID_PARAMETER: u32 = 87;
pub const ERROR_DEVICE_NOT_CONNECTED: u32 = 1167;

pub const XGAMEINPUT_PLAYER_MAX: usize = 8;
pub const XGAMEINPUT_TOUCHPAD_MAX_FINGERS: usize = 5;
pub const XGAMEINPUT_MOTION_SENSORS_AXIS: usize = 3;

pub const XGAMEINPUT_GAMEPAD_AXIS_MAX: i16 = 32767;
pub const XGAMEINPUT_GAMEPAD_AXIS_MIN: i16 = -32768;
pub const XGAMEINPUT_GAMEPAD_TRIGGER_MAX: i16 = 32767;
pub const XGAMEINPUT_GAMEPAD_TRIGGER_MIN: i16 = 0;
pub const XGAMEINPUT_GAMEPAD_LEFT_STICK_DEADZONE: i16 = 7849;
pub const XGAMEINPUT_GAMEPAD_RIGHT_STICK_DEADZONE: i16 = 8689;
pub const XGAMEINPUT_GAMEPAD_TRIGGER_THRESHOLD: i16 = 3855;

pub const XGAMEINPUT_BUTTON_SOUTH: u32 = 1 << 0;
pub const XGAMEINPUT_BUTTON_EAST: u32 = 1 << 1;
pub const XGAMEINPUT_BUTTON_WEST: u32 = 1 << 2;
pub const XGAMEINPUT_BUTTON_NORTH: u32 = 1 << 3;
pub const XGAMEINPUT_BUTTON_SELECT: u32 = 1 << 4;
pub const XGAMEINPUT_BUTTON_GUIDE: u32 = 1 << 5;
pub const XGAMEINPUT_BUTTON_START: u32 = 1 << 6;
pub const XGAMEINPUT_BUTTON_LEFT_STICK: u32 = 1 << 7;
pub const XGAMEINPUT_BUTTON_RIGHT_STICK: u32 = 1 << 8;
pub const XGAMEINPUT_BUTTON_LEFT_SHOULDER: u32 = 1 << 9;
pub const XGAMEINPUT_BUTTON_RIGHT_SHOULDER: u32 = 1 << 10;
pub const XGAMEINPUT_BUTTON_DPAD_UP: u32 = 1 << 11;
pub const XGAMEINPUT_BUTTON_DPAD_DOWN: u32 = 1 << 12;
pub const XGAMEINPUT_BUTTON_DPAD_LEFT: u32 = 1 << 13;
pub const XGAMEINPUT_BUTTON_DPAD_RIGHT: u32 = 1 << 14;
pub const XGAMEINPUT_BUTTON_RIGHT_PADDLE1: u32 = 1 << 15;
pub const XGAMEINPUT_BUTTON_LEFT_PADDLE1: u32 = 1 << 16;
pub const XGAMEINPUT_BUTTON_RIGHT_PADDLE2: u32 = 1 << 17;
pub const XGAMEINPUT_BUTTON_LEFT_PADDLE2: u32 = 1 << 18;
pub const XGAMEINPUT_BUTTON_TOUCHPAD: u32 = 1 << 19;
pub const XGAMEINPUT_BUTTON_MISC1: u32 = 1 << 20;
pub const XGAMEINPUT_BUTTON_MISC2: u32 = 1 << 21;
pub const XGAMEINPUT_BUTTON_MISC3: u32 = 1 << 22;
pub const XGAMEINPUT_BUTTON_MISC4: u32 = 1 << 23;
pub const XGAMEINPUT_BUTTON_MISC5: u32 = 1 << 24;
pub const XGAMEINPUT_BUTTON_MISC6: u32 = 1 << 25;

pub const XGAMEINPUT_IS_WIRELESS: u16 = 1 << 0;
pub const XGAMEINPUT_HAS_RUMBLE: u16 = 1 << 1;
pub const XGAMEINPUT_HAS_TRIGGER_RUMBLE: u16 = 1 << 2;
pub const XGAMEINPUT_HAS_LED: u16 = 1 << 3;
pub const XGAMEINPUT_HAS_PLAYER_LED: u16 = 1 << 4;
pub const XGAMEINPUT_HAS_TOUCHPAD: u16 = 1 << 5;
pub const XGAMEINPUT_HAS_ACCEL: u16 = 1 << 6;
pub const XGAMEINPUT_HAS_GYRO: u16 = 1 << 7;
pub const XGAMEINPUT_HAS_CAPSENSE_STICK: u16 = 1 << 8;
pub const XGAMEINPUT_HAS_CAPSENSE_GRIP: u16 = 1 << 9;

#[repr(i32)]
#[derive(Debug, Copy, Clone, PartialEq, Eq)]
pub enum GamepadType {
    Unknown = 0,
    Xbox360 = 1,
    XboxOne = 2,
    PS3 = 3,
    PS4 = 4,
    PS5 = 5,
    SwitchPro = 6,
    Steam = 7,
}

#[repr(i32)]
#[derive(Debug, Copy, Clone, PartialEq, Eq)]
pub enum BatteryState {
    Unknown = 0,
    Charged = 1,
    Charging = 2,
    Discharging = 3,
}

#[repr(i32)]
#[derive(Debug, Copy, Clone, PartialEq, Eq)]
pub enum TriggerEffect {
    Both = 0,
    Left = 1,
    Right = 2,
}

#[repr(i32)]
#[derive(Debug, Copy, Clone, PartialEq, Eq)]
pub enum TriggerEffectMode {
    Off = 0,
    Feedback = 1,
    Weapon = 2,
    Vibration = 3,
}

#[repr(C)]
#[derive(Debug, Default, Copy, Clone)]
pub struct Gamepad {
    pub Buttons: u32,
    pub TriggerLeft: i16,
    pub TriggerRight: i16,
    pub StickLeftX: i16,
    pub StickLeftY: i16,
    pub StickRightX: i16,
    pub StickRightY: i16,
}

#[repr(C)]
#[derive(Debug, Default, Copy, Clone)]
pub struct Vibration {
    pub LeftMotorSpeed: u16,
    pub RightMotorSpeed: u16,
    pub LeftTriggerSpeed: u16,
    pub RightTriggerSpeed: u16,
}

#[repr(C)]
#[derive(Debug, Default, Copy, Clone)]
pub struct GamepadLed {
    pub Red: u8,
    pub Green: u8,
    pub Blue: u8,
}

#[repr(C)]
#[derive(Debug, Copy, Clone)]
pub struct BatteryInformation {
    pub State: BatteryState,
    pub Percent: u8,
}
impl Default for BatteryInformation {
    fn default() -> Self { Self { State: BatteryState::Unknown, Percent: 0 } }
}

#[repr(C)]
#[derive(Debug, Copy, Clone)]
pub struct Capabilities {
    pub Type: GamepadType,
    pub VendorId: u16,
    pub ProductId: u16,
    pub ProductVersion: u16,
    pub Flags: u16,
    pub nbrButtons: u32,
    pub nbrTouchPads: u16,
}
impl Default for Capabilities {
    fn default() -> Self {
        Self { Type: GamepadType::Unknown, VendorId: 0, ProductId: 0, ProductVersion: 0,
            Flags: 0, nbrButtons: 0, nbrTouchPads: 0 }
    }
}

// C bool is one byte in this struct; use false/true as 0/1 via `down()`.
#[repr(C)]
#[derive(Debug, Default, Copy, Clone)]
pub struct TouchpadFinger {
    pub Down: u8,
    pub X: f32,
    pub Y: f32,
    pub Pressure: f32,
}
impl TouchpadFinger {
    pub fn down(&self) -> bool { self.Down != 0 }
}

#[repr(C)]
#[derive(Debug, Copy, Clone)]
pub struct Touchpad {
    pub FingerCount: u16,
    pub Fingers: [TouchpadFinger; XGAMEINPUT_TOUCHPAD_MAX_FINGERS],
}
impl Default for Touchpad {
    fn default() -> Self {
        Self { FingerCount: 0, Fingers: [TouchpadFinger::default(); XGAMEINPUT_TOUCHPAD_MAX_FINGERS] }
    }
}

#[repr(C)]
#[derive(Debug, Copy, Clone, Default)]
pub struct MotionSensors {
    pub Accel: [f32; 3],
    pub AccelRate: f32,
    pub AccelLeft: [f32; 3],
    pub AccelLeftRate: f32,
    pub AccelRight: [f32; 3],
    pub AccelRightRate: f32,
    pub Gyro: [f32; 3],
    pub GyroRate: f32,
    pub GyroLeft: [f32; 3],
    pub GyroLeftRate: f32,
    pub GyroRight: [f32; 3],
    pub GyroRightRate: f32,
}

#[repr(C)]
#[derive(Debug, Default, Copy, Clone)]
pub struct TriggerEffectParam {
    pub position: u8,
    pub strength: u8,
    pub amplitude: u8,
    pub frequency: u8,
    pub start: u8,
    pub end: u8,
}

type GetStateFn = unsafe extern "system" fn(u32, *mut Gamepad) -> u32;
type SetStateFn = unsafe extern "system" fn(u32, *const Vibration) -> u32;
type GetCapabilitiesFn = unsafe extern "system" fn(u32, *mut Capabilities) -> u32;
type GetBatteryInfoFn = unsafe extern "system" fn(u32, *mut BatteryInformation) -> u32;
type GetSteamHandleFn = unsafe extern "system" fn(u32, *mut u64) -> u32;
type SetLedFn = unsafe extern "system" fn(u32, *const GamepadLed) -> u32;
type GetTouchpadFn = unsafe extern "system" fn(u32, u16, *mut Touchpad) -> u32;
type GetMotionSensorsFn = unsafe extern "system" fn(u32, *mut MotionSensors) -> u32;
type SetTriggerEffectFn = unsafe extern "system" fn(u32, TriggerEffect, TriggerEffectMode, *const TriggerEffectParam) -> u32;

#[derive(Debug)]
pub enum LoadError {
    LoadLibraryFailed(u32),
    MissingExport(&'static str),
}
impl core::fmt::Display for LoadError {
    fn fmt(&self, f: &mut core::fmt::Formatter<'_>) -> core::fmt::Result {
        match self {
            Self::LoadLibraryFailed(e) => write!(f, "LoadLibraryW(XGameInput.dll) failed: Windows error {e}"),
            Self::MissingExport(s) => write!(f, "XGameInput.dll is missing export {s}"),
        }
    }
}
impl std::error::Error for LoadError {}

/// Loaded XGameInput DLL. Function pointers use the Windows system ABI.
pub struct XGameInput {
    module: HMODULE,
    get_state: GetStateFn,
    set_state: SetStateFn,
    get_capabilities: GetCapabilitiesFn,
    get_battery_info: GetBatteryInfoFn,
    get_steam_handle: GetSteamHandleFn,
    set_led: SetLedFn,
    get_touchpad: GetTouchpadFn,
    get_motion_sensors: GetMotionSensorsFn,
    set_trigger_effect: SetTriggerEffectFn,
}

unsafe impl Send for XGameInput {}
unsafe impl Sync for XGameInput {}

impl XGameInput {
    /// Loads XGameInput.dll from the normal Windows DLL search path.
    pub fn load() -> Result<Self, LoadError> {
        let wide: Vec<u16> = "XGameInput.dll".encode_utf16().chain(Some(0)).collect();
        let module = unsafe { LoadLibraryW(wide.as_ptr()) };
        if module.is_null() {
            return Err(LoadError::LoadLibraryFailed(unsafe { windows_sys::Win32::Foundation::GetLastError() }));
        }

        unsafe fn resolve<T: Copy>(module: HMODULE, name: &'static str) -> Result<T, LoadError> {
            let c_name = std::ffi::CString::new(name).unwrap();
            let address = GetProcAddress(module, c_name.as_ptr().cast());
            let Some(address) = address else { return Err(LoadError::MissingExport(name)); };
            // Function pointer types are declared to match the header's WINAPI signatures.
            // All requested types are function pointers with the same pointer-sized representation.
            Ok(unsafe { core::mem::transmute_copy(&address) })
        }

        let loaded = unsafe {
            (|| {
                Ok(Self {
                    module,
                    get_state: resolve(module, "XGameInputGetState")?,
                    set_state: resolve(module, "XGameInputSetState")?,
                    get_capabilities: resolve(module, "XGameInputGetCapabilities")?,
                    get_battery_info: resolve(module, "XGameInputGetBatteryInfo")?,
                    get_steam_handle: resolve(module, "XGameInputGetSteamHandle")?,
                    set_led: resolve(module, "XGameInputSetLED")?,
                    get_touchpad: resolve(module, "XGameInputGetTouchpad")?,
                    get_motion_sensors: resolve(module, "XGameInputGetMotionSensors")?,
                    set_trigger_effect: resolve(module, "XGameInputSetTriggerEffect")?,
                })
            })()
        };
        if loaded.is_err() {
            unsafe { FreeLibrary(module); }
        }
        loaded
    }

    pub fn get_state(&self, player_index: u32, out: &mut Gamepad) -> u32 {
        unsafe { (self.get_state)(player_index, out) }
    }
    pub fn set_state(&self, player_index: u32, vibration: &Vibration) -> u32 {
        unsafe { (self.set_state)(player_index, vibration) }
    }
    pub fn get_capabilities(&self, player_index: u32, out: &mut Capabilities) -> u32 {
        unsafe { (self.get_capabilities)(player_index, out) }
    }
    pub fn get_battery_info(&self, player_index: u32, out: &mut BatteryInformation) -> u32 {
        unsafe { (self.get_battery_info)(player_index, out) }
    }
    pub fn get_steam_handle(&self, player_index: u32, out: &mut u64) -> u32 {
        unsafe { (self.get_steam_handle)(player_index, out) }
    }
    pub fn set_led(&self, player_index: u32, rgb: &GamepadLed) -> u32 {
        unsafe { (self.set_led)(player_index, rgb) }
    }
    pub fn get_touchpad(&self, player_index: u32, touchpad_index: u16, out: &mut Touchpad) -> u32 {
        unsafe { (self.get_touchpad)(player_index, touchpad_index, out) }
    }
    pub fn get_motion_sensors(&self, player_index: u32, out: &mut MotionSensors) -> u32 {
        unsafe { (self.get_motion_sensors)(player_index, out) }
    }
    /// Pass `None` when `mode == TriggerEffectMode::Off`, as permitted by the C header.
    pub fn set_trigger_effect(
        &self,
        player_index: u32,
        trigger: TriggerEffect,
        mode: TriggerEffectMode,
        param: Option<&TriggerEffectParam>,
    ) -> u32 {
        let ptr = param.map_or(ptr::null(), |p| p as *const _);
        unsafe { (self.set_trigger_effect)(player_index, trigger, mode, ptr) }
    }
}

impl Drop for XGameInput {
    fn drop(&mut self) {
        if !self.module.is_null() {
            unsafe { FreeLibrary(self.module); }
            self.module = ptr::null_mut();
        }
    }
}
