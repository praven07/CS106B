// This is the CPP file you will edit and turn in.
// Also remove these comments here and add your own, along with
// comments on every function and on complex code sections.
// TODO: write comment header for this file; remove this comment

#include "PathfinderAlgorithms.h"
#include "queue.h"
#include "priorityqueue.h"
// TODO: include any other headers you need; remove this comment

using namespace std;

// The minimum difference to be a valid alternate path
const double SUFFICIENT_DIFFERENCE = 0.2;

bool depthFirstSearch(const BasicGraph &graph, Vertex* start, Vertex* end, Vector<Vertex*> &path) {

    start->setColor(GREEN);
    path.add(start);

    if (start->name == end->name) {
        return true;
    } else {

        for (Vertex* neighbor: graph.getNeighbors(start->name)) {

            if (neighbor->getColor() == UNCOLORED) {
                if (depthFirstSearch(graph, neighbor, end, path)) {
                    return true;
                }
            }
        }

        start->setColor(GRAY);
        path.removeBack();
    }

    return false;
}

Vector<Vertex*> depthFirstSearch(const BasicGraph& graph, Vertex* start, Vertex* end) {

    Vector<Vertex*> path;
    depthFirstSearch(graph, start, end, path);

    return path;
}


Vector<Vertex*> breadthFirstSearch(const BasicGraph& graph, Vertex* start, Vertex* end) {


    Queue<Vector<Vertex*>> queue;

    Vector<Vertex*> path;
    path.add(start);

    queue.enqueue(path);

    while (!queue.isEmpty()) {

        path = queue.dequeue();

        Vertex* current = path.back();
        current->setColor(GREEN);

        if (current == end) {
            break;
        }

        for (Vertex* neighbor: graph.getNeighbors(current->name)) {

            if (neighbor->getColor() == UNCOLORED) {

                neighbor->setColor(YELLOW);

                Vector<Vertex*> newPath = path;
                newPath.add(neighbor);

                queue.add(newPath);
            }
        }
    }

    return path;
}

void traversePath(Map<Vertex*, Vertex*> &edges, Vector<Vertex*> &path, Vertex* start, Vertex* end) {

    if (!edges.containsKey(end)) {
        return;
    }

    if (end != nullptr) {

        path.insert(0, end);

        traversePath(edges, path, start, edges[end]);
    }
}

Vector<Vertex*> dijkstrasAlgorithm(const BasicGraph& graph, Vertex* start, Vertex* end) {

    Map<Vertex*, Vertex*> previousEdge;
    Map<Vertex*, double> costs;
    PriorityQueue<Vertex*> queue;


    queue.enqueue(start, 0);
    costs.add(start, 0.0);
    previousEdge.add(start, nullptr);

    while(!queue.isEmpty()) {

        Vertex* current = queue.dequeue();
        current->setColor(GREEN);

        if (current == end) {
            break;
        }

        for (Vertex* neighbor: graph.getNeighbors(current->name)) {

            double cost =  graph.getEdge(current, neighbor)->cost + costs[current];

            if (neighbor->getColor() != GREEN) {

                if (neighbor->getColor() == YELLOW && cost < costs[neighbor]) {
                    queue.changePriority(neighbor, cost);
                    costs[neighbor] = cost;
                    previousEdge[neighbor] = current;
                }

                if (neighbor->getColor() != YELLOW){
                    neighbor->setColor(YELLOW);
                    queue.enqueue(neighbor, cost);
                    costs[neighbor] = cost;
                    previousEdge[neighbor] = current;
                }
            }
        }
    }

    Vector<Vertex*> path;
    traversePath(previousEdge, path, start, end);
    return path;
}


