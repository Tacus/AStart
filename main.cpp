#include <iostream>
#include <vector>
#include <cstdlib>
#include <math.h>

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

    T *find(int x, int y) {
//        typename std::vector<T>::iterator it;
//        it = find_if(m_container.begin(), m_container.end(), [x,y](T const &obj) { return x == obj->x && y == obj->y; });
//        return it != m_container.end()?it.base():NULL;
        for (int i = 0; i < m_container.size(); ++i) {
            auto value = m_container[i];
            if (*value->x == x && *value->y == y) {
                return value;
            }
        }
    }

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

template<>
Point **MinHeap<Point *>::find(int x, int y) {
//        typename std::vector<T>::iterator it;
//        it = find_if(m_container.begin(), m_container.end(), [x,y](T const &obj) { return x == obj->x && y == obj->y; });
//        return it != m_container.end()?it.base():NULL;
    for (int i = 0; i < m_container.size(); ++i) {
        Point *value = m_container[i];
        if (value->x == x && value->y == y) {
            return &value;
        }
    }

}

class SearchAlgorithm {
protected:
    Point *m_StartPoint, *m_EndPoint;
    int minx, maxx, miny, maxy;

public:
    virtual void initSize(const int minx, const int maxx, const int miny, const int maxy) = 0;

    virtual void startFind() = 0;

    bool printPath() {
        const Point *path = m_EndPoint;
        while (NULL != path) {
            printf("(%d,%d)->\n", path->x, path->y);
            path = path->father;
        }
    }

    virtual bool canWalkable(int x, int y) {
        if (x > maxx || x < minx || y > maxy || y < miny) return false;
        return true;
//        return (x) % 2 == 0 || (x) % 3 == 0 || (y) % 7 == 0;
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
        map = vector<vector<Point *>>(maxx + 1, vector<Point *>(maxy + 1, 0));
    }

    void startFind() {
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
        printPath();
    }


    bool canWalkable(int x, int y) {
//        return true;
        if (x > maxx || x < minx || y > maxy || y < miny) return false;

        return (x) % 2 == 0 || (x) % 3 == 0 || (y) % 7 == 0;
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
        Point **temp = openList.find(x, y);
        if (temp == NULL) return NULL;
        return *temp;
    }

    bool hasAddedCloseList(int x, int y) {
        for (int i = 0; i < closeList.size(); ++i) {
            if (x == closeList[i]->x && y == closeList[i]->y)
                return true;
        }
    }
};

class DFS : public SearchAlgorithm {
private:
    vector<Point *> visited;
    vector<Point> stack;
    vector<vector<int>> map;
    int min = INT_MAX;
    int neighbor[4][2] = {{1,0},{-1,0},{0,1},{0,-1}};
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
        map = vector<vector<int>>(maxx + 1, vector<int>(maxy + 1, 0));
    }

    void startFind() {
        startFind(m_StartPoint);
//        startFind(m_StartPoint->x,m_StartPoint->y);
        printPath();
    }

    bool printPath() {
        if (stack.size() != 0) {
            for (int i = 0; i < stack.size(); ++i) {
                printf("{%d,%d}->\n", stack[i].x, stack[i].y);
            }
        }if (visited.size() != 0) {
            for (int i = 0; i < visited.size(); ++i) {
                printf("{%d,%d}->\n", visited[i]->x, visited[i]->y);
            }
        }
    }

    bool hasWalked(int x,int y)
    {
        return 1 == map[x][y];
    }

    bool reachEnd(int x, int y)
    {
        return (x == m_EndPoint->x && y == m_EndPoint->y);
    }

    //查找有效路径
    bool startFind(int x,int y) {
        Point* point = new Point(x,y,0,0,NULL);
        visited.push_back (point);
        map[x][y] = 1;
        for (int i = 0; i < 4; ++i) {
            int *curneigh = neighbor[i];
            int nextx = x + curneigh[0];
            int nexty = y + curneigh[1];
            auto walkable = canWalkable(nextx, nexty);
            if(walkable && !hasWalked(nextx, nexty))
            {
                bool reached =reachEnd(nextx, nexty);
                if (reached) {
                    visited.push_back (m_EndPoint);
                    return true;
                }
                else
                {
                    bool reached = startFind(nextx, nexty);
                    if(reached) return true;
                }
            }
        }

        visited.pop_back();
        delete point;
        return false;
    }

    //查找最短路径
    bool startFind(Point *curPoint) {
        visited.push_back (curPoint);
        map[curPoint->x][curPoint->y] = 1;
        for (int i = 0; i < 4; ++i) {
            int *curneigh = neighbor[i];
            int nextx = curPoint->x + curneigh[0];
            int nexty = curPoint->y + curneigh[1];
            auto walkable = canWalkable(nextx, nexty);
            if(walkable && !hasWalked(nextx, nexty))
            {
                bool reached =reachEnd(nextx, nexty);
                if (reached && curPoint->f+1 < min) {
                    min = curPoint->f + 1;
                    stack.clear();
                    for (int j = 0; j < visited.size(); ++j) {
                        stack.push_back(*visited[j]);
                    }
                    stack.push_back(*m_EndPoint);
                }
                else
                {
                    Point* next = new Point(nextx,nexty,1,curPoint->f+1,NULL);
                    startFind(next);
                }
            }
        }

        visited.pop_back();
        delete curPoint;
        //如果需要获取最短路ing需要开启
        map[curPoint->x][curPoint->y] = 0;
        return false;
    }
};

class BFS : public SearchAlgorithm {
private:
    vector<Point *> visited;
    vector<Point> stack;
    vector<vector<int>> map;
    int min = INT_MAX;
    int neighbor[4][2] = {{1,0},{-1,0},{0,1},{0,-1}};
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
        map = vector<vector<int>>(maxx + 1, vector<int>(maxy + 1, 0));
    }

    void startFind() {
//        startFind(m_StartPoint);
//        startFind(m_StartPoint->x,m_StartPoint->y);
        printPath();
//        while()
    }

    bool printPath() {
        if (stack.size() != 0) {
            for (int i = 0; i < stack.size(); ++i) {
                printf("{%d,%d}->\n", stack[i].x, stack[i].y);
            }
        }if (visited.size() != 0) {
            for (int i = 0; i < visited.size(); ++i) {
                printf("{%d,%d}->\n", visited[i]->x, visited[i]->y);
            }
        }
    }
};

int main(int argc, char const *argv[]) {
    Point *startPoint = new Point(0, 0, 0, 0, NULL);
    Point *endPoint = new Point(2, 2, 0, 0, NULL);

//    AStart start = AStart(startPoint,endPoint);
//    start.initSize(0,1000,0,1000);
//    start.startFind();

    DFS dfs = DFS(startPoint, endPoint);
    dfs.initSize(0, 4, 0,4);
    dfs.startFind();


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
}

