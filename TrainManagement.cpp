#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

using namespace std;

//membuat Class Node
class TrainNode
{
public:
    TrainNode(const string &cityName) : name(cityName) {}
    virtual ~TrainNode() {}

    string getName() const
    {
        return name;
    }

protected:
    string name;
};
// Graph
class TrainGraph
{
public:
    virtual ~TrainGraph() {}

    virtual void addNode(TrainNode *node) = 0;
    virtual void addEdge(int node1Index, int node2Index) = 0;
    virtual void display() const = 0;
};

//Adjency Matrix
class TrainGraphAdjencyMatrix : public TrainGraph
{
public:
    TrainGraphAdjencyMatrix(int size) : nodeCount(0), AdjencyMatrix(size, vector<int>(size, 0)) {}

    ~TrainGraphAdjencyMatrix() override
    {
        for (int i = 0; i < nodeCount; i++)
        {
            delete nodes[i];
        }
    }

    void addNode(TrainNode *node) override
    {
        nodes.push_back(node);
        nodeCount++;
    }

    void addEdge(int node1Index, int node2Index) override
    {
        AdjencyMatrix[node1Index][node2Index] = 1;
        AdjencyMatrix[node2Index][node1Index] = 1;
    }

    void display() const override
    {
        cout << "-------------------------------------------------------" << endl;
        cout << "Train Route Graph (Adjency Matrix):" << endl;

        for (int i = 0; i < nodeCount; i++)
        {
            cout << nodes[i]->getName() << " connected to: ";

            for (int j = 0; j < nodeCount; j++)
            {
                if (AdjencyMatrix[i][j] == 1)
                {
                    cout << nodes[j]->getName() << " - ";
                }
            }
            cout << endl;
        }
        cout << "-------------------------------------------------------" << endl;
    }

protected:
    int nodeCount;
    vector<TrainNode *> nodes;
    vector<vector<int>> AdjencyMatrix;
};

class TrainTransRoute
{
public:
    TrainTransRoute(const string &start, const string &end) : startLocation(start), endLocation(end) {}

private:
    string startLocation;
    string endLocation;
};

class TrainRoute : public TrainGraphAdjencyMatrix
{
public:
    TrainRoute(int size) : TrainGraphAdjencyMatrix(size) {}

    int findNodeIndex(const string &nodeName);
    void displayShortestRoute(const string &startLocation, const string &endLocation);
    void displayAllRoutes(const string &startLocation, const string &endLocation);
    void deleteRoute(int node1Index, int node2Index);
    void addRoute(int node1Index, int node2Index);
    void displayAdjencyMatrix() const;

private:
    void dfs(int currentNode, int targetNode, vector<int> &visited, vector<int> &path, bool &found);
    int minDistance(const vector<int> &distance, const vector<bool> &visited);
    void printShortestRoute(int start, int end, const vector<int> &parent);
    void dfsAllRoutes(int currentNode, int targetNode, vector<int> &visited, vector<int> &path);
    void printRouteHeader() const;
    void printDeletedRoute(int node1Index, int node2Index);
    void printAddedRoute(int node1Index, int node2Index);
};

void TrainRoute::displayAdjencyMatrix() const
{
    cout << "-------------------------------------------------------" << endl;
    cout << "Train Route Graph (Adjency Matrix):" << endl;

    for (int i = 0; i < nodeCount; i++)
    {
        cout << nodes[i]->getName() << " connected to: ";

        for (int j = 0; j < nodeCount; j++)
        {
            if (AdjencyMatrix[i][j] == 1)
            {
                cout << nodes[j]->getName() << " - ";
            }
        }
        cout << endl;
    }
    cout << "-------------------------------------------------------" << endl;
}

void TrainRoute::addRoute(int node1Index, int node2Index)
{
    if (node1Index >= 0 && node1Index < nodeCount && node2Index >= 0 && node2Index < nodeCount &&
        AdjencyMatrix[node1Index][node2Index] == 0 && AdjencyMatrix[node2Index][node1Index] == 0)
    {
        AdjencyMatrix[node1Index][node2Index] = 1;
        AdjencyMatrix[node2Index][node1Index] = 1;
        printAddedRoute(node1Index, node2Index);
    }
    else
    {
        cout << "Invalid route. Route already exists.\n";
    }
}

void TrainRoute::printAddedRoute(int node1Index, int node2Index)
{
    printRouteHeader();
    cout << "Route added: " << nodes[node1Index]->getName() << " - " << nodes[node2Index]->getName() << endl;
    printRouteHeader();
}

void TrainRoute::deleteRoute(int node1Index, int node2Index)
{
    if (node1Index >= 0 && node1Index < nodeCount && node2Index >= 0 && node2Index < nodeCount &&
        AdjencyMatrix[node1Index][node2Index] == 1 && AdjencyMatrix[node2Index][node1Index] == 1)
    {
        AdjencyMatrix[node1Index][node2Index] = 0;
        AdjencyMatrix[node2Index][node1Index] = 0;
        printDeletedRoute(node1Index, node2Index);
    }
    else
    {
        cout << "Invalid route. Route does not exist.\n";
    }
}

