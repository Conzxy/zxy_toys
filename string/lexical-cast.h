#ifndef _ZXY_LEXICAL_CAST_H
#define _ZXY_LEXICAL_CAST_H

#include "lexical-stream.h"
#include <string>

namespace zxy{
	
template<
	typename Dst, 
	typename Src, 
	bool = std::is_same<Dst, Src>::value>
class LexicalCast
{
public:
	static void apply(Src const& ) 
	{
		static_assert(sizeof(Src) < 0,
			"lexical_cast only support string-to-numric or numric-to-string");
	}
};

#define DST_STRING_SPECIALIZATION(type) \
template<typename Src> \
class LexicalCast<type, Src, false> \
{ \
public: \
	static type apply(Src const& src) \
	{ \
		type ret; \
		fsstream << src; \
		ret = fsstream.data(); \
		fsstream.reset(); \
		return ret; \
	} \
};

DST_STRING_SPECIALIZATION(char const*)
DST_STRING_SPECIALIZATION(std::string)

template<typename Dst, typename Src>
Dst lexical_cast(Src const& src)
{ return LexicalCast<Dst, Src>::apply(src); }

} // namespace zxy

#endif // _ZXY_LEXICAL_CAST_H
