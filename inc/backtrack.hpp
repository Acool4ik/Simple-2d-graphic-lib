#pragma once
#include <vector>

namespace bt {

template<typename T>
void reqBacktrack(size_t k, size_t n, size_t i0, std::vector<bool>& used, T callback)
{
    if (k == n)
    {
        callback();
        return;
    }

    for (size_t i = i0; i < used.size(); i++)
    {
        if (!used[i])
        {
            used[i] = true;
            reqBacktrack(k + 1, n, i + 1, used, callback);
            used[i] = false;
        }
    }
}

}
