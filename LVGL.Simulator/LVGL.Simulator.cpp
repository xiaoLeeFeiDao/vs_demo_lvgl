/*
 * PROJECT:   LVGL PC Simulator using Visual Studio
 * FILE:      LVGL.Simulator.cpp
 * PURPOSE:   Implementation for LVGL ported to Windows Desktop
 *
 * LICENSE:   The MIT License
 *
 * DEVELOPER: Mouri_Naruto (Mouri_Naruto AT Outlook.com)
 */

#include <Windows.h>

#include "resource.h"

#if _MSC_VER >= 1200
 // Disable compilation warnings.
#pragma warning(push)
// nonstandard extension used : bit field types other than int
#pragma warning(disable:4214)
// 'conversion' conversion from 'type1' to 'type2', possible loss of data
#pragma warning(disable:4244)
#endif

#include "lvgl/lvgl.h"
#include "lvgl/examples/lv_examples.h"
#include "lvgl/demos/lv_demos.h"

#include "win32drv.h"

#if _MSC_VER >= 1200
// Restore compilation warnings.
#pragma warning(pop)
#endif

bool single_display_mode_initialization()
{
    if (!lv_win32_init(
        GetModuleHandleW(NULL),
        SW_SHOW,
        800,
        600,
        LoadIconW(GetModuleHandleW(NULL), MAKEINTRESOURCE(IDI_LVGL))))
    {
        return false;
    }

    lv_win32_add_all_input_devices_to_group(NULL);

    return true;
}

static void lv_example_slider_2(void)
{
    /*Create a transition*/
    static const lv_style_prop_t props[] = { LV_STYLE_BG_COLOR, 0 };
    static lv_style_transition_dsc_t transition_dsc;
    lv_style_transition_dsc_init(&transition_dsc, props, lv_anim_path_linear, 300, 0, NULL);

    static lv_style_t style_main;
    static lv_style_t style_indicator;
    static lv_style_t style_knob;
    static lv_style_t style_pressed_color;
    lv_style_init(&style_main);
    lv_style_set_bg_opa(&style_main, LV_OPA_COVER);
    lv_style_set_bg_color(&style_main, lv_color_hex3(0xbbb));
    lv_style_set_radius(&style_main, LV_RADIUS_CIRCLE);
    lv_style_set_pad_ver(&style_main, -2); /*Makes the indicator larger*/

    lv_style_init(&style_indicator);
    lv_style_set_bg_opa(&style_indicator, LV_OPA_COVER);
    lv_style_set_bg_color(&style_indicator, lv_palette_main(LV_PALETTE_CYAN));
    lv_style_set_radius(&style_indicator, LV_RADIUS_CIRCLE);
    lv_style_set_transition(&style_indicator, &transition_dsc);

    lv_style_init(&style_knob);
    lv_style_set_bg_opa(&style_knob, LV_OPA_COVER);
    lv_style_set_bg_color(&style_knob, lv_palette_main(LV_PALETTE_CYAN));
    lv_style_set_border_color(&style_knob, lv_palette_darken(LV_PALETTE_CYAN, 3));
    lv_style_set_border_width(&style_knob, 2);
    lv_style_set_radius(&style_knob, LV_RADIUS_CIRCLE);
    lv_style_set_pad_all(&style_knob, 6); /*Makes the knob larger*/
    lv_style_set_transition(&style_knob, &transition_dsc);

    lv_style_init(&style_pressed_color);
    lv_style_set_bg_color(&style_pressed_color, lv_palette_darken(LV_PALETTE_CYAN, 2));

    /*Create a slider and add the style*/
    lv_obj_t* slider = lv_slider_create(lv_scr_act());
    lv_obj_remove_style_all(slider);        /*Remove the styles coming from the theme*/

    lv_obj_add_style(slider, &style_main, LV_PART_MAIN);
    lv_obj_add_style(slider, &style_indicator, LV_PART_INDICATOR);
    lv_obj_add_style(slider, &style_pressed_color, LV_PART_INDICATOR | LV_STATE_PRESSED);
    lv_obj_add_style(slider, &style_knob, LV_PART_KNOB);
    lv_obj_add_style(slider, &style_pressed_color, LV_PART_KNOB | LV_STATE_PRESSED);

    lv_obj_center(slider);
}
static void lv_demo_watchface(void)
{
    lv_obj_t* root = lv_obj_create(lv_scr_act());
    lv_obj_set_size(root, 480, 480);
    lv_obj_center(root);

    lv_obj_t* parent = lv_obj_create(root);
    lv_obj_set_size(parent, 260, 260);
}
#if 0
static lv_style_t style_label;
static lv_obj_t* cont;
static lv_obj_t* cont_main;

