#include "window.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <windows.h>
#include <windowsx.h>

struct EditorWindow {
    char title[64];
    char overlay_text[16384];
    bool loading_active;
    char loading_title[96];
    char loading_subtitle[160];
    int loading_progress_percent;
    WindowUIAction actions[WINDOW_UI_MAX_ACTIONS];
    int action_count;
    int hover_action;
    char pending_command[128];
    int pressed_action;
    HWND hwnd;
    HDC back_dc;
    HBITMAP back_bmp;
    HBITMAP back_old_bmp;
    int back_w;
    int back_h;
    int width, height;
    bool open;
    bool tracking_mouse_leave;
};

static const char *EDITOR_WIN_CLASS = "IDEEditorWindowClass";
static int g_class_registered = 0;

static void fill_rect_color(HDC dc, const RECT *r, COLORREF color) {
    HBRUSH b = CreateSolidBrush(color);
    FillRect(dc, r, b);
    DeleteObject(b);
}

static void frame_rect_color(HDC dc, const RECT *r, COLORREF color) {
    HBRUSH b = CreateSolidBrush(color);
    FrameRect(dc, r, b);
    DeleteObject(b);
}

static void fill_rect_xy(HDC dc, int x, int y, int w, int h, COLORREF color) {
    RECT r;
    r.left = x;
    r.top = y;
    r.right = x + w;
    r.bottom = y + h;
    fill_rect_color(dc, &r, color);
}

static void frame_rect_xy(HDC dc, int x, int y, int w, int h, COLORREF color) {
    RECT r;
    r.left = x;
    r.top = y;
    r.right = x + w;
    r.bottom = y + h;
    frame_rect_color(dc, &r, color);
}

