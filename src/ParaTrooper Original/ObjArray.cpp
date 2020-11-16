
template <class Type> class CObjArray  
{
public:
	CObjArray();
	CObjArray(size_t size,size_t start_index);
	CObjArray(size_t size,const Type& t,size_t start_index);
	CObjArray(const CObjArray<Type>& darr2);
	virtual ~CObjArray();

	size_t size() const;
	int empty() const;
	size_t min_index() const;
	size_t max_index() const;

	Type& operator[] (size_t j);
	Type& operator[] (size_t j) const;
	CObjArray<Type>& operator = (const CObjArray<Type>& darr2);
	CObjArray<Type>& operator = (const Type& t);
private:
	Type* arr;
	size_t sz;
	size_t stind;
	void init(size_t size,size_t start);

};

template<class Type>
void CObjArray<Type>::init(size_t size,size_t start)
{
	sz=size;
	stind=start;
	arr=new Type[sz];
}

template<class Type>
CObjArray<Type>::CObjArray()
{
 init(0,0);
}

template<class Type>
CObjArray<Type>::CObjArray(size_t size, size_t start)
{
	init(size,start);
}

template<class Type>
CObjArray<Type>::CObjArray(size_t size, const Type& t, size_t start)
{
	init(size,start);
	for(size_t j =0;j<sz;j++)
		arr[j]=t;
}
template<class Type>
CObjArray<Type>::CObjArray(const CObjArray<Type>& daar2)
{
	init(darr2.sz,darr2.stind);
	for(size_t j=0;j<sz;j++)
		arr[j]=darr2.arr[j];
}
template<class Type>
CObjArray<Type>::~CObjArray()
{
 delete[] arr;
}

template<class Type>
size_t CObjArray<Type>::size() const
{
	return sz;
}

template<class Type>
int CObjArray<Type>::empty() const
{
	return (sz==0);
}

template<class Type>
size_t CObjArray<Type>::min_index()const
{
	return stind;
}

template<class Type>
size_t CObjArray<Type>::max_index() const
{
	if(empty())
		return min_index();
	return sz+min_index()-1;
}

template<class Type>
Type& CObjArray<Type>::operator [](size_t j)const
{
	if((j<min_index()) || (j> max_index()))
		return arr[0];
	return arr[j-min_index()];
}

template<class Type>
CObjArray<Type>& CObjArray<Type>::operator =  (const CObjArray<Type>& darr2)
{
	if(this==&darr2)
		return *this;
	delete [] arr;
	init(darr2.sz,darr2.stind);
	dor(size_t j=0;j<sz;j++)
		arr[j]=darr2.arr[j];
	return *this;
}

template<class Type>
CObjArray<Type>& CObjArray<Type>::operator =(const Type& t)
{
	delete [] arr;
	size_t old=stind;
	init(1,0);
	stind=old;
	arr[0]=t;
	return *this;
}
