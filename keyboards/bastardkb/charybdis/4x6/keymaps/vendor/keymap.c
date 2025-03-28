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
#include QMK_KEYBOARD_H

#ifdef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE
#    include "timer.h"
#endif // CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE

// ================================================
// レイヤー定義
// ================================================
enum charybdis_keymap_layers {
    LAYER_BASE = 0,   // 基本レイヤー
    LAYER_NUM,        // 数字/記号レイヤー
    LAYER_FUNC,       // ファンクションレイヤー
    LAYER_POINTER,    // マウス/ポインター操作レイヤー
};

// 自動スナイピング機能でポインターレイヤーを有効にするための定義
#define CHARYBDIS_AUTO_SNIPING_ON_LAYER LAYER_POINTER

#ifdef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE
static uint16_t auto_pointer_layer_timer = 0;

#    ifndef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS
#        define CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS 1000  // タイムアウト時間（ミリ秒）
#    endif // CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS

#    ifndef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD
#        define CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD 8     // マウス移動の閾値（ピクセル数など）
#    endif // CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD
#endif // CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE

// ================================================
// キー定義（レイヤーホールド、タップダンス、マウスクリックなど）
// ================================================

// レイヤー切り替えホールドキーの定義
#define BASE_LBRC LT(LAYER_BASE, KC_LBRC)
#define BASE_RBRC LT(LAYER_BASE, KC_RBRC)
#define BASE_DEL  LT(LAYER_BASE, KC_DEL)
#define NUM_BRC   LT(LAYER_NUM, KC_LBRC)
#define FUNC_BRC  LT(LAYER_FUNC, KC_RBRC)
#define PT_DEL    LT(LAYER_POINTER, KC_DEL)
#define PT_SLSH   LT(LAYER_POINTER, KC_SLSH)
#define PT_Z      LT(LAYER_POINTER, KC_Z)

// タップダンスキーの定義
#define ALT_GRV   ALT_T(KC_GRV)
#define ESC_CTL   CTL_T(KC_ESC)
#define GUI_ENT   CMD_T(KC_ENT)
#define GUI_ESC   CMD_T(KC_ESC)
#define GUI_TAB   CMD_T(KC_TAB)
#define SFT_BSPC  SFT_T(KC_BSPC)
#define SFT_SPC   SFT_T(KC_SPC)