static void draw_gb_studio_preview(HDC dc, const RECT *viewport, const char *overlay_text) {
    int pad = 10;
    int frame_x;
    int frame_y;
    int frame_w;
    int frame_h;
    int screen_x;
    int screen_y;
    int screen_w;
    int screen_h;
    int tile_w;
    int tile_h;
    int gx;
    int gy;
    RECT text_rect;
    HFONT font;
    HFONT old_font;

    if (!dc || !viewport) return;

    frame_x = viewport->left + pad;
    frame_y = viewport->top + pad;
    frame_w = (viewport->right - viewport->left) - (pad * 2);
    frame_h = (viewport->bottom - viewport->top) - (pad * 2);
    if (frame_w < 320 || frame_h < 220) return;

    fill_rect_xy(dc, frame_x, frame_y, frame_w, frame_h, RGB(10, 16, 24));
    frame_rect_xy(dc, frame_x, frame_y, frame_w, frame_h, RGB(88, 126, 190));

    screen_x = frame_x + 26;
    screen_y = frame_y + 36;
    screen_w = frame_w - 250;
    screen_h = frame_h - 92;
    if (screen_w < 320) screen_w = 320;
    if (screen_h < 200) screen_h = 200;

    fill_rect_xy(dc, screen_x, screen_y, screen_w, screen_h, RGB(170, 198, 150));
    frame_rect_xy(dc, screen_x - 2, screen_y - 2, screen_w + 4, screen_h + 4, RGB(54, 88, 54));

    tile_w = screen_w / 20;
    tile_h = screen_h / 18;
    for (gy = 0; gy < 18; gy++) {
        for (gx = 0; gx < 20; gx++) {
            COLORREF c = ((gx + gy) % 2 == 0) ? RGB(158, 188, 142) : RGB(146, 178, 130);
            fill_rect_xy(dc, screen_x + gx * tile_w, screen_y + gy * tile_h, tile_w, tile_h, c);
        }
    }

    for (gx = 0; gx <= 20; gx++) {
        int x = screen_x + gx * tile_w;
        fill_rect_xy(dc, x, screen_y, 1, tile_h * 18, RGB(118, 152, 110));
    }
    for (gy = 0; gy <= 18; gy++) {
        int y = screen_y + gy * tile_h;
        fill_rect_xy(dc, screen_x, y, tile_w * 20, 1, RGB(118, 152, 110));
    }

    fill_rect_xy(dc, screen_x + tile_w * 9, screen_y + tile_h * 8, tile_w, tile_h, RGB(52, 74, 188));
    frame_rect_xy(dc, screen_x + tile_w * 9, screen_y + tile_h * 8, tile_w, tile_h, RGB(230, 238, 255));

    fill_rect_xy(dc, screen_x + tile_w * 6, screen_y + tile_h * 6, tile_w, tile_h, RGB(178, 86, 66));
    frame_rect_xy(dc, screen_x + tile_w * 6, screen_y + tile_h * 6, tile_w, tile_h, RGB(255, 220, 186));
    fill_rect_xy(dc, screen_x + tile_w * 14, screen_y + tile_h * 10, tile_w, tile_h, RGB(178, 86, 66));
    frame_rect_xy(dc, screen_x + tile_w * 14, screen_y + tile_h * 10, tile_w, tile_h, RGB(255, 220, 186));

    frame_rect_xy(dc, screen_x + tile_w * 3, screen_y + tile_h * 3, tile_w * 4, tile_h * 2, RGB(255, 176, 86));
    frame_rect_xy(dc, screen_x + tile_w * 12, screen_y + tile_h * 12, tile_w * 5, tile_h * 3, RGB(255, 176, 86));

    fill_rect_xy(dc, screen_x, screen_y + screen_h - 18, screen_w, 18, RGB(18, 34, 28));
    fill_rect_xy(dc, screen_x + 8, screen_y + screen_h - 14, 68, 10, RGB(198, 64, 72));
    fill_rect_xy(dc, screen_x + 82, screen_y + screen_h - 14, 34, 10, RGB(76, 110, 204));
    fill_rect_xy(dc, screen_x + 122, screen_y + screen_h - 14, 44, 10, RGB(212, 188, 70));

    fill_rect_xy(dc, frame_x + frame_w - 204, frame_y + 44, 172, 30, RGB(16, 26, 42));
    frame_rect_xy(dc, frame_x + frame_w - 204, frame_y + 44, 172, 30, RGB(98, 148, 224));
    fill_rect_xy(dc, frame_x + frame_w - 204, frame_y + 82, 172, 92, RGB(16, 26, 42));
    frame_rect_xy(dc, frame_x + frame_w - 204, frame_y + 82, 172, 92, RGB(98, 148, 224));
    fill_rect_xy(dc, frame_x + frame_w - 204, frame_y + 182, 172, 74, RGB(16, 26, 42));
    frame_rect_xy(dc, frame_x + frame_w - 204, frame_y + 182, 172, 74, RGB(98, 148, 224));

    font = CreateFontA(
        15, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
        FF_DONTCARE, "Consolas"
    );
    old_font = (HFONT)SelectObject(dc, font);
    SetBkMode(dc, TRANSPARENT);
    SetTextColor(dc, RGB(220, 236, 252));

    text_rect.left = frame_x + 26;
    text_rect.top = frame_y + 10;
    text_rect.right = frame_x + frame_w - 220;
    text_rect.bottom = frame_y + 30;
    DrawTextA(dc, "Game Screen (GB Studio 20x18)", -1, &text_rect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

    text_rect.left = frame_x + frame_w - 194;
    text_rect.top = frame_y + 50;
    text_rect.right = frame_x + frame_w - 40;
    text_rect.bottom = frame_y + 70;
    DrawTextA(dc, "Scene: Overworld_01", -1, &text_rect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

    text_rect.left = frame_x + frame_w - 194;
    text_rect.top = frame_y + 88;
    text_rect.right = frame_x + frame_w - 40;
    text_rect.bottom = frame_y + 168;
    DrawTextA(dc, "Actors\n- Hero\n- NPC_Merchant\n- Enemy_Slime", -1, &text_rect, DT_LEFT | DT_TOP | DT_WORDBREAK);

    text_rect.left = frame_x + frame_w - 194;
    text_rect.top = frame_y + 188;
    text_rect.right = frame_x + frame_w - 40;
    text_rect.bottom = frame_y + 250;
    DrawTextA(dc, "Triggers\n- Shop Zone\n- Exit Warp", -1, &text_rect, DT_LEFT | DT_TOP | DT_WORDBREAK);

    text_rect.left = frame_x + 26;
    text_rect.top = frame_y + frame_h - 46;
    text_rect.right = frame_x + frame_w - 20;
    text_rect.bottom = frame_y + frame_h - 10;
    DrawTextA(dc, overlay_text ? overlay_text : "", -1, &text_rect, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);

    SelectObject(dc, old_font);
    DeleteObject(font);
}

static LRESULT CALLBACK editor_window_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
    EditorWindow *win = (EditorWindow *)GetWindowLongPtrA(hwnd, GWLP_USERDATA);

    if (msg == WM_NCCREATE) {
        CREATESTRUCTA *cs = (CREATESTRUCTA *)lparam;
        SetWindowLongPtrA(hwnd, GWLP_USERDATA, (LONG_PTR)cs->lpCreateParams);
        return TRUE;
    }

    switch (msg) {
        case WM_ERASEBKGND:
            return 1; /* Prevent default background erase to reduce flicker */
        case WM_SIZE:
            if (win) {
                win->width = LOWORD(lparam);
                win->height = HIWORD(lparam);
            }
            return 0;
        case WM_MOUSEMOVE:
            if (win) {
                int mx = GET_X_LPARAM(lparam);
                int my = GET_Y_LPARAM(lparam);
                int i;
                TRACKMOUSEEVENT tme;
                int prev_hover = win->hover_action;
                win->hover_action = -1;
                if (!win->tracking_mouse_leave) {
                    tme.cbSize = sizeof(tme);
                    tme.dwFlags = TME_LEAVE;
                    tme.hwndTrack = hwnd;
                    tme.dwHoverTime = 0;
                    if (TrackMouseEvent(&tme)) {
                        win->tracking_mouse_leave = true;
                    }
                }
                for (i = 0; i < win->action_count; i++) {
                    const WindowUIAction *a = &win->actions[i];
                    if (mx >= a->x && mx < (a->x + a->w) && my >= a->y && my < (a->y + a->h)) {
                        win->hover_action = i;
                        break;
                    }
                }
                if (prev_hover != win->hover_action) {
                    InvalidateRect(hwnd, NULL, FALSE);
                }
            }
            return 0;
        case WM_MOUSELEAVE:
            if (win) {
                win->tracking_mouse_leave = false;
                if (win->hover_action != -1) {
                    win->hover_action = -1;
                    InvalidateRect(hwnd, NULL, FALSE);
                }
            }
            return 0;
        case WM_LBUTTONDOWN:
            if (win) {
                int mx = GET_X_LPARAM(lparam);
                int my = GET_Y_LPARAM(lparam);
                int i;
                SetFocus(hwnd);
                win->pressed_action = -1;
                for (i = 0; i < win->action_count; i++) {
                    const WindowUIAction *a = &win->actions[i];
                    if (mx >= a->x && mx < (a->x + a->w) && my >= a->y && my < (a->y + a->h)) {
                        win->pressed_action = i;
                        SetCapture(hwnd);
                        break;
                    }
                }
            }
            return 0;
        case WM_LBUTTONUP:
            if (win) {
                int mx = GET_X_LPARAM(lparam);
                int my = GET_Y_LPARAM(lparam);
                int i;
                int released_action = -1;
                if (GetCapture() == hwnd) {
                    ReleaseCapture();
                }
                for (i = 0; i < win->action_count; i++) {
                    const WindowUIAction *a = &win->actions[i];
                    if (mx >= a->x && mx < (a->x + a->w) && my >= a->y && my < (a->y + a->h)) {
                        released_action = i;
                        break;
                    }
                }
                if (released_action >= 0 && released_action == win->pressed_action) {
                    const WindowUIAction *a = &win->actions[released_action];
                    strncpy(win->pending_command, a->command_id, sizeof(win->pending_command) - 1);
                    win->pending_command[sizeof(win->pending_command) - 1] = '\0';
                    InvalidateRect(hwnd, NULL, FALSE);
                }
                win->pressed_action = -1;
            }
            return 0;
        case WM_CAPTURECHANGED:
            if (win) {
                win->pressed_action = -1;
            }
            return 0;
        case WM_CLOSE:
            if (win) win->open = false;
            DestroyWindow(hwnd);
            return 0;
        case WM_DESTROY:
            if (win) win->open = false;
            PostQuitMessage(0);
            return 0;
        default:
            return DefWindowProcA(hwnd, msg, wparam, lparam);
    }
}

static bool register_window_class(void) {
    WNDCLASSEXA wc;
    HINSTANCE hinst = GetModuleHandleA(NULL);

    if (g_class_registered) return true;
    memset(&wc, 0, sizeof(wc));
    wc.cbSize = sizeof(wc);
    wc.lpfnWndProc = editor_window_proc;
    wc.hInstance = hinst;
    wc.lpszClassName = EDITOR_WIN_CLASS;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    g_class_registered = RegisterClassExA(&wc) ? 1 : 0;
    return g_class_registered ? true : false;
}

EditorWindow *window_create(const char *title, int width, int height) {
    EditorWindow *win = (EditorWindow *)malloc(sizeof(EditorWindow));
    RECT r;
    DWORD style = WS_OVERLAPPEDWINDOW;
    if (!win) return NULL;

    if (!register_window_class()) {
        free(win);
        return NULL;
    }

    snprintf(win->title, 64, "%s", title ? title : "IDE Editor");
    win->width = width;
    win->height = height;
    win->open = true;
    win->overlay_text[0] = '\0';
    win->loading_active = false;
    snprintf(win->loading_title, sizeof(win->loading_title), "%s", "Loading");
    win->loading_subtitle[0] = '\0';
    win->loading_progress_percent = 0;
    win->action_count = 0;
    win->hover_action = -1;
    win->pressed_action = -1;
    win->pending_command[0] = '\0';
    win->hwnd = NULL;
    win->back_dc = NULL;
    win->back_bmp = NULL;
    win->back_old_bmp = NULL;
    win->back_w = 0;
    win->back_h = 0;
    win->tracking_mouse_leave = false;

    r.left = 0;
    r.top = 0;
    r.right = width;
    r.bottom = height;
    AdjustWindowRect(&r, style, FALSE);

    win->hwnd = CreateWindowExA(
        0,
        EDITOR_WIN_CLASS,
        win->title,
        style,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        r.right - r.left,
        r.bottom - r.top,
        NULL,
        NULL,
        GetModuleHandleA(NULL),
        win
    );
    if (!win->hwnd) {
        free(win);
        return NULL;
    }
    return win;
}

void window_show(EditorWindow *win) {
    if (!win || !win->hwnd) return;
    ShowWindow(win->hwnd, SW_SHOW);
    UpdateWindow(win->hwnd);
    printf("Showing window: %s (%dx%d)\n", win->title, win->width, win->height);
}

bool window_is_open(EditorWindow *win) {
    if (!win) return false;
    return win->open;
}

void window_poll_events(EditorWindow *win) {
    MSG msg;
    if (!win) return;

    while (PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE)) {
        if (msg.message == WM_QUIT) {
            win->open = false;
        }
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }
}

