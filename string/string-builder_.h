#ifndef _ZXY_STRING_BUILDER_H
#define _ZXY_STRING_BUILDER_H

#include <string>
#include <utility>

namespace zxy{

class StringWrapper
{
public:
	StringWrapper(std::string const& str)
		: str_(std::move(str))
	{ }
	
	StringWrapper(char const* str)
		: str_(str)
	{ }
	
	operator std::string() const
	{ return str_; }
	
	std::string asString() const noexcept
	{ return str_; }

	std::size_t size() const noexcept
	{ return str_.size(); }

	void append(std::string const& s) noexcept
	{ str_.append(s); }
private:
	std::string str_;
};

template<typename T>
struct Concat;

template<typename A, typename B>
class StringBuilder
{
public:
	StringBuilder(A const& a, B const& b)
		: a_(a), b_(b)
	{ }
	
	std::string convert()
	{
		std::string ret;
		ret.reserve(size() + 1);
		Concat<	StringBuilder<A, B> >::appendTo(*this, ret);
				
		return ret;
	}

	std::size_t size() const noexcept
	{ return a_.size() + b_.size(); }
	
	A const& a_;
	B const& b_;
};

template<>
struct Concat< StringWrapper >
{
	using type = StringWrapper;

	template<typename T>
	static void appendTo(StringWrapper const& s, T& data)
	{
		data.append(s.asString());
	}
};

template<typename A, typename B>
struct Concat< StringBuilder<A, B> >
{
	using type = StringBuilder<A, B>;

	template<typename T>
	static void appendTo(type const& b, T& data)
	{
		Concat<A>::appendTo(b.a_, data);
		Concat<B>::appendTo(b.b_, data);
	}
};


template<typename ...Ts>
using Void_t = void;

template<typename , typename = void>
struct isConcat : std::false_type {};

template<typename T>
struct isConcat	< T, Void_t<typename Concat<T>::type> > : std::true_type {};

template<typename A, typename B, bool = isConcat<A>::value &&  isConcat<B>::value>
struct StringAdd
{
	static StringBuilder<typename Concat<A>::type, typename Concat<B>::type>
	add(A const& a, B const& b)
	{ return StringBuilder<typename Concat<A>::type, typename Concat<B>::type>{ a, b }; }
};

template<typename A, typename B>
struct StringAdd < A, B, false >
{
	static void 
	add(A const& , B const& )
	{ 
		static_assert(sizeof(A) < 0,
			"Given argument not specified concatable type");
	}
};

template<typename A, typename B>
StringBuilder<typename Concat<A>::type, typename Concat<B>::type>
operator+(A const& a, B const& b)
{
	return StringBuilder<typename Concat<A>::type, typename Concat<B>::type>{ a, b };
	//return StringAdd<A, B>::add(a, b);
}

} // namespace zxy

#endif // _ZXY_STRING_BUILDER_H
