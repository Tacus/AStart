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

    void initSort(int start, int end) {
        int childNode = 2 * start + 1;
        int parentNode = start;
        if (childNode + 1 <= end && m_container[childNode] < m_container[parentNode + 1])
            ++childNode;
        if (m_container[childNode] < m_container[parentNode]) {
            swap(&m_container[childNode], &m_container[parentNode]);
        }
    }

    void sortup(int start, int end) {
        int childNode = 2 * start + 1;
        int parentNode = start;
        while (parentNode >= 0) {
            if (childNode + 1 <= end && m_container[childNode] < m_container[parentNode + 1])
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
            if (childNode + 1 <= end && m_container[childNode] < m_container[parentNode + 1])
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

    void insert(T &value) {
        m_container.push_back(value);
        int len = m_container.size();
        sortup(len / 2 - 1, len - 1);
    }

    bool isEmpty() {
        return 0 != m_container.size();
    }

    T extract() {
        T value = m_container.front();
        int len = m_container.size();
        m_container[0] = m_container[len - 1];
        m_container.pop_back();
        sortdown(0, len - 1);
        return value;
    }

    bool find(T &t) {
        typename std::vector<T>::iterator it;
        it = find_if(m_container.begin(), m_container.end(), [](T const &obj) { return t.x == obj.x && t.y == obj.y; });
        return it != m_container.end();
    }

    T* find(int x, int y) {
        typename std::vector<T>::iterator it;
        it = find_if(m_container.begin(), m_container.end(), [](T const &obj) { return x == obj.x && y == obj.y });
        return it != m_container.end()?it.base():NULL;
    }
};

template<class T>
void MinHeap<T>::buildHeap(std::vector<T> &v) {
    m_container = v;
    int len = m_container.size();
    for (int i = len / 2 - 1; i >= 0; --i) {
        sortdown(i, len - 1);
    }
}


struct Point {
    Point(int x, int y, float g, float f,const Point* father) {
        this->x = x;
        this->y = y;
        this->g = g;
        this->f = f;
        this->father = father;
    }

    void update(float g, float f,const Point* father)
    {
        this->g = g;
        this->f = f;
        this->father = father;
    }

    int x, y;
    float f, g, h;
    const Point* father;

    bool operator==(const Point b) const {
        return this->f == b.f;
    }

    bool operator!=(const Point b) const {
        return this->f != b.f;
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

class AStart {
private:
    MinHeap<Point> openList;
    vector<Point> closeList;
    Point m_StartPoint, m_EndPoint;
    int minx,maxx,miny,maxy;
public:
    void construct(vector<int, int> &set, Point &startPoint, Point &endPoint) {
        m_StartPoint = startPoint;
        m_EndPoint = m_EndPoint;
    }

    void StartFind() {
        openList.insert(m_StartPoint);
        while (!openList.isEmpty()) {
            Point cur = openList.extract();
            closeList.push_back(cur);
            if (cur == m_EndPoint)
                return;
            addNeighbor(&cur);
        }
    }

    bool canWalkable(const Point point) {

        return true;
    }

//    TODO
    bool canWalkable(int x, int y) {
        return (x + y) % 7 == 0;
    }

    void addNeighbor(const Point* currentPoint) {
        //add left
        tryAddLeft(currentPoint);
        tryAddRight(currentPoint);
        tryAddUp(currentPoint);
        tryAddDown(currentPoint);
    }

    void tryAddLeft(const Point* currentPoint)
    {
        int x = currentPoint->x - 1;
        int y = currentPoint->y;
        if (x >= minx && canWalkable(x, y) && !hasAddedCloseList(x, y)) {
            Point* value = hasAddedOpenList(x, y);
            if (NULL != value) {
                int newweight = currentPoint->g+leftweight;
                if(newweight < value->g)
                {
                    float f = currentPoint->g+leftweight + pow(m_EndPoint.x - x,2)+pow(m_EndPoint.y - y,2);
                    value->update(newweight,f, currentPoint);
                }
            } else {
                float f = currentPoint->g+leftweight + pow(m_EndPoint.x - x,2)+pow(m_EndPoint.y - y,2);
                Point *point = new Point(x, y, leftweight,f,currentPoint);
                openList.insert(*point);
            }
        }
    }
    void tryAddRight(const Point* currentPoint)
    {
        int x = currentPoint->x + 1;
        int y = currentPoint->y;
        if (x <= maxx && canWalkable(x, y) && !hasAddedCloseList(x, y)) {
            Point* value = hasAddedOpenList(x, y);
            if (NULL != value) {
                int newweight = currentPoint->g+rightweight;
                if(newweight < value->g)
                {
                    float f = currentPoint->g+rightweight + pow(m_EndPoint.x - x,2)+pow(m_EndPoint.y - y,2);
                    value->update(newweight,f, currentPoint);
                }
            } else {
                float f = currentPoint->g+rightweight + pow(m_EndPoint.x - x,2)+pow(m_EndPoint.y - y,2);
                Point *point = new Point(x, y, rightweight,f,currentPoint);
                openList.insert(*point);
            }
        }
    }

    void tryAddUp(const Point* currentPoint)
    {
        int x = currentPoint->x;
        int y = currentPoint->y - 1;
        if (y >= miny && canWalkable(x, y) && !hasAddedCloseList(x, y)) {
            Point* value = hasAddedOpenList(x, y);
            if (NULL != value) {
                int newweight = currentPoint->g+upweight;
                if(newweight < value->g)
                {
                    float f = currentPoint->g+upweight + pow(m_EndPoint.x - x,2)+pow(m_EndPoint.y - y,2);
                    value->update(newweight,f, currentPoint);
                }
            } else {
                float f = currentPoint->g+upweight + pow(m_EndPoint.x - x,2)+pow(m_EndPoint.y - y,2);
                Point *point = new Point(x, y, upweight,f,currentPoint);
                openList.insert(*point);
            }
        }
    }

    void tryAddDown(const Point* currentPoint)
    {
        int x = currentPoint->x;
        int y = currentPoint->y + 1;
        if (y <= maxy && canWalkable(x, y) && !hasAddedCloseList(x, y)) {
            Point* value = hasAddedOpenList(x, y);
            if (NULL != value) {
                int newweight = currentPoint->g+downweight;
                if(newweight < value->g)
                {
                    float f = currentPoint->g+downweight + pow(m_EndPoint.x - x,2)+pow(m_EndPoint.y - y,2);
                    value->update(newweight,f, currentPoint);
                }
            } else {
                float f = currentPoint->g+downweight + pow(m_EndPoint.x - x,2)+pow(m_EndPoint.y - y,2);
                Point *point = new Point(x, y, downweight,f,currentPoint);
                openList.insert(*point);
            }
        }
    }

    Point* hasAddedOpenList(int x, int y) {
        return openList.find(x, y);
    }

    bool hasAddedCloseList(int x, int y) {
        for (int i = 0; i < closeList.size(); ++i) {
            if (x == closeList[i].x && y == closeList[i].y)
                return true;
        }
    }

//    void getPoint(int x,int y,)



};

int main(int argc, char const *argv[]) {
    /* code */
    std::vector<int> v;
    srand(1);
    int capacity = 1000000;

    for (int i = 0; i < capacity; ++i) {
        v.push_back(random(capacity));

    }
    MinHeap<int> min_heap;
    min_heap.buildHeap(v);
    cout << v.size() << endl;
    return 0;
}

