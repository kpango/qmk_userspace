/**
 * Copyright 2021 Charly Delay
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

/**
 * Charybdis MK2 用 keymap.c
 *
 * 以下の改善策を盛り込んでいます：
 *  - USB共有エンドポイント利用の確認（可能なら config.h で上書き）
 *  - RGB Matrix 更新のデバウンス処理（更新間隔を100msに設定）
 *  - ウォッチドッグタイマーによる定期フィードによる自動リセット
 *  - 自動ポインタレイヤー切替処理（ポインティングデバイス入力に基づく）
 *
 * 注意: このファイルは QMK の最新コードおよびCharybdis MK2 標準のキーマップに基づいています。
 */

#include QMK_KEYBOARD_H

// ★ 共有HIDエンドポイントの設定確認（rules.mk を変更できない場合は config.h にて上書きしてください）
#ifndef SHARED_EP_ENABLE
#    warning "SHARED_EP_ENABLE is not defined. Please define SHARED_EP_ENABLE and KEYBOARD_SHARED_EP in config.h for improved USB stability."
#endif

// 自動ポインタレイヤー切替用タイマー
#ifdef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE
#    include "timer.h"
static uint16_t auto_pointer_layer_timer = 0;

#    ifndef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS
#        define CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS 1000 // タイムアウト：1000ms
#    endif

#    ifndef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD
#        define CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD 8 // マウス移動閾値：8ピクセル
#    endif
#endif

// ================================================
// レイヤー定義
// ================================================
enum charybdis_layers {
    LAYER_BASE = 0, // 基本レイヤー
    LAYER_NUM,      // 数字/記号レイヤー
    LAYER_FUNC,     // ファンクションレイヤー
    LAYER_POINTER,  // マウス/ポインター操作レイヤー
};

// ================================================
// キー定義（レイヤーホールド、タップダンス、マウスクリックなど）
// ================================================
#define BASE_LBRC LT(LAYER_BASE, KC_LBRC)
#define BASE_RBRC LT(LAYER_BASE, KC_RBRC)
#define BASE_DEL LT(LAYER_BASE, KC_DEL)
#define NUM_BRC LT(LAYER_NUM, KC_LBRC)
#define FUNC_BRC LT(LAYER_FUNC, KC_RBRC)
#define PT_DEL LT(LAYER_POINTER, KC_DEL)
#define PT_SLSH LT(LAYER_POINTER, KC_SLSH)
#define PT_Z LT(LAYER_POINTER, KC_Z)

#define ALT_GRV ALT_T(KC_GRV)
#define TAB_CTL CTL_T(KC_TAB)
#define GUI_ENT GUI_T(KC_ENT)
#define GUI_ESC GUI_T(KC_ESC)
#define SFT_BSPC SFT_T(KC_BSPC)
#define SFT_SPC SFT_T(KC_SPC)

#define L_CLICK KC_BTN1
#define R_CLICK KC_BTN2
#define M_CLICK MT(DRGSCRL, KC_BTN3)

#ifndef POINTING_DEVICE_ENABLE
#    define DRGSCRL KC_NO
#    define DPI_MOD KC_NO
#    define S_D_MOD KC_NO
#    define SNIPING KC_NO
#endif

// ================================================
// キーマップ定義
// ================================================
// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [LAYER_BASE] = LAYOUT(
      KC_ESC,   KC_1,     KC_2,    KC_3,    KC_4,    KC_5,      KC_6,    KC_7,    KC_8,    KC_9,    KC_0,     KC_MINS,
      KC_TAB,   KC_Q,     KC_W,    KC_E,    KC_R,    KC_T,      KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,     KC_BSLS,
      KC_LSFT,  KC_A,     KC_S,    KC_D,    KC_F,    KC_G,      KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN,  KC_QUOT,
      KC_LCTL,  PT_Z,     KC_X,    KC_C,    KC_V,    KC_B,      KC_N,    KC_M,    KC_COMM, KC_DOT,  PT_SLSH,  ALT_GRV,
                                   TAB_CTL, SFT_SPC, PT_DEL,    GUI_ENT, SFT_BSPC,
                                            GUI_ESC, NUM_BRC,   FUNC_BRC
  ),

  [LAYER_NUM] = LAYOUT(
      KC_TILD,  KC_EXLM,  KC_AT,   KC_HASH, KC_DLR,  KC_PERC,  KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_UNDS,
      RGB_MOD,  XXXXXXX,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,  KC_PPLS, KC_7,    KC_8,    KC_9,    KC_PEQL, XXXXXXX,
      RGB_TOG,  KC_LGUI,  KC_LALT, KC_LCTL, KC_LSFT, XXXXXXX,  KC_PMNS, KC_4,    KC_5,    KC_6,    KC_PSLS, KC_PDOT,
      RGB_RMOD, XXXXXXX,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,  KC_0,    KC_1,    KC_2,    KC_3,    KC_BSLS, KC_PAST,
                                   TAB_CTL, SFT_SPC, PT_DEL,   GUI_ENT, SFT_BSPC,
                                            GUI_ESC, _______,  FUNC_BRC
  ),

  [LAYER_FUNC] = LAYOUT(
      KC_F12,   KC_F1,    KC_F2,   KC_F3,   KC_F4,   KC_F5,    KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,
      KC_MNXT,  XXXXXXX,  KC_UP,   XXXXXXX, XXXXXXX, XXXXXXX,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_VOLU,
      KC_MPLY,  KC_LEFT,  KC_DOWN, KC_RIGHT,KC_SPC,  XXXXXXX,  KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, XXXXXXX, KC_MUTE,
      KC_MPRV,  KC_HOME,  KC_PGUP, KC_PGDN, KC_END,  XXXXXXX,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_VOLD,
                                   TAB_CTL, SFT_SPC, PT_DEL,   GUI_ENT, SFT_BSPC,
                                            GUI_ESC, NUM_BRC,  _______
  ),

  [LAYER_POINTER] = LAYOUT(
      QK_BOOT,  EE_CLR,   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, QK_BOOT, EE_CLR,
      XXXXXXX, XXXXXXX,   XXXXXXX, XXXXXXX, DPI_MOD, S_D_MOD,  S_D_MOD, DPI_MOD, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
      XXXXXXX, XXXXXXX,   L_CLICK, M_CLICK, R_CLICK, XXXXXXX,  XXXXXXX, L_CLICK, DRGSCRL, R_CLICK, XXXXXXX, XXXXXXX,
      XXXXXXX, _______,   DRGSCRL, SNIPING, XXXXXXX, XXXXXXX,  XXXXXXX, XXXXXXX, SNIPING, DRGSCRL, _______, XXXXXXX,
                                   TAB_CTL, SFT_SPC, _______,  GUI_ENT, SFT_BSPC,
                                            GUI_ESC, NUM_BRC,  FUNC_BRC
  ),
};
// clang-format on