void TrainRoute::printDeletedRoute(int node1Index, int node2Index)
{
    printRouteHeader();
    cout << "Route deleted: " << nodes[node1Index]->getName() << " - " << nodes[node2Index]->getName() << endl;
    printRouteHeader();
}

int TrainRoute::findNodeIndex(const string &nodeName)
{
    string lowerNodeName = nodeName;
    transform(lowerNodeName.begin(), lowerNodeName.end(), lowerNodeName.begin(), ::tolower);

    for (int i = 0; i < nodeCount; ++i)
    {
        string currentNodeName = nodes[i]->getName();
        transform(currentNodeName.begin(), currentNodeName.end(), currentNodeName.begin(), ::tolower);

        if (currentNodeName == lowerNodeName)
        {
            return i;
        }
    }
    return -1;
}

void TrainRoute::printRouteHeader() const
{
    cout << "-------------------------------------------------------" << endl;
}

void TrainRoute::displayShortestRoute(const string &startLocation, const string &endLocation)
{
    int startIndex = findNodeIndex(startLocation);
    int endIndex = findNodeIndex(endLocation);

    if (startIndex != -1 && endIndex != -1)
    {
        vector<int> distance(nodeCount, INT_MAX);
        vector<int> parent(nodeCount, -1);
        vector<bool> visited(nodeCount, false);

        distance[startIndex] = 0;

        for (int count = 0; count < nodeCount - 1; ++count)
        {
            int u = minDistance(distance, visited);
            visited[u] = true;

            for (int v = 0; v < nodeCount; ++v)
            {
                if (!visited[v] && AdjencyMatrix[u][v] && distance[u] != INT_MAX &&
                    distance[u] + AdjencyMatrix[u][v] < distance[v])
                {
                    distance[v] = distance[u] + AdjencyMatrix[u][v];
                    parent[v] = u;
                }
            }
        }

        printShortestRoute(startIndex, endIndex, parent);

        TrainTransRoute route(startLocation, endLocation);
    }
    else
    {
        cout << "Invalid start or end location." << endl;
    }
}

int TrainRoute::minDistance(const vector<int> &distance, const vector<bool> &visited)
{
    int minDist = INT_MAX, minIndex = -1;

    for (int v = 0; v < nodeCount; ++v)
    {
        if (!visited[v] && distance[v] <= minDist)
        {
            minDist = distance[v];
            minIndex = v;
        }
    }

    return minIndex;
}

void TrainRoute::printShortestRoute(int start, int end, const vector<int> &parent)
{
    printRouteHeader();
    if (parent[end] == -1)
    {
        cout << "No route from " << nodes[start]->getName() << " to " << nodes[end]->getName() << endl;
        return;
    }

    vector<int> path;
    for (int at = end; at != -1; at = parent[at])
    {
        path.push_back(at);
    }

    cout << "Shortest Route from " << nodes[start]->getName() << " to " << nodes[end]->getName() << ": ";
    for (int i = path.size() - 1; i >= 0; --i)
    {
        cout << nodes[path[i]]->getName();
        if (i > 0)
        {
            cout << " - ";
        }
    }
    cout << endl;
    printRouteHeader();
}

void TrainRoute::displayAllRoutes(const string &startLocation, const string &endLocation)
{
    printRouteHeader();
    int startIndex = findNodeIndex(startLocation);
    int endIndex = findNodeIndex(endLocation);

    if (startIndex != -1 && endIndex != -1)
    {
        vector<int> visited(nodeCount, false);
        vector<int> path;

        cout << "All possible routes from " << startLocation << " to " << endLocation << ":\n";
        dfsAllRoutes(startIndex, endIndex, visited, path);
    }
    else
    {
        cout << "Invalid start or end location." << endl;
    }
    printRouteHeader();
}

void TrainRoute::dfsAllRoutes(int currentNode, int targetNode, vector<int> &visited, vector<int> &path)
{
    visited[currentNode] = true;
    path.push_back(currentNode);

    if (currentNode == targetNode)
    {
        cout << "Route: ";
        for (int i = 0; i < path.size(); ++i)
        {
            cout << nodes[path[i]]->getName();
            if (i < path.size() - 1)
            {
                cout << " - ";
            }
        }
        cout << endl;
    }
    else
    {
        for (int neighbor = 0; neighbor < nodeCount; ++neighbor)
        {
            if (!visited[neighbor] && AdjencyMatrix[currentNode][neighbor] == 1)
            {
                dfsAllRoutes(neighbor, targetNode, visited, path);
            }
        }
    }

    visited[currentNode] = false;
    path.pop_back();
}

