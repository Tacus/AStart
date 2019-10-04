#include <iostream>
#include <vector>
#include <cstdlib>
#include <math.h>
#include <map>
#include <queue>
#include <chrono>
using namespace std;
#define  random(t) rand()%t
#define leftweight 1
#define rightweight 1
#define upweight 1
#define downweight 1
//假设地图为坡形，则前后左右可能weight不同

struct Point {
    Point() {}

    Point(int x, int y, float g, float f, const Point *father) {
        this->x = x;
        this->y = y;
        this->g = g;
        this->f = f;
        this->father = father;
    }

    void update(float g, float f, const Point *father) {
        this->g = g;
        this->f = f;
        this->father = father;
    }

    int x, y;
    float f, g;
    const Point *father;

    bool operator==(const Point b) const {
        return this->x == b.x && this->y == b.y;
    }

    bool operator!=(const Point b) const {
        return this->x != b.x || this->y != b.y;
    }

    bool operator<=(const Point b) const {
        return this->f <= b.f;
    }

    bool operator<(const Point b) const {
        return this->f < b.f;
    }

    bool operator>=(const Point b) const {
        return this->f >= b.f;
    }

    bool operator>(const Point b) const {
        return this->f > b.f;
    }
};

template<class T>
class MinHeap {
private:
    std::vector<T> m_container;
public:
    void buildHeap(std::vector<T> &v);

    void swap(T *l, T *r) {
        T temp = *l;
        *l = *r;
        *r = temp;
    }

    void insert(T value) {
        m_container.push_back(value);
        int len = m_container.size();
        sortup(len / 2 - 1, len - 1);
    }

    bool isEmpty() {
        return 0 == m_container.size();
    }

    T extract() {
        T value = m_container.front();
        int len = m_container.size();
        m_container[0] = m_container[len - 1];
        m_container.pop_back();
        sortdown(0, len - 2);
        return value;
    }

    void resort() {
        int len = m_container.size();
        for (int i = len / 2 - 1; i >= 0; --i) {
            sortdown(i, len - 1);
        }
    }

    bool find(T &t) {
        typename std::vector<T>::iterator it;
        it = find_if(m_container.begin(), m_container.end(),
                     [t](T const &obj) { return t.x == obj.x && t.y == obj.y; });
        return it != m_container.end();
    }

//    T *find(int x, int y) {
////        typename std::vector<T>::iterator it;
////        it = find_if(m_container.begin(), m_container.end(), [x,y](T const &obj) { return x == obj->x && y == obj->y; });
////        return it != m_container.end()?it.base():NULL;
//        for (int i = 0; i < m_container.size(); ++i) {
//            auto value = m_container[i];
//            if (*value->x == x && *value->y == y) {
//                return value;
//            }
//        }
//    }

    void sortup(int start, int end) {
        int childNode = 2 * start + 1;
        int parentNode = start;
        while (parentNode >= 0) {
            if (childNode + 1 <= end && m_container[childNode] > m_container[childNode + 1])
                ++childNode;
            if (m_container[childNode] < m_container[parentNode]) {
                swap(&m_container[childNode], &m_container[parentNode]);
                childNode = parentNode;
                parentNode = (childNode - 1) / 2;
            } else {
                return;
            }
        }
    }

    void sortdown(int start, int end) {
        int childNode = 2 * start + 1;
        int parentNode = start;
        while (childNode <= end) {
            if (childNode + 1 <= end && m_container[childNode] > m_container[childNode + 1])
                ++childNode;
            if (m_container[childNode] < m_container[parentNode]) {
                swap(&m_container[childNode], &m_container[parentNode]);
                parentNode = childNode;
                childNode = 2 * childNode + 1;
            } else {
                return;
            }
        }
    }

};

template<class T>
void MinHeap<T>::buildHeap(std::vector<T> &v) {
    m_container = v;
    resort();
}