// マウスクリックキーの定義
#define L_CLICK   KC_BTN1
#define R_CLICK   KC_BTN2
#define M_CLICK   MT(DRGSCRL, KC_BTN3)

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
#    define HSV_PURPLE 170, 255, 255  // 紫色: 色相170, 彩度255, 明度255（必要に応じて調整）
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
                                      GUI_TAB, SFT_SPC,  PT_DEL,   GUI_ENT, SFT_BSPC,
                                               ESC_CTL,  NUM_BRC,  FUNC_BRC
    //                           ╰────────────────────────────────────────╯  ╰──────────────────────────────╯
  ),

  [LAYER_NUM] = LAYOUT(
    // ╭──────────────────────────────────────────────────────╮  ╭──────────────────────────────────────────────────────╮
         KC_TILD, KC_EXLM,   KC_AT,   KC_HASH, KC_DLR,  KC_PERC,     KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_UNDS,
    // ├──────────────────────────────────────────────────────┤  ├──────────────────────────────────────────────────────┤
         RGB_MOD, XXXXXXX,  XXXXXXX,  XXXXXXX, XXXXXXX, XXXXXXX,   KC_PPLS, KC_P7,   KC_P8,   KC_P9,  KC_PEQL,   XXXXXXX,
    // ├──────────────────────────────────────────────────────┤  ├──────────────────────────────────────────────────────┤
         RGB_TOG, KC_LGUI,  KC_LALT, KC_LCTL, KC_LSFT, XXXXXXX,   KC_PMNS, KC_P4,   KC_P5,   KC_P6,  KC_PSLS, KC_PDOT,
    // ├──────────────────────────────────────────────────────┤  ├──────────────────────────────────────────────────────┤
        RGB_RMOD, XXXXXXX,  XXXXXXX,  XXXXXXX, XXXXXXX, XXXXXXX,   KC_P0, KC_P1,   KC_P2,   KC_P3,  KC_BSLS, KC_PAST,
    // ╰──────────────────────────────────────────────────────╯  ╰──────────────────────────────────────────────────────╯
                                    GUI_TAB, SFT_SPC,  PT_DEL,   GUI_ENT, SFT_BSPC,
                                             ESC_CTL, BASE_LBRC, FUNC_BRC
    //                           ╰────────────────────────────────────────╯  ╰──────────────────────────────╯
  ),

  [LAYER_FUNC] = LAYOUT(
    // ╭──────────────────────────────────────────────────────╮  ╭──────────────────────────────────────────────────────╮
         KC_F12,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,     KC_F6,   KC_F7,   KC_F8,   KC_F9,  KC_F10,  KC_F11,
    // ├──────────────────────────────────────────────────────┤  ├──────────────────────────────────────────────────────┤
         KC_MNXT, XXXXXXX,  KC_UP,   XXXXXXX, XXXXXXX, XXXXXXX,   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_VOLU,
    // ├──────────────────────────────────────────────────────┤  ├──────────────────────────────────────────────────────┤
         KC_MPLY, KC_LEFT,  KC_DOWN, KC_RIGHT,KC_SPC,  XXXXXXX,   KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, XXXXXXX, KC_MUTE,
    // ├──────────────────────────────────────────────────────┤  ├──────────────────────────────────────────────────────┤
         KC_MPRV, KC_HOME,  KC_PGUP, KC_PGDN, KC_END,  XXXXXXX,   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_VOLD,
    // ╰──────────────────────────────────────────────────────╯  ╰──────────────────────────────────────────────────────╯
                                    GUI_TAB, SFT_SPC,  PT_DEL,   GUI_ENT, SFT_BSPC,
                                             ESC_CTL, NUM_BRC,  BASE_RBRC
    //                           ╰────────────────────────────────────────╯  ╰──────────────────────────────╯
  ),

  [LAYER_POINTER] = LAYOUT(
    // ╭──────────────────────────────────────────────────────╮  ╭──────────────────────────────────────────────────────╮
         QK_BOOT,  EE_CLR,   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, QK_BOOT,  EE_CLR,
    // ├──────────────────────────────────────────────────────┤  ├──────────────────────────────────────────────────────┤
         XXXXXXX, XXXXXXX,  XXXXXXX, XXXXXXX, DPI_MOD, S_D_MOD,  S_D_MOD, DPI_MOD, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
    // ├──────────────────────────────────────────────────────┤  ├─────────────────────────────────────────────────────┤
         XXXXXXX, XXXXXXX,  L_CLICK, M_CLICK, R_CLICK, XXXXXXX,  XXXXXXX, L_CLICK, M_CLICK, R_CLICK, XXXXXXX, XXXXXXX,
    // ├──────────────────────────────────────────────────────┤  ├──────────────────────────────────────────────────────┤
         XXXXXXX, _______,  DRGSCRL, SNIPING,  XXXXXXX, XXXXXXX,  XXXXXXX, XXXXXXX, SNIPING, DRGSCRL, _______, XXXXXXX,
    // ╰──────────────────────────────────────────────────────╯  ╰─────────────────────────────────────────────────────╯
                                    GUI_TAB, SFT_SPC,  BASE_DEL, GUI_ENT, SFT_BSPC,
                                             ESC_CTL, NUM_BRC,  FUNC_BRC
    //                           ╰────────────────────────────────────────╯  ╰──────────────────────────────╯
  ),
};
// clang-format on

// ================================================
// RGB更新関数の共通化（各レイヤーに応じたRGB LED設定）
// ================================================
#ifdef RGB_MATRIX_ENABLE
/**
 * @brief 指定されたレイヤーに応じたRGB LEDの設定を行う
 *
 * @param layer 現在アクティブなレイヤー
 */
