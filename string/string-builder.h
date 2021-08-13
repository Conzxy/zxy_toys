#ifndef _ZXY_STRING_BUILDER
#define _ZXY_STRING_BUILDER

#include <string>
#include <utility> // std::move

namespace zxy{

template<typename S, typename OP1, typename OP2>
class BasicStringBuilder;

template<typename T>
struct Concat;

template<typename S, typename Rep = S>
class BasicString
{
public:
	using data_type = S;
	using size_type = typename data_type::size_type;
	using Self = BasicString;
	
	BasicString() = default;
	
	// copy and move compiler-defined
	
	BasicString(Rep const& rep)
		: data_(std::move(rep))
	{ }
	
	BasicString(char const* str)
		: data_(str)
	{ }

	template<typename Rep2>
	BasicString(BasicString<S, Rep2> const& other)
	{
		build(other);
	}

	
	template<typename Rep2>
	Self& operator=(BasicString<S, Rep2> const& other)
	{
		build(other);
		return *this;
	}
	
	template<typename S2, typename Rep2>
	void build(BasicString<S2, Rep2> const& other)
	{
		data_.reserve(other.size() + 1);
		other.append(data_);
	}

	template<typename T = S>
	void append(T& s) const
	{ 
		data_.convert(s);
	}

	size_type size() const noexcept
	{ return data_.size(); }

	Rep const& rep() const noexcept
	{ return data_; }

	Rep& rep() noexcept
	{ return data_; }

	// convert to S(e.g. std::string) object
	operator S() const noexcept
	{ return data_; }
private:
	Rep data_;
};



template<typename S, typename OP1, typename OP2>
class BasicStringBuilder
{
public:
	using size_type = typename OP1::size_type;

	BasicStringBuilder(OP1 const& op1, OP2 const& op2)
		: lhs_(op1), rhs_(op2)
	{ }

	size_type size() const noexcept
	{ return lhs_.size() + rhs_.size(); }
	
	void convert(S& s) const 
	{
		Concat< BasicStringBuilder<S, OP1, OP2> >::appendTo(*this, s);	
	}

	OP1 const& lhs_;
	OP2 const& rhs_;
};

template<typename T>
struct Concat
{
	using type = void;

	template<typename S>
	static void  appendTo(T const&, S& )
	{
		static_assert(sizeof(T) < 0, "");
	}
};

template<typename S, typename OP1, typename OP2>
struct Concat< BasicStringBuilder<S, OP1, OP2> >
{
	using type = BasicStringBuilder<S, OP1, OP2>;

	static void appendTo(type const& builder, S& out)
	{
		Concat<OP1>::appendTo(builder.lhs_, out);
		Concat<OP2>::appendTo(builder.rhs_, out);
	}
};

template<>
struct Concat< std::string >
{
	using type = std::string; 

	static void appendTo(std::string const& s, std::string& out)
	{
		out.append(s);
	}
};

template< 
	typename S,
	typename L, 
	typename R>
BasicString<S, BasicStringBuilder<S, L, R>>
operator+(BasicString<S, L> const& lhs, BasicString<S, R> const& rhs) noexcept
{
	return BasicStringBuilder<S, L, R>{ lhs.rep(), rhs.rep() };
}

template class BasicString<std::string>;

using String = BasicString<std::string>;

} // namespace zxy

#endif // _ZXY_STRING_BUILDER
