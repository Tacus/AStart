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

    Point(unsigned int x, unsigned int y, float g, float f, Point *father) {
        this->x = x;
        this->y = y;
        this->g = g;
        this->f = f;
        this->father = father;
    }

    void update(float g, float f, Point *father) {
        this->g = g;
        this->f = f;
        this->father = father;
    }

    unsigned x, y;
    float f, g;
    Point *father;

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

static int getCurTimeInMille();

static int getPathArray();

class SearchAlgorithm {
protected:
    Point *m_StartPoint, *m_EndPoint;
    int minx, maxx, miny, maxy;
    int timeInMille;
    int pathStep;
    const unsigned char* worldMap;
    unsigned int* pathMap;


public:
    virtual void initSize(const int minx, const int maxx, const int miny, const int maxy) = 0;

    virtual int startFind() {
        int eslapse, cur = getCurTimeInMille();
        doFind();
        eslapse = getCurTimeInMille() - cur;
        timeInMille = eslapse;
        return timeInMille;
    }

    int getElapse() {
        return timeInMille;
    }

    int getPathStep() {
        return pathStep;
    }

//    virtual void addPoint2Array(Point *p, int offset) {
//        unsigned char *temp = (unsigned char *) &p->x;
//        for (int i = 0; i < 4; ++i) {
//            array[offset + i] = temp[i];
//        }
//        temp = (unsigned char *) &p->y;
//        for (int i = 0; i < 4; ++i) {
//            array[offset + i + 4] = temp[i];
//        }
//    }

        virtual void addPoint2Array(Point *p, int offset) {
//        unsigned char *temp = (unsigned char *) &p->x;
//        for (int i = 0; i < 4; ++i) {
//            array[offset + i] = temp[i];
//        }
        pathMap[offset] = p->x;
        pathMap[offset+1] = p->y;
    }

    virtual void getPath() {
        vector<Point *> vect;
        int index = 0;
        Point *path = m_EndPoint;
        while (NULL != path) {
            vect.push_back(path);
            ++index;
            path = path->father;
        }
        if(index == 1) return;
        int offset = 0;
        pathStep = index;
        for (int i = 0; i < vect.size(); ++i) {
            auto p = vect[vect.size() - i - 1];
            addPoint2Array(p, offset);
            offset = (i + 1) * 2;
        }
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
        int index = x*maxx+y;
        return worldMap[index] == 1;
    }
};

class AStart : public SearchAlgorithm {
private:
    MinHeap<Point *> openList;
    vector<Point *> closeList;
    vector<vector<Point *> > hasWalkedMap;
public:
    ~AStart() {
        while (!openList.isEmpty()) {
            Point *cur = openList.extract();
            delete cur;
        }
        for (int i = 0; i < closeList.size(); ++i) {
            delete closeList[i];
        }
//        printf("~Astart\n");
    }

    AStart(Point *startPoint, Point *endPoint) {
        m_StartPoint = startPoint;
        m_EndPoint = endPoint;
    }

    AStart(const unsigned char*worldMap,unsigned int* pathMap,Point *startPoint, Point *endPoint) {
        m_StartPoint = startPoint;
        m_EndPoint = endPoint;
        this->worldMap = worldMap;
        this->pathMap = pathMap;
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
        hasWalkedMap = vector<vector<Point *>>(maxx, vector<Point *>(maxy, 0));
    }

    void doFind() {
        openList.insert(m_StartPoint);
        while (!openList.isEmpty()) {
            Point *cur = openList.extract();
            closeList.push_back(cur);
            if (*cur == *m_EndPoint) {
                delete m_EndPoint;
                m_EndPoint = cur;
                break;
            }
            addNeighbor(cur);
        }
    }

    void addNeighbor(Point *currentPoint) {
        //add left
        tryAddLeft(currentPoint);
        tryAddRight(currentPoint);
        tryAddUp(currentPoint);
        tryAddDown(currentPoint);

        // 可以加斜角走
    }

    void tryAddLeft(Point *currentPoint) {
        int x = currentPoint->x - 1;
        int y = currentPoint->y;
        tryAddPoint(x, y, currentPoint, leftweight);
    }

