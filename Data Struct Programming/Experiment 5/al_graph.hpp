//
// Created by xianfei on 2019/11/18.
//

#ifndef GRAPH_AL_GRAPH_HPP
#define GRAPH_AL_GRAPH_HPP

#include <stdexcept>
#include <ostream>
#include <tuple>
#include <vector>
#include "queue.hpp"
#include "stack.hpp"

template<typename VertexType>
class AdjacencyListGraph {
    struct ArcNode {
        int adjVex;/*邻接点域*/
        int weight;/*表示边上信息的域info*/
        ArcNode *next = nullptr;/*指向下一个邻接点的指针域*/
        ArcNode(int adjVex, int weight) : adjVex(adjVex), weight(weight) {}
    };

    bool _isDirected = false;
    int _vexNum = 0, _arcNum = 0;

    struct _vNode {
        VertexType v;
        ArcNode *next = nullptr;
    } *_vexList = nullptr;

    Stack<int> spanningTreeStack;

private:  // dfs 的递归调用函数
    template<typename Function>
    void _dfs(int n, bool *visited, Function visit) {
        if(!spanningTreeStack.empty())
            std::cout << _vexList[spanningTreeStack.top()].v << '-' << _vexList[n].v << std::endl;
        spanningTreeStack.push(n);
        visited[n] = true;
        visit(_vexList[n].v);
        auto ptr = _vexList[n].next;
        if (ptr) {
            while (ptr->next != nullptr) {
                if (!visited[ptr->adjVex])_dfs(ptr->adjVex, visited, visit);
                ptr = ptr->next;
            }
            if (!visited[ptr->adjVex])_dfs(ptr->adjVex, visited, visit);
        }
        spanningTreeStack.pop();
    }

public:  // 返回vector的深度优先搜素算法

    std::vector<VertexType> dfs(){ return dfs(_vexList[0].v);}

    std::vector<VertexType> dfs(VertexType startVex) { // 指定开始节点
        int start=locateVex(startVex);
        bool *visited = new bool[_vexNum];
        std::vector<VertexType> result;
        for (int i = 0; i < _vexNum; ++i)visited[i] = false;
        for (int i = 0; i < _vexNum; ++i)
            if (!visited[(i+start)%_vexNum])
                _dfs((i+start)%_vexNum, visited, [&result](VertexType v) -> void { result.push_back(v); });
        delete[] visited;
        return result;
    }
    std::vector<VertexType> dfs_noRes(){ return dfs_noRes(_vexList[0].v);}

    std::vector<VertexType> dfs_noRes(VertexType startVex) {
        bool *visited = new bool[_vexNum];
        std::vector<VertexType> result;
        int start=locateVex(startVex);
        Stack<int> needVisited;
        for (int i = 0; i < _vexNum; ++i)visited[i] = false;
        for (int i = 0; i < _vexNum; ++i){
            int nextVisit = (i+start)%_vexNum;
            if (visited[nextVisit])continue;
            do {
                do {
                    if (visited[nextVisit])break;
                    visited[nextVisit] = true;
                    result.push_back(_vexList[nextVisit].v);
                    auto ptr = _vexList[nextVisit].next;
                    if (ptr) {
                        while (visited[ptr->adjVex]) {
                            ptr = ptr->next;
                            if (!ptr->next)break;
                        }
                        nextVisit = ptr->adjVex;
                        ptr = ptr->next;
                    } else break;
                    if (ptr) {
                        while (ptr->next != nullptr) {
                            if (!visited[ptr->adjVex])needVisited.push(ptr->adjVex);
                            ptr = ptr->next;
                        }
                        if (!visited[ptr->adjVex])needVisited.push(ptr->adjVex);
                    }
                } while (true);
                if(needVisited.empty())break;
                else nextVisit=needVisited.pop();
            }while(true);
        }
        delete[] visited;
        return result;
    }

    std::vector<VertexType> bfs(){ return bfs(_vexList[0].v);}

