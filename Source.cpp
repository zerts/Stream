#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

const int INF = INT_MAX;

class Graph
{
    size_t numberOfVertex, numberOfEdges;
    std::vector< std::vector<int> > graph;
    void findExtraWay(std::vector< std::vector<int> > &currGraph, std::vector<int> &extraStream,
        std::vector<size_t> &previousVertex, size_t beginVertex, size_t endVertex, int inputStream)
    {
        std::vector<size_t> que;
        que.push_back(beginVertex);
        size_t begin = 0, end = 1;
        extraStream[beginVertex] = inputStream;
        while (begin < end)
        {
            for (size_t i = 0; i < numberOfVertex; i++)
            {
                if (currGraph[que[begin]][i] > 0 && !extraStream[i])
                {
                    previousVertex[i] = que[begin];
                    extraStream[i] = std::min(currGraph[que[begin]][i], extraStream[que[begin]]);
                    que.push_back(i);
                    end++;
                }
            }
            begin++;
        }
    }

    void eraseExtraWay(std::vector< std::vector<int> > &currGraph, std::vector<int> &extraStream,
        std::vector<size_t> &previousVertex, size_t beginVertex, size_t endVertex)
    {
        size_t currVertex = endVertex;
        int sizeOfExtraStream = extraStream[endVertex];
        while (currVertex != beginVertex)
        {
            currGraph[previousVertex[currVertex]][currVertex] -= sizeOfExtraStream;
            currVertex = previousVertex[currVertex];
        }
    }

    void addTheWay(std::vector< std::vector<int> > &currGraph, size_t beginVertex, size_t endVertex)
    {
        std::vector<int> extraStream(numberOfVertex, 0);
        std::vector<size_t> previousVertex(numberOfVertex, -1);
        previousVertex[beginVertex] = -1;
        findExtraWay(currGraph, extraStream, previousVertex, beginVertex, endVertex, INF);
        eraseExtraWay(currGraph, extraStream, previousVertex, beginVertex, endVertex);
    }

public:
    Graph(size_t v, size_t e)
    {
        numberOfVertex = v;
        numberOfEdges = e;
        graph.resize(v);
        for (size_t i = 0; i < v; i++)
            graph[i].assign(v, -1);
    }

    size_t getNumberOfVertex()
    {
        return numberOfVertex;
    }

    size_t getNumberOfEdges()
    {
        return  numberOfEdges;
    }

    bool isEdgeBetween(size_t a, size_t b)
    {
        return graph[a][b] != -1;
    }

    int getEdge(size_t a, size_t b)
    {
        return graph[a][b];
    }

    void in()
    {
        for (size_t i = 0; i < numberOfEdges; i++)
        {
            size_t a, b;
            int c;
            std::cin >> a >> b >> c;
            a--;
            b--;
            graph[a][b] = c;
        }
    }

    void dfs(std::vector< std::vector<int> > &extraGraph, std::vector <int> &flags, size_t currVertex, int flag)
    {
        if (flags.size() < numberOfVertex)
            flags.assign(numberOfVertex, 0);
        flags[currVertex] = flag;
        for (size_t i = 0; i < numberOfVertex; i++)
            if (extraGraph[currVertex][i] > 0 && !flags[i])
                dfs(extraGraph, flags, i, flag);
    }

    bool isCoherent(std::vector< std::vector<int> > &extraGraph, size_t beginVertex, size_t endVertex)
    {
        std::vector <int> flags(numberOfVertex, 0);
        dfs(extraGraph, flags, beginVertex, 1);
        return flags[endVertex] != 0;
    }

    int findMaxStream()
    {
        std::vector< std::vector<int> > extraGraph(numberOfVertex);
        for (size_t i = 0; i < numberOfVertex; i++)
        {
            extraGraph[i].resize(numberOfVertex);
            for (size_t j = 0; j < numberOfVertex; j++)
                extraGraph[i][j] = graph[i][j];
        }

        while (isCoherent(extraGraph, 0, numberOfVertex - 1))
            addTheWay(extraGraph, 0, numberOfVertex - 1);

        int bestStream = 0;
        for (size_t i = 0; i < numberOfVertex; i++)
        {
            if (extraGraph[0][i] >= 0)
                bestStream += graph[0][i] - extraGraph[0][i];
        }
        return bestStream;
    }
};

int main()
{
    size_t v, e;
    std::cin >> v >> e;
    Graph G(v, e);
    G.in();
    std::cout << G.findMaxStream();
    return 0;
}

/*
6 7
1 2 2
1 3 5
2 4 1
3 4 3
3 5 3
5 6 2
4 6 4
*/

/*
4 5
1 2 2
1 3 3
3 2 1
2 4 3
3 4 2
*/