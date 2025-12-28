#ifndef TEMP2_ALGORITHMS_GRAPH_HPP
#define TEMP2_ALGORITHMS_GRAPH_HPP

#include <vector>
#include <map>
#include <set>
#include <queue>
#include <optional>
#include <limits>

namespace temp2::algorithms {

/**
 * @brief Edge in a weighted graph
 */
struct Edge {
    int to;
    double weight;

    Edge(int to, double weight = 1.0) : to(to), weight(weight) {}
};

/**
 * @brief Graph represented as adjacency list
 */
class Graph {
public:
    Graph();
    explicit Graph(int num_vertices, bool directed = false);

    void add_vertex();
    void add_edge(int from, int to, double weight = 1.0);
    void remove_edge(int from, int to);
    bool has_edge(int from, int to) const;

    int vertex_count() const;
    int edge_count() const;
    bool is_directed() const;

    std::vector<int> neighbors(int vertex) const;
    std::vector<Edge> edges(int vertex) const;
    double edge_weight(int from, int to) const;

    // Traversals
    std::vector<int> bfs(int start) const;
    std::vector<int> dfs(int start) const;
    std::vector<int> topological_sort() const;

    // Shortest paths
    std::vector<double> dijkstra(int start) const;
    std::vector<double> bellman_ford(int start) const;
    std::vector<std::vector<double>> floyd_warshall() const;

    // Path finding
    std::optional<std::vector<int>> shortest_path(int from, int to) const;
    bool path_exists(int from, int to) const;

    // Properties
    bool is_connected() const;
    bool is_bipartite() const;
    bool has_cycle() const;
    std::vector<std::vector<int>> connected_components() const;
    std::vector<std::vector<int>> strongly_connected_components() const;

    // Minimum spanning tree
    std::vector<std::pair<int, int>> kruskal_mst() const;
    std::vector<std::pair<int, int>> prim_mst() const;

private:
    std::vector<std::vector<Edge>> adj_;
    bool directed_;
    int edge_count_;

    void dfs_helper(int v, std::vector<bool>& visited, std::vector<int>& result) const;
    bool dfs_cycle(int v, std::vector<int>& color) const;
    void topo_dfs(int v, std::vector<bool>& visited, std::vector<int>& result) const;
};

/**
 * @brief Union-Find (Disjoint Set Union) data structure
 */
class UnionFind {
public:
    explicit UnionFind(int n);

    int find(int x);
    bool unite(int x, int y);
    bool connected(int x, int y);
    int component_count() const;
    int component_size(int x);

private:
    std::vector<int> parent_;
    std::vector<int> rank_;
    int components_;
};

}  // namespace temp2::algorithms

#endif  // TEMP2_ALGORITHMS_GRAPH_HPP