    std::vector<VertexType> bfs(VertexType startVex) {
        Stack<int> spanningTreeStack;
        int start=locateVex(startVex);
        std::vector<VertexType> result;
        bool *visited = new bool[_vexNum];
        for (int i = 0; i < _vexNum; ++i)visited[i] = false;
        Queue<int> queue;
        for (int i = 0; i < _vexNum; ++i) {
            if (!visited[(i+start)%_vexNum]) {
                queue.push((i+start)%_vexNum);
                result.push_back(_vexList[(i+start)%_vexNum].v);
                visited[(i+start)%_vexNum] = true;
            } else continue;
            while (!queue.empty()) {
                spanningTreeStack.push(queue.front());
                auto ptr = _vexList[queue.pop()].next;
                if (ptr) {
                    while (ptr->next != nullptr) {
                        if (!visited[ptr->adjVex]) {
                            visited[ptr->adjVex] = true;
                            queue.push(ptr->adjVex);
                            result.push_back(_vexList[ptr->adjVex].v);
                            if(!spanningTreeStack.empty())
                                std::cout << _vexList[spanningTreeStack.top()].v << '-' << _vexList[ptr->adjVex].v << std::endl;

                        }
                        ptr = ptr->next;
                    }
                    if (!visited[ptr->adjVex]) {
                        visited[ptr->adjVex] = true;
                        queue.push(ptr->adjVex);
                        result.push_back(_vexList[ptr->adjVex].v);
                        if(!spanningTreeStack.empty())
                            std::cout << _vexList[spanningTreeStack.top()].v << '-' << _vexList[ptr->adjVex].v << std::endl;

                    }
                }
                spanningTreeStack.pop();
            }
        }

        delete[] visited;
        return result;
    }

    AdjacencyListGraph(int vexNum, int arcNum, bool isDirected) : _vexNum(vexNum), _arcNum(arcNum),
                                                                  _isDirected(isDirected) {
        _vexList = new _vNode[vexNum];
    }

    virtual ~AdjacencyListGraph() {
        delete[] _vexList;
    }

    int locateVex(VertexType target) {
        for (int i = 0; i < _vexNum; i++) {
            if (_vexList[i].v == target)return i;
        }
        return -1;
    }

    void setVexes(std::vector<VertexType> list) {
        int i = -1;
        for (const auto &item : list) {
            if (i + 1 == _vexNum)throw std::out_of_range("");
            _vexList[++i].v = item;
        }
    }

    void setArcs(std::vector<std::tuple<VertexType, VertexType, int>> list) {
        for (const auto &[v1, v2, weight] : list) {
            int i = locateVex(v1), j = locateVex(v2);
            auto ptr = _vexList[i].next;
            if (ptr) {
                while (ptr->next != nullptr)ptr = ptr->next;
                ptr->next = new ArcNode(j, weight);
            } else _vexList[i].next = new ArcNode(j, weight);
            if (!_isDirected) {
                ptr = _vexList[j].next;
                if (ptr) {
                    while (ptr->next != nullptr)ptr = ptr->next;
                    ptr->next = new ArcNode(i, weight);
                } else _vexList[j].next = new ArcNode(i, weight);
            }
        }
    }

    friend std::ostream &operator<<(std::ostream &os, const AdjacencyListGraph &graph) {
        for (int i = 0; i < graph._vexNum; i++) {
            os << i << '\t' <<graph._vexList[i].v << "\t->\t";
            auto ptr = graph._vexList[i].next;
            if (ptr) {
                while (ptr->next != nullptr) {
                    os << ptr->adjVex << '\t' << ptr->weight << "\t->\t";
                    ptr = ptr->next;
                }
                os << ptr->adjVex << '\t' << ptr->weight << "\t->\t^\t";
            } else os << "^\t";
            os << std::endl;
        }
        return os;
    }

};

#endif //GRAPH_AL_GRAPH_HPP
