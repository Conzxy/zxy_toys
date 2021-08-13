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
	static Dst apply(Src const& ) 
	{
		static_assert(sizeof(Dst) < 0,
			"lexical_cast only support string-to-numric or numric-to-string");
	}
};

#define DST_STRING_SPECIALIZATION(type) \
template<typename Src> \
class LexicalCast<type, Src, true> \
{ \
	static type apply(Src const& src) \
	{ \
		return (LexicalStream<400>{} << src).data(); \
	} \
};

DST_STRING_SPECIALIZATION(char const*)
DST_STRING_SPECIALIZATION(std::string)
} // namespace zxy

#endif // _ZXY_LEXICAL_CAST_H
