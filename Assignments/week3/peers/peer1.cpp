/* 
 * Solution for Week 3
 * The underlying Graph class uses connectivity matrix for two reasons:
 * a) it performs well on a large graph
 * b) real world graphs are large graphs
 */

#include <iostream>
#include <random>
#include <vector>
#include <set>
#include <map>

using namespace std;

class Graph {
    public:
        /*
         * In case if client code wants to fill graph manually,
         * then the size only should be provided (or default accepted).
         * Otherwise client code has to provide density and costs range
         */
        Graph(int size = 50, bool fillRandomly = false, double density = 0.2, const vector<double> &costRange = {0, 10.0}): size(size) {
            this->connectivityMatrix = vector<vector<bool>>(size);
            for (int i = 0; i < size; i++) {
                this->connectivityMatrix[i] = vector<bool>(size, false);
            }
            if (fillRandomly) {
                this->fillRandomly(density, costRange);
            }
        }
        /*
         * Shows how many vertices this graph is holding
         */
        int getVerticiesCount() const {
            return this->size;
        }
        /*
         * Shows whether two vertices belongs to the same edge
         */
        bool isAdjacent(int vertexA, int vertexB) const {
            return this->connectivityMatrix[vertexA][vertexB];
        }
        /*
         * Finds all signling vertices for a given vertex.
         * Only direct siblings are returned
         */
        vector<int> getVertextNeighbors(int vertex) const {
            vector<int> neighbors;
            vector<bool> vertexInfo = this->connectivityMatrix[vertex];
            for (int i = 0; i < vertexInfo.size(); i++) {
                if (vertexInfo[i]) {
                    neighbors.push_back(i);
                }
            }
            return neighbors;
        }
        void addEdge(int vertexA, int vertexB, double edgeCost) {
            this->connectivityMatrix[vertexA][vertexB] = true;
            this->connectivityMatrix[vertexB][vertexA] = true;
            set<int> edge = {vertexA, vertexB};
            this->costMap[edge] = edgeCost;
        }
        void deleteEdge(int vertexA, int vertexB) {
            this->disjointVertexes(vertexA, vertexB);
            set<int> edge = {vertexA, vertexB};
            this->costMap.erase(edge);
        }
        /*
         * Fills graph according to given density and costs range
         */
        void fillRandomly(double density, const vector<double> &costRange) {
            // obtain a random number from hardware
            random_device generator;
            // seed the generator
            mt19937 engine(generator());
            // initialize the distributions
            uniform_real_distribution<> edgePrices(costRange[0], costRange[1]);
            uniform_real_distribution<> probabilities(0, 1);
            for (int i = 0; i < this->size; i++) {
                for (int j = 0; j < this->size; j++) {
                    /*
                     * two vertices should be disjointed in case if it
                     * is the same vertex (we don't want loops), or it
                     * the probability for that given edge is more, than
                     * provided density
                     */
                    double edgeProbability = probabilities(engine);
                    if (i == j) {
                        this->disjointVertexes(i, j);
                    } else if (edgeProbability < density) {
                        this->addEdge(i, j, edgePrices(engine));
                    }
                }
            }
        }
        double getEdgeCost(int vertextA, int vertexB) const {
            if (!this->isAdjacent(vertextA, vertexB)) {
                return 0;
            }
            set<int> edge = {vertextA, vertexB};
            return this->costMap.at(edge);
        }
        void setEdgeCost(int vertexA, int vertexB, double newCost) {
            if (!this->isAdjacent(vertexA, vertexB)) {
                return;
            }
            this->addEdge(vertexA, vertexB, newCost);
        }
        /*
         * Prints connectivity matrix and a list with
         * edges cost
         */
        void print() const {
            cout << "Connectivity matrix" << endl;
            for (const auto &row : this->connectivityMatrix) {
                for (const bool &isThereAnEdge : row) {
                    cout << isThereAnEdge << " ";
                }
                cout << endl;
            }
            cout << endl << "Edges costs" << endl;
            for (const auto &pair : this->costMap) {
                vector<int> vertices(pair.first.begin(), pair.first.end());
                cout << "Edge from " << vertices[0] << " to " << vertices[1] << " costs " << pair.second << endl;
            }
        }
    private:
        int size = 0;
        vector<vector<bool>> connectivityMatrix;
        map<set<int>, double> costMap;
        void disjointVertexes(int vertexA, int vertexB) {
            this->connectivityMatrix[vertexA][vertexB] = false;
        }
};

