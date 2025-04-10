/**
 * Copyright 2021 Charly Delay <charly@codesink.dev> (@0xcharly)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once

#ifdef VIA_ENABLE
#    define DYNAMIC_KEYMAP_LAYER_COUNT 4
#endif // VIA_ENABLE

// config.h 内に以下を追加
#ifndef SHARED_EP_ENABLE
#    define SHARED_EP_ENABLE yes
#endif

#ifndef KEYBOARD_SHARED_EP
#    define KEYBOARD_SHARED_EP yes
#endif

// もし USB サスペンド回避が必要なら
#ifndef NO_USB_SUSPEND
#    define NO_USB_SUSPEND
#endif

// ウォッチドッグの有効化（必要に応じて）
#ifndef WATCHDOG_ENABLE
#    define WATCHDOG_ENABLE yes
#endif
#ifndef WATCHDOG_TIMEOUT
#    define WATCHDOG_TIMEOUT 3000 // 3000msなど適切なタイムアウト時間に設定
#endif

#ifndef __arm__
/* Disable unused features. */
#    define NO_ACTION_ONESHOT
#endif // __arm__

/* Charybdis-specific features. */

#ifdef POINTING_DEVICE_ENABLE
// Automatically enable the pointer layer when moving the trackball.  See also:
// - `CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS`
// - `CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD`
// #define CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE
#endif // POINTING_DEVICE_ENABLE