void window_render(EditorWindow *win) {
    HDC dc;
    RECT rc;
    RECT top_bar;
    RECT menu_bar;
    RECT tool_bar;
    RECT left_dock;
    RECT right_dock;
    RECT bottom_dock;
    RECT center_view;
    RECT status_bar;
    RECT text_rect;
    RECT action_rect;
    int w, h;
    HFONT font_ui;
    HFONT font_old;
    int top_h = 30;
    int menu_h = 24;
    int tools_h = 34;
    int status_h = 22;
    int left_w = 240;
    int right_w = 290;
    int bottom_h = 150;
    const char *fallback_text = "IDE Editor GUI Active - Press Q in console or close window to exit";
    const char *text = (win && win->overlay_text[0]) ? win->overlay_text : fallback_text;

    if (!win || !win->hwnd || !win->open) return;

    dc = GetDC(win->hwnd);
    if (!dc) return;

    GetClientRect(win->hwnd, &rc);
    w = rc.right - rc.left;
    h = rc.bottom - rc.top;
    if (w <= 0 || h <= 0) {
        ReleaseDC(win->hwnd, dc);
        return;
    }

    if (!win->back_dc) {
        win->back_dc = CreateCompatibleDC(dc);
    }
    if (!win->back_dc) {
        ReleaseDC(win->hwnd, dc);
        return;
    }

    if (!win->back_bmp || win->back_w != w || win->back_h != h) {
        if (win->back_bmp) {
            SelectObject(win->back_dc, win->back_old_bmp);
            DeleteObject(win->back_bmp);
            win->back_bmp = NULL;
            win->back_old_bmp = NULL;
        }
        win->back_bmp = CreateCompatibleBitmap(dc, w, h);
        if (!win->back_bmp) {
            ReleaseDC(win->hwnd, dc);
            return;
        }
        win->back_old_bmp = (HBITMAP)SelectObject(win->back_dc, win->back_bmp);
        win->back_w = w;
        win->back_h = h;
    }

    /* Base background */
    fill_rect_color(win->back_dc, &rc, RGB(18, 22, 30));

    /* Layout regions */
    top_bar.left = 0; top_bar.top = 0; top_bar.right = w; top_bar.bottom = top_h;
    menu_bar.left = 0; menu_bar.top = top_bar.bottom; menu_bar.right = w; menu_bar.bottom = menu_bar.top + menu_h;
    tool_bar.left = 0; tool_bar.top = menu_bar.bottom; tool_bar.right = w; tool_bar.bottom = tool_bar.top + tools_h;
    status_bar.left = 0; status_bar.right = w; status_bar.bottom = h; status_bar.top = h - status_h;

    left_dock.left = 0; left_dock.top = tool_bar.bottom; left_dock.right = left_w; left_dock.bottom = status_bar.top - bottom_h;
    right_dock.left = w - right_w; right_dock.top = tool_bar.bottom; right_dock.right = w; right_dock.bottom = status_bar.top - bottom_h;
    bottom_dock.left = 0; bottom_dock.top = status_bar.top - bottom_h; bottom_dock.right = w; bottom_dock.bottom = status_bar.top;
    center_view.left = left_dock.right; center_view.top = tool_bar.bottom; center_view.right = right_dock.left; center_view.bottom = bottom_dock.top;

    fill_rect_color(win->back_dc, &top_bar, RGB(12, 15, 22));
    fill_rect_color(win->back_dc, &menu_bar, RGB(22, 28, 38));
    fill_rect_color(win->back_dc, &tool_bar, RGB(26, 34, 47));
    fill_rect_color(win->back_dc, &left_dock, RGB(24, 31, 43));
    fill_rect_color(win->back_dc, &right_dock, RGB(24, 31, 43));
    fill_rect_color(win->back_dc, &center_view, RGB(16, 20, 28));
    fill_rect_color(win->back_dc, &bottom_dock, RGB(20, 26, 36));
    fill_rect_color(win->back_dc, &status_bar, RGB(14, 18, 26));

    frame_rect_color(win->back_dc, &menu_bar, RGB(48, 60, 82));
    frame_rect_color(win->back_dc, &tool_bar, RGB(48, 60, 82));
    frame_rect_color(win->back_dc, &left_dock, RGB(48, 60, 82));
    frame_rect_color(win->back_dc, &right_dock, RGB(48, 60, 82));
    frame_rect_color(win->back_dc, &center_view, RGB(72, 92, 124));
    frame_rect_color(win->back_dc, &bottom_dock, RGB(48, 60, 82));

    font_ui = CreateFontA(
        16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
        FF_DONTCARE, "Consolas"
    );
    font_old = (HFONT)SelectObject(win->back_dc, font_ui);
    SetBkMode(win->back_dc, TRANSPARENT);
    SetTextColor(win->back_dc, RGB(220, 230, 245));

    /* Top/title areas */
    text_rect = top_bar;
    text_rect.left += 10;
    DrawTextA(win->back_dc, "Labyrinth IDE Editor - Unreal/GB Layout", -1, &text_rect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
    text_rect = menu_bar;
    text_rect.left += 10;
    DrawTextA(win->back_dc, "File  Edit  View  Tools  Blueprint  Build  GB Studio  Window  Help", -1, &text_rect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
    text_rect = tool_bar;
    text_rect.left += 10;
    DrawTextA(win->back_dc, "[New] [Open] [Save] [Undo] [Redo] [Play] [Stop] [Build] [Validate] [Palette] [Script] [Actor] [Trigger]", -1, &text_rect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

    /* Dock labels */
    text_rect = left_dock; text_rect.left += 8; text_rect.top += 6;
    DrawTextA(win->back_dc, "Left Dock\n- World Hierarchy\n- Scene Tree\n- Asset Outliner\n- Actor List", -1, &text_rect, DT_LEFT | DT_TOP);
    text_rect = right_dock; text_rect.left += 8; text_rect.top += 6;
    DrawTextA(win->back_dc, "Right Dock\n- Details\n- Inspector\n- Tile Palette\n- Profiler", -1, &text_rect, DT_LEFT | DT_TOP);
    text_rect = bottom_dock; text_rect.left += 8; text_rect.top += 6;
    DrawTextA(win->back_dc, "Bottom Dock\n- Content Browser\n- Output Log\n- Timeline\n- Command Palette", -1, &text_rect, DT_LEFT | DT_TOP);

    /* Main viewport content: GB Studio-like game editing screen */
    draw_gb_studio_preview(win->back_dc, &center_view, text);

    text_rect = status_bar;
    text_rect.left += 10;
    SetTextColor(win->back_dc, RGB(170, 190, 220));
    DrawTextA(win->back_dc, "TAB Menu Mode | Arrows Navigate | Enter Execute | Q/Esc Quit", -1, &text_rect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

    /* Interactive buttons/menus/actions */
    {
        int i;
        for (i = 0; i < win->action_count; i++) {
            const WindowUIAction *a = &win->actions[i];
            COLORREF fill = (i == win->hover_action) ? RGB(80, 120, 180) : RGB(46, 66, 98);
            action_rect.left = a->x;
            action_rect.top = a->y;
            action_rect.right = a->x + a->w;
            action_rect.bottom = a->y + a->h;
            fill_rect_color(win->back_dc, &action_rect, fill);
            frame_rect_color(win->back_dc, &action_rect, RGB(120, 160, 220));
            text_rect = action_rect;
            text_rect.left += 6;
            SetTextColor(win->back_dc, RGB(240, 246, 255));
            DrawTextA(win->back_dc, a->label, -1, &text_rect, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);
        }
    }

    if (win->loading_active) {
        RECT shade;
        RECT modal;
        RECT progress_outer;
        RECT progress_inner;
        char progress_label[32];
        HFONT font_loading;
        HFONT font_loading_old;
        int modal_w = 520;
        int modal_h = 190;
        int usable_w;

        shade.left = 0;
        shade.top = 0;
        shade.right = w;
        shade.bottom = h;
        fill_rect_color(win->back_dc, &shade, RGB(8, 12, 18));

        modal.left = (w - modal_w) / 2;
        modal.top = (h - modal_h) / 2;
        modal.right = modal.left + modal_w;
        modal.bottom = modal.top + modal_h;
        fill_rect_color(win->back_dc, &modal, RGB(18, 30, 54));
        frame_rect_color(win->back_dc, &modal, RGB(124, 178, 255));

        font_loading = CreateFontA(
            22, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
            FF_DONTCARE, "Consolas"
        );
        font_loading_old = (HFONT)SelectObject(win->back_dc, font_loading);
        SetBkMode(win->back_dc, TRANSPARENT);
        SetTextColor(win->back_dc, RGB(236, 246, 255));

        text_rect = modal;
        text_rect.left += 22;
        text_rect.top += 18;
        text_rect.right -= 22;
        text_rect.bottom = text_rect.top + 30;
        DrawTextA(win->back_dc, win->loading_title, -1, &text_rect, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);

        SelectObject(win->back_dc, font_old);
        DeleteObject(font_loading);

        SetTextColor(win->back_dc, RGB(188, 210, 236));
        text_rect = modal;
        text_rect.left += 22;
        text_rect.top += 54;
        text_rect.right -= 22;
        text_rect.bottom = text_rect.top + 42;
        DrawTextA(win->back_dc, win->loading_subtitle, -1, &text_rect, DT_LEFT | DT_TOP | DT_WORDBREAK | DT_END_ELLIPSIS);

        progress_outer.left = modal.left + 22;
        progress_outer.right = modal.right - 22;
        progress_outer.top = modal.bottom - 58;
        progress_outer.bottom = progress_outer.top + 20;
        fill_rect_color(win->back_dc, &progress_outer, RGB(8, 16, 30));
        frame_rect_color(win->back_dc, &progress_outer, RGB(96, 146, 220));

        usable_w = (progress_outer.right - progress_outer.left - 2) * win->loading_progress_percent / 100;
        if (usable_w < 0) usable_w = 0;
        progress_inner.left = progress_outer.left + 1;
        progress_inner.top = progress_outer.top + 1;
        progress_inner.bottom = progress_outer.bottom - 1;
        progress_inner.right = progress_inner.left + usable_w;
        if (progress_inner.right > progress_outer.right - 1) progress_inner.right = progress_outer.right - 1;
        if (progress_inner.right > progress_inner.left) {
            fill_rect_color(win->back_dc, &progress_inner, RGB(74, 134, 224));
        }

        snprintf(progress_label, sizeof(progress_label), "%d%%", win->loading_progress_percent);
        SetTextColor(win->back_dc, RGB(220, 236, 255));
        text_rect = progress_outer;
        DrawTextA(win->back_dc, progress_label, -1, &text_rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

        SelectObject(win->back_dc, font_loading_old);
    }

    SelectObject(win->back_dc, font_old);
    DeleteObject(font_ui);

    BitBlt(dc, 0, 0, w, h, win->back_dc, 0, 0, SRCCOPY);

    ReleaseDC(win->hwnd, dc);
}

void window_set_overlay_text(EditorWindow *win, const char *text) {
    if (!win) return;
    if (!text) {
        win->overlay_text[0] = '\0';
        return;
    }
    strncpy(win->overlay_text, text, sizeof(win->overlay_text) - 1);
    win->overlay_text[sizeof(win->overlay_text) - 1] = '\0';
}

void window_set_loading_screen(EditorWindow *win, bool active, const char *title, const char *subtitle, int progress_percent) {
    if (!win) return;
    win->loading_active = active;
    if (title && title[0]) {
        strncpy(win->loading_title, title, sizeof(win->loading_title) - 1);
        win->loading_title[sizeof(win->loading_title) - 1] = '\0';
    } else {
        snprintf(win->loading_title, sizeof(win->loading_title), "%s", "Loading");
    }
    if (subtitle && subtitle[0]) {
        strncpy(win->loading_subtitle, subtitle, sizeof(win->loading_subtitle) - 1);
        win->loading_subtitle[sizeof(win->loading_subtitle) - 1] = '\0';
    } else {
        win->loading_subtitle[0] = '\0';
    }
    if (progress_percent < 0) progress_percent = 0;
    if (progress_percent > 100) progress_percent = 100;
    win->loading_progress_percent = progress_percent;
}

void window_set_ui_actions(EditorWindow *win, const WindowUIAction *actions, int count) {
    int i;
    if (!win) return;
    if (!actions || count <= 0) {
        win->action_count = 0;
        return;
    }
    if (count > WINDOW_UI_MAX_ACTIONS) count = WINDOW_UI_MAX_ACTIONS;
    win->action_count = count;
    for (i = 0; i < count; i++) {
        win->actions[i] = actions[i];
    }
}

bool window_pop_ui_action(EditorWindow *win, char *out_command, int out_len) {
    if (!win || !out_command || out_len <= 0) return false;
    if (!win->pending_command[0]) return false;
    strncpy(out_command, win->pending_command, (size_t)(out_len - 1));
    out_command[out_len - 1] = '\0';
    win->pending_command[0] = '\0';
    return true;
}

void window_destroy(EditorWindow *win) {
    if (!win) return;
    if (win->back_dc) {
        if (win->back_bmp && win->back_old_bmp) {
            SelectObject(win->back_dc, win->back_old_bmp);
        }
        if (win->back_bmp) {
            DeleteObject(win->back_bmp);
            win->back_bmp = NULL;
        }
        DeleteDC(win->back_dc);
        win->back_dc = NULL;
    }
    if (win->hwnd && IsWindow(win->hwnd)) {
        DestroyWindow(win->hwnd);
        win->hwnd = NULL;
    }
    free(win);
}
