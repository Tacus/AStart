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

    void insert(T &value) {
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

    void resort()
    {
        int len = m_container.size();
        for (int i = len / 2 - 1; i >= 0; --i) {
            sortdown(i, len - 1);
        }
    }

    bool find(T &t) {
        typename std::vector<T>::iterator it;
        it = find_if(m_container.begin(), m_container.end(), [t](T const &obj) { return t.x == obj.x && t.y == obj.y; });
        return it != m_container.end();
    }

    T* find(int x, int y) {
        typename std::vector<T>::iterator it;
        it = find_if(m_container.begin(), m_container.end(), [x,y](T const &obj) { return x == obj.x && y == obj.y; });
        return it != m_container.end()?it.base():NULL;
    }
};

template<class T>
void MinHeap<T>::buildHeap(std::vector<T> &v) {
    m_container = v;
    resort();
}

struct Point {

    Point(){}
    Point(int x, int y, float g, float f,const Point* father) {
        this->x = x;
        this->y = y;
        this->g = g;
        this->f = f;
        this->father = father;
        printf("create Point{%d,%d}\n",x,y);
    }

    void update(float g, float f,const Point* father)
    {
        this->g = g;
        this->f = f;
        this->father = father;
    }

    int x, y;
    float f, g;
    const Point* father;

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

class AStart {
private:
    MinHeap<Point> openList;
    vector<Point> closeList;
    Point m_StartPoint, m_EndPoint;
    int minx,maxx,miny,maxy;
public:
    AStart( Point &startPoint, Point &endPoint) {
        m_StartPoint = startPoint;
        m_EndPoint = endPoint;
    }

    AStart(int startx,int starty,int endx,int endy) {
        m_StartPoint = Point(startx,starty,0,0,NULL);
        m_EndPoint = Point(endx,endy,0,0,NULL);
    }

    void initSize(int minx,int maxx,int miny,int maxy)
    {
        this->minx = minx;
        this->maxx = maxx;
        this->miny = miny;
        this->maxy = maxy;
    }

    void startFind() {
        openList.insert(m_StartPoint);
        while (!openList.isEmpty()) {
            Point cur = openList.extract();
            closeList.push_back(cur);
            if (cur == m_EndPoint)
                break;
            addNeighbor(&cur);
        }
        printPath();
    }

//    TODO
    bool printPath() {
        const Point* path = &m_EndPoint;
        while(NULL != path)
        {
            printf("(%d,%d)->",path->x,path->y);
            path = path->father;
        }
    }

    bool canWalkable(int x, int y) {
        return true;
    }

    void addNeighbor(const Point* currentPoint) {
        //add left
        tryAddLeft(currentPoint);
        tryAddRight(currentPoint);
        tryAddUp(currentPoint);
        tryAddDown(currentPoint);

        // 可以加斜角走
    }

    void tryAddLeft(const Point* currentPoint)
    {
        int x = currentPoint->x - 1;
        int y = currentPoint->y;
        tryAddPoint(x,y,currentPoint,leftweight);
    }
    void tryAddRight(const Point* currentPoint)
    {
        int x = currentPoint->x + 1;
        int y = currentPoint->y;
        tryAddPoint(x,y,currentPoint,rightweight);
    }

    void tryAddUp(const Point* currentPoint)
    {
        int x = currentPoint->x;
        int y = currentPoint->y - 1;
        tryAddPoint(x,y,currentPoint,upweight);
    }

    void tryAddDown(const Point* currentPoint)
    {
        int x = currentPoint->x;
        int y = currentPoint->y + 1;
        tryAddPoint(x,y,currentPoint,downweight);
    }

    void tryAddPoint(int x, int y,const Point* currentPoint,float weight)
    {
        if(x > maxx || x < minx || y > maxy || y < miny) return;
        if (canWalkable(x, y) && !hasAddedCloseList(x, y)) {
            Point* value = hasAddedOpenList(x, y);
            int newweight = currentPoint->g+weight;
            float f = newweight+ pow(m_EndPoint.x - x,2)+pow(m_EndPoint.y - y,2);

            if (NULL != value && newweight < value->g)
            {
                value->update(newweight,f, currentPoint);
                openList.resort();
            }
            else if(NULL == value){
                Point *point = new Point(x, y, newweight,f,currentPoint);
                if(x == m_EndPoint.x && y == m_EndPoint.y)
                {
                    int a = 1;
                }
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
};

int main(int argc, char const *argv[]) {
    AStart start = AStart(0,2,4,3);
    start.initSize(0,4,0,4);
    start.startFind();
    return 0;

//    /* code */
//    std::vector<int> v;
//    srand(1);
//    int capacity = 100;
//
//    for (int i = 0; i < capacity; ++i) {
//        v.push_back(random(capacity));
//    }
//    MinHeap<int> min_heap;
//    min_heap.buildHeap(v);
//    while (!min_heap.isEmpty())
//    {
//        int value = min_heap.extract();
//        printf("%d\n",value);
//    }
//
//    return 0;
}