int main()
{
    TrainRoute TrainRoute(20);
// menambahkan kota
    TrainRoute.addNode(new TrainNode("Jakarta"));      // 0
    TrainRoute.addNode(new TrainNode("Kediri"));       // 1
    TrainRoute.addNode(new TrainNode("Malang"));       // 2
    TrainRoute.addNode(new TrainNode("Surabaya"));     // 3
    TrainRoute.addNode(new TrainNode("Banyuwangi"));   // 4
    TrainRoute.addNode(new TrainNode("Bandung"));      // 5
    TrainRoute.addNode(new TrainNode("Semarang"));     // 6
    TrainRoute.addNode(new TrainNode("Kutoarjo"));     // 7
    TrainRoute.addNode(new TrainNode("Purwokerto"));   // 8
    TrainRoute.addNode(new TrainNode("Yogyakarta"));   // 9
    TrainRoute.addNode(new TrainNode("Solo"));         // 10
    TrainRoute.addNode(new TrainNode("Nganjuk"));      // 11
    TrainRoute.addNode(new TrainNode("Blitar"));       // 12

    TrainRoute.addEdge(0, 1);   // Jakarta - Kediri
    TrainRoute.addEdge(0, 3);   // Jakarta - Surabaya
    TrainRoute.addEdge(0, 4);   // Jakarta - Banyuwangi
    TrainRoute.addEdge(0, 5);   // Jakarta - Bandung
    TrainRoute.addEdge(0, 6);   // Jakarta - Semarang
    TrainRoute.addEdge(0, 7);   // Jakarta - Kutoarjo
    TrainRoute.addEdge(1, 2);   // Kediri - Malang
    TrainRoute.addEdge(1, 3);   // Kediri - Surabaya
    TrainRoute.addEdge(1, 4);   // Kediri - Banyuwangi
    TrainRoute.addEdge(3, 4);   // Surabaya - Banyuwangi
    TrainRoute.addEdge(6, 7);   // Semarang - Kutoarjo
    TrainRoute.addEdge(7, 8);   // Kutoarjo - Purwokerto
    TrainRoute.addEdge(7, 9);   // Kutoarjo - Yogyakarta
    TrainRoute.addEdge(7, 10);  // Kutoarjo - Solo
    TrainRoute.addEdge(8, 9);   // Purwokerto - Yogyakarta
    TrainRoute.addEdge(10, 11); // Solo - Nganjuk
    TrainRoute.addEdge(10, 12); // Solo - Blitar
    TrainRoute.addEdge(11, 12); // Nganjuk - Blitar
    TrainRoute.display();

    int choice;
    do
    {
        cout << "\nMenu:\n";
        cout << "1. Menunjukkan Rute Terpendek\n";
        cout << "2. Menunjukkan Semua Rute\n";
        cout << "3. Tambah Kota\n";
        cout << "4. Tambah Rute\n";
        cout << "5. Hapus Rute\n";
        cout << "6. Menunjukkan Adjency Matrix\n";
        cout << "0. Exit\n";
        cout << "Pilih Menu: ";
        cin >> choice;

        cin.ignore();

        switch (choice)
        {
        case 1:
        {
            string startLocation, endLocation;
            cout << "Pilih Keberangkatan: ";
            getline(cin, startLocation);

            cout << "Pilih Tujuan: ";
            getline(cin, endLocation);

            TrainRoute.displayShortestRoute(startLocation, endLocation);
            break;
        }
        case 2:
        {
            string startLocation, endLocation;
            cout << "Pilih Keberangkatan: ";
            getline(cin, startLocation);

            cout << "Pilih Tujuan: ";
            getline(cin, endLocation);

            TrainRoute.displayAllRoutes(startLocation, endLocation);
            break;
        }
        case 3:
        {
            string cityName;
            cout << "Masukkan Nama Kota: ";
            getline(cin, cityName);

            TrainRoute.addNode(new TrainNode(cityName));
            cout << "Kota Berhasil Ditambahkan.\n";
            break;
        }
        case 4:
        {
            string city1, city2;
            cout << "Tambahkan 2 Nama Kota yang akan ditambahkan Ke Rute: ";
            cin >> city1 >> city2;

            int node1 = TrainRoute.findNodeIndex(city1);
            int node2 = TrainRoute.findNodeIndex(city2);

            if (node1 != -1 && node2 != -1)
            {
                TrainRoute.addRoute(node1, node2);
            }
            else
            {
                cout << "Kota Tidak Ada. Masukkan kota yang tersedia.\n";
            }
            break;
        }
        case 5:
        {
            string city1, city2;
            cout << "Masukkan 2 Nama Kota yang rutenya akan dihapus: ";
            cin >> city1 >> city2;

            int node1 = TrainRoute.findNodeIndex(city1);
            int node2 = TrainRoute.findNodeIndex(city2);

            if (node1 != -1 && node2 != -1)
            {
                TrainRoute.deleteRoute(node1, node2);
            }
            else
            {
                cout << "Kota Tidak Tersedi. Masukkan Kota Yang Tersedia.\n";
            }
            break;
        }
        case 6:
        {
            TrainRoute.displayAdjencyMatrix();
            break;
        }
        case 0:
            cout << "Keluar...\n";
            break;
        default:
            cout << "Pilihan Tidak Ada. Pilih 0 - 6.\n";
        }
    } while (choice != 0);

    return 0;
}