void test_demo(void)
{
    lv_obj_t* cont = lv_obj_create(lv_scr_act());               // 创建一个对象容器 cont
    lv_obj_set_size(cont, 350, 420);                            // 设置对象容器大小
    lv_obj_align(cont, LV_ALIGN_TOP_MID, 0, 5);                 // 设置对象容器基于屏幕中间对齐
    lv_obj_set_style_pad_all(cont, 25, LV_PART_MAIN);           // 设置对象容器内部 item 与容器边的上下左右间距
    lv_obj_set_style_pad_row(cont, 10, LV_PART_MAIN);           // 设置对象容器内部 item 之间的行间距
    lv_obj_set_style_pad_column(cont, 10, LV_PART_MAIN);        // 设置对象容器内部 item 之间的列间距
    lv_obj_clear_flag(cont, LV_OBJ_FLAG_SCROLLABLE);            // 取消滚动条
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_ROW_WRAP);          // 设置对象容器使用基于行的流失弹性布局flex，设置超出部分换行模式

    /**
     * 设置容器弹性模式
     * 1. 容器指针
     * 2. LV_FLEX_ALIGN_SPACE_EVENLY 均匀分部子元素之间的间距
     * 3. LV_FLEX_ALIGN_END          容器中所有的子元素底部对齐
     * 4. LV_FLEX_ALIGN_CENTER       容器中内容的子元素向容器中心对齐
    */
    lv_obj_set_flex_align(cont, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_END, LV_FLEX_ALIGN_START);

    static lv_style_t btn_style;
    lv_style_init(&btn_style);                           // 初始化样式
    lv_style_set_radius(&btn_style, 5);                  // 设置样式圆角为0

    lv_obj_t* btn1 = lv_btn_create(cont);                // 基于cont对象添加button对象
    lv_obj_set_size(btn1, 100, 80);                       // 设置对象大小

    lv_obj_t* btn1_label = lv_label_create(btn1);        // 创建按键对象创建label对象
    lv_label_set_text(btn1_label, "Btn1");               // 设置label文本
    lv_obj_center(btn1_label);                           // label对象居中显示
    lv_obj_add_style(btn1, &btn_style, LV_PART_MAIN);    // 配置样式

    lv_obj_t* btn2 = lv_btn_create(cont);                // 基于cont对象添加button对象
    lv_obj_set_size(btn2, 80, 110);                      // 设置对象大小

    lv_obj_t* btn2_label = lv_label_create(btn2);        // 创建按键对象创建label对象
    lv_label_set_text(btn2_label, "Btn2");               // 设置label文本
    lv_obj_center(btn2_label);                           // label对象居中显示
    lv_obj_add_style(btn2, &btn_style, LV_PART_MAIN);    // 配置样式

    lv_obj_t* btn3 = lv_btn_create(cont);                // 基于cont对象添加button对象
    lv_obj_set_size(btn3, 45, 30);                       // 设置对象大小

    lv_obj_t* btn3_label = lv_label_create(btn3);        // 创建按键对象创建label对象
    lv_label_set_text(btn3_label, "Btn3");               // 设置label文本
    lv_obj_center(btn3_label);                           // label对象居中显示
    lv_obj_add_style(btn3, &btn_style, LV_PART_MAIN);    // 配置样式

    lv_obj_t* btn4 = lv_btn_create(cont);                // 基于cont对象添加button对象
    lv_obj_set_size(btn4, 60, 35);                       // 设置对象大小

    lv_obj_t* btn4_label = lv_label_create(btn4);        // 创建按键对象创建label对象
    lv_label_set_text(btn4_label, "Btn4");               // 设置label文本
    lv_obj_center(btn4_label);                           // label对象居中显示
    lv_obj_add_style(btn4, &btn_style, LV_PART_MAIN);    // 配置样式

    lv_obj_t* btn5 = lv_btn_create(cont);                // 基于cont对象添加button对象
    lv_obj_set_size(btn5, 70, 50);                       // 设置对象大小

    lv_obj_t* btn5_label = lv_label_create(btn5);        // 创建按键对象创建label对象
    lv_label_set_text(btn5_label, "Btn5");               // 设置label文本
    lv_obj_center(btn5_label);                           // label对象居中显示
    lv_obj_add_style(btn5, &btn_style, LV_PART_MAIN | LV_STATE_PRESSED);    // 配置样式
}
#endif

