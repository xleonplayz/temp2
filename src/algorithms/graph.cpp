#include "algorithms/graph.hpp"
#include <algorithm>
#include <stack>
#include <queue>

namespace temp2::algorithms {

// =============================================================================
// Graph
// =============================================================================

Graph::Graph() : directed_(false), edge_count_(0) {}

Graph::Graph(int num_vertices, bool directed)
    : adj_(num_vertices), directed_(directed), edge_count_(0) {}

void Graph::add_vertex() {
    adj_.emplace_back();
}

void Graph::add_edge(int from, int to, double weight) {
    adj_[from].emplace_back(to, weight);
    if (!directed_) {
        adj_[to].emplace_back(from, weight);
    }
    ++edge_count_;
}

void Graph::remove_edge(int from, int to) {
    auto& edges = adj_[from];
    edges.erase(std::remove_if(edges.begin(), edges.end(),
        [to](const Edge& e) { return e.to == to; }), edges.end());

    if (!directed_) {
        auto& back_edges = adj_[to];
        back_edges.erase(std::remove_if(back_edges.begin(), back_edges.end(),
            [from](const Edge& e) { return e.to == from; }), back_edges.end());
    }
    --edge_count_;
}

bool Graph::has_edge(int from, int to) const {
    for (const auto& e : adj_[from]) {
        if (e.to == to) return true;
    }
    return false;
}

int Graph::vertex_count() const { return adj_.size(); }
int Graph::edge_count() const { return edge_count_; }
bool Graph::is_directed() const { return directed_; }

std::vector<int> Graph::neighbors(int vertex) const {
    std::vector<int> result;
    for (const auto& e : adj_[vertex]) {
        result.push_back(e.to);
    }
    return result;
}

std::vector<Edge> Graph::edges(int vertex) const {
    return adj_[vertex];
}

double Graph::edge_weight(int from, int to) const {
    for (const auto& e : adj_[from]) {
        if (e.to == to) return e.weight;
    }
    return std::numeric_limits<double>::infinity();
}

std::vector<int> Graph::bfs(int start) const {
    std::vector<int> result;
    std::vector<bool> visited(adj_.size(), false);
    std::queue<int> q;

    q.push(start);
    visited[start] = true;

    while (!q.empty()) {
        int v = q.front();
        q.pop();
        result.push_back(v);

        for (const auto& e : adj_[v]) {
            if (!visited[e.to]) {
                visited[e.to] = true;
                q.push(e.to);
            }
        }
    }
    return result;
}

void Graph::dfs_helper(int v, std::vector<bool>& visited, std::vector<int>& result) const {
    visited[v] = true;
    result.push_back(v);
    for (const auto& e : adj_[v]) {
        if (!visited[e.to]) {
            dfs_helper(e.to, visited, result);
        }
    }
}

std::vector<int> Graph::dfs(int start) const {
    std::vector<int> result;
    std::vector<bool> visited(adj_.size(), false);
    dfs_helper(start, visited, result);
    return result;
}

void Graph::topo_dfs(int v, std::vector<bool>& visited, std::vector<int>& result) const {
    visited[v] = true;
    for (const auto& e : adj_[v]) {
        if (!visited[e.to]) {
            topo_dfs(e.to, visited, result);
        }
    }
    result.push_back(v);
}

std::vector<int> Graph::topological_sort() const {
    std::vector<int> result;
    std::vector<bool> visited(adj_.size(), false);

    for (int i = 0; i < static_cast<int>(adj_.size()); ++i) {
        if (!visited[i]) {
            topo_dfs(i, visited, result);
        }
    }

    std::reverse(result.begin(), result.end());
    return result;
}

std::vector<double> Graph::dijkstra(int start) const {
    int n = adj_.size();
    std::vector<double> dist(n, std::numeric_limits<double>::infinity());
    dist[start] = 0;

    using P = std::pair<double, int>;
    std::priority_queue<P, std::vector<P>, std::greater<P>> pq;
    pq.push({0, start});

    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();

        if (d > dist[u]) continue;

        for (const auto& e : adj_[u]) {
            double new_dist = dist[u] + e.weight;
            if (new_dist < dist[e.to]) {
                dist[e.to] = new_dist;
                pq.push({new_dist, e.to});
            }
        }
    }

    return dist;
}

