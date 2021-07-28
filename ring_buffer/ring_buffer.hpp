#pragma once
#include <stddef.h>
#include <limits>
#include <stdexcept>
#include <iterator>
#include <memory>

namespace yvr{
	template <typename T, typename A = std::allocator<T>>
	class ring_buffer{
		public:
			typedef ring_buffer<T, A>											    self_type;						// The type of the class itself
			typedef A																			    allocator_type;				// The type of allocator

			typedef typename allocator_type::value_type 	    value_type; 					// The type of the container's elements
			typedef typename allocator_type::pointer			    pointer;							// A pointer to the element type
			typedef	typename allocator_type::const_pointer	  const_pointer;				// A constant pointer to the element type
			typedef typename allocator_type::reference			  reference;						// A reference to the element type
			typedef	typename allocator_type::const_reference  const_reference;			// A constant reference to element type
			typedef	typename allocator_type::size_type				size_type;						// The type used to index into the container
			typedef	typename allocator_type::difference_type	difference_type;			// The type of the result of subtracting two container iterators
			 
							
			 /*
        * Constructor and deconstructor
        */
			explicit ring_buffer(const size_type __n = 1, const allocator_type& allocator = allocator_type())
			:capacity_(__n),
       allocator_(allocator),
       array_(allocator_.allocate(capacity_)),
			 head_(1), 
       tail_(0),
			 contents_size_(0){

       }
			  
			~ring_buffer(){
        // Destroy or decontruct the value of content 
        clear();
        //
        allocator_.deallocate(array_, capacity_);
      }

      // Note: copy-initializer constructor must be non-explicit for operator=():
      ring_buffer(const ring_buffer& other)
      :capacity_(other.capacity_),
       allocator_(other.allocator_),
       array_(allocator_.allocate(capacity_)),
       head_(other.head_),
       tail_(other.tail_),
       contents_size_(other.contents_size_){
        try{
          this->assign_into(other.begin(), other.end());
        }
        catch(...){
          clear();
          allocator_.deallocate(array_, capacity_);
          throw std::bad_alloc();
        }
       }

      template<typename II>
      ring_buffer(const II& from, const II& to)
      :capacity_(std::distance(from, to)),
       allocator_(allocator_type()),
       array_(allocator_.allocate(capacity_)),
       head_(1),
       tail_(0),
       contents_size_(0){
        try{
          this->assign_into(from, to);
        }
        catch(...){
          clear();
          allocator_.deallocate(array_, capacity_);
          throw std::bad_alloc();
        }
      }

      // Note: copy-swap idiom using value parameter
      // So other will not be modified
      self_type& operator=(self_type other){
        other.swap(*this);
        return *this;
      }

      void swap(self_type& other){
        std::swap(array_, other.array_);
        // no need to swap the allocator member as circular_buffer types are identical:
        // std::swap(allocator_, other.allocator_);
        std::swap(head_, other.head_);
        std::swap(tail_, other.tail_);
        std::swap(contents_size_, other.contents_size_);
      }

      allocator_type get_allocator() const{
        return allocator_;
      }

      /*
       * ring_buffer iterator class
       */

      template <typename E, typename EN>
      class iterator_ : public std::iterator<std::random_access_iterator_tag, EN>{
        public:
          typedef E                                           elem_type;
          // non-const element type
          typedef EN                                          elem_type_nc;
          typedef ring_buffer<T, A>                           ring_buffer_type;
          typedef typename ring_buffer_type::difference_type  difference_type;

          /*
           * Constructor and Deconstructor
           */
          iterator_ (ring_buffer_type * const buf, const size_type& pos)
          :buf_(buf),
           pos_(pos){

          }

          iterator_ (const ring_buffer_type * const buf, const size_type& pos)
          :buf_(buf),
           pos_(pos){

          }

          // Use compiler generated copy ctor and dtor
  
          // convert from iterator to const_iterator:
  
          friend class iterator_<const elem_type, elem_type_nc>;

