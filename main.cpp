#include <iostream>
#include <vector>
#include <cstdlib>
#include <time.h>
using namespace std;
#define  random(t) rand()%t
template <class T> class MinHeap
{
private:
	std::vector<T> m_container;
public:
	void buildHeap(std::vector<T> &v);

	void swap(T *left,T*right)
	{
        T temp = *left;
        *left = *right;
        *right = temp;
	}

	void initSort(int start,int end)
	{
		int childNode = 2*start+1;
		int parentNode = start;
        if( childNode+1 <= end && m_container[childNode] > m_container[childNode+1])
        {
		    ++childNode;
        }
		if(m_container[childNode]< m_container[parentNode])
		{
		    cout<<"swap"<<endl;
			swap(&m_container[childNode],&m_container[parentNode]);
		}
    }

	void sortup(int start,int end)
	{
		int childNode = 2*start+1;
		int parentNode = start;
		while(parentNode>=0){
			if( childNode+1 <= end && m_container[childNode]>m_container[childNode+1])
				++childNode;
			if(m_container[childNode]< m_container[parentNode])
			{
				swap(&m_container[childNode],&m_container[parentNode]);
				childNode = parentNode;
				parentNode = (childNode -1)/2;
			}
			else
			{
				return;
			}
		}
	}

	void sortdown(int start,int end)
	{
		int childNode = 2*start+1;
		int parentNode = start;
		while(childNode<=end){
			if( childNode+1 <= end && m_container[childNode]>m_container[childNode+1])
				++childNode;
			if(m_container[childNode]< m_container[parentNode])
			{
				swap(&m_container[childNode],&m_container[parentNode]);
				parentNode = childNode;
				childNode = 2*childNode+1;
			}
			else
			{
				return;
			}
		}
	}

	void insert(T &value){
		m_container.push_back(value);
		int len = m_container.size();
		sortup(len/2-1,len-1);
	}

	 bool isEmpty()
     {
        return m_container.size()==0;
     }
	 T extract()
	{
		T value = m_container.front();
		int len = m_container.size();
		m_container[0] = m_container[len-1];
		m_container.pop_back();
		sortdown(0,len-1);
		return value;
	}

	virtual void printValue() {
            for (int j = 0; j < m_container.size(); ++j) {
                std::cout << m_container[j] << std::endl;
            }
        }
};

template <class T> void MinHeap<T>::buildHeap(std::vector<T> &v)
{
	m_container = v;
	int len = m_container.size();
	for(int i = len/2-1;i>=0;--i){
		sortdown(i,len-1);
	}

}

int main(int argc, char const *argv[])
{
	/* code */
	std::vector<int> v;
	srand(1);
	int capacity = 9999999;

	for(int i = 0;i<capacity;++i){
	    int value = random(capacity);
		v.push_back(value);
    }

    MinHeap<int> min_heap;
	time_t start = time(NULL),finish;
	double duration;
	min_heap.buildHeap(v);
    finish = time(NULL);
    duration = finish - start;
    printf("build minHeap cost:%lu s\n",finish - start);
    start = time(NULL);
	while (!min_heap.isEmpty())
    {
        min_heap.extract();
    }
    finish = time(NULL);
    duration = finish - start;
    printf("extract minHeap cost:%d ms",finish - start);
	return 0;
}

