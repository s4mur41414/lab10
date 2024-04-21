#include "TTaskScheduler.h"

void TTaskScheduler::ExecuteAll() const {
    std::stack<unsigned> ans = graph.TopSort();
    for (size_t i = 0; i < ans.size(); ++i) {
        graph.nodes[ans.top()]->Calculate();
        ans.pop();
    }
}