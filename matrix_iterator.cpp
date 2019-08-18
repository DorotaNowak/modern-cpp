#include<iostream>
#include <algorithm>
#include <memory>
#include <iomanip>
#include <numeric>
#include <stdexcept> 
#include<vector>
#include<iterator>
using namespace std;


template<typename T, int N, int M>
class Matrix{
	public:
		T* coeff;
	public:
		Matrix() noexcept;
		Matrix(initializer_list<initializer_list<T>> lst) noexcept;
		constexpr int numberOfRows() const noexcept;
		constexpr int numberOfColumns() const noexcept;
		const T& operator()(int i, int j) const;
		T& operator()(int i, int j);
		Matrix(const Matrix &b) noexcept;
		~Matrix(){ delete coeff;}
		

	class iterator : public std::iterator<std::input_iterator_tag, T>{
	  T* p;
	public:
	  iterator(T* x) :p(x) {}
	  iterator(const iterator& r) : p(r.p) {}
	  iterator& operator++() {++p;return *this;}
	  iterator operator++(int) {iterator tmp(*this); operator++(); return tmp;}
	  bool operator==(const iterator& r) const {return p==r.p;}
	  bool operator!=(const iterator& r) const {return p!=r.p;}
	  T& operator*() {return *p;}
	  iterator& operator+(int i){p=p+i; return *this;
	  }
	};
	
	class row_iterator : public std::iterator<std::input_iterator_tag, T>{
	  T* p;
	public:
	  row_iterator(T* x) :p(x) {}
	  row_iterator(const iterator& mit) : p(mit.p) {}
	  row_iterator& operator++() {++p;return *this;}
	  row_iterator operator++(int) {row_iterator tmp(*this); operator++(); return tmp;}
	  bool operator==(const row_iterator& rhs) const {return p==rhs.p;}
	  bool operator!=(const row_iterator& rhs) const {return p!=rhs.p;}
	  T& operator*() {return *p;}
	};
	
	class col_iterator : public std::iterator<std::input_iterator_tag, T>{
	  T* p;
	public:
	  col_iterator(T* x) :p(x) {}
	  col_iterator(const iterator& mit) : p(mit.p) {}
	  col_iterator& operator++() {p=p+M;return *this;}
	  col_iterator operator++(int) {col_iterator tmp(*this); operator++(); return tmp;}
	  bool operator==(const col_iterator& rhs) const {return p==rhs.p;}
	  bool operator!=(const col_iterator& rhs) const {return p!=rhs.p;}
	  T& operator*() {return *p;}
	};
	
	T* begin(){ return coeff;}
	T* end(){return coeff+M*N;}
	T* row_begin(int i){ return coeff+M*i;}
	T* row_end(int i){return coeff+M*i+M;}
	T* col_begin(int i){return coeff+i;}
	T* col_end(int i){return coeff+M*N+i;}
 };

template<typename T, int N, int M>
Matrix<T, N, M>::Matrix(initializer_list<initializer_list<T>> lst) noexcept{
	cout << "constructor of " << N << "x" << M << " matrix from initializer_list" << endl;
	int i=0;
	int j=0;
	coeff = new T[N*M];
   	for(const auto& l : lst){
        for(const auto& v : l){
            coeff[i*M+j] = v;
            ++j;
        }
        j=0;
        ++i;
    }
}

template<typename T, int N, int M>
Matrix<T,N,M>::Matrix() noexcept{
	for(int i=0; i<N; i++){
		for(int j=0; j<M; j++){
			coeff[i*M+j]=0;
		}
	}
}

template<typename T, int N, int M>
constexpr int Matrix<T,N,M>::numberOfRows() const noexcept{
	return N;
}

template<typename T, int N, int M>
constexpr int Matrix<T,N,M>::numberOfColumns() const noexcept{
	return M;
}


template<typename T, int N, int M>
const T& Matrix<T,N,M>::operator()(int i, int j) const{
	if(i>this->numberOfRows() || j>this->numberOfColumns())
		throw invalid_argument("Invalid argument!");
    return coeff[i*M+j];
}

template<typename T, int N, int M>
T& Matrix<T,N,M>::operator()(int i, int j){
	if(i>this->numberOfRows() || j>this->numberOfColumns())
		throw invalid_argument("Invalid argument!");
   return coeff[i*N+j];
}

template<typename T, int N, int M>
Matrix<T,N,M>::Matrix(const Matrix& b) noexcept{
	cout << "copy constructor" << endl;
	for(int i=0; i<=N; i++){
		for(int j=0; j<=M; j++){
			coeff[i*M+j] = b.coeff[i*M+j];
		}
	}
}

template <typename M>
void printMatrix(const M& m, int width = 10){	
	for(int i = 0; i< m.numberOfRows(); ++i){
		for(int j = 0; j < m.numberOfColumns(); ++j){
			if(j != 0) 
				cout << " ";
			cout << setw(width) <<  m(i,j) ;
		}
		cout << endl;
	}
}






int main(){
	using DMatrix = Matrix<double, 3, 4>;
	DMatrix m({{1,  2,  3,  4},  // constructor from 
		       { 5,  6,  7,  8},  // initialization_list 
		       { 9, 10, 11, 12}});  
	cout <<fixed << setprecision(2);
	printMatrix(m);
	
	for(DMatrix::iterator it = m.begin(); it != m.end(); ++it)
		cout << setw(6) << *it << " ";
	
	cout << endl;
	auto sum = std::accumulate(m.begin(), m.end(), 0.0);     
	cout << " sum = " << sum << endl;
	
	auto prod = std::inner_product(m.row_begin(1), m.row_end(1), m.row_begin(2), 0.0);	
	cout << " inner product of 1st and 2nd row = " << prod << endl;
	
	for(DMatrix::iterator it = m.row_begin(1); it != m.row_end(1); ++it) //iterator - prints the first row
		cout << setw(6) << *it << " ";
	cout << endl;
	
	for(DMatrix::row_iterator it = m.row_begin(0); it != m.row_end(0); ++it) //row iterator
		cout << setw(6) << *it << " ";
	cout << endl;
	
	for(DMatrix::col_iterator it = m.col_begin(3); it != m.col_end(3); ++it) //col iterator
		cout << setw(6) << *it << " ";
	cout << endl;
	
	for(DMatrix::iterator it = m.col_begin(1); it != m.col_end(1); it=it+m.numberOfColumns()) //iterator - prints the first column
		cout << setw(6) << *it << " ";
	cout << endl;
	
	auto prod2 = std::inner_product(m.col_begin(1), m.col_end(1), m.col_begin(3), 0.0);
	cout << " inner product of 1st and 3rd collumn = " << prod2 << endl;
	
	return 0;
}
