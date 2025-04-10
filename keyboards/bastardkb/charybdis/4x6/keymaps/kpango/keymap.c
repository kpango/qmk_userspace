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

#include QMK_KEYBOARD_H

// ★ 注意: 安定性向上のため、必ず rules.mk 内で下記設定を行ってください。
// SHARED_EP_ENABLE = yes
// KEYBOARD_SHARED_EP = yes
#ifndef SHARED_EP_ENABLE
#    warning "SHARED_EP_ENABLE is not defined. For improved stability with RGB Matrix and pointing device, please enable SHARED_EP_ENABLE and KEYBOARD_SHARED_EP in rules.mk."
#endif

#ifdef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE
#    include "timer.h"
#endif // CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE

// ================================================
// レイヤー定義
// ================================================
enum charybdis_keymap_layers {
    LAYER_BASE = 0, // 基本レイヤー
    LAYER_NUM,      // 数字/記号レイヤー
    LAYER_FUNC,     // ファンクションレイヤー
    LAYER_POINTER,  // マウス/ポインター操作レイヤー
};

// 自動スナイピング機能でポインターレイヤーを有効にするための定義
#define CHARYBDIS_AUTO_SNIPING_ON_LAYER LAYER_POINTER

#ifdef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE
static uint16_t auto_pointer_layer_timer = 0;

#    ifndef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS
#        define CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS 1000 // タイムアウト時間（ミリ秒）
#    endif                                                           // CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS

#    ifndef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD
#        define CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD 8 // マウス移動の閾値（ピクセル数など）
#    endif                                                       // CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD
#endif                                                           // CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE

// ================================================
// キー定義（レイヤーホールド、タップダンス、マウスクリックなど）
// ================================================

// レイヤー切り替えホールドキーの定義
#define BASE_LBRC LT(LAYER_BASE, KC_LBRC)
#define BASE_RBRC LT(LAYER_BASE, KC_RBRC)
#define BASE_DEL LT(LAYER_BASE, KC_DEL)
#define NUM_BRC LT(LAYER_NUM, KC_LBRC)
#define FUNC_BRC LT(LAYER_FUNC, KC_RBRC)
#define PT_DEL LT(LAYER_POINTER, KC_DEL)
#define PT_SLSH LT(LAYER_POINTER, KC_SLSH)
#define PT_Z LT(LAYER_POINTER, KC_Z)

// タップダンスキーの定義
#define ALT_GRV ALT_T(KC_GRV)
#define TAB_CTL CTL_T(KC_TAB)
#define GUI_ENT GUI_T(KC_ENT)
#define GUI_ESC GUI_T(KC_ESC)
#define SFT_BSPC SFT_T(KC_BSPC)
#define SFT_SPC SFT_T(KC_SPC)

// マウスクリックキーの定義
#define L_CLICK KC_BTN1
#define R_CLICK KC_BTN2
#define M_CLICK MT(DRGSCRL, KC_BTN3)

// ポインティングデバイスが有効でない場合のダミー定義
#ifndef POINTING_DEVICE_ENABLE
#    define DRGSCRL KC_NO
#    define DPI_MOD KC_NO
#    define S_D_MOD KC_NO
#    define SNIPING KC_NO
#endif // !POINTING_DEVICE_ENABLE

// ================================================
// RGB LED用の色定義補足（HSV_PURPLE が未定義の場合）
// ================================================
#ifndef HSV_PURPLE
#    define HSV_PURPLE 170, 255, 255 // 紫色: 色相170, 彩度255, 明度255
#endif
#ifndef HSV_BLUE
#    define HSV_BLUE 128, 255, 255 // 青色: 色相128, 彩度255, 明度255
#endif
#ifndef HSV_RED
#    define HSV_RED 0, 255, 255 // 赤色: 色相0, 彩度255, 明度255
#endif