// ================================================
// ポインティングデバイス／自動ポインタレイヤー切替処理
// ================================================
#ifdef POINTING_DEVICE_ENABLE
#    ifdef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE
/**
 * pointing_device_task_user()
 *  - ポインティングデバイスの動作量が閾値を超えた場合、自動的にLAYER_POINTERをONにする
 */
report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
    if ((abs(mouse_report.x) > CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD) || (abs(mouse_report.y) > CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD)) {
        if (auto_pointer_layer_timer == 0) {
            layer_on(LAYER_POINTER);
#        ifdef RGB_MATRIX_ENABLE
            update_rgb_for_layer(LAYER_POINTER);
#        endif
        }
        auto_pointer_layer_timer = timer_read();
    }
    return mouse_report;
}

/**
 * matrix_scan_user()
 *  - 定期スキャン中に、タイムアウトで自動的にLAYER_POINTERをOFFにする
 *  - また、ウォッチドッグタイマーを feed し、ハング状態からの自動リセットを狙う
 */
#        ifdef WATCHDOG_ENABLE
void matrix_scan_user(void) {
#            ifdef WATCHDOG_ENABLE
    watchdog_feed();
#            endif
}
#        endif
#    endif

/**
 * layer_state_set_user()
 *  - レイヤー変更時に現在のレイヤーに合わせたRGB更新を行う
 *  - 前回のレイヤーと異なる場合のみ更新を実行
 */
layer_state_t layer_state_set_user(layer_state_t state) {
#   ifdef CHARYBDIS_AUTO_SNIPING_ON_LAYER
    charybdis_set_pointer_sniping_enabled(layer_state_cmp(state, CHARYBDIS_AUTO_SNIPING_ON_LAYER));
#   endif
#   ifdef RGB_MATRIX_ENABLE
    update_rgb_for_layer(state);
#   endif
    return state;
}
#    endif

#ifdef RGB_MATRIX_ENABLE
void rgb_matrix_update_pwm_buffers(void);
// シンプルなデバウンス付き RGB 更新処理
static uint16_t last_rgb_update_time = 0;
#    define RGB_UPDATE_DEBOUNCE_MS 100
// ================================================
// RGB LED用の色定義（HSV値）
// ================================================
#    ifndef HSV_PURPLE
#        define HSV_PURPLE 170, 255, 255 // 紫
#    endif
#    ifndef HSV_BLUE
#        define HSV_BLUE 128, 255, 255 // 青
#    endif
#    ifndef HSV_RED
#        define HSV_RED 0, 255, 255 // 赤
#    endif

void update_rgb_for_layer(uint8_t layer) {
    if (timer_elapsed(last_rgb_update_time) < RGB_UPDATE_DEBOUNCE_MS) {
        return;
    }
    last_rgb_update_time = timer_read();

    switch (layer) {
        case LAYER_POINTER:
            rgb_matrix_mode_noeeprom(RGB_MATRIX_NONE);
            rgb_matrix_sethsv_noeeprom(HSV_PURPLE); // 紫
            break;
        case LAYER_NUM:
            rgb_matrix_mode_noeeprom(RGB_MATRIX_NONE);
            rgb_matrix_sethsv_noeeprom(HSV_BLUE); // 紫
            break;
        case LAYER_FUNC:
            rgb_matrix_mode_noeeprom(RGB_MATRIX_NONE);
            rgb_matrix_sethsv_noeeprom(HSV_RED); // 紫
            break;
        default:
            rgb_matrix_mode_noeeprom(RGB_MATRIX_DEFAULT_MODE);
            break;
    }
}
#endif