          iterator_ (const iterator_<elem_type_nc, elem_type_nc>& other)
          :buf_(other.buf_),
           pos_(other.pos_){

          }

          self_type& operator=(const self_type& other){
            this->buf_ = other.buf_;
            this->pos_ = other.pos_;

            return *this;
          }

          /*
           * iterator access
           */
          elem_type& operator*(){
            return (*buf_)[pos_];
          }

          elem_type* operator->(){
            return &(operator*());
          }

          /*
           * iterator modifier
           */
          // ++iterator
          self_type& operator++(){
            pos_ += 1;
            return *this;
          }

          // iterator++
          self_type operator++(int){
            self_type tmp(*this);
            ++(*this);
            return tmp;
          }

          // --iterator
          self_type& operator--(){
            pos_ -= 1;
            return *this;
          }

          // iterator--
          self_type operator--(int){
            self_type tmp(*this);
            --(*this);
            return tmp;
          }

          self_type operator+(const difference_type& __n) const{
            self_type tmp(*this);
            tmp.pos_ += __n;
            return tmp;
          }

          self_type& operator+=(const difference_type& __n){
            pos_ += __n;
            return *this;
          }

          self_type operator-(const difference_type& __n) const{
            self_type tmp(*this);
            tmp.pos_ -= __n;
            return *this;
          }

          self_type& operator-=(const difference_type& __n){
            pos_ -= __n;
            return *this;
          }

          difference_type operator-(const self_type& __c){
            return this->pos_ - __c.pos_;
          }

          bool operator==(const self_type& __c) const{
            return (this->pos_ == __c.pos_) && (this->buf_ == __c.buf_);
          }

          bool operator!=(const self_type& __c) const{
            return (this->pos_ != __c.pos_) || (this->buf_ != __c.buf_);
          }

          bool operator>(const self_type& __c) const{
            return this->pos_ > __c.pos_;
          }

          bool operator>=(const self_type& __c) const{
            return this->pos_ >= __c.pos_;
          }

          bool operator<(const self_type& __c) const{
            return this->pos_ < __c.pos_;
          }

          bool operator<=(const self_type& __c) const{
            return this->pos_ <= __c.pos_;
          }            
        private:
          ring_buffer_type* buf_;
          size_type pos_;          
      };

      /*
       * iterator
       */
      
      typedef iterator_<value_type, value_type>         iterator;
      typedef iterator_<const value_type, value_type>   const_iterator;
      typedef std::reverse_iterator<iterator>           reverse_iterator;
      typedef std::reverse_iterator<const_iterator>     const_reverse_iterator;

      iterator begin() {
        return iterator(this, 0);
      }

      iterator end() {
        return iterator(this, size());
      }

      const_iterator begin() const {
        return const_iterator(this, 0);
      }

      const_iterator end() const {
        return const_iterator(this, size());
      }

      reverse_iterator rbegin() {
        return reverse_iterator(this->end());
      }

      reverse_iterator rend() {
        return reverse_iterator(this->begin());
      }

      const_reverse_iterator rbegin() const{
        return const_reverse_iterator(this->end());
      }

      const_reverse_iterator rend() const{
        return const_reverse_iterator(this->begin());
      }

      /*
       * size accessors
       */
      bool empty() const {
        return 0 == contents_size_;
      }

      size_type capacity() const{
        return capacity_;
      }

      size_type size() const{
        return contents_size_;
      }

      size_type max_size() const{
        return allocator_.max_size();
      }

      // reserve shrinks or expands the internal buffer to the size given
      // if the buffer shrinks, keep at most the last new_size items
      void reserve(const size_type __n){
        if(__n != this->capacity()){
          self_type tmp(__n);
          const size_type offset = std::max(0, size() - __n);

          tmp.assign_into(this->begin() + offset, this->end());
          this->swap(tmp);
        }
      }

      void resize(const size_type __n) = delete;

      /*
       * content accessors
       */

			reference front() { return array_[head_]; }
			
			const_reference front() const { return array_[head_]; }
			
			reference back() { 
        return array_[tail_]; 
      }
			