// ================================================
// キーマップ定義
// ================================================
// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [LAYER_BASE] = LAYOUT(
    // ╭─────────────────────────────────────────────────────╮  ╭──────────────────────────────────────────────────────╮
         KC_ESC,     KC_1,    KC_2,    KC_3,    KC_4,    KC_5,      KC_6,    KC_7,    KC_8,    KC_9,    KC_0,      KC_MINS,
    // ├─────────────────────────────────────────────────────┤  ├──────────────────────────────────────────────────────┤
         KC_TAB,     KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,      KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,      KC_BSLS,
    // ├─────────────────────────────────────────────────────┤  ├──────────────────────────────────────────────────────┤
         KC_LSFT,    KC_A,    KC_S,    KC_D,    KC_F,    KC_G,      KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN,   KC_QUOT,
    // ├─────────────────────────────────────────────────────┤  ├──────────────────────────────────────────────────────┤
         KC_LCTL,    PT_Z,    KC_X,    KC_C,    KC_V,    KC_B,      KC_N,    KC_M,    KC_COMM, KC_DOT,  PT_SLSH,   ALT_GRV,
    // ╰─────────────────────────────────────────────────────╯  ╰──────────────────────────────────────────────────────╯
                                      TAB_CTL, SFT_SPC,  PT_DEL,   GUI_ENT, SFT_BSPC,
                                               GUI_ESC,  NUM_BRC,  FUNC_BRC
    //                           ╰────────────────────────────────────────╯  ╰──────────────────────────────╯
  ),

  [LAYER_NUM] = LAYOUT(
    // ╭──────────────────────────────────────────────────────╮  ╭──────────────────────────────────────────────────────╮
         KC_TILD, KC_EXLM,   KC_AT,   KC_HASH, KC_DLR,  KC_PERC,  KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_UNDS,
    // ├──────────────────────────────────────────────────────┤  ├──────────────────────────────────────────────────────┤
         RGB_MOD, XXXXXXX,  XXXXXXX,  XXXXXXX, XXXXXXX, XXXXXXX,  KC_PPLS, KC_7,    KC_8,    KC_9,    KC_PEQL, XXXXXXX,
    // ├──────────────────────────────────────────────────────┤  ├──────────────────────────────────────────────────────┤
         RGB_TOG, KC_LGUI,  KC_LALT,  KC_LCTL, KC_LSFT, XXXXXXX,  KC_PMNS, KC_4,    KC_5,    KC_6,    KC_PSLS, KC_PDOT,
    // ├──────────────────────────────────────────────────────┤  ├──────────────────────────────────────────────────────┤
        RGB_RMOD, XXXXXXX,  XXXXXXX,  XXXXXXX, XXXXXXX, XXXXXXX,  KC_0,    KC_1,    KC_2,    KC_3,    KC_BSLS, KC_PAST,
    // ╰──────────────────────────────────────────────────────╯
                                      TAB_CTL, SFT_SPC, PT_DEL,   GUI_ENT, SFT_BSPC,
                                               GUI_ESC, NUM_BRC,  FUNC_BRC
  ),

  [LAYER_FUNC] = LAYOUT(
    // ╭──────────────────────────────────────────────────────╮  ╭──────────────────────────────────────────────────────╮
         KC_F12,  KC_F1,    KC_F2,   KC_F3,   KC_F4,   KC_F5,     KC_F6,   KC_F7,   KC_F8,   KC_F9,  KC_F10,  KC_F11,
    // ├──────────────────────────────────────────────────────┤  ├──────────────────────────────────────────────────────┤
         KC_MNXT, XXXXXXX,  KC_UP,   XXXXXXX, XXXXXXX, XXXXXXX,   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_VOLU,
    // ├──────────────────────────────────────────────────────┤  ├──────────────────────────────────────────────────────┤
         KC_MPLY, KC_LEFT,  KC_DOWN, KC_RIGHT,KC_SPC,  XXXXXXX,   KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, XXXXXXX, KC_MUTE,
    // ├──────────────────────────────────────────────────────┤  ├──────────────────────────────────────────────────────┤
         KC_MPRV, KC_HOME,  KC_PGUP, KC_PGDN, KC_END,  XXXXXXX,   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_VOLD,
    // ╰──────────────────────────────────────────────────────╯
                                     TAB_CTL, SFT_SPC, PT_DEL,    GUI_ENT, SFT_BSPC,
                                              GUI_ESC, NUM_BRC,   FUNC_BRC
  ),

  [LAYER_POINTER] = LAYOUT(
    // ╭──────────────────────────────────────────────────────╮  ╭──────────────────────────────────────────────────────╮
         QK_BOOT,  EE_CLR,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, QK_BOOT,  EE_CLR,
    // ├──────────────────────────────────────────────────────┤  ├──────────────────────────────────────────────────────┤
         XXXXXXX, XXXXXXX,  XXXXXXX, XXXXXXX, DPI_MOD, S_D_MOD,  S_D_MOD, DPI_MOD, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
    // ├──────────────────────────────────────────────────────┤  ├──────────────────────────────────────────────────────┤
         XXXXXXX, XXXXXXX,  L_CLICK, M_CLICK, R_CLICK, XXXXXXX,  XXXXXXX, L_CLICK, DRGSCRL, R_CLICK, XXXXXXX, XXXXXXX,
    // ├──────────────────────────────────────────────────────┤  ├──────────────────────────────────────────────────────┤
         XXXXXXX, _______,  DRGSCRL, SNIPING,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, SNIPING, DRGSCRL, _______, XXXXXXX,
    // ╰──────────────────────────────────────────────────────╯
                                      TAB_CTL, SFT_SPC,  PT_DEL,   GUI_ENT, SFT_BSPC,
                                              GUI_ESC,  NUM_BRC,  FUNC_BRC
  ),
};
// clang-format on

