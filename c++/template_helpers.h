#ifndef TEMPLATE_HELPERS_H
#define TEMPLATE_HELPERS_H 1

#include <vector>
#include <list>


template<typename T>
class LessThan {

		const T val; /* value compare against */
		
		public:
			LessThan(const T& v) : val(v) {}
			bool operator()(const T& x) const { return x < val; } /* Call operator */

};

template<typename T>
class MoreThan {

		const T val; /* value compare against */
		
		public:
			MoreThan(const T& v) : val(v) {}
			bool operator()(const T& x) const { return x > val; } /* Call operator */

};

template<typename T>
class WithinRange {

		const T valMin; /* value compare against */
		const T valMax; /* value compare against */
		
		public:
			WithinRange(const T& vmin, const T& vmax) : valMin(vmin),valMax(vmax) {}
			bool operator()(const T& x) const { return (x >= valMin) && (x <= valMax); } /* Call operator */

};

using namespace std;

/* Vector with range check build in */
template<typename T>
class Vec : public vector<T> {
		
	public:
		using vector<T>::vector;

		T& operator[](int i)
			{ return vector<T>::at(i); }
			
		const T& operator[](int i) const
			{ return vector<T>::at(i); }
};

/* Iterate and find all */
template<typename T>
using Iterator = typename T::iterator;


template<typename C, typename V>
vector<Iterator<C>> find_all(C& c, V v)
{
	vector<Iterator<C>> res;
	for( auto p =c.begin(); p!= c.end(); ++p)
	{
		if (*p == v)
			res.push_back(p);
	}
	return res;
}


#endif /* TEMPLATE_HELPERS_H */
