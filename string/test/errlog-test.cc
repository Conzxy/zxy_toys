#include "../errlog.h"
#include <gtest/gtest.h>

using namespace zxy;

char test[3096];

char hexDigit[] = "0123456789ABCDEF";

TEST(errlogTest, errlog) {
	unsigned i = 0;
	for ( ; i < sizeof(test); ++i)
		test[i] = hexDigit[i % 16];
	test[i] = 0;

	errlog(test, '\n', test, "\n");

}

TEST(errLogTest, plain) {
	char buf[4096];

	snprintf(buf, sizeof buf, "%s\n",
			test);

	//fputs(buf, stderr);
}

int main(int argc, char* argv[])
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