template<>
void MinHeap<Point *>::sortup(int start, int end) {
    int childNode = 2 * start + 1;
    int parentNode = start;
    while (parentNode >= 0) {
        if (childNode + 1 <= end && *m_container[childNode] > *m_container[childNode + 1])
            ++childNode;
        if (*m_container[childNode] < *m_container[parentNode]) {
            swap(&m_container[childNode], &m_container[parentNode]);
            childNode = parentNode;
            parentNode = (childNode - 1) / 2;
        } else {
            return;
        }
    }
}

template<>
void MinHeap<Point *>::sortdown(int start, int end) {
    int childNode = 2 * start + 1;
    int parentNode = start;
    while (childNode <= end) {
        if (childNode + 1 <= end && *m_container[childNode] > *m_container[childNode + 1])
            ++childNode;
        if (*m_container[childNode] < *m_container[parentNode]) {
            swap(&m_container[childNode], &m_container[parentNode]);
            parentNode = childNode;
            childNode = 2 * childNode + 1;
        } else {
            return;
        }
    }
}

//template<>
//Point **MinHeap<Point *>::find(int x, int y) {
////        typename std::vector<T>::iterator it;
////        it = find_if(m_container.begin(), m_container.end(), [x,y](T const &obj) { return x == obj->x && y == obj->y; });
////        return it != m_container.end()?it.base():NULL;
//    for (int i = 0; i < m_container.size(); ++i) {
//        Point *value = m_container[i];
//        if (value->x == x && value->y == y) {
//            return &value;
//        }
//    }

//}

static long getCurTimeInMille();

class SearchAlgorithm {
protected:
    Point *m_StartPoint, *m_EndPoint;
    int minx, maxx, miny, maxy;

public:
    virtual void initSize(const int minx, const int maxx, const int miny, const int maxy) = 0;

    virtual long startFind() {
        long eslapse,cur = getCurTimeInMille();
        doFind();
//        printPath();
        eslapse = getCurTimeInMille() - cur;
//        printf("spend time:%d",eslapse);
        return eslapse;
    };

    virtual void doFind() = 0;

    void printPath() {
        const Point *path = m_EndPoint;
        while (NULL != path) {
            printf("(%d,%d)->\n", path->x, path->y);
            path = path->father;
        }
    }

    virtual bool canWalkable(int x, int y) {
        if (x >= maxx || x < minx || y >= maxy || y < miny) return false;
        return (x) % 2 == 0 || (x) % 3 == 0 || (y) % 7 == 0;
    }
};

class AStart : public SearchAlgorithm {
private:
    MinHeap<Point *> openList;
    vector<Point *> closeList;
    vector<vector<Point *> > map;
public:
    AStart(Point *startPoint, Point *endPoint) {
        m_StartPoint = startPoint;
        m_EndPoint = endPoint;
    }

    AStart(int startx, int starty, int endx, int endy) {
        m_StartPoint = new Point(startx, starty, 0, 0, NULL);
        m_EndPoint = new Point(endx, endy, 0, 0, NULL);
    }

    void initSize(const int minx, const int maxx, const int miny, const int maxy) {
        this->minx = minx;
        this->maxx = maxx;
        this->miny = miny;
        this->maxy = maxy;
        map = vector<vector<Point *>>(maxx, vector<Point *>(maxy, 0));
    }

    void doFind() {
        openList.insert(m_StartPoint);
        while (!openList.isEmpty()) {
            Point *cur = openList.extract();
            closeList.push_back(cur);
            if (*cur == *m_EndPoint) {
                m_EndPoint = cur;
                break;
            }
            addNeighbor(cur);
        }
    }

    void addNeighbor(const Point *currentPoint) {
        //add left
        tryAddLeft(currentPoint);
        tryAddRight(currentPoint);
        tryAddUp(currentPoint);
        tryAddDown(currentPoint);

        // 可以加斜角走
    }