static void lv_example_flex(void)
{
    /*Create a container with ROW flex direction*/
    lv_obj_t* cont_row = lv_obj_create(lv_scr_act());
    lv_obj_set_size(cont_row, 300, 75);
    lv_obj_align(cont_row, LV_ALIGN_TOP_MID, 0, 5);
    lv_obj_set_flex_flow(cont_row, LV_FLEX_FLOW_ROW);

    /*Create a container with COLUMN flex direction*/
    lv_obj_t* cont_col = lv_obj_create(lv_scr_act());
    lv_obj_set_size(cont_col, 200, 150);
    lv_obj_align_to(cont_col, cont_row, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);
    lv_obj_set_flex_flow(cont_col, LV_FLEX_FLOW_COLUMN);

    uint32_t i;
    for (i = 0; i < 10; i++) {
        lv_obj_t* obj;
        lv_obj_t* label;

        /*Add items to the row*/
        obj = lv_btn_create(cont_row);
        lv_obj_set_size(obj, 100, LV_PCT(100));

        label = lv_label_create(obj);
        lv_label_set_text_fmt(label, "Item: %d", i);
        lv_obj_center(label);

        /*Add items to the column*/
        obj = lv_btn_create(cont_col);
        lv_obj_set_size(obj, LV_PCT(100), LV_SIZE_CONTENT);

        label = lv_label_create(obj);
        lv_label_set_text_fmt(label, "Item: %d", i);
        lv_obj_center(label);
    }
}

