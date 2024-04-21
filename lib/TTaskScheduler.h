#pragma once

#include <stack>
#include "Graph.h"

class TTaskScheduler {
public:
    template <typename Expression, typename T>
    [[nodiscard]] Point Add(Expression&& expression, const T& argument1, const T& argument2) {
        Point new_node = CreateNode(std::move(expression), argument1, argument2);
        graph.InsertNode(new_node);

        return new_node;
    }

    template <typename Expression, typename T>
    [[nodiscard]] Point Add(Expression&& expression, const T& argument1, const T* argument2) {
        Point new_node = CreateNode(std::move(expression), argument1, argument2);
        graph.InsertNode(new_node);

        return new_node;
    }

    template <typename Expression, typename T>
    [[nodiscard]] Point Add(Expression&& expression, const T* argument1, const T* argument2) {
        Point new_node = CreateNode(std::move(expression), argument1, argument2);
        graph.InsertNode(new_node);

        return new_node;
    }

    template <typename Expression, typename T>
    [[nodiscard]] Point Add(Expression&& expression, const T& argument1) {
        Point new_node = CreateNode(std::move(expression), argument1);
        graph.InsertNode(new_node);

        return new_node;
    }

    template <typename Expression, typename T>
    [[nodiscard]] Point Add(Expression&& expression, const T* argument1) {
        Point new_node = CreateNode(std::move(expression), argument1);
        graph.InsertNode(new_node);

        return new_node;
    }

    template <typename T>
    [[nodiscard]] T GetResult(Point& node) const {
        if (!node->GetIsResultCalculated())
            ExecuteAll();

        return *static_cast<const T*>(node->GetResult());
    }

    template <typename T>
    [[nodiscard]] inline const T* GetFutureResult(const Point& node) {
        graph.InsertToList(node->NodePosition(), graph.count);

        return static_cast<const T*>(node->GetResult());
    }

    void ExecuteAll() const;

private:
    Graph graph;
};