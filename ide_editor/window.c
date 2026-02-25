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
    WindowUIAction actions[WINDOW_UI_MAX_ACTIONS];
    int action_count;
    int hover_action;
    char pending_command[64];
    HWND hwnd;
    HDC back_dc;
    HBITMAP back_bmp;
    HBITMAP back_old_bmp;
    int back_w;
    int back_h;
    int width, height;
    bool open;
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
                win->hover_action = -1;
                for (i = 0; i < win->action_count; i++) {
                    const WindowUIAction *a = &win->actions[i];
                    if (mx >= a->x && mx < (a->x + a->w) && my >= a->y && my < (a->y + a->h)) {
                        win->hover_action = i;
                        break;
                    }
                }
            }
            return 0;
        case WM_LBUTTONDOWN:
            if (win) {
                int mx = GET_X_LPARAM(lparam);
                int my = GET_Y_LPARAM(lparam);
                int i;
                for (i = 0; i < win->action_count; i++) {
                    const WindowUIAction *a = &win->actions[i];
                    if (mx >= a->x && mx < (a->x + a->w) && my >= a->y && my < (a->y + a->h)) {
                        strncpy(win->pending_command, a->command_id, sizeof(win->pending_command) - 1);
                        win->pending_command[sizeof(win->pending_command) - 1] = '\0';
                        break;
                    }
                }
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
    win->action_count = 0;
    win->hover_action = -1;
    win->pending_command[0] = '\0';
    win->hwnd = NULL;
    win->back_dc = NULL;
    win->back_bmp = NULL;
    win->back_old_bmp = NULL;
    win->back_w = 0;
    win->back_h = 0;

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

    /* Main viewport content (overlay text) */
    text_rect = center_view;
    text_rect.left += 12;
    text_rect.top += 10;
    text_rect.right -= 12;
    text_rect.bottom -= 10;
    DrawTextA(win->back_dc, text, -1, &text_rect, DT_LEFT | DT_TOP | DT_WORDBREAK);

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
