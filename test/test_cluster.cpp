#include "gtest.h"
#include "cluster.h"

TEST(TCluster, canCreateCluster) {
	vector<TProgram> v;
	for (int i = 0; i < 11; i++) {
		TProgram tp;
		tp.name = char(i + 64);
		tp.tStart = (i * 12345 + 13) % 17;
		tp.p = i % 3 + 1;
		tp.tWork = (i * 8 + 13) % 15;
		tp.alpha = double(i + 1) / 22.0;
		v.push_back(tp);
	}

	EXPECT_NO_THROW(TCluster tc(24, 100));
}

TEST(TCluster, canPerform) {
	vector<TProgram> v;
	for (int i = 0; i < 11; i++) {
		TProgram tp;
		tp.name = char(i + 64);
		tp.tStart = (i * 12345 + 13) % 17;
		tp.p = i % 3 + 1;
		tp.tWork = (i * 8 + 13) % 15 + 1;
		tp.alpha = double(i + 1) / 60.0;
		v.push_back(tp);
	}
	TCluster tc(24, 100);

	EXPECT_NO_THROW(tc.perform(v));
}

TEST(TCluster, performWorksWhenEveryProgramIsCompleted) {
	vector<TProgram> v;

	TProgram p1{ "a", 3, 4, 5, 1.0 };
	TProgram p2{ "b", 3, 18, 4, 1.0 };
	TProgram p3{ "c", 3, 2, 2, 1.0 };
	v.push_back(p1);
	v.push_back(p2);
	v.push_back(p3);

	TCluster tc(24, 10, 1);
	tc.perform(v);

	LogInfo res = tc.getLogInfo(9);
	LogInfo checkRes = { 3, 3, 0, 0.4 };
	EXPECT_EQ(checkRes, res);
}

TEST(TCluster, performWorksWhenNotEveryProgramIsCompleted) {
	vector<TProgram> v;

	TProgram p1{ "a", 0, 4, 4, 1.0 };
	TProgram p2{ "b", 3, 24, 100, 1.0 };
	v.push_back(p1);
	v.push_back(p2);

	TCluster tc(24, 10, 1);
	tc.perform(v);

	LogInfo res = tc.getLogInfo(9);
	LogInfo checkRes = { 2, 1, 1, 0.666666666666 };
	EXPECT_EQ(checkRes, res);
}

TEST(TCluster, performWorksWhenNoProgramIsStarted) {
	vector<TProgram> v;

	TProgram p1{ "a", 1230, 4, 4, 1.0 };
	TProgram p2{ "b", 3123, 24, 100, 1.0 };
	v.push_back(p1);
	v.push_back(p2);

	TCluster tc(24, 10, 1);
	tc.perform(v);

	LogInfo res = tc.getLogInfo(9);
	LogInfo checkRes = { 2, 0, 0, 0.0 };
	EXPECT_EQ(checkRes, res);
}

TEST(TCluster, performWorksWhenNoProgramIsCompleted) {
	vector<TProgram> v;

	TProgram p1{ "a", 1, 4, 10, 1.0 };
	TProgram p2{ "b", 7, 20, 100, 1.0 };
	v.push_back(p1);
	v.push_back(p2);

	TCluster tc(24, 10, 1);
	tc.perform(v);

	LogInfo res = tc.getLogInfo(9);
	LogInfo checkRes = { 2, 0, 2, 0.4 };
	EXPECT_EQ(checkRes, res);
}

TEST(TCluster, switchModeWorks) {
	vector<TProgram> v;

	TProgram p1{ "a", 1, 4, 10, 1.0 };
	TProgram p2{ "b", 7, 20, 100, 1.0 };
	v.push_back(p1);
	v.push_back(p2);

	TCluster tc(24, 10);
	tc.switchMode();
	tc.perform(v);

	LogInfo res = tc.getLogInfo(9);
	LogInfo checkRes = { 2, 0, 2, 0.4 };
	EXPECT_EQ(checkRes, res);
}

TEST(TCluster, doubleSwitchModeDoesNothing) {
	vector<TProgram> v;

	TProgram p1{ "a", 1, 4, 10, 1.0 };
	TProgram p2{ "b", 7, 20, 100, 1.0 };
	v.push_back(p1);
	v.push_back(p2);

	TCluster tc(24, 10, 1);
	tc.switchMode();
	tc.switchMode();
	tc.perform(v);

	LogInfo res = tc.getLogInfo(9);
	LogInfo checkRes = { 2, 0, 2, 0.4 };
	EXPECT_EQ(checkRes, res);
}

TEST(TCluster, performWorksWhenProgramIsCompletedOnLastCycle) {
	vector<TProgram> v;

	TProgram p1{ "a", 3, 4, 6, 1.0 };
	v.push_back(p1);

	TCluster tc(24, 10, 1);
	tc.perform(v);

	LogInfo res = tc.getLogInfo(9);
	LogInfo checkRes = { 1, 1, 0, 0.1 };
	EXPECT_EQ(checkRes, res);
}

TEST(TCluster, multiplePerformsWorks) {
	vector<TProgram> v;

	TProgram p1{ "a", 0, 4, 4, 1.0 };
	TProgram p2{ "b", 3, 24, 100, 1.0 };
	v.push_back(p1);
	v.push_back(p2);

	TCluster tc(24, 10, 1);
	tc.perform(v);
	tc.perform(v);
	tc.perform(v);
	tc.perform(v);

	LogInfo res = tc.getLogInfo(9);
	LogInfo checkRes = { 2, 1, 1, 0.666666666666 };
	EXPECT_EQ(checkRes, res);
}

TEST(TCluster, randomPerformDoesNotThrow) {
	vector<TProgram> v;

	TProgram p1{ "a", 0, 4, 4, 0.5345 };
	TProgram p2{ "b", 3, 24, 100, 0.11123 };
	v.push_back(p1);
	v.push_back(p2);

	TCluster tc(24, 1000, 0);

	EXPECT_NO_THROW(tc.perform(v));
}

TEST(TCluster, cannotGetLogInfoBeforePerform) {
	vector<TProgram> v;

	TProgram p1{ "a", 0, 4, 4, 0.5345 };
	TProgram p2{ "b", 3, 24, 100, 0.11123 };
	v.push_back(p1);
	v.push_back(p2);

	TCluster tc(24, 1000, 0);

	EXPECT_ANY_THROW(tc.getLogInfo(0));
}

TEST(TCluster, cannotGetLogInfoWithIncorrectIndex) {
	vector<TProgram> v;

	TProgram p1{ "a", 0, 4, 4, 0.5345 };
	TProgram p2{ "b", 3, 24, 100, 0.11123 };
	v.push_back(p1);
	v.push_back(p2);

	TCluster tc(24, 1000, 0);
	tc.perform(v);
	EXPECT_ANY_THROW(tc.getLogInfo(1000));
}
