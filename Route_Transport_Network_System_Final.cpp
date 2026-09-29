#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <limits>
#include <algorithm>

using namespace std;

class TransportNetwork
{
private:
    int V;
    vector<string> location;
    vector<vector<pair<int, int>>> adj;

    void findAllRoutesUtil(int current, int destination,
                           vector<bool>& visited,
                           vector<int>& path,
                           int& routeCount)
    {
        visited[current] = true;
        path.push_back(current);

        if (current == destination)
        {
            routeCount++;
            cout << "Route " << routeCount << ": ";

            for (size_t i = 0; i < path.size(); i++)
            {
                cout << location[path[i]];
                if (i + 1 < path.size())
                    cout << " -> ";
            }
            cout << endl;
        }
        else
        {
            for (auto edge : adj[current])
            {
                int neighbour = edge.first;

                if (!visited[neighbour])
                {
                    findAllRoutesUtil(neighbour, destination,
                                      visited, path, routeCount);
                }
            }
        }

        path.pop_back();
        visited[current] = false;
    }

public:
    TransportNetwork(int vertices)
    {
        V = vertices;
        location.resize(V);
        adj.resize(V);
    }

    void setLocation(int index, const string& name)
    {
        location[index] = name;
    }

    void addRoute(int u, int v, int distance)
    {
        adj[u].push_back({v, distance});
        adj[v].push_back({u, distance});
    }

    bool validLocation(int x) const
    {
        return x >= 0 && x < V;
    }

    void displayLocations() const
    {
        cout << "\nAvailable Locations:\n";

        for (int i = 0; i < V; i++)
            cout << i << ". " << location[i] << endl;
    }

    void displayNetwork() const
    {
        cout << "\n========== TRANSPORT NETWORK ==========\n";

        for (int i = 0; i < V; i++)
        {
            cout << location[i] << " -> ";

            for (size_t j = 0; j < adj[i].size(); j++)
            {
                int neighbour = adj[i][j].first;
                int distance = adj[i][j].second;

                cout << location[neighbour]
                     << "(" << distance << " km)";

                if (j + 1 < adj[i].size())
                    cout << " -> ";
            }

            cout << endl;
        }
    }

    void DFSUtil(int current, vector<bool>& visited) const
    {
        visited[current] = true;
        cout << location[current] << " ";

        for (auto edge : adj[current])
        {
            int neighbour = edge.first;

            if (!visited[neighbour])
                DFSUtil(neighbour, visited);
        }
    }

    void DFS(int start) const
    {
        vector<bool> visited(V, false);

        cout << "\nDFS Traversal: ";
        DFSUtil(start, visited);
        cout << endl;
    }

    void BFS(int start) const
    {
        vector<bool> visited(V, false);
        queue<int> q;

        visited[start] = true;
        q.push(start);

        cout << "\nBFS Traversal: ";

        while (!q.empty())
        {
            int current = q.front();
            q.pop();

            cout << location[current] << " ";

            for (auto edge : adj[current])
            {
                int neighbour = edge.first;

                if (!visited[neighbour])
                {
                    visited[neighbour] = true;
                    q.push(neighbour);
                }
            }
        }

        cout << endl;
    }

    void findAllRoutes(int source, int destination) const
    {
        vector<bool> visited(V, false);
        vector<int> path;
        int routeCount = 0;

        cout << "\n========== AVAILABLE ROUTES ==========\n";
        cout << "From " << location[source]
             << " to " << location[destination] << ":\n";

        const_cast<TransportNetwork*>(this)
            ->findAllRoutesUtil(source, destination,
                                visited, path, routeCount);

        if (routeCount == 0)
        {
            cout << "No route exists between "
                 << location[source] << " and "
                 << location[destination] << ".\n";
        }
        else
        {
            cout << "Total available simple routes: "
                 << routeCount << endl;
        }
    }

    void BFSMinimumStops(int source, int destination) const
    {
        vector<bool> visited(V, false);
        vector<int> parent(V, -1);
        queue<int> q;

        visited[source] = true;
        q.push(source);

        while (!q.empty())
        {
            int current = q.front();
            q.pop();

            if (current == destination)
                break;

            for (auto edge : adj[current])
            {
                int neighbour = edge.first;

                if (!visited[neighbour])
                {
                    visited[neighbour] = true;
                    parent[neighbour] = current;
                    q.push(neighbour);
                }
            }
        }

        if (!visited[destination])
        {
            cout << "\nNo route exists between "
                 << location[source] << " and "
                 << location[destination] << ".\n";
            return;
        }

        vector<int> path;

        for (int current = destination;
             current != -1;
             current = parent[current])
            path.push_back(current);

        reverse(path.begin(), path.end());

        cout << "\n========== MINIMUM-STOP ROUTE ==========\n";
        cout << "Route: ";

        for (size_t i = 0; i < path.size(); i++)
        {
            cout << location[path[i]];

            if (i + 1 < path.size())
                cout << " -> ";
        }

        cout << "\nNumber of edges/stops: "
             << path.size() - 1 << endl;
    }