    void tryAddRight(Point *currentPoint) {
        int x = currentPoint->x + 1;
        int y = currentPoint->y;
        tryAddPoint(x, y, currentPoint, rightweight);
    }

    void tryAddUp(Point *currentPoint) {
        int x = currentPoint->x;
        int y = currentPoint->y - 1;
        tryAddPoint(x, y, currentPoint, upweight);
    }

    void tryAddDown(Point *currentPoint) {
        int x = currentPoint->x;
        int y = currentPoint->y + 1;
        tryAddPoint(x, y, currentPoint, downweight);
    }

    void tryAddPoint(int x, int y, Point *currentPoint, float weight) {
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
                hasWalkedMap[x][y] = point;
            }
        }
    }

    Point *hasAddedOpenList(int x, int y) {
        return hasWalkedMap[x][y];
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

    ~DFS() {
        for (int i = 0; i < visited.size(); ++i) {
            auto p = visited[i];
            delete p;
        }
//        printf("~DFS\n");
    }

    DFS(int startx, int starty, int endx, int endy) {
        m_StartPoint = new Point(startx, starty, 0, 0, NULL);
        m_EndPoint = new Point(endx, endy, 0, 0, NULL);
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
        findByCoordinate(m_StartPoint->x, m_StartPoint->y);
    }

    long doFindMin() {
        long eslapse, cur = getCurTimeInMille();
        findByPoint(m_StartPoint);
        eslapse = getCurTimeInMille() - cur;
        timeInMille = eslapse;
        return timeInMille;
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

    virtual void getMinPath() {
        pathStep = stack.size();
        int offset = 0;
        for (int i = 0; i < stack.size(); ++i) {
            auto p = stack[i];
            addPoint2Array(&p, offset);
            offset = (i + 1) * 8;
        }
    };

    virtual void getPath() {
        pathStep = visited.size();
        int offset = 0;
        for (int i = 0; i < visited.size(); ++i) {
            auto p = visited[i];
            addPoint2Array(p, offset);
            offset = (i + 1) * 8;
        }
    };

    bool hasWalked(int x, int y, int fatherx, int fathery,float curf) {
        char key[15];
        sprintf(key, "%d_%d", fatherx, fathery);
        auto map = fatherMap[x][y];
        auto iter = map.find(key);
        if (iter != map.end()) {
            int value = iter->second;
            return 1 == interMap[x][y] || value < curf;
        }
        return 1 == interMap[x][y];
    }

    bool reachEnd(int x, int y) {
        return (x == m_EndPoint->x && y == m_EndPoint->y);
    }

    char *getKey(int x, int y) {
        char *key = new char[15];
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
            if (walkable && !hasWalked(nextx, nexty, x, y,0)) {
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
    void findByPoint(Point *curPoint) {
        visited.push_back(curPoint);
        interMap[curPoint->x][curPoint->y] = 1;
        for (int i = 0; i < 4; ++i) {
            int *curneigh = neighbor[i];
            int nextx = curPoint->x + curneigh[0];
            int nexty = curPoint->y + curneigh[1];
            auto walkable = canWalkable(nextx, nexty);
            float curf = curPoint->f+1;
            if (walkable && !hasWalked(nextx, nexty, curPoint->x, curPoint->y,curf)) {
                bool reached = reachEnd(nextx, nexty);
                if (reached) {
                    if (curf < min) {
                        min = curf;
                        stack.clear();
                        for (int j = 0; j < visited.size(); ++j) {
                            stack.push_back(*visited[j]);
                        }
                        auto endp = Point(nextx, nexty, 1, min, NULL);
                        stack.push_back(endp);
                        break;
                    }
                } else {
                    char key[15];
                    sprintf(key, "%d_%d", curPoint->x, curPoint->y);
                    auto map = fatherMap[nextx][nexty];
                    map[key] = curf;
                    fatherMap[nextx][nexty] = map;
                    if(curf<min)
                    {
                        Point *next = new Point(nextx, nexty, 1, curf, NULL);
                        findByPoint(next);
                    }
                }
            }
        }

        visited.pop_back();
        //如果需要获取最短路ing需要开启
        interMap[curPoint->x][curPoint->y] = 0;
        delete curPoint;
    }
};

class BFS : public SearchAlgorithm {
private:
    queue<Point *> visited;
    vector<Point> stack;
    vector<int> walked;
    int min = INT_MAX;
    vector<Point *> cache;
    int neighbor[4][2] = {{1,  0},
                          {-1, 0},
                          {0,  1},
                          {0,  -1}};
public:
    BFS(Point *startPoint, Point *endPoint) {
        m_StartPoint = startPoint;
        m_EndPoint = endPoint;
    }

    ~BFS() {
        for (int i = 0; i < cache.size(); ++i) {
            auto p = cache[i];
            delete p;
        }
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
        cache.push_back(m_StartPoint);
        cache.push_back(m_EndPoint);
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
                        cache.push_back(p);
                        num = nextx * maxx + nexty;
                        walked[num] = 1;
                    }
                }
            }
        }
    }
};