    void tryAddLeft(const Point *currentPoint) {
        int x = currentPoint->x - 1;
        int y = currentPoint->y;
        tryAddPoint(x, y, currentPoint, leftweight);
    }

    void tryAddRight(const Point *currentPoint) {
        int x = currentPoint->x + 1;
        int y = currentPoint->y;
        tryAddPoint(x, y, currentPoint, rightweight);
    }

    void tryAddUp(const Point *currentPoint) {
        int x = currentPoint->x;
        int y = currentPoint->y - 1;
        tryAddPoint(x, y, currentPoint, upweight);
    }

    void tryAddDown(const Point *currentPoint) {
        int x = currentPoint->x;
        int y = currentPoint->y + 1;
        tryAddPoint(x, y, currentPoint, downweight);
    }

    void tryAddPoint(int x, int y, const Point *currentPoint, float weight) {
        if (canWalkable(x, y) && !hasAddedCloseList(x, y)) {
            Point *value = hasAddedOpenList(x, y);
            int newweight = currentPoint->g + weight;
            float f = newweight + pow(m_EndPoint->x - x, 2) + pow(m_EndPoint->y - y, 2);

            if (NULL != value && newweight < value->g) {
                value->update(newweight, f, currentPoint);
                openList.resort();
            } else if (NULL == value) {
                Point *point = new Point(x, y, newweight, f, currentPoint);
                openList.insert(point);
                map[x][y] = point;
            }
        }
    }

    Point *hasAddedOpenList(int x, int y) {
        return map[x][y];
//        Point **temp = openList.find(x, y);
//        if (temp == NULL) return NULL;
//        return *temp;
    }

    bool hasAddedCloseList(int x, int y) {
        for (int i = 0; i < closeList.size(); ++i) {
            if (x == closeList[i]->x && y == closeList[i]->y)
                return true;
        }
        return false;
    }
};

class DFS : public SearchAlgorithm {
private:
    vector<Point *> visited;
    vector<Point> stack;
    vector<vector<int>> interMap;
    vector<vector<map<string, int>>> fatherMap;
    int min = INT_MAX;
    int neighbor[4][2] = {{1,  0},
                          {-1, 0},
                          {0,  1},
                          {0,  -1}};
public:
    DFS(Point *startPoint, Point *endPoint) {
        m_StartPoint = startPoint;
        m_EndPoint = endPoint;
    }

    void initSize(const int minx, const int maxx, const int miny, const int maxy) {
        this->minx = minx;
        this->maxx = maxx;
        this->miny = miny;
        this->maxy = maxy;
        interMap = vector<vector<int>>(maxx, vector<int>(maxy, 0));
        fatherMap = vector<vector<map<string, int>>>(maxx, vector<map<string, int>>(maxy));
    }

    void doFind() {
        findByPoint(m_StartPoint);
//        findByCoordinate(m_StartPoint->x,m_StartPoint->y);
    }

    void printPath() {
        if (stack.size() != 0) {
            for (int i = 0; i < stack.size(); ++i) {
                printf("{%d,%d}->\n", stack[i].x, stack[i].y);
            }
        }
        if (visited.size() != 0) {
            for (int i = 0; i < visited.size(); ++i) {
                printf("{%d,%d}->\n", visited[i]->x, visited[i]->y);
            }
        }
    }

    bool hasWalked(int x, int y, int fatherx, int fathery) {
        char key[20];
        sprintf(key, "%d_%d", fatherx, fathery);
        auto map = fatherMap[x][y];
        auto iter = map.find(key);
        if (iter != map.end()) {
            int value = iter->second;
            return 1 == interMap[x][y] || value == 1;
        }
        return 1 == interMap[x][y];
    }

    bool reachEnd(int x, int y) {
        return (x == m_EndPoint->x && y == m_EndPoint->y);
    }

    char *getKey(int x, int y) {
        char *key = new char[20];
        sprintf(key, "%d_%d", x, y);
        return key;
    }