class ShortestPathAlgo {
    public:
        ShortestPathAlgo(const Graph &graph): graph(graph) {
        }
        /*
         * Calculates and returns an average value
         * for a set of shortest paths in a given graph
         */
        double getAverage() const {
            double total = 0;
            double paths = 0;
            int graphSize = this->graph.getVerticiesCount();
            for (int i = 0; i < graphSize - 1; i++) {
                for (int j = i + 1; j < graphSize; j++) {
                    double pathPrice = this->getPathCost(i, j);
                    if (pathPrice) {
                        total += pathPrice;
                        paths++;
                    }
                }
            }
            return total / paths;
        }
        /*
         * Returns the cost of the shortest path
         * between two vertices.
         * In case if there is no such path a zero
         * is going to be returned
         */
        double getPathCost(int vertexA, int vertexB) const {
            if (vertexA == vertexB) {
                return 0;
            }
            double pathCost = 0;
            double DOUBLE_MAX = numeric_limits<double>::max();
            int graphSize = this->graph.getVerticiesCount();
            vector<double> shortestPath(graphSize, DOUBLE_MAX);
            vector<bool> visitesVertices(graphSize, false);
            shortestPath[vertexA] = 0;
            for (int i = 0; i < graphSize - 1; i++) {
                double minDistanceVertex = this->findMinimumDistance(shortestPath, visitesVertices);
                visitesVertices[minDistanceVertex] = true;
                for (int j = 0; j < graphSize; j++) {
                    if (!visitesVertices[j] &&
                        this->graph.isAdjacent(minDistanceVertex, j) &&
                        shortestPath[minDistanceVertex] != DOUBLE_MAX &&
                        shortestPath[minDistanceVertex] + this->graph.getEdgeCost(minDistanceVertex, j) < shortestPath[j]
                    ) {
                        shortestPath[j] = shortestPath[minDistanceVertex] + this->graph.getEdgeCost(minDistanceVertex, j);
                    }
                }
            }
            for (int i = 0, j = shortestPath.size(); i < j; i++) {
                if (i == vertexB) {
                    pathCost = shortestPath[i];
                    break;
                }
            }
            if (pathCost == DOUBLE_MAX) {
                pathCost = 0;
            }
            return pathCost;
        }
    private:
        /*
         * Helper method for finding an unvisited edge
         * with a minimum cost of traveling to it
         */
        double findMinimumDistance(const vector<double> &shortestPath, const vector<bool> &visitesVertices) const {
            double minDistance = numeric_limits<double>::max();
            int vertexOfMinCost = 0;
            for (int i = 0; i < this->graph.getVerticiesCount(); i++) {
                if (visitesVertices[i] == false && shortestPath[i] <= minDistance) {
                    minDistance = shortestPath[i];
                    vertexOfMinCost = i;
                }
            }
            return vertexOfMinCost;
        }
        Graph graph;
};

int main() {
    // 20% density graph with 50 nodes
    Graph graphA(50, true, 0.2);
    ShortestPathAlgo shortestPathAlgorithmForGraphA(graphA);
    cout << "An average path length for a graph with 20% density and 50 nodes is: " << shortestPathAlgorithmForGraphA.getAverage() << endl;

    // 40% density graph with 50 nodes
    Graph graphB(50, true, 0.4);
    ShortestPathAlgo shortestPathAlgorithmForGraphB(graphB);
    cout << "An average path length for a graph with 40% density and 50 nodes is: " << shortestPathAlgorithmForGraphB.getAverage() << endl;
}
