#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <utility>

using namespace std;

template<typename Container, typename Predicate>
class Filter{
	private:
	Container c;
    Predicate p;
    
    public:
    Filter(Container& c, Predicate p) : c(c), p(p){} 

    class iterator{
    	private: 
    	Container c;
        Predicate p;
        typedef remove_reference<Container> container;
        typename container::type::iterator it; 
        
        public:
        iterator(Container& c, Predicate p, typename container::type::iterator it) : c(c), p(p), it(it){}
        bool operator==(const iterator& r) const { return it==r.it; }
        bool operator!=(const iterator& r) const { return it!=r.it; }
  		const int operator*(){ return *it; }

        iterator& operator++(){
            ++it;
            while(it!=c.end() && !p(*it)){
            	++it;
			}
            return *this;
        }
    };
    
    const int operator[](int x) const { return c[x]; }

    iterator begin(){
        auto it=c.begin();
        while(!p(*it)){
        	++it;
		}
        return iterator(c,p,it);
    }

    iterator end(){
        auto it=c.end();
        return iterator(c,p,it);
    }  
};

template<typename Container, typename Predicate>
Filter<Container, Predicate> make_filter(Container&& c, Predicate p){
    Filter<Container, Predicate> new_filter (forward<Container>(c), p);
    return new_filter;
}

int main(){
	auto message = [](int x, bool v){ if(!v) cout << " - removing " << x << endl; return v; };
    auto isEven = [message](int x){return x%2 == 1; };
    auto isPrime  = [](int x){
		if(x<=1) return false;
		for(auto i = 2; i*i <= x; ++i)
			if( x % i == 0 ) return false;
	    return true;
	};
	auto addMessage = [message](auto f){ 
		return [message, f](int x){ return message(x, f(x));}; 
	};
			
	std::vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
	
	auto even = make_filter(v, addMessage(isEven));
	
	auto evenPrime = make_filter(even, addMessage(isPrime));
	
    for( auto x : evenPrime){
	   cout << x << endl;
    }
    
	cout << " ========== second pass =============\n";
	v[2] = 1001; 
	v.push_back(101);
	
	for(auto x : make_filter(evenPrime, addMessage([](int x){return x>10; }))){
	   cout << ">> " << x << endl;
    }

	return 0;
}