			const_reference back() const { return array_[tail_]; }
			
      const_reference operator[](const size_type __n) const{
        return _at_unchecked(__n);
      }

      const_reference at(const size_type __n) const{
        return _at_checked(__n);
      }

      pointer getimpl() const{
        return array_;
      }

      /*
       * content modifiers
       */
			void clear() {
        for(size_type i = 0; i < contents_size_; ++i){
          allocator_.destroy(array_ + _index_to_subscript(i));
        }
				head_ = 1; 
				tail_ = contents_size_ = 0; 
			}

      void push_back(const value_type& __v){
        size_type next = _next_tail();
        if(contents_size_ == capacity_){
          // buffer is full, throw the element at head
          array_[next] = __v;
          _increment_head();
        }else{
          allocator_.construct(array_ + next, __v);
        }
        _increment_tail();
      }

      void pop_front(){
        size_type destroy_pos = head_;
        _increment_head();
        allocator_.destroy(array_ + destroy_pos);
      }

      reference operator[](const size_type __n){
        return _at_unchecked(__n);
      }

      reference at(const size_type __n){
        return _at_checked(__n);
      }
		private:
      reference _at_unchecked(const size_type __n) const{
        return array_[_index_to_subscript(__n)];
      }

      reference _at_checked(const size_type __n) const{
        _range_check(__n);
        return _at_unchecked(__n);
      }

      // round an unbounded to an index into m_array
      size_type _normalize(const size_type __n) const{
        return __n % capacity_;
      }
      
      // convert external index to an array subscript
      size_type _index_to_subscript(const size_type __n) const{
        return _normalize(head_ + __n);
      }

      void _increment_head(){
        --contents_size_;
        if(++head_ == capacity_) head_ = 0;
			}
		
			void _increment_tail(){
        ++contents_size_;
        tail_ = _next_tail();
			}

      size_type _next_tail() const{
        return (tail_ + 1 == capacity_) ? 0 : tail_ + 1;
      }
			
			void _range_check(size_type __n) const{
				if (__n >= this->size()){
					__throw_out_of_range_fmt(__N("ring_buffer::_range_check: __n "
					"(which is %zu)>= this->size() "
				  "(which is %zu)"), 
				  __n, this->size());
				}
			}

      template<typename I>
      void assign_into(I from, I to){
        if(contents_size_ > 0) clear();
        while(from != to){
          push_back(*from);
          ++from;
        }
      }
		
		private:
    	size_type       capacity_;
      allocator_type  allocator_;
			pointer         array_;
			size_type       head_;
			size_type       tail_;
			size_type       contents_size_;
	};

  /*
   * Comparison operators
   */
  template <typename T, typename A>
  bool operator==(const ring_buffer<T, A>& lhs, const ring_buffer<T, A>& rhs){
    return lhs.size() == rhs.size() && std::equal(lhs.begin(), lhs.end(), rhs.begin()); 
  }

  template<typename T, typename A>
  bool operator!=(const ring_buffer<T, A>& lhs, const ring_buffer<T, A>& rhs){
    return !(lhs == rhs);
  }

  template<typename T, typename A>
  bool operator<(const ring_buffer<T, A>& lhs, const ring_buffer<T, A>& rhs){
    return std::lexicographical_compare(lhs.begin(), lhs.end(),
                                        rhs.begin(), rhs.end());
  }

  template<typename T, typename A>
  typename ring_buffer<T, A>::iterator
  begin(ring_buffer<T, A>& rb){
    return rb.begin();
  }

  template<typename T, typename A>
  typename ring_buffer<T, A>::iterator
  end(ring_buffer<T, A>& rb){
    return rb.end();
  }

  template<typename T, typename A>
  typename ring_buffer<T, A>::const_iterator
  begin(ring_buffer<const T, A>& rb){
    return rb.begin();
  }

  template<typename T, typename A>
  typename ring_buffer<T, A>::const_iterator
  end(ring_buffer<const T, A>& rb){
    return rb.end();
  }      

}
