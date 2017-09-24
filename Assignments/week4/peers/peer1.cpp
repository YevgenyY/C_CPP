//
//  main.cpp
//  HomeWork3
//
//  Copyright © 2017 Ludvik Drobnič. All rights reserved.
//

#include <iomanip>
#include <iostream>

#include <algorithm>
#include <cassert>
#include <fstream>
#include <iterator>
#include <limits>
#include <random>
#include <set>
#include <sstream>
#include <string>
#include <vector>

// -----------------------------------------------------------------------------
// Google C++ style madness: no "using namespace std"
// -----------------------------------------------------------------------------
using std::cout;
using std::setw;
using std::endl;
using std::setfill;

// -----------------------------------------------------------------------------
// Forward declaration of node Node & Edge are mutually dependent
// -----------------------------------------------------------------------------
class Node;

// =============================================================================
// Class Edge, defines an edge between this node two other node
// =============================================================================
template <class FwNode = Node>
class FwEdge {
 public:
  FwEdge() = delete;
  FwEdge(const FwNode& from, const FwNode& to, unsigned weight)
      : weight(weight), from(from), to(to){};
  const FwNode& get_from() const { return this->from; };
  const FwNode& get_to() const { return this->to; };
  unsigned get_weight() const { return this->weight; };
  // ---------------------------------------------------------------------------
  // Define an order inside the Edge class, first by weight, and after by dest
  // ---------------------------------------------------------------------------
  bool operator<(const FwEdge& b) const {
    if (this->weight == b.weight)
      return (this->to.get_nodeid() < b.to.get_nodeid());
    return (this->weight < b.weight);
  }

 private:
  const FwNode& from;
  const FwNode& to;
  unsigned weight;
};

// =============================================================================
// Class Node, defines a node
// =============================================================================
typedef FwEdge<> Edge;  // Only to not to carry the brackets everywhere
class Node {
 public:
  Node(int t) : vertex(t){};
  Node(const Node& n);
  void add_neighbor(const Node& to, unsigned weight);
  int get_nodeid() const { return this->vertex; };
  bool is_adjacent(const Node& other) const;
  unsigned num_neighbors() const { return this->neighborhood.size(); };
  void print_neighbors();
  const Node& closest_neighbor() const;
  std::set<Edge> get_neighborhood();
  // ---------------------------------------------------------------------------
  // Required to sort the nodes in ascendig order
  // ---------------------------------------------------------------------------
  bool operator<(const Node& other) const {
    return this->vertex < other.vertex;
  };
  bool operator==(const Node& other) const {
    return this->vertex == other.vertex;
  };

 private:
  int vertex;
  std::set<Edge> neighborhood;
};

// -----------------------------------------------------------------------------
// Copy constructor
// -----------------------------------------------------------------------------
Node::Node(const Node& n) {
  this->vertex = n.vertex;
  this->neighborhood = n.neighborhood;
}

// -----------------------------------------------------------------------------
// Member function to add an edge from node to node
// -----------------------------------------------------------------------------
void Node::add_neighbor(const Node& destination, unsigned weight) {
  Edge edge(*this, destination, weight);
  this->neighborhood.insert(edge);
}

// -----------------------------------------------------------------------------
// Comprobar si otro nodo es adyacente a éste
// -----------------------------------------------------------------------------
bool Node::is_adjacent(const Node& candidate) const {
  for (auto n = this->neighborhood.begin(); n != this->neighborhood.end();
       ++n) {
    if ((*n).get_to().get_nodeid() == candidate.get_nodeid()) return true;
  }
  return false;
}

// -----------------------------------------------------------------------------
// Member function to determine which node is closest to this one.
// -----------------------------------------------------------------------------
const Node& Node::closest_neighbor() const {
  // The whole neighborhood is sorted in ascending order by the distance from
  // it to the current node. Therefore, the first node in the set is the node
  // closest to this.
  assert(neighborhood.size() > 0);
  return (*(neighborhood.begin())).get_to();
}

