#pragma once
#include <vector>
#include <functional>

namespace bt {

void reqBacktrack(size_t k, size_t n, size_t i0,
                  std::vector<bool>& used,
                  std::function<void()> callback);

}
