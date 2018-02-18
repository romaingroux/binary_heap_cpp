
#include <iostream>
#include <vector>
#include <algorithm> // swap
#include <stdexcept>
#include <limits>


/*!
 * \brief The binary_heap class implements a maximum binary heap which elements are stored in a
 * sorted vector.
 * Changing this binary heap to a minimum binary heap only requires to modify the code in the
 * sift_up and sift_down methods.
 */
template<class T>
class binary_heap
{

    public:
        binary_heap() = delete ;
        /*!
         * \brief Constructs an empty binary heap with a given
         * maximum size.
         * \param sizeMax the maximum size of the heap.
         */
        binary_heap(size_t sizeMax) ;
        /*!
         * \brief Constructs a binary heap from a given vector.
         * The maximum size is set to the vector size.
         * \param v a vector to construct the binary heap from.
         */
        binary_heap(const std::vector<T>& v) ;

        // methods
        /*!
         * \brief Returns the maximum value of the heap.
         * \return the maximum value.
         */
        T top() const ;
        /*!
         * \brief Removes and return the maximum value of the
         * heap.
         * \return the maximum value.
         */
        T extract_top() ;

        /*!
         * \brief Insert a given value within the heap.
         * \param value a value to insert.
         * \throw std::runtime_error if the heap is full.
         */
        void insert(T value) throw(std::runtime_error) ;
        /*!
         * \brief Removes the value at the given index.
         * \param index the index of the value to remove.
         */
        void remove(int index) ;
        /*!
         * \brief Changes the priority of the value located
         * at the given index with the given value.
         * \param index the index of the priority value to change.
         * \param priority the new priority value.
         */
        void change_priority(int index, T priority) ;

        /*!
         * \brief Searches the heap for the given value and returns
         * its index. If it could not be found, -1 is returned. This
         * method is not time efficient (O(n)).
         * \param value a value to find in the heap.
         * \return the index of the value if it has been found,
         * -1 otherwise.
         */
        int find(T value) ;

        /*!
         * \brief Checks whether the heap is empty.
         * \return whether the heap is empty.
         */
        bool empty() const ;
        /*!
         * \brief Checks whether the heap is full (its
         * size is equal to the maximum size).
         * \return whether the heap is full.
         */
        bool full() const ;
        /*!
         * \brief Returns the current size of the heap.
         * \return the size of the heap.
         */
        size_t size() const ;

    public:
        // friendly functions
        /*!
         * \brief Overload the << operator to send a string representation of
         * a binary heap to a stream.
         * \param stream an output stream of interest.
         * \param h a binary heap of interest.
         * \return a reference to the stream.
         */
        template<class U>
        friend std::ostream& operator << (std::ostream& stream, const binary_heap<U>& h) ;

    private:
        // methods
        /*!
         * \brief Sifts up the element located at a given index.
         * \param index the index of the element to sift up.
         */
        void sift_up(int index) ;
        /*!
         * \brief Sifts down the element located at a given index.
         * \param index the index of the element to sift down.
         */
        void sift_down(int index) ;

        /*!
         * \brief Builds a heap from a vector. Sets the
         * maximum size to the vector size.
         * \param v a vector of interest.
         */
        void build_heap(const std::vector<T>& v) ;

        /*!
         * \brief Returns the index of the parent of the
         * element located at the given index.
         * \param index the index of the element of interest.
         * \return the index of the parent.
         */
        int parent(int index) const ;
        /*!
         * \brief Returns the index of the left child of the
         * element located at the given index.
         * \param index the index of the element of interest.
         * \return the index of the left child.
         */
        int left_child(int index) const ;
        /*!
         * \brief Returns the index of the right chold of the
         * element located at the given index.
         * \param index the index of the element of interest.
         * \return the index of the right child.
         */
        int right_child(int index) const ;

        // fields
        /*!
         * \brief The maximum size of the heap.
         */
        size_t _sizeMax ;
        /*!
         * \brief The current size of the heap.
         */
        size_t _size ;
        /*!
         * \brief The vector storing the heap.
         */
        std::vector<T> _heap ;
} ;