// -----------------------------------------------------------------------------
// Member function to print neighboring nodes to this one
// -----------------------------------------------------------------------------
void Node::print_neighbors() {
  cout << "[" << setw(2) << setfill('0') << this->get_nodeid() << "] "
       << setw(2) << this->neighborhood.size() << " -> ";

  for (auto it = this->neighborhood.begin(); it != this->neighborhood.end();
       ++it) {
    Edge e = *it;
    cout << "(" << setw(2) << setfill('0') << e.get_to().get_nodeid() << ") "
         << setw(2) << e.get_weight() << " | ";
  }
  cout << endl;
}

// -----------------------------------------------------------------------------
// Member function to retrieve all the neighbors of the node
// -----------------------------------------------------------------------------
std::set<Edge> Node::get_neighborhood() { return this->neighborhood; }

// =============================================================================
// Auxiliary class PriorityQueue: Provides the member functions necessary to
// maintain ordered the edges between nodes, so that those with a lower weight
// occupy the first positions.
// =============================================================================
class PriorityQueue {
 public:
  void add_edges_from_node(const Node& node, const std::vector<Node>& closed);
  void clean_nodes(const std::vector<Node>& nodes);
  const Edge& get_minimum_weight();

 private:
  std::set<Edge> pq;
};

// -----------------------------------------------------------------------------
// Member function to include all the edges that start from the node received as
// argument and link to other nodes of the graph.
// -----------------------------------------------------------------------------
void PriorityQueue::add_edges_from_node(const Node& node,
                                        const std::vector<Node>& closed) {
  std::set<Edge> edges = const_cast<Node&>(node).get_neighborhood();
  for (auto e = edges.begin(); e != edges.end(); ++e) {
    this->pq.emplace(*e);
  }
  this->clean_nodes(closed);
}

// -----------------------------------------------------------------------------
// Member function to clear all edges that link to nodes that are already in the
// closed set.
// -----------------------------------------------------------------------------
void PriorityQueue::clean_nodes(const std::vector<Node>& nodes) {
  for (auto node = nodes.begin(); node != nodes.end(); ++node) {
    int n = (*node).get_nodeid();
    for (auto e = pq.begin(); e != pq.end(); ++e) {
      if ((*e).get_to().get_nodeid() == n) {
        e = pq.erase(e);
      }
    }
  }
}

// -----------------------------------------------------------------------------
// Member function to retrieve the edge with the minimum distance.
// -----------------------------------------------------------------------------
const Edge& PriorityQueue::get_minimum_weight() { return *(pq.begin()); }

// =============================================================================
// Class Graph
// =============================================================================
class Graph {
 public:
  Graph() : vertices_(0), size_(50), connected_(false){};

  // Constructor with three parameters:
  //   - A size (number of node)
  //   - A density of the graph
  //   - A maximum distance between any two nodes
  Graph(unsigned size, double density, unsigned max_distance);

  // Copy constructor (bad code this is the default behavior)
  Graph(Graph& g) : size_(g.size_), vertices_(g.vertices_), nodes(g.nodes){};

  // Constructor from text file
  Graph(const std::string file_name);

  virtual ~Graph(){};

  bool is_adjacent(const Node& from, const Node& to) const;
  void add_edge(int from, int to, unsigned weight);
  void print_nodes();
  bool is_connected() const { return this->connected_; };

  // Print MST using Jarník - Prim's algorithm
  void jarnik_prim_MST();

 private:
  // Maximum number of vertices.
  unsigned size_;

  // Current number of vertices.
  unsigned vertices_;

  // The current graph is connected
  bool connected_;

  // Vector that holds existing nodes
  std::vector<Node> nodes;

  double probability();

  Node& add_node(int key);

  std::random_device generator;
};

