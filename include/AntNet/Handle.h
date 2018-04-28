//
// Created by jpfeltracco on 4/28/18.
//

#ifndef ANTNET_HANDLE_H
#define ANTNET_HANDLE_H

#include <cstddef>
#include <optional>

class Handle {
public:
    Handle() = default;
    explicit Handle(std::size_t val) : handle(val) { }

    auto set(std::size_t val) { handle = val; }
    auto get() { return *handle; }
    auto is_valid() { return handle.has_value(); }
private:
    std::optional<std::size_t> handle;
};

#endif //ANTNET_HANDLE_H