template<class T>
binary_heap<T>::binary_heap(size_t sizeMax)
    : _sizeMax(sizeMax), _size(0), _heap(sizeMax)
{}

template<class T>
binary_heap<T>::binary_heap(const std::vector<T>& v)
{	this->build_heap(v) ; }


template<class T>
T binary_heap<T>::top() const
{	return this->_heap[0] ; }

template<class T>
T binary_heap<T>::extract_top()
{	T top = this->_heap[0] ;
    this->_heap[0] = this->_heap[this->size()-1] ;
    this->_size-- ;
    this->sift_down(0) ;
    return top ;
}


template<class T>
void binary_heap<T>::insert(T value) throw(std::runtime_error)
{	if(this->full())
    {	throw std::runtime_error("binary_heap is full!") ; }

    this->_size++ ;
    this->_heap[this->size()-1] = value ;
    this->sift_up(this->size()-1) ;
}

template<class T>
void binary_heap<T>::remove(int index)
{	this->_heap[index] = std::numeric_limits<T>::max() ;
    this->sift_up(index) ;
    this->extract_top() ;
}


template<class T>
void binary_heap<T>::change_priority(int index, T priority)
{	T old_priority = this->_heap[index] ;
    this->_heap[index] = priority ;
    if(priority > old_priority)
    {	this->sift_up(index) ; }
    else
    {	this->sift_down(index) ; }
}


template<class T>
int binary_heap<T>::find(T value)
{   for(size_t i=0; i<this->size(); i++)
    {   if(this->_heap[i] == value)
        {   return i ; }
    }
    return -1 ;
}


template<class T>
bool binary_heap<T>::empty() const
{	return this->size() == 0 ? true : false ; }

template<class T>
bool binary_heap<T>::full() const
{	if(this->size() == this->_sizeMax)
    {	return true ; }
    return false ;
}

template<class T>
size_t binary_heap<T>::size() const
{	return this->_size ; }


template<class T>
void binary_heap<T>::sift_up(int index)
{	// std::cerr << "-- sift up " << index << " -- " << std::endl ;

    while((index > 0) and (this->_heap[index]) > this->_heap[this->parent(index)]) // change > to < for min heap
    {	std::swap(this->_heap[this->parent(index)], this->_heap[index]) ;
        index = this->parent(index) ;
    }
}

template<class T>
void binary_heap<T>::sift_down(int index)
{	// std::cerr << "-- sift down " << index << " -- " << std::endl ;
    int maxIndex = index ;

    int child_l = this->left_child(index) ;
    int child_r = this->right_child(index) ;

    if((child_l <= static_cast<int>(this->size())) and (this->_heap[child_l] > this->_heap[maxIndex])) // change > to < for min heap
    {	maxIndex = child_l ; }
    if((child_r <= static_cast<int>(this->size())) and (this->_heap[child_r] > this->_heap[maxIndex])) // change > to < for min heap
    {	maxIndex = child_r ; }
    if(index != maxIndex)
    {	std::swap(this->_heap[index], this->_heap[maxIndex]) ;
        this->sift_down(maxIndex) ;
    }
}


template<class T>
void binary_heap<T>::build_heap(const std::vector<T>& v)
{	// std::cerr << "-- build_heap -- " << std::endl ;
    this->_heap = v ;
    this->_sizeMax = v.size() ;
    this->_size = v.size() ;
    // enforce binary heap for all non-leaf nodes
    for(int i=static_cast<int>(this->size())/2; i>=0; i--)
    {	this->sift_down(i) ; }
}

template<class T>
int binary_heap<T>::parent(int index) const
{	return (index-1) / 2 ; }

template<class T>
int binary_heap<T>::left_child(int index) const
{	return (2*index) + 1 ; }

template<class T>
int binary_heap<T>::right_child(int index) const
{	return (2*index) + 2 ; }


template<class T>
std::ostream& operator << (std::ostream& stream, const binary_heap<T>& h)
{	for(const auto& i : h._heap)
    {	stream << i << ' ' ; }
    return stream ;
}

template<class T,class U>
std::ostream& operator << (std::ostream& stream, const std::pair<T,U>& p)
{	stream << '<' << p.first << ' ' << p.second << '>' << ' ' ;
    return stream ;
}
