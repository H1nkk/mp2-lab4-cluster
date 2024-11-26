#include "gtest.h"
#include "myQueue.h"

TEST(TQueue, canCreateQueue) {
	EXPECT_NO_THROW(TQueue<int> st);
}

TEST(TQueue, getSizeWorks) {
	TQueue<int> st;

	for (int i = 0; i < 10; i++) {
		st.push(i);
	}

	EXPECT_EQ(st.getSize(), 10);
}

TEST(TQueue, getSizeWorksWhenEmpty) {
	TQueue<int> st;

	for (int i = 5; i < 10; i++) {
		st.push(i);
	}
	for (int i = 5; i < 10; i++) {
		st.pop();
	}

	EXPECT_EQ(st.getSize(), 0);
}

TEST(TQueue, topWorks) {
	TQueue<int> st;

	for (int i = 6; i < 10; i++) {
		st.push(i);
	}

	EXPECT_EQ(st.top(), 6);
}

TEST(TQueue, popWorks) {
	TQueue<int> st;

	for (int i = 43; i < 67; i++) {
		st.push(i);
	}

	EXPECT_TRUE((st.pop() == 43) && (st.getSize() == (67 - 43 - 1)));
}

TEST(TQueue, isEmptyWorksWhenEmptyAndUnchanged) {
	TQueue<int> st;

	EXPECT_TRUE(st.isEmpty());
}

TEST(TQueue, isEmptyWorksWhenBecameEmpty) {
	TQueue<int> st;

	for (int i = 43; i < 67; i++) {
		st.push(i);
	}
	for (int i = 43; i < 67; i++) {
		st.pop();
	}

	EXPECT_TRUE(st.isEmpty());
}

TEST(TQueue, isEmptyWorksWhenNotEmpty) {
	TQueue<int> st;

	for (int i = 43; i < 67; i++) {
		st.push(i);
	}
	for (int i = 44; i < 67; i++) {
		st.pop();
	}

	EXPECT_FALSE(st.isEmpty());
}

TEST(TQueue, cannotPopFromEmptyQueue) {
	TQueue<int> st;

	for (int i = 43; i < 67; i++) {
		st.push(i);
	}
	for (int i = 43; i < 67; i++) {
		st.pop();
	}

	EXPECT_ANY_THROW(st.pop());
}

TEST(TQueue, cannotTakeTopOfEmptyQueue) {
	TQueue<int> st;

	for (int i = 43; i < 67; i++) {
		st.push(i);
	}
	for (int i = 43; i < 67; i++) {
		st.pop();
	}

	EXPECT_ANY_THROW(st.top());
}