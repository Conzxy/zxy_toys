#ifndef _ZXY_ERRLOG_H
#define _ZXY_ERRLOG_H

#include "lexical-stream.h"
#include <stdio.h>

namespace zxy {

namespace detail {

template<typename T>
void errLogOne(SmallLexicalStream& s, T const& x)
{
	// err message always less than 4000 bytes
	// ensure by user
	auto old = s.size();
	s << x;

	if (old == s.size())
	{
		fputs(s.data(), stderr);
		s.reset();
		s << x;
	}
}

void errLogOne(SmallLexicalStream& s, StringView const& sv);
void errLogOne(SmallLexicalStream& s, char const*);

template<unsigned SZ>
void errLogOne(SmallLexicalStream& s, char const str[SZ])
{
	errLogOne(s, StringView(str, SZ));
}

template<typename... Args>
void errLogImpl(SmallLexicalStream& stream, Args const&... args)
{
	int dummy[] = {
		(errLogOne(stream, args), 0)...
	};
}

} // namespace detail

class ErrLog : noncopyable
{
public:

	template<typename ...Args>
	void operator()(Args const&... args)
	{ detail::errLogImpl(stream_, args...); }
	
	void flush()
	{
		fputs(stream_.data(), stderr);
		stream_.reset();
	}
	~ErrLog()
	{
		fputs(stream_.data(), stderr);
	}
private:
	SmallLexicalStream stream_;
};

extern ErrLog errlog;

} // namespace zxy

#endif // _ZXY_ERRLOG_H