void update_rgb_for_layer(uint8_t layer) {
    // 固定色モードに設定
    rgb_matrix_mode_noeeprom(RGB_MATRIX_NONE);
    switch (layer) {
        case LAYER_POINTER:
            // ポインターレイヤー：紫色
            rgb_matrix_sethsv_noeeprom(HSV_PURPLE);
            break;
        case LAYER_NUM:
            // 数字/記号レイヤー：青色
            rgb_matrix_sethsv_noeeprom(HSV_BLUE);
            break;
        case LAYER_FUNC:
            // ファンクションレイヤー：赤色
            rgb_matrix_sethsv_noeeprom(HSV_RED);
            break;
        default:
            // 基本レイヤー：イルミネーション（虹色エフェクト例）
            rgb_matrix_mode_noeeprom(RGB_MATRIX_RAINBOW_MOOD);
            break;
    }
}
#endif // RGB_MATRIX_ENABLE

// ================================================
// ポインティングデバイス及びレイヤー変更処理
// ================================================
#ifdef POINTING_DEVICE_ENABLE

    #ifdef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE
/**
 * @brief マウス移動量に応じてポインターレイヤーを自動で有効化する
 *
 * マウスの移動が設定した閾値を超えると、ポインターレイヤーをONにし、
 * その後のRGB LED更新は layer_state_set_user() で実施します。
 */
report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
    if (abs(mouse_report.x) > CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD ||
        abs(mouse_report.y) > CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD) {
        if (auto_pointer_layer_timer == 0) {
            layer_on(LAYER_POINTER);
#ifdef RGB_MATRIX_ENABLE
            // 初回ON時にRGB更新処理を呼び出し
            update_rgb_for_layer(LAYER_POINTER);
#endif // RGB_MATRIX_ENABLE
        }
        auto_pointer_layer_timer = timer_read();
    }
    return mouse_report;
}

/**
 * @brief 定期スキャン処理でポインターレイヤーのタイムアウトを監視する
 *
 * 一定時間経過後、ポインターレイヤーを自動でOFFにし、RGB LEDをデフォルト状態に戻します。
 */
void matrix_scan_user(void) {
    if (auto_pointer_layer_timer != 0 &&
        TIMER_DIFF_16(timer_read(), auto_pointer_layer_timer) >= CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS) {
        auto_pointer_layer_timer = 0;
        layer_off(LAYER_POINTER);
#ifdef RGB_MATRIX_ENABLE
        // タイムアウトでRGB LEDをデフォルトモードへ戻す
        rgb_matrix_mode_noeeprom(RGB_MATRIX_DEFAULT_MODE);
#endif // RGB_MATRIX_ENABLE
    }
}
    #endif // CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE

    #ifdef CHARYBDIS_AUTO_SNIPING_ON_LAYER
/**
 * @brief レイヤー変更時に呼ばれるユーザー定義関数
 *
 * ポインターレイヤーのスナイピング機能を有効にし、
 * 現在のアクティブレイヤーに応じたRGB LED更新を行います。
 */
layer_state_t layer_state_set_user(layer_state_t state) {
    // 現在アクティブなレイヤーを取得（biton()は最上位レイヤーを返します）
    uint8_t active_layer = biton(state);

    // ポインターレイヤーのスナイピング機能を有効化
    charybdis_set_pointer_sniping_enabled(active_layer == CHARYBDIS_AUTO_SNIPING_ON_LAYER);

#ifdef RGB_MATRIX_ENABLE
    // 前回のレイヤーと比較して変化があった場合のみRGB更新を実行
    static uint8_t prev_active_layer = 0;
    if (active_layer != prev_active_layer) {
        update_rgb_for_layer(active_layer);
        prev_active_layer = active_layer;
#ifdef DEBUG_PRINT
        uprintf("Layer changed: %u\n", active_layer);
#endif
    }
#endif

    return state;
}
    #endif // CHARYBDIS_AUTO_SNIPING_ON_LAYER
#endif // POINTING_DEVICE_ENABLE

#ifdef RGB_MATRIX_ENABLE
/**
 * @brief RGB MatrixのPWMバッファ更新用外部関数宣言
 */
void rgb_matrix_update_pwm_buffers(void);
#endif
