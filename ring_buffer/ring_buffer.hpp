#pragma once
#include <stddef.h>
#include <limits>
#include <stdexcept>
#include <iterator>
#include <memory>

namespace yvr{
	template<typename T, 
					 typename elem_type = typename T::value_type> 																
	class ring_buffer_iterator{
		public:
			typedef		std::bidirectional_iterator_tag				iterator_category;    // The type defined by the C++ standard that defines what operations are valid on the iterator
			typedef 	T 																		value_type; 					// The type of the ring buffer which iterator points to
			typedef		T::size_type													size_type;						// The container's index type
			typedef		T::difference_type										difference_type;			// The container's difference type
			typedef		T::pointer														pointer;							// The type of a pointer to element
			typedef		T::const_pointer											const_pointer;				// The type of a constant pointer to element
			typedef		T::reference													reference;						// The type of a reference to element
			typedef		T::const_reference										const_reference;			// The type of a constant reference to element
			typedef		ring_buffer_iterator<T>								self_type;						// The type of the class itself
			
			
			ring_buffer_iterator(value_type* buf, size_type start_pos)
			:buf_(buf), pos_(start_pos){}
			
			ring_buffer_iterator(const ring_buffer_iterator& rhs)
			:buf_(rhs.buf_),pos_(rhs.pos_){}
			
			elem_type& operator*(){
				return (*buf_)[pos_];
			}
			
			elem_type* operator->(){
				return &(operator*());
			}
			
			// ++iterator
			self_type& operator++(){
				++pos_;
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
				--pos_;
				return *this;
			}
			
			// iterator--
			self_type operator--(int){
				self_type tmp(*this);
				--(*this);
				return tmp;
			}
			
			self_type operator+(difference_type __n){
				self_type tmp(*this);
				tmp.pos_ += __n;
				return tmp;
			}
			
			self_type& operator+=(difference_type __n){
				pos_ += __n;
				return *this;
			}
			
			self_type operator-(difference_type __n){
				self_type tmp(*this);
				tmp.pos_ -= __n;
				return tmp;
			}
			
			self_type& operator-=(difference_type __n){
				pos_ -= __n;
				return *this;
			}
			
			bool operator==(const self_type& rhs){
				return (this->buf_ == rhs.buf_) && (this->pos_ == rhs.pos_); 
			}
			
			bool operator!=(const self_type& rhs){
				return !(*this == rhs);
			}
			
		private:
			value_type* buf_;
			size_type pos_;
	};


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
			typedef	ring_buffer_iterator<self_type>				    iterator;							// The type of iterator
			 
							
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
       array_(alloctaor_.allocate(capacity_)),
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
          typedef E                                           element_type;
          // non-const element type
          typedef EN                                          element_type_nc;
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
            return (this->pos_ != __c.pos_) || (this->buf_ != __c.buf_));
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

      iterator begin() const{
        return iterator(this, 0);
      }

      iterator end() const{
        return iterator(this, size());
      }

      


			
			reference front() { return array_[head_]; }
			
			const_reference front() const { return array_[head_]; }
			
			reference back() { return array_[tail_]; }
			
			const_reference back() const { return array_[tail_]; }
			
			bool empty() const { return !!contents_size_; }
			
			size_type size() const { return contents_size_; }
			
			size_type capacity() const { return array_size_; }
			
			size_type max_size() const { return std::numeric_limits<size_type>::max(); }
			
			void clear() {
				head_ = 1; 
				tail_ = contents_size_ = 0; 
			}
			
			void push_back(const value_type& __x){
				increment_tail();
				if (contents_size_ == array_size_)
					increment_head();
				array_[tail_] = __x;
			}
			
			// TODO
			void push_back(value_type&& __x){
				throw std::runtime_error("Not implemented!");
			}
			
			void pop_front(){
				_increment_head(); 
			}
			
			reference operator[](size_type __n){
				return array_[__n];
			}
			
			const_reference operator[](size_type __n) const{
				return array_[__n];
			}
			
			reference at(size_type __n){
				_range_check(__n);
				return (*this)[__n];
			}
			
			const_reference at(size_type __n) const{
				_range_check(__n);
				return (*this)[__n];
			}
			
			iterator begin() { return iterator(this, 0); }  
			
			iterator end() { return iterator(this, size()); }
		
		private:
			void _increment_head(){
				// precondition: !empty()
				++head_;
				--contents_size_;
				if(head__ = array_size_) head_ = 0;
			}
		
			void _increment_tail(){
				++tail_;
				++contents_size_;
				if(tail_ == array_size_) tail_ = 0;
			}
			
			void _range_check(size_type __n) const{
				if (__n >= this->size()){
					__throw_out_of_range_fmt(__N("ring_buffer::_range_check: __n "
					"(which is %zu)>= this->size() "
				  "(which is %zu)"), 
				  __n, this->size());
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
}
