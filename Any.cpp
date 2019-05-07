struct ICloneable
{
	virtual ICloneable * clone() const = 0;
	virtual ~ICloneable() {}
};

template <typename T>
struct ValueHolder : ICloneable
{
    T data_;
    ValueHolder(T const & data_) : data_(data_) {}
    virtual ValueHolder * clone() const
    {
        return new ValueHolder(data_);
    }
};

class Any
{
    ICloneable * ptr;
public:
    Any() : ptr(0) {}
    template <typename T>
    Any(T const & value)
    {
        ptr = new ValueHolder<T>(value);
    }
    ~Any()
    {
        delete ptr;
    }
    Any(Any const & tmp)
    {
        ptr = 0;
        if (tmp.ptr)
            ptr = tmp.ptr->clone();
    }
    template <typename T>
    Any & operator=(T const & value)
    {
        delete ptr;
        ptr = new ValueHolder<T>(value);
        return *this;
    }
    Any & operator=(Any const & tmp)
    {
        if (this != &tmp)
        {
            delete ptr;
            ptr = 0;
            if (tmp.ptr)
                ptr = tmp.ptr->clone();
        }
        return *this;
    }
    template <typename T>
    T * cast() const
    {
        ValueHolder<T> * cur = dynamic_cast <ValueHolder<T> *>(ptr);
        return cur ? &cur->data_ : 0;
    }
};