// ----------------------------------------------------------------------------
// Constructor with three parameters:
//   - A size (number of node), by default 50
//   - A density of the graph, by default 0.20
//   - A maximum distance between node, by default 10
// ----------------------------------------------------------------------------
Graph::Graph(unsigned size, double density, unsigned max_distance)
    : size_(size) {
  // Random number distribution that produces integer values between 1 and
  // max_distance according to a uniform discrete distribution
  std::uniform_int_distribution<int> distribution(1, max_distance);

  nodes.reserve(size);
  for (unsigned i = 0; i < size; i++) {
    for (unsigned j = 0; j < i; j++) {
      if (probability() < density) {
        add_edge(i, j, distribution(this->generator));
      }
    }
  }

  // Sort node using default comparison (operator <) between begin() and end(),
  // including the element pointed by begin() but not the element pointed by
  // end().
  std::sort(nodes.begin(), nodes.end());
}

// ----------------------------------------------------------------------------
// Constructor from text file
// ----------------------------------------------------------------------------
Graph::Graph(const std::string file_name) {
  assert(file_name != "");

  std::ifstream ifs(file_name);
  unsigned from, to, distance;
  if (!ifs) {
    throw std::runtime_error("Error opening file " + file_name);
  }

  // The file format will be an initial integer that is the node size of the
  // graph and the further values will be integer triples: (i, j, cost).
  std::string first_line;
  std::getline(ifs,first_line);
  if (ifs.good()) {
    std::istringstream iss(first_line);
    iss >> size_;
    // Reserve enough space to hold the whole vector
    nodes.reserve(size_);
    // Parse the rest of lines
    std::string line;
    while (std::getline(ifs, line)) {
      std::istringstream iss(line);
      iss >> from >> to >> distance;
      add_edge(from, to, distance);
    }
  }

  size_ = nodes.size();
  connected_ = true;
  for (auto n = nodes.begin(); n != nodes.end(); ++n) {
    if ((*n).num_neighbors() == 0) {
      connected_ = false;
    }
    cout << "[" << setw(2) << setfill('0') << (*n).get_nodeid() << "]" << endl;
    (*n).print_neighbors();
  }

  // Sort node using default comparison (operator <) between begin() and end(),
  // including the element pointed by begin() but not the element pointed by
  // end().
  std::sort(nodes.begin(), nodes.end());
}

// -----------------------------------------------------------------------------
// Add a node -> check if the identifier exists, if it does not exist, insert it
// and return a reference to the node found or created.
// -----------------------------------------------------------------------------
Node& Graph::add_node(int key) {
  // Check if a node with this same key already exists
  for (auto it = nodes.begin(); it != nodes.end(); ++it) {
    if ((*it).get_nodeid() == key) {
      return *it;
    }
  }

  // Include the node in the set of nodes of the graph
  nodes.push_back(Node(key));
  return const_cast<Node&>(nodes.back());
}

// -----------------------------------------------------------------------------
// Check if there is a direct path between both nodes
// -----------------------------------------------------------------------------
bool Graph::is_adjacent(const Node& from, const Node& to) const {
  // Check if "from" node belongs to the graph
  for (auto it = nodes.begin(); it != nodes.end(); ++it) {
    // If it exists, checking for a direct path is trivial
    if ((*it) == from) return (*it).is_adjacent(to);
  }
  return false;
}

// -----------------------------------------------------------------------------
// Add a new path to the graph.
// -----------------------------------------------------------------------------
void Graph::add_edge(int from, int to, unsigned weight) {
  // Add both nodes to the graph if they are not yet added.
  Node& orig = this->add_node(from);
  Node& dest = this->add_node(to);

  // If there is a path between both nodes, insert each one in the neighbor
  // list of the other.
  if (!is_adjacent(orig, dest)) {
    orig.add_neighbor(dest, weight);
    dest.add_neighbor(orig, weight);
    vertices_++;
  }
}