static lv_style_t style_label;
static lv_obj_t* cont;
void test_demo(void)
{
    lv_obj_t* root = lv_obj_create(lv_scr_act());
    lv_obj_set_size(root, 466, 466);                            // 设置对象容器大小
    lv_obj_align(root, LV_ALIGN_CENTER, 0, 0);
    lv_obj_clear_flag(root, LV_OBJ_FLAG_SCROLLABLE);            // 取消滚动条
    static lv_style_t root_style;
    lv_style_init(&root_style);
    lv_style_set_radius(&root_style, 233);
    lv_obj_add_style(root, &root_style, LV_PART_MAIN);
    lv_style_set_bg_color(&root_style, lv_color_hex(0xFFDAB9));

#define PREVIEW_NUM 4
#define CUR_INDEX 0
#define CONTAINER_WIDTH (206 + 32 * (PREVIEW_NUM -1) + 266 * PREVIEW_NUM)
    cont = lv_obj_create(root);                       // 创建一个对象容器 cont
    //lv_obj_set_size(cont, 1064, 260);                         // 设置对象容器大小
    //lv_obj_set_size(cont, CONTAINER_WIDTH, 466);                         // 设置对象容器大小
    lv_obj_set_size(cont, 466, 466);                         // 设置对象容器大小
    lv_obj_align_to(cont, root, LV_ALIGN_CENTER, 0, 0);                  // 设置对象容器基于屏幕中间对齐
    lv_obj_set_style_pad_all(cont, 103, LV_PART_MAIN);           // 设置对象容器内部 item 与容器边的上下左右间距
    lv_obj_set_style_pad_row(cont, 0, LV_PART_MAIN);           // 设置对象容器内部 item 之间的行间距
    lv_obj_set_style_pad_column(cont, 32, LV_PART_MAIN);        // 设置对象容器内部 item 之间的列间距
    //lv_obj_clear_flag(cont, LV_OBJ_FLAG_SCROLLABLE);            // 取消滚动条
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_ROW);          // 设置对象容器使用基于行的流失弹性布局flex，设置超出部分换行模式

    /**
     * 设置容器弹性模式
     * 1. 容器指针
     * 2. LV_FLEX_ALIGN_SPACE_EVENLY 均匀分部子元素之间的间距
     * 3. LV_FLEX_ALIGN_END          容器中所有的子元素底部对齐
     * 4. LV_FLEX_ALIGN_CENTER       容器中内容的子元素向容器中心对齐
    */
    //lv_obj_set_flex_align(cont, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_END, LV_FLEX_ALIGN_START);

    static lv_style_t btn_style;
    lv_style_init(&btn_style);                           // 初始化样式
    lv_style_set_radius(&btn_style, 130);                  // 设置样式圆角为0
    lv_style_set_bg_color(&btn_style, lv_color_hex(0x7FFFD4));
    for (int i = 0; i < PREVIEW_NUM; i++) {
        lv_obj_t* img = lv_btn_create(cont);
        lv_obj_set_size(img, 260, 260);
        lv_obj_t* img_label = lv_label_create(img);
        lv_label_set_text_fmt(img_label, "Btn%d", i);
        lv_obj_center(img_label);
        lv_obj_add_style(img, &btn_style, LV_PART_MAIN);
    }
    //lv_obj_align_to();
    lv_obj_t* cur_img = lv_obj_get_child(cont, 3);
    lv_obj_align_to(cur_img, cont, LV_ALIGN_CENTER, 0, 0);
#if 0
    lv_obj_t* btn1 = lv_btn_create(cont);                // 基于cont对象添加button对象
    lv_obj_set_size(btn1, 260, 260);                       // 设置对象大小
    lv_obj_t* btn1_label = lv_label_create(btn1);        // 创建按键对象创建label对象
    lv_label_set_text(btn1_label, "Btn1");               // 设置label文本
    lv_obj_center(btn1_label);                           // label对象居中显示
    lv_obj_add_style(btn1, &btn_style, LV_PART_MAIN);    // 配置样式

    lv_obj_t* btn2 = lv_btn_create(cont);                // 基于cont对象添加button对象
    lv_obj_set_size(btn2, 260, 260);                      // 设置对象大小
    lv_obj_t* btn2_label = lv_label_create(btn2);        // 创建按键对象创建label对象
    lv_label_set_text(btn2_label, "Btn2");               // 设置label文本
    lv_obj_center(btn2_label);                           // label对象居中显示
    lv_obj_add_style(btn2, &btn_style, LV_PART_MAIN);    // 配置样式

    lv_obj_t* btn3 = lv_btn_create(cont);                // 基于cont对象添加button对象
    lv_obj_set_size(btn3, 260, 260);                       // 设置对象大小
    lv_obj_t* btn3_label = lv_label_create(btn3);        // 创建按键对象创建label对象
    lv_label_set_text(btn3_label, "Btn3");               // 设置label文本
    lv_obj_center(btn3_label);                           // label对象居中显示
    lv_obj_add_style(btn3, &btn_style, LV_PART_MAIN);    // 配置样式

    lv_obj_t* btn4 = lv_btn_create(cont);                // 基于cont对象添加button对象
    lv_obj_set_size(btn4, 260, 260);                       // 设置对象大小
    lv_obj_t* btn4_label = lv_label_create(btn4);        // 创建按键对象创建label对象
    lv_label_set_text(btn4_label, "Btn4");               // 设置label文本
    lv_obj_center(btn4_label);                           // label对象居中显示
    lv_obj_add_style(btn4, &btn_style, LV_PART_MAIN);    // 配置样式

    lv_obj_t* btn5 = lv_btn_create(cont);                // 基于cont对象添加button对象
    lv_obj_set_size(btn5, 260, 260);                       // 设置对象大小
    lv_obj_t* btn5_label = lv_label_create(btn5);        // 创建按键对象创建label对象
    lv_label_set_text(btn5_label, "Btn5");               // 设置label文本
    lv_obj_center(btn5_label);                           // label对象居中显示
    lv_obj_add_style(btn5, &btn_style, LV_PART_MAIN | LV_STATE_PRESSED);    // 配置样式