int static getCurTimeInMille() {
    chrono::milliseconds ms = chrono::duration_cast<chrono::milliseconds>(
            chrono::system_clock::now().time_since_epoch()
    );
    return ms.count();
}

extern "C"
{
extern int AStartAlgWithTime(int startx, int starty, int endx, int endy, int width, int height) {
    Point *startPoint = new Point(startx, starty, 0, 0, NULL);
    Point *endPoint = new Point(endx, endy, 0, 0, NULL);
    AStart start = AStart(startPoint, endPoint);
    start.initSize(0, width, 0, height);
    return start.startFind();
}

extern int DFSAlgWithTime(int startx, int starty, int endx, int endy, int with, int height) {
    Point *startPoint = new Point(startx, starty, 0, 0, NULL);
    Point *endPoint = new Point(endx, endy, 0, 0, NULL);
    DFS dfs = DFS(startPoint, endPoint);
    dfs.initSize(0, with, 0, height);
    return dfs.startFind();
}
extern int DFSAlgFindMinWithTime(int startx, int starty, int endx, int endy, int with, int height) {
    Point *startPoint = new Point(startx, starty, 0, 0, NULL);
    Point *endPoint = new Point(endx, endy, 0, 0, NULL);
    DFS dfs = DFS(startPoint, endPoint);
    dfs.initSize(0, with, 0, height);
    return dfs.doFindMin();
}

extern int BFSAlgWithTime(int startx, int starty, int endx, int endy, int with, int height) {
    Point *startPoint = new Point(startx, starty, 0, 0, NULL);
    Point *endPoint = new Point(endx, endy, 0, 0, NULL);

    BFS bfs = BFS(startPoint, endPoint);
    bfs.initSize(0, with, 0, height);
    return bfs.startFind();
}
//
extern unsigned int
AStartAlgWithPath(const unsigned char*worlMap, unsigned int*pathMap,  int startx, int starty, int endx, int endy, int width, int height, int *time) {
    Point *startPoint = new Point(startx, starty, 0, 0, NULL);
    Point *endPoint = new Point(endx, endy, 0, 0, NULL);

    AStart start = AStart(worlMap,pathMap,startPoint, endPoint);
    start.initSize(0, width, 0, height);
    start.startFind();
    start.getPath();
    *time = start.getElapse();
    return start.getPathStep();
}

extern void
DFSAlgWithPath(const unsigned char*worlMap, int startx, int starty, int endx, int endy, int width, int height, int *time) {
//    Point *startPoint = new Point(startx, starty, 0, 0, NULL);
//    Point *endPoint = new Point(endx, endy, 0, 0, NULL);
//    DFS dfs = DFS(startPoint, endPoint);
//    dfs.initSize(0, width, 0, height);
//    dfs.startFind();
//    unsigned char *path = dfs.getPath();
//    *time = dfs.getElapse();
}

extern void
DFSAlgWithMinPath(const unsigned char*worlMap,int startx, int starty, int endx, int endy, int width, int height, int *time) {
//    Point *startPoint = new Point(startx, starty, 0, 0, NULL);
//    Point *endPoint = new Point(endx, endy, 0, 0, NULL);
//    DFS dfs = DFS(startPoint, endPoint);
//    dfs.initSize(0, width, 0, height);
//    dfs.doFindMin();
//    unsigned char *path = dfs.getMinPath();
//    *time = dfs.getElapse();
}

extern void
BFSAlgWithPath(const unsigned char*worlMap,int startx, int starty, int endx, int endy, int with, int height, int *time) {
//    Point *startPoint = new Point(startx, starty, 0, 0, NULL);
//    Point *endPoint = new Point(endx, endy, 0, 0, NULL);
//
//    BFS bfs = BFS(startPoint, endPoint);
//    bfs.initSize(0, with, 0, height);
//    bfs.startFind();
//    unsigned char *path = bfs.getPath();
//    *time = bfs.getElapse();
}
extern void releaseBuffer(unsigned char *ptr) {
    delete ptr;
}
}