    void Dijkstra(int source, int destination) const
    {
        const int INF = numeric_limits<int>::max();

        vector<int> distance(V, INF);
        vector<int> parent(V, -1);

        priority_queue<
            pair<int, int>,
            vector<pair<int, int>>,
            greater<pair<int, int>>
        > pq;

        distance[source] = 0;
        pq.push({0, source});

        while (!pq.empty())
        {
            int currentDistance = pq.top().first;
            int current = pq.top().second;

            pq.pop();

            if (currentDistance != distance[current])
                continue;

            for (auto edge : adj[current])
            {
                int neighbour = edge.first;
                int weight = edge.second;

                if (distance[current] != INF &&
                    distance[current] + weight < distance[neighbour])
                {
                    distance[neighbour] =
                        distance[current] + weight;

                    parent[neighbour] = current;

                    pq.push({distance[neighbour], neighbour});
                }
            }
        }

        if (distance[destination] == INF)
        {
            cout << "\nNo route exists between "
                 << location[source] << " and "
                 << location[destination] << ".\n";
            return;
        }

        vector<int> path;

        for (int current = destination;
             current != -1;
             current = parent[current])
            path.push_back(current);

        reverse(path.begin(), path.end());

        cout << "\n========== SHORTEST ROUTE ==========\n";
        cout << "Route: ";

        for (size_t i = 0; i < path.size(); i++)
        {
            cout << location[path[i]];

            if (i + 1 < path.size())
                cout << " -> ";
        }

        cout << "\nMinimum Distance: "
             << distance[destination] << " km\n";
    }
};

int main()
{
    int V;

    cout << "============================================\n";
    cout << "     ROUTE AND TRANSPORT NETWORK SYSTEM\n";
    cout << "============================================\n";

    cout << "\nEnter number of locations: ";
    cin >> V;

    if (V <= 0)
    {
        cout << "Number of locations must be positive.\n";
        return 0;
    }

    TransportNetwork network(V);

    cout << "\nEnter names of the locations:\n";

    for (int i = 0; i < V; i++)
    {
        string name;

        cout << "Location " << i << ": ";
        cin >> name;

        network.setLocation(i, name);
    }

    int E;

    cout << "\nEnter number of routes: ";
    cin >> E;

    if (E < 0)
    {
        cout << "Number of routes cannot be negative.\n";
        return 0;
    }

    cout << "\nEnter route information:\n";
    cout << "Format: Source Destination Distance(km)\n\n";

    for (int i = 0; i < E; i++)
    {
        int u, v, distance;

        cout << "Route " << i + 1 << ": ";
        cin >> u >> v >> distance;

        if (!network.validLocation(u) ||
            !network.validLocation(v) ||
            distance < 0)
        {
            cout << "Invalid route! Enter valid location numbers "
                 << "and a non-negative distance.\n";
            i--;
            continue;
        }

        network.addRoute(u, v, distance);
    }

    int choice;

    do
    {
        cout << "\n\n============================================\n";
        cout << "              MAIN MENU\n";
        cout << "============================================\n";
        cout << "1. Display Transport Network\n";
        cout << "2. Find All Available Routes (DFS)\n";
        cout << "3. DFS Traversal\n";
        cout << "4. BFS Traversal\n";
        cout << "5. Find Route with Minimum Stops (BFS)\n";
        cout << "6. Find Shortest Distance using Dijkstra\n";
        cout << "7. Display Locations\n";
        cout << "8. Exit\n";

        cout << "\nEnter your choice: ";
        cin >> choice;

        int source, destination;

        switch (choice)
        {
        case 1:
            network.displayNetwork();
            break;

        case 2:
            network.displayLocations();

            cout << "\nEnter source location: ";
            cin >> source;

            cout << "Enter destination location: ";
            cin >> destination;

            if (network.validLocation(source) &&
                network.validLocation(destination))
                network.findAllRoutes(source, destination);
            else
                cout << "Invalid location!\n";

            break;

        case 3:
            network.displayLocations();

            cout << "\nEnter starting location: ";
            cin >> source;

            if (network.validLocation(source))
                network.DFS(source);
            else
                cout << "Invalid location!\n";

            break;

        case 4:
            network.displayLocations();

            cout << "\nEnter starting location: ";
            cin >> source;

            if (network.validLocation(source))
                network.BFS(source);
            else
                cout << "Invalid location!\n";

            break;

        case 5:
            network.displayLocations();

            cout << "\nEnter source location: ";
            cin >> source;

            cout << "Enter destination location: ";
            cin >> destination;

            if (network.validLocation(source) &&
                network.validLocation(destination))
                network.BFSMinimumStops(source, destination);
            else
                cout << "Invalid location!\n";

            break;

        case 6:
            network.displayLocations();

            cout << "\nEnter source location: ";
            cin >> source;

            cout << "Enter destination location: ";
            cin >> destination;

            if (network.validLocation(source) &&
                network.validLocation(destination))
                network.Dijkstra(source, destination);
            else
                cout << "Invalid location!\n";

            break;

        case 7:
            network.displayLocations();
            break;

        case 8:
            cout << "\nThank you for using the system!\n";
            break;

        default:
            cout << "\nInvalid choice! Please try again.\n";
        }

    } while (choice != 8);

    return 0;
}