// ================================================
// RGB更新関数の共通化（各レイヤーに応じたRGB LED設定）
// ================================================
#ifdef RGB_MATRIX_ENABLE

// デバウンス用の変数と間隔定義（単位: ミリ秒）
static uint16_t last_rgb_update_time = 0;
#    define RGB_UPDATE_DEBOUNCE_MS 100

/**
 * @brief 指定されたレイヤーに応じたRGB LEDの設定を行う
 *
 * 基本レイヤーではデフォルトのイルミネーションに戻し、
 * その他のレイヤーでは固定色に設定します。なお、連続更新を防ぐため
 * 更新間隔が短い場合は処理をスキップします。
 *
 * @param layer 現在のアクティブなレイヤー番号
 */
void update_rgb_for_layer(uint8_t layer) {
    if (timer_elapsed(last_rgb_update_time) < RGB_UPDATE_DEBOUNCE_MS) {
        return;
    }
    last_rgb_update_time = timer_read();

    switch (layer) {
        case LAYER_POINTER:
            rgb_matrix_mode_noeeprom(RGB_MATRIX_NONE);
            rgb_matrix_sethsv_noeeprom(HSV_PURPLE);
            break;
        case LAYER_NUM:
            rgb_matrix_mode_noeeprom(RGB_MATRIX_NONE);
            rgb_matrix_sethsv_noeeprom(HSV_BLUE);
            break;
        case LAYER_FUNC:
            rgb_matrix_mode_noeeprom(RGB_MATRIX_NONE);
            rgb_matrix_sethsv_noeeprom(HSV_RED);
            break;
        default:
            rgb_matrix_mode_noeeprom(RGB_MATRIX_DEFAULT_MODE);
            break;
    }
}
#endif // RGB_MATRIX_ENABLE

// ================================================
// ポインティングデバイス及びレイヤー変更処理
// ================================================
#ifdef POINTING_DEVICE_ENABLE
#    ifdef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE
/**
 * @brief マウス移動量に応じてポインターレイヤーを自動で有効化する
 *
 * マウスの移動が設定した閾値を超えると、ポインターレイヤーをONにし、
 * RGB LEDの更新も実施します。
 */
report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
    if (abs(mouse_report.x) > CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD || abs(mouse_report.y) > CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD) {
        if (auto_pointer_layer_timer == 0) {
            layer_on(LAYER_POINTER);
#        ifdef RGB_MATRIX_ENABLE
            update_rgb_for_layer(LAYER_POINTER);
#        endif // RGB_MATRIX_ENABLE
        }
        auto_pointer_layer_timer = timer_read();
    }
    return mouse_report;
}

/**
 * @brief 定期スキャン処理でポインターレイヤーのタイムアウトを監視する
 *
 * 一定時間経過後、ポインターレイヤーを自動でOFFにし、RGB LEDをデフォルト状態に戻します。
 * また、ウォッチドッグタイマーを feed してシステムの自動リセットを狙います。
 */
void matrix_scan_user(void) {
#        ifdef WATCHDOG_ENABLE
    watchdog_feed();
#        endif

    if (auto_pointer_layer_timer != 0 && TIMER_DIFF_16(timer_read(), auto_pointer_layer_timer) >= CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS) {
        auto_pointer_layer_timer = 0;
        layer_off(LAYER_POINTER);
#        ifdef RGB_MATRIX_ENABLE
        rgb_matrix_mode_noeeprom(RGB_MATRIX_DEFAULT_MODE);
#        endif
    }
}
#    endif // CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE

#    ifdef CHARYBDIS_AUTO_SNIPING_ON_LAYER
/**
 * @brief レイヤー変更時に呼ばれるユーザー定義関数
 *
 * ポインターレイヤーのスナイピング機能を有効にし、
 * 現在のアクティブレイヤーに応じたRGB LEDの更新を実施します。
 * 前回のレイヤー状態と比較し、変更があった場合のみ更新を行います。
 */
layer_state_t layer_state_set_user(layer_state_t state) {
    static uint8_t last_layer    = LAYER_BASE;
    uint8_t        current_layer = get_highest_layer(state);

    if (current_layer != last_layer) {
        last_layer = current_layer;
#        ifdef RGB_MATRIX_ENABLE
        update_rgb_for_layer(current_layer);
#        endif
    }
    charybdis_set_pointer_sniping_enabled(layer_state_cmp(state, CHARYBDIS_AUTO_SNIPING_ON_LAYER));
    return state;
}
#    endif // CHARYBDIS_AUTO_SNIPING_ON_LAYER
#endif     // POINTING_DEVICE_ENABLE

#ifdef RGB_MATRIX_ENABLE
/**
 * @brief RGB MatrixのPWMバッファ更新用外部関数宣言
 */
void rgb_matrix_update_pwm_buffers(void);
#endif