long jiechen(int n) {
    if (n == 1)
        return 1;
    return 1 * jiechen(n - 1);
}

void addPoint(vector<Point> &vect) {
    for (int i = 0; i < 100; ++i) {
        auto p = Point(1, 2, 1, 1, NULL);
        vect.push_back(p);
    }
}

//char *getKey(int x, int y) {
//    char key[20];
//    sprintf(key, "%d_%d", x, y);
//    return key;
//}

bool am_little_endian() {
    unsigned short i = 1;
    return (int) *((char *) (&i)) ? true : false;
}

//unsigned char *getcharbyte(int num) {
////    unsigned char* p = new unsigned char[20*4];
//    unsigned char p[num * 4];
//    srand(1);
//    for (int i = 0; i < num; ++i) {
////        int a = random(num);
//        int a = 900;
//        printf("random value:%d\n", a);
//        auto temp = (unsigned char *) &a;
//        for (int j = 0; j < 4; ++j) {
//            p[i * 4 + j] = temp[j];
//        }
//    }
//    return p;
//}

void test() {
//    int num = 1;
//    unsigned char* array = getcharbyte(num);
//    for (int i = 0; i < num*5; ++i) {
//        printf("testchar:%d\n",array[i]);
//    }
    //    vector<Point> vect;
//    addPoint(vect);
//    for (int i = 0; i < vect.size(); ++i) {
//        printf("p:{%d,%d}",vect[i].x,vect[i].y);
//    }
//    long value = jiechen(1000000);
//    cout<< value;

//printf("%s\n",getKey(121,123));
//printf("%s\n",getKey(121,123));
//printf("%s\n",getKey(121,123));
//printf("%s\n",getKey(1,1));

//    if(am_little_endian())
//    {
//        printf("本机字节序为小段序!\n");
//    }else
//    {
//        printf("本机字节序为大段序!\n");
//    }
//    unsigned int value = 1;
//    unsigned char* charp = (unsigned char*)&value;
//    printf("%d\n",charp[0]);
//    printf("%d\n",charp[1]);
//    printf("%d\n",charp[2]);
//    printf("%d\n",charp[3]);
}

//
//int main(int argc, char const *argv[]) {
//
//    int time, pathStep;
//    DFSAlgWithMinPath(0, 0, 98, 98, 100, 100, &time, &pathStep);
//    DFSAlgWithMinPath(0, 0, 7, 7, 10, 10, &time, &pathStep);
//    printf("DFSAlgWithMinPath with time:%d and pathStep:%d\n", time, pathStep);
//    DFSAlgWithPath(0, 0, 98, 98, 100, 100, &time, &pathStep);
//    printf("DFSAlgWithPath with time:%d and pathStep:%d\n", time, pathStep);
//    AStartAlgWithPath(0, 0, 98, 98, 100, 100, &time, &pathStep);
//    printf("AStartAlgWithPath with time:%d and pathStep:%d\n", time, pathStep);
////
//    BFSAlgWithPath(0, 0, 7, 7, 10, 10, &time, &pathStep);
//    printf("BFSAlgWithPath with time:%d and pathStep:%d\n", time, pathStep);
//    printf("DFSAlg:%d\n", DFSAlgWithTime(0, 0, 978, 500, 1000, 100));
//    printf("DFSAlgFindMin:%d\n", DFSAlgFindMinWithTime(0, 0, 978, 500, 1000, 100));
//    printf("BFSAlg:%d", BFSAlgWithTime(0, 0, 978, 998, 1000, 1000));
//    test();
//}


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

//    return 0;
//}
////}
