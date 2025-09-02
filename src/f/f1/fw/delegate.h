#include <iostream>
#include <vector>
#include <functional>
#include <memory>
#include <cstdint>

// 定义句柄类型（用于标识已添加的函数）
using DelegateHandle = uint64_t;

// 模板委托类，支持添加(+=)和移除(-=)函数
template <typename... Args>
class Delegate {
private:
    // 存储函数和对应的唯一标识
    struct FunctionEntry {
        std::function<void(Args...)> func;
        DelegateHandle handle;
    };

    std::vector<FunctionEntry> functions;
    DelegateHandle nextHandle = 1;  // 句柄从1开始（0作为无效值）

public:
    // 添加函数，返回唯一句柄
    DelegateHandle operator+=(const std::function<void(Args...)>& func) {
        DelegateHandle handle = nextHandle++;
        functions.push_back({func, handle});
        return handle;  // 返回句柄，用于后续移除
    }

    // 通过句柄移除函数
    bool operator-=(DelegateHandle handle) {
        for (auto it = functions.begin(); it != functions.end(); ++it) {
            if (it->handle == handle) {
                functions.erase(it);
                return true;  // 成功移除
            }
        }
        return false;  // 未找到该函数
    }

    // 调用所有注册的函数
    void operator()(Args... args) const {
        for (const auto& entry : functions) {
            entry.func(args...);
        }
    }

    // 清空所有函数
    void clear() {
        functions.clear();
    }
};