    //查找有效路径
    bool findByCoordinate(int x, int y) {
        Point *point = new Point(x, y, 0, 0, NULL);
        visited.push_back(point);
        interMap[x][y] = 1;
        for (int i = 0; i < 4; ++i) {
            int *curneigh = neighbor[i];
            int nextx = x + curneigh[0];
            int nexty = y + curneigh[1];
            auto walkable = canWalkable(nextx, nexty);
            if (walkable && !hasWalked(nextx, nexty, x, y)) {
                bool reached = reachEnd(nextx, nexty);
                if (reached) {
                    visited.push_back(m_EndPoint);
                    return true;
                } else {
                    bool reached = findByCoordinate(nextx, nexty);
                    if (reached) return true;
                }
            }
        }

        visited.pop_back();
        delete point;
        return false;
    }

    //查找最短路径
    bool findByPoint(Point *curPoint) {
        visited.push_back(curPoint);
        interMap[curPoint->x][curPoint->y] = 1;
        for (int i = 0; i < 4; ++i) {
            int *curneigh = neighbor[i];
            int nextx = curPoint->x + curneigh[0];
            int nexty = curPoint->y + curneigh[1];
            auto walkable = canWalkable(nextx, nexty);
            if (walkable && !hasWalked(nextx, nexty, curPoint->x, curPoint->y)) {
                bool reached = reachEnd(nextx, nexty);

                if (reached && curPoint->f + 1 < min) {
                    min = curPoint->f + 1;
                    stack.clear();
                    for (int j = 0; j < visited.size(); ++j) {
                        stack.push_back(*visited[j]);
                    }
                    stack.push_back(*m_EndPoint);
                } else {
                    Point *next = new Point(nextx, nexty, 1, curPoint->f + 1, NULL);
                    char key[20];
                    sprintf(key, "%d_%d", curPoint->x, curPoint->y);
                    auto map = fatherMap[nextx][nexty];
                    map[key] = 1;
                    fatherMap[nextx][nexty] = map;
                    findByPoint(next);
                }
            }
        }

        visited.pop_back();
        delete curPoint;
        //如果需要获取最短路ing需要开启
        interMap[curPoint->x][curPoint->y] = 0;
        return false;
    }
};

class BFS : public SearchAlgorithm {
private:
    queue<Point *> visited;
    vector<Point> stack;
    vector<int> walked;
    int min = INT_MAX;
    int neighbor[4][2] = {{1,  0},
                          {-1, 0},
                          {0,  1},
                          {0,  -1}};
public:
    BFS(Point *startPoint, Point *endPoint) {
        m_StartPoint = startPoint;
        m_EndPoint = endPoint;
    }

    void initSize(const int minx, const int maxx, const int miny, const int maxy) {
        this->minx = minx;
        this->maxx = maxx;
        this->miny = miny;
        this->maxy = maxy;
        walked = vector<int>(maxx * maxy, 0);
    }

    bool hasWalked(int x, int y) {
        int num = x * maxx + y;
        return walked[num] == 1;
    }

    bool reachEnd(int x, int y) {
        return (x == m_EndPoint->x && y == m_EndPoint->y);
    }

    virtual void doFind() {
        int num = m_StartPoint->x * maxx + m_StartPoint->y;
        walked[num] = 1;
        visited.push(m_StartPoint);
        while (0 != visited.size()) {
            auto cur = visited.front();
            visited.pop();
            for (int i = 0; i < 4; ++i) {
                int *curneigh = neighbor[i];
                int nextx = cur->x + curneigh[0];
                int nexty = cur->y + curneigh[1];
                auto walkable = canWalkable(nextx, nexty);
                if (walkable && !hasWalked(nextx, nexty)) {
                    bool reached = reachEnd(nextx, nexty);
                    if (reached) {
                        m_EndPoint->update(1, cur->f + 1, cur);
                        return;
                    } else {
                        Point *p = new Point(nextx, nexty, 1, cur->f + 1, cur);
                        visited.push(p);
                        num = nextx * maxx + nexty;
                        walked[num] = 1;
                    }
                }
            }
        }
    }