std::vector<double> Graph::bellman_ford(int start) const {
    int n = adj_.size();
    std::vector<double> dist(n, std::numeric_limits<double>::infinity());
    dist[start] = 0;

    for (int i = 0; i < n - 1; ++i) {
        for (int u = 0; u < n; ++u) {
            for (const auto& e : adj_[u]) {
                if (dist[u] + e.weight < dist[e.to]) {
                    dist[e.to] = dist[u] + e.weight;
                }
            }
        }
    }

    return dist;
}

std::optional<std::vector<int>> Graph::shortest_path(int from, int to) const {
    int n = adj_.size();
    std::vector<double> dist(n, std::numeric_limits<double>::infinity());
    std::vector<int> prev(n, -1);
    dist[from] = 0;

    using P = std::pair<double, int>;
    std::priority_queue<P, std::vector<P>, std::greater<P>> pq;
    pq.push({0, from});

    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();

        if (u == to) break;
        if (d > dist[u]) continue;

        for (const auto& e : adj_[u]) {
            double new_dist = dist[u] + e.weight;
            if (new_dist < dist[e.to]) {
                dist[e.to] = new_dist;
                prev[e.to] = u;
                pq.push({new_dist, e.to});
            }
        }
    }

    if (dist[to] == std::numeric_limits<double>::infinity()) {
        return std::nullopt;
    }

    std::vector<int> path;
    for (int v = to; v != -1; v = prev[v]) {
        path.push_back(v);
    }
    std::reverse(path.begin(), path.end());
    return path;
}

bool Graph::path_exists(int from, int to) const {
    std::vector<bool> visited(adj_.size(), false);
    std::queue<int> q;
    q.push(from);
    visited[from] = true;

    while (!q.empty()) {
        int v = q.front();
        q.pop();
        if (v == to) return true;

        for (const auto& e : adj_[v]) {
            if (!visited[e.to]) {
                visited[e.to] = true;
                q.push(e.to);
            }
        }
    }
    return false;
}

bool Graph::is_connected() const {
    if (adj_.empty()) return true;
    auto reachable = bfs(0);
    return reachable.size() == adj_.size();
}

bool Graph::dfs_cycle(int v, std::vector<int>& color) const {
    color[v] = 1;  // Gray
    for (const auto& e : adj_[v]) {
        if (color[e.to] == 1) return true;  // Back edge
        if (color[e.to] == 0 && dfs_cycle(e.to, color)) return true;
    }
    color[v] = 2;  // Black
    return false;
}

bool Graph::has_cycle() const {
    std::vector<int> color(adj_.size(), 0);  // 0=white, 1=gray, 2=black
    for (int i = 0; i < static_cast<int>(adj_.size()); ++i) {
        if (color[i] == 0 && dfs_cycle(i, color)) {
            return true;
        }
    }
    return false;
}

std::vector<std::vector<int>> Graph::connected_components() const {
    std::vector<std::vector<int>> components;
    std::vector<bool> visited(adj_.size(), false);

    for (int i = 0; i < static_cast<int>(adj_.size()); ++i) {
        if (!visited[i]) {
            std::vector<int> component;
            dfs_helper(i, visited, component);
            components.push_back(component);
        }
    }

    return components;
}

// =============================================================================
// UnionFind
// =============================================================================

UnionFind::UnionFind(int n) : parent_(n), rank_(n, 0), components_(n) {
    for (int i = 0; i < n; ++i) {
        parent_[i] = i;
    }
}

int UnionFind::find(int x) {
    if (parent_[x] != x) {
        parent_[x] = find(parent_[x]);  // Path compression
    }
    return parent_[x];
}

bool UnionFind::unite(int x, int y) {
    int px = find(x);
    int py = find(y);

    if (px == py) return false;

    // Union by rank
    if (rank_[px] < rank_[py]) std::swap(px, py);
    parent_[py] = px;
    if (rank_[px] == rank_[py]) ++rank_[px];

    --components_;
    return true;
}

bool UnionFind::connected(int x, int y) {
    return find(x) == find(y);
}

int UnionFind::component_count() const {
    return components_;
}

int UnionFind::component_size(int x) {
    int root = find(x);
    int size = 0;
    for (int i = 0; i < static_cast<int>(parent_.size()); ++i) {
        if (find(i) == root) ++size;
    }
    return size;
}

}  // namespace temp2::algorithms
