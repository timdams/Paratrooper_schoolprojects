
template <class Type>
class CObjArray  
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
