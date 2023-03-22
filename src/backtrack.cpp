#include "../inc/backtrack.hpp"

using namespace bt;

void bt::reqBacktrack(size_t k, size_t n, size_t i0,
                  std::vector<bool>& used,
                  std::function<void()> callback)
{
    if (k == n)
    {
        callback();
        return;
    }

    for (size_t i = i0; i < used.size(); i++)
    if (!used[i])
    {
        used[i] = true;
        reqBacktrack(k + 1, n, i + 1, used, callback);
        used[i] = false;
    }
}
