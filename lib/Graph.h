#pragma once

#include <type_traits>
#include <memory>
#include <cassert>
#include <vector>
#include <stack>

static unsigned position = -1;

class Node {
public:
    [[nodiscard]] bool GetIsResultCalculated() const {
        return is_answer;
    }
    
    [[nodiscard]] unsigned NodePosition() const {
        return id;
    }

    void ResultCalculated() {
        is_answer = true;
    }

public:
    [[nodiscard]] virtual const void* GetResult() const = 0;

    virtual void Calculate() = 0;

private:
    unsigned id = ++position;
    bool is_answer = false;
};

template <typename Expression, typename T>
class TwoArguments : public Node {
public:
    TwoArguments(Expression&& exp, const T& arg1, const T& arg2) : expression(std::move(exp)), first(arg1), second(arg2) {}

    ~TwoArguments() {
        --position;
    }

    void Calculate() override {
        static_assert(std::is_same_v<decltype(first), const T> && std::is_same_v<decltype(second), const T>, "Different types");
        result = expression(first, second);
        ResultCalculated();
    }

    const void* GetResult() const override {
        return &result;
    }

private:
    const T first;
    const T second;
    const Expression expression;
    T result;
};

template <typename Expression, typename T>
class CommonAndFutureArguments : public Node {
public:
    CommonAndFutureArguments(Expression&& exp, const T& arg1, const T* arg2) : expression(std::move(exp)), first(arg1), second(arg2) {}

    ~CommonAndFutureArguments() {
        --position;
    }

    void Calculate() override {
        static_assert(std::is_same_v<decltype(first), const T> && std::is_same_v<decltype(second), const T*>, "Different types");
        result = expression(first, *second);
        ResultCalculated();
    }

    const void* GetResult() const override {
        return &result;
    }

private:
    const T first;
    const T* second;
    const Expression expression;
    T result;
};

template <typename Expression, typename T>
class TwoFutureArguments : public Node {
public:
    TwoFutureArguments(Expression&& exp, const T* arg1, const T* arg2) : expression(std::move(exp)), first(arg1), second(arg2) {}

    ~TwoFutureArguments() {
        --position;
    }

    void Calculate() override {
        static_assert(std::is_same_v<decltype(first), const T*> && std::is_same_v<decltype(second), const T*>, "Different types");
        result = expression(*first, *second);
        ResultCalculated();
    }

    const void* GetResult() const override {
        return &result;
    }

private:
    const T* first;
    const T* second;
    const Expression expression;
    T result;
};

template <typename Expression, typename T>
class CommonArgument : public Node {
public:
    CommonArgument(Expression&& exp, const T& arg1) : expression(std::move(exp)), first(arg1) {}

    ~CommonArgument() {
        --position;
    }

    void Calculate() override {
        static_assert(std::is_same_v<decltype(first), const T>, "Different types");
        result = expression(first);
        ResultCalculated();
    }

    const void* GetResult() const override {
        return &result;
    }

private:
    const T first;
    const Expression expression;
    T result;
};

template <typename Expression, typename T>
class FutureArgument : public Node {
public:
    FutureArgument(Expression&& exp, const T* arg1) : expression(std::move(exp)), first(arg1) {}

    ~FutureArgument() {
        --position;
    }

    void Calculate() override {
        static_assert(std::is_same_v<decltype(first), const T*>, "Different types");
        result = expression(*first);
        ResultCalculated();
    }

    const void* GetResult() const override {
        return &result;
    }

private:
    const T* first;
    const Expression expression;
    T result;
};


using Point = std::shared_ptr<Node>;

template <typename Expression, typename T>
Point CreateNode(Expression&& exp, const T& arg1, const T& arg2) {
    return std::make_shared<TwoArguments<Expression, T>>(std::move(exp), arg1, arg2);
}

template <typename Expression, typename T>
Point CreateNode(Expression&& exp, const T& arg1, const T* arg2) {
    return std::make_shared<CommonAndFutureArguments<Expression, T>>(std::move(exp), arg1, arg2);
}

template <typename Expression, typename T>
Point CreateNode(Expression&& exp, const T* arg1, const T* arg2) {
    return std::make_shared<TwoFutureArguments<Expression, T>>(std::move(exp), arg1, arg2);
}

template <typename Expression, typename T>
Point CreateNode(Expression&& exp, const T& arg1) {
    return std::make_shared<CommonArgument<Expression, T>>(std::move(exp), arg1);
}

template <typename Expression, typename T>
Point CreateNode(Expression&& exp, const T* arg1) {
    return std::make_shared<FutureArgument<Expression, T>>(std::move(exp), arg1);
}

class Graph {
public:
    std::vector<Point> nodes;
    unsigned count = 0;

public:
    void InsertNode(const Point& node);

    void InsertToList(const unsigned& parent, const unsigned& new_node);

    [[nodiscard]] std::stack<unsigned> TopSort() const;

    [[nodiscard]] inline const Point& GetNodePointer(const unsigned& id) const {
        return nodes[id];
    }

private:
    std::vector<std::vector<unsigned>> adjacency_list;

    void CheckSize();

    void DFS(std::vector<unsigned>& visited, std::stack<unsigned>& stack, int v) const;
};

void Graph::InsertNode(const Point& node) {
    nodes.push_back(node);
    ++count;

    CheckSize();
}

void Graph::InsertToList(const unsigned& parent, const unsigned& new_node) {
    adjacency_list[parent].push_back(new_node);
    CheckSize();
}

[[nodiscard]] std::stack<unsigned> Graph::TopSort() const {
    std::vector<unsigned> visited(nodes.size(), 0);
    std::stack<unsigned> st;

    for (int i = 0; i < nodes.size(); ++i)
        if (!visited[i])
            DFS(visited, st, i);
    return st;
}

void Graph::CheckSize() {
    if (adjacency_list.size() <= nodes.size() + 1) {
        adjacency_list.resize(adjacency_list.size() * 1.5 + 1);
    }
}

void Graph::DFS(std::vector<unsigned>& visited, std::stack<unsigned>& st, int v) const {
    visited[v] = 1;
    for (int u: adjacency_list[v])
        if (!visited[u])
            DFS(visited, st, u);
    st.push(v);
}