    void printPath() {
        const Point *father = m_EndPoint;
        while (NULL != father) {
            printf("{%d,%d}->\n", father->x, father->y);
            father = father->father;
        }
    }
};

long static getCurTimeInMille()
{
    chrono::milliseconds ms = chrono::duration_cast< chrono::milliseconds >(
            chrono::system_clock::now().time_since_epoch()
    );
    return ms.count();
}

extern "C"
{
     extern int AStartAlg(int startx, int starty, int endx, int endy, int width, int height) {
        Point *startPoint = new Point(startx, starty, 0, 0, NULL);
        Point *endPoint = new Point(endx, endy, 0, 0, NULL);

        AStart start = AStart(startPoint, endPoint);
        start.initSize(0, width, 0, height);
        return start.startFind();
    }

     extern int DFSAlg(int startx, int starty, int endx, int endy, int with, int height) {
        Point *startPoint = new Point(startx, starty, 0, 0, NULL);
        Point *endPoint = new Point(endx, endy, 0, 0, NULL);
        DFS dfs = DFS(startPoint, endPoint);
        dfs.initSize(0, with, 0, height);
        return dfs.startFind();
    }

     extern int BFSAlg(int startx, int starty, int endx, int endy, int with, int height) {
        Point *startPoint = new Point(startx, starty, 0, 0, NULL);
        Point *endPoint = new Point(endx, endy, 0, 0, NULL);

        BFS bfs = BFS(startPoint, endPoint);
        bfs.initSize(0, with, 0, height);
        return bfs.startFind();
    }
}
//
//int main(int argc, char const *argv[]) {
//    printf("ASTARTtime:%d",BFSAlg(0,0,0,0,12,999));
//    return 0;
//    AStart start = AStart(startPoint,endPoint);
//    start.initSize(0,1000,0,1000);
//    start.startFind();
//
////    DFS dfs = DFS(startPoint, endPoint);
////    dfs.initSize(0, 999, 0,999);
////    dfs.startFind();
//
//    BFS bfs = BFS(startPoint, endPoint);
//    bfs.initSize(0, 1000, 0, 1000);
//    bfs.startFind();

//    auto fatherMap = vector<vector<map<string,int>>>(5, vector<map<string,int>>(5));
//
//    for (int i = 0; i < 5; ++i) {
//        for (int j = 0; j < 5; ++j) {
//            auto value = fatherMap[i][j];
//            char key[20];
//            sprintf(key,"%d_%d",i,j);
//            value[key] = 1;
//            fatherMap[i][j] = value;
//        }
//    }
//
//    for (int i = 0; i < 5; ++i) {
//        for (int j = 0; j < 5; ++j) {
//            auto value = fatherMap[i][j];
//            map<string,int>::iterator strmap_iter2 = value.begin();
//            for(;strmap_iter2 !=value.end();strmap_iter2++)
//            {
//                cout<<strmap_iter2->first<<' '<<strmap_iter2->second<<endl;
//            }
//        }
//    }


//    MinHeap<Point*> min_heap;
//    srand(1);
//    int x = 1000;
//    int y = 1000;
//    int capacity = x*y;
//    for (int i = 0; i < x; ++i) {
//        for (int j = 0; j < y; ++j) {
//            auto p = new Point(i,j,1,random(capacity), nullptr);
//            min_heap.insert(p);
//        }
//    }
//
//    for (int i = 0; i < x; ++i) {
//        for (int j = 0; j < y; ++j) {
//            min_heap.find(i,j);
//        }
//    }
//    while (!min_heap.isEmpty())
//    {
//        auto value = min_heap.extract();
//        printf("{%d,%d},f:%.2f\n",value->x,value->y,value->f);
//    }
//
//    return 0;
//}
//}