#endif
}

static void lv_example_flex_2(void)
{
    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_flex_flow(&style, LV_FLEX_FLOW_ROW_WRAP);
    lv_style_set_flex_main_place(&style, LV_FLEX_ALIGN_SPACE_EVENLY);
    lv_style_set_layout(&style, LV_LAYOUT_FLEX);

    lv_obj_t* cont = lv_obj_create(lv_scr_act());
    lv_obj_set_size(cont, 300, 220);
    lv_obj_center(cont);
    lv_obj_add_style(cont, &style, 0);

    uint32_t i;
    for (i = 0; i < 8; i++) {
        lv_obj_t* obj = lv_obj_create(cont);
        lv_obj_set_size(obj, 70, LV_SIZE_CONTENT);
        lv_obj_add_flag(obj, LV_OBJ_FLAG_CHECKABLE);

        lv_obj_t* label = lv_label_create(obj);
        lv_label_set_text_fmt(label, "%d", i);
        lv_obj_center(label);
    }
}

static void lv_example_flex_3(void)
{
    lv_obj_t* cont = lv_obj_create(lv_scr_act());
    lv_obj_set_size(cont, 300, 220);
    lv_obj_center(cont);
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_ROW);

    lv_obj_t* obj;
    obj = lv_obj_create(cont);
    lv_obj_set_size(obj, 40, 40);           /*Fix size*/

    obj = lv_obj_create(cont);
    lv_obj_set_height(obj, 40);
    lv_obj_set_flex_grow(obj, 1);           /*1 portion from the free space*/

    obj = lv_obj_create(cont);
    lv_obj_set_height(obj, 40);
    lv_obj_set_flex_grow(obj, 2);           /*2 portion from the free space*/

    obj = lv_obj_create(cont);
    lv_obj_set_size(obj, 40, 40);           /*Fix size. It is flushed to the right by the "grow" items*/
}

#if LV_USE_FLEX
static void row_gap_anim(void* obj, int32_t v)
{
    lv_obj_set_style_pad_row((lv_obj_t*)obj, v, 0);
}

static void column_gap_anim(void* obj, int32_t v)
{
    lv_obj_set_style_pad_column((lv_obj_t*)obj, v, 0);
}

/**
 * Demonstrate the effect of column and row gap style properties
 */
void lv_example_flex5(void)
{
    lv_obj_t* cont = lv_obj_create(lv_scr_act());
    lv_obj_set_size(cont, 300, 220);
    lv_obj_center(cont);
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_ROW_WRAP);

    uint32_t i;
    for (i = 0; i < 9; i++) {
        lv_obj_t* obj = lv_obj_create(cont);
        lv_obj_set_size(obj, 70, LV_SIZE_CONTENT);

        lv_obj_t* label = lv_label_create(obj);
        lv_label_set_text_fmt(label, "%d", i);
        lv_obj_center(label);
    }

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, cont);
    lv_anim_set_values(&a, 0, 10);
    lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);

    lv_anim_set_exec_cb(&a, row_gap_anim);
    lv_anim_set_time(&a, 500);
    lv_anim_set_playback_time(&a, 500);
    lv_anim_start(&a);

    lv_anim_set_exec_cb(&a, column_gap_anim);
    lv_anim_set_time(&a, 3000);
    lv_anim_set_playback_time(&a, 3000);
    lv_anim_start(&a);
}