Vector<Vertex*> aStar(const BasicGraph& graph, Vertex* start, Vertex* end) {
    Map<Vertex*, Vertex*> previousEdge;
    Map<Vertex*, double> costs;
    PriorityQueue<Vertex*> queue;


    double initialCost = 0.0;
    queue.enqueue(start, initialCost);
    costs.add(start, initialCost);
    previousEdge.add(start, nullptr);

    while(!queue.isEmpty()) {

        Vertex* current = queue.dequeue();
        current->setColor(GREEN);

        if (current == end) {
            break;
        }

        for (Vertex* neighbor: graph.getNeighbors(current->name)) {

            if (neighbor->getColor() != GREEN) {

                Edge* edge = graph.getEdge(current, neighbor);

                double cost = costs[current] + edge->cost;

                if (neighbor->getColor() == YELLOW && cost < costs[neighbor]) {
                    queue.changePriority(neighbor, cost + heuristicFunction(neighbor, end));
                    costs[neighbor] = cost;
                    previousEdge[neighbor] = current;
                }

                if (neighbor->getColor() != YELLOW){
                    neighbor->setColor(YELLOW);
                    queue.enqueue(neighbor, cost + heuristicFunction(neighbor, end));
                    costs[neighbor] = cost;
                    previousEdge[neighbor] = current;
                }
            }
        }
    }

    Vector<Vertex*> path;
    traversePath(previousEdge, path, start, end);
    return path;
}


Vector<Vertex*> alternateAStar(const BasicGraph& graph, Vertex* start, Vertex* end, Edge* ignore) {

    Map<Vertex*, Vertex*> previousEdge;
    Map<Vertex*, double> costs;
    PriorityQueue<Vertex*> queue;


    double initialCost = heuristicFunction(start, end);
    queue.enqueue(start, initialCost);
    costs.add(start, initialCost);
    previousEdge.add(start, nullptr);

    while(!queue.isEmpty()) {

        Vertex* current = queue.dequeue();
        current->setColor(GREEN);

        if (current == end) {
            break;
        }

        for (Vertex* neighbor: graph.getNeighbors(current->name)) {

            Edge* edge = graph.getEdge(current, neighbor);

            if (neighbor->getColor() != GREEN && edge != ignore) {

                double cost = costs[current] + edge->cost;

                if (neighbor->getColor() == YELLOW && cost < costs[neighbor]) {
                    queue.changePriority(neighbor, cost + heuristicFunction(neighbor, end));
                    costs[neighbor] = cost;
                    previousEdge[neighbor] = current;
                }

                if (neighbor->getColor() != YELLOW){
                    neighbor->setColor(YELLOW);
                    queue.enqueue(neighbor, cost + + heuristicFunction(neighbor, end));
                    costs[neighbor] = cost;
                    previousEdge[neighbor] = current;
                }
            }
        }
    }


    Vector<Vertex*> path;
    traversePath(previousEdge, path, start, end);

    return path;
}


double difference(Vector<Vertex*> &alternate, Vector<Vertex*> &best) {

    Set<Vertex*> ap;
    Set<Vertex*> bp;


    for (Vertex* v: alternate) {
        ap.add(v);
    }

    for (Vertex* v: best) {
        bp.add(v);
    }

    return (double)(ap - bp).size() / bp.size();
}


double calculatePathCost(const BasicGraph &graph, Vector<Vertex*> &path) {

    double cost = 0.0;

    for (int i = 1; i < path.size(); i++) {
        cost += graph.getEdge(path[i-1], path[i])->cost;
    }

    return cost;
}


Vector<Vertex*> alternatePath(const BasicGraph& graph, Vertex* start, Vertex* end) {

    Vector<Vertex*> bestPath = aStar(graph, start, end);

    Vector<Vertex*> alternatePath;
    double minCost = -1;

    for (int i = 1; i < bestPath.size(); i++) {

        for (Vertex* v: graph.getVertexSet()) {
            v->resetData();
        }

        Vertex* curr = bestPath[i-1];
        Vertex* next = bestPath[i];

        Edge* ignore = graph.getEdge(curr, next);

        Vector<Vertex*> alternateCandidate = alternateAStar(graph, start, end, ignore);

        double pathCost = calculatePathCost(graph, alternateCandidate);
        double diff = difference(alternateCandidate, bestPath);

        if (diff > SUFFICIENT_DIFFERENCE && (minCost == -1 || pathCost < minCost)) {

            alternatePath = alternateCandidate;
            minCost = pathCost;
        }
    }

    return alternatePath;
}
