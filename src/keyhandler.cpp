#include "keyhandler.h"

HHOOK KeyHandler::hook = nullptr;
State* KeyHandler::state = nullptr;

LRESULT CALLBACK KeyHandler::key_hook(int code, WPARAM wparam, LPARAM lparam) {
    if (code == HC_ACTION && wparam == WM_KEYDOWN) {
        notify(*reinterpret_cast<LPKBDLLHOOKSTRUCT>(lparam));
    }

    return CallNextHookEx(hook, code, wparam, lparam);
}

void KeyHandler::notify(KBDLLHOOKSTRUCT info) {
    auto cb = state->find_callback(info.vkCode);

    if (cb)
        cb.value()->enabled = !cb.value()->enabled;
}

void KeyHandler::install_hook() {
    hook = SetWindowsHookExA(WH_KEYBOARD_LL, key_hook, GetModuleHandleA(nullptr), 0);
}

void KeyHandler::uninstall_hook() {
    UnhookWindowsHookEx(hook);
}