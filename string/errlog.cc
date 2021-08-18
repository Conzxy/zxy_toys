#include "errlog.h"

namespace zxy {
	
namespace detail {


void errLogOne(SmallLexicalStream& s, StringView const& sv)
{
	auto old = s.size();
	s << sv;

	if(old == s.size())
	{
		fputs(sv.data(), stderr);
		fflush(stderr);
		s.reset();

		errLogOne(s, sv);
	}
	else if(sv.ends_with("\n"))
	{
		s << sv;
		//perror(s.data());
		fputs(s.data(), stderr);
		fflush(stderr);
		s.reset();
	}
}

void errLogOne(SmallLexicalStream& s, char const* str)
{ errLogOne(s, StringView(str)); }

} // namespace detail

ErrLog errlog{};

} // namespace zxy