#endif
static void lv_example_flex_4(void)
{

    lv_obj_t* cont = lv_obj_create(lv_scr_act());
    lv_obj_set_size(cont, 300, 220);
    lv_obj_center(cont);
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_COLUMN_REVERSE);

    uint32_t i;
    for (i = 0; i < 6; i++) {
        lv_obj_t* obj = lv_obj_create(cont);
        lv_obj_set_size(obj, 100, 50);

        lv_obj_t* label = lv_label_create(obj);
        lv_label_set_text_fmt(label, "Item: %d", i);
        lv_obj_center(label);
    }
}

static void lv_example_flex6(void)
{
    lv_obj_t* cont = lv_obj_create(lv_scr_act());
    lv_obj_set_style_base_dir(cont, LV_BASE_DIR_RTL, 0);
    lv_obj_set_size(cont, 300, 220);
    lv_obj_center(cont);
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_ROW_WRAP);

    uint32_t i;
    for (i = 0; i < 20; i++) {
        lv_obj_t* obj = lv_obj_create(cont);
        lv_obj_set_size(obj, 70, LV_SIZE_CONTENT);

        lv_obj_t* label = lv_label_create(obj);
        lv_label_set_text_fmt(label, "%d", i);
        lv_obj_center(label);
    }
}
/**
 * A simple grid
 */
static void lv_example_grid1(void)
{
    static lv_coord_t col_dsc[] = { 70, 70, 70, LV_GRID_TEMPLATE_LAST };
    static lv_coord_t row_dsc[] = { 50, 50, 50, LV_GRID_TEMPLATE_LAST };

    /*Create a container with grid*/
    lv_obj_t* cont = lv_obj_create(lv_scr_act());
    lv_obj_set_style_grid_column_dsc_array(cont, col_dsc, 0);
    lv_obj_set_style_grid_row_dsc_array(cont, row_dsc, 0);
    lv_obj_set_size(cont, 300, 220);
    lv_obj_center(cont);
    lv_obj_set_layout(cont, LV_LAYOUT_GRID);

    lv_obj_t* label;
    lv_obj_t* obj;

    uint32_t i;
    for (i = 0; i < 9; i++) {
        uint8_t col = i % 3;
        uint8_t row = i / 3;

        obj = lv_btn_create(cont);
        /*Stretch the cell horizontally and vertically too
         *Set span to 1 to make the cell 1 column/row sized*/
        lv_obj_set_grid_cell(obj, LV_GRID_ALIGN_STRETCH, col, 1,
            LV_GRID_ALIGN_STRETCH, row, 1);

        label = lv_label_create(obj);
        lv_label_set_text_fmt(label, "c%d, r%d", col, row);
        lv_obj_center(label);
    }
}

