#include "gtest.h"
#include "cluster.h"

TEST(TQueue, canCreateQueue) {
	EXPECT_NO_THROW(TQueue<int> st);
}