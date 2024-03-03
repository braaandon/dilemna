#pragma once

#include "state.h"

class KeyHandler {
public:
    static void install_hook();
    static void uninstall_hook();

    static State* state;

private:
    static HHOOK hook;

    static LRESULT CALLBACK key_hook(int code, WPARAM wparam, LPARAM lparam);
    static void notify(KBDLLHOOKSTRUCT info);
};