void test_lvgl_my_demo()
{
    //lv_example_flex();
    //lv_example_flex6();
    lv_example_grid1();
    //lv_demo_watchface();
    //test_demo();
#if 0
    lv_obj_t* root = lv_obj_create(lv_scr_act());
    //lv_obj_set_width(root, 300);
    //lv_obj_set_height(root, 500);
    lv_obj_set_size(root, 466, 466);
    //lv_obj_set_size(root, 300, 150);
    //LV_LOG_INFO("w = %d\n", lv_obj_get_width(root));
    //LV_LOG_INFO("h = %d\n", lv_obj_get_height(root));

    //lv_obj_set_x(root, 200);
    //lv_obj_set_y(root, 50);
    //lv_obj_set_pos(root, 200, 200);
    //lv_obj_set_align(root, LV_ALIGN_CENTER);
    lv_obj_align(root, LV_ALIGN_CENTER, 0, 0);
    //lv_obj_center(root);
    //lv_obj_set_style_border_width(root, 10, 0);
    //lv_obj_set_style_border_color(root, lv_color_hex(0x8470FF), 0);
    //lv_obj_set_style_outline_width(root, 5, 0);
    //lv_obj_set_style_outline_color(root, lv_color_hex(0xE48838), 0);
#endif

#define STYLE_T 0
#if STYLE_T
    lv_style_t style_obj2obj;
    lv_style_init(&style_obj2obj);
    lv_style_set_bg_color(&style_obj2obj, lv_color_hex(0xFF00FF));
    lv_obj_add_style(root, &style_obj2obj, LV_STATE_PRESSED | LV_PART_MAIN);
#endif

#define IMG_T 0
#if IMG_T
    lv_obj_t* img = lv_img_create(root);

#endif

#define TILEVIEW_T 0
#if TILEVIEW_T
    lv_obj_t* tileview = lv_tileview_create(root);

    lv_obj_t* tile1 = lv_tileview_add_tile(root, 0, 0, LV_DIR_LEFT | LV_DIR_RIGHT);
    lv_obj_t* tile1_obj = lv_obj_create(tile1);
    lv_obj_set_size(tile1_obj, 260, 260);
    static lv_style_t style_tile1_obj;
    lv_style_init(&style_tile1_obj);
    lv_style_set_bg_color(&style_tile1_obj, lv_color_hex(0xFF00FF));
    lv_obj_add_style(tile1_obj, &style_tile1_obj, 0);
    lv_obj_center(tile1_obj);

    lv_obj_t* tile2 = lv_tileview_add_tile(root, 1, 0, LV_DIR_LEFT | LV_DIR_RIGHT);
    lv_obj_t* tile2_obj = lv_obj_create(tile2);
    lv_obj_set_size(tile2_obj, 260, 260);
    static lv_style_t style_tile2_obj;
    lv_style_init(&style_tile2_obj);
    lv_style_set_bg_color(&style_tile2_obj, lv_color_hex(0xFF00FF));
    lv_obj_add_style(tile2_obj, &style_tile2_obj, 0);
    lv_obj_center(tile2_obj);

    lv_obj_update_layout(tileview);
    lv_obj_set_tile(tileview, tile2, LV_ANIM_OFF);
    //lv_obj_set_tile_id(tileview, 0, 0, LV_ANIM_OFF);
#endif
#define LABLE_T 0
#if LABLE_T
    lv_obj_t* obj = lv_obj_create(root);
    lv_obj_center(obj);
    lv_obj_t* label = lv_label_create(obj);
    lv_label_set_text(label, "hello lvgl!");
    lv_obj_align_to(label, obj, LV_ALIGN_TOP_MID, 0, 0);

    //static lv_style_t style_obj;
    //lv_style_init(&style_obj);
    //lv_style_set_bg_color(&style_obj, lv_color_hex(0x0FF000));
    //lv_style_set_bg_opa(&style_obj, 30);
    //lv_obj_add_style(obj, &style_obj, LV_STATE_PRESSED);
#endif

#define SLIDER_T 0
#if SLIDER_T
    // 滑杆
    lv_example_slider_2();
#endif
}

static void test_lvgl_origin_demo()
{
    lv_demo_widgets();
    lv_demo_benchmark(LV_DEMO_BENCHMARK_MODE_RENDER_AND_DRIVER);
}

int main()
{
    lv_init();

    if (!single_display_mode_initialization())
    {
        return -1;
    }

    //test_lvgl_origin_demo();
    test_lvgl_my_demo();
    while (!lv_win32_quit_signal)
    {
        lv_task_handler();
        Sleep(1);
    }

    return 0;
}