// -----------------------------------------------------------------------------
// Member function to format and print all nodes in the graph
// -----------------------------------------------------------------------------
void Graph::print_nodes() {
  for (auto it = nodes.begin(); it != nodes.end(); ++it) {
    Node n = *it;
    n.print_neighbors();
  }
}

// -----------------------------------------------------------------------------
// Function to construct and print MST for a graph represented using adjacency
// matrix representation
// -----------------------------------------------------------------------------
void Graph::jarnik_prim_MST() {
  std::vector<Node> open;
  std::vector<Node> closed;
  PriorityQueue pq;
  unsigned distance = 0;

  auto it = this->nodes.begin();

  // Add start vertex to the close set
  closed.push_back(*it);
  pq.add_edges_from_node((*it), closed);

  // Add all other vertices to the open set
  for (++it; it != nodes.end(); ++it) open.push_back(*it);

  while (!open.empty()) {
    // Find the minimum distance from any node in the closed set to any node
    // in the open set
    auto e = pq.get_minimum_weight();
    auto n = e.get_to();
    distance += e.get_weight();
    for (auto x = open.begin(); x != open.end() && !open.empty(); ++x) {
      if (n == (*x)) {
        x = open.erase(x);
        break;
      }
    }
    closed.push_back(n);
    pq.add_edges_from_node(n, closed);
  }
  cout << "===========================================================" << endl;
  cout << "Distance : " << distance << endl;
  cout << "===========================================================" << endl;
  for (auto n = closed.begin(); n != closed.end(); ++n) {
    cout << "(" << setw(2) << setfill('0') << (*n).get_nodeid() << ")" << endl;
  }
}

// -----------------------------------------------------------------------------
// Function to produce a random probability that there is path between two nodes
// -----------------------------------------------------------------------------
double Graph::probability() {
  // Random number distribution that produces floating-point values according
  // to a uniform distribution
  std::uniform_real_distribution<double> distribution(0.0, 1.0);

  return distribution(this->generator);
}

int main(int argc, char** argv) {
  unsigned graph_size = 50;
  double density = 0.20;
  unsigned distance = 10;
  bool print = false;
  const std::string debugStr = "DEBUG";

  // ---------------------------------------------------------------------------
  // We provide a method to input different configuration parameters
  // for testing pourposes. There is no validation. The programa expects
  // the following input:
  //  - A string with the value "DEBUG" if you want verbose information.
  //  - An integer with the desired graph size
  //  - A double containing the density of the graph
  //  - An integer with the maximum distance.
  // ---------------------------------------------------------------------------
  switch (argc) {
    case 2:
      print = (std::string(argv[1]).compare(debugStr) == 0);
      break;

    case 3:
      print = (std::string(argv[1]).compare(debugStr) == 0);
      graph_size = static_cast<unsigned>(atoi(argv[2]));
      break;

    case 4:
      print = (std::string(argv[1]).compare(debugStr) == 0);
      graph_size = static_cast<unsigned>(atoi(argv[2]));
      density = atof((argv[3]));
      break;

    case 5:
      print = (std::string(argv[1]).compare(debugStr) == 0);
      graph_size = static_cast<unsigned>(atoi(argv[2]));
      density = atof((argv[3]));
      distance = static_cast<unsigned>(atoi(argv[4]));
      break;

    default:
      break;
  }

  // ---------------------------------------------------------------------------
  // Compute for a set of randomly generated graphs an average shortest path.
  // ---------------------------------------------------------------------------
  // Graph g(graph_size, density, distance);
  Graph g("test_data.txt");

  cout << "------------- C O N F I G U R A T I O N ------------------" << endl;
  cout << "Graph size : " << graph_size << endl;
  cout << "Density    : " << density << endl;
  cout << "Distance   : " << distance << endl;
  cout << "-----------------------------------------------------------" << endl;

  g.print_nodes();
  g.jarnik_prim_MST();
  return 0;
}
