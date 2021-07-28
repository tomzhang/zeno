#pragma once

#include <string>
#include <cstring>

namespace zpp {

void __attach_debugger();

struct exception : std::exception {
public:
    exception() noexcept {
        __attach_debugger();
    }
};

struct signal_exception : exception {
    int _M_signo;

    signal_exception(int signo) noexcept
        : _M_signo(signo) {}

    char const *what() const noexcept {
        return strsignal(_M_signo);
    }
};

void __attach_debugger();

}
