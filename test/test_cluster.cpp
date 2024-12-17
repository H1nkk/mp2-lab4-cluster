#include "gtest.h"
#include "cluster.h"

TEST(TCluster, canCreateCluster) {
	EXPECT_NO_THROW(TCluster tc(24, 100));
}

TEST(TCluster, canperform) {
	TCluster tc(24, 100);

	EXPECT_NO_THROW(tc.perform(0.5, 4, 6, 7));
}

TEST(TCluster, cannotCreateClusterWithNegativeCoreNumber) {
	EXPECT_ANY_THROW(TCluster tc(-1, 23));
}

TEST(TCluster, cannotCreateClusterWithTooBigCoreNumber) {
	EXPECT_ANY_THROW(TCluster tc(411, 23));
}

TEST(TCluster, cannotCreateClusterWithNegativeCycleNumber) {
	EXPECT_ANY_THROW(TCluster tc(21, -4));
}

TEST(TCluster, cannotPerformWithNegativeSpawnChance) {
	TCluster tc(21, 123);
	EXPECT_ANY_THROW(tc.perform(-1, 3, 5, 4));
}

TEST(TCluster, cannotPerformWithTooBigSpawnChance) {
	TCluster tc(21, 123);
	EXPECT_ANY_THROW(tc.perform(1.1, 3, 5, 4));
}

TEST(TCluster, cannotPerformWithNegativeProgramsPerTactNumber) {
	TCluster tc(21, 123);
	EXPECT_ANY_THROW(tc.perform(0.5, -2, 5, 4));
}

TEST(TCluster, cannotPerformWithZeroMaxCoreNumber) {
	TCluster tc(21, 123);
	EXPECT_ANY_THROW(tc.perform(1, 3, 0, 4));
}

TEST(TCluster, cannotPerformWithNegativeMaxCoreNumber) {
	TCluster tc(21, 123);
	EXPECT_ANY_THROW(tc.perform(1, 3, -4, 4));
}

TEST(TCluster, cannotPerformWithZeroMaxWorkTimeNumber) {
	TCluster tc(21, 123);
	EXPECT_ANY_THROW(tc.perform(1, 3, 4, 0));
}

TEST(TCluster, cannotPerformWithNegativeMaxWorkTimeNumber) {
	TCluster tc(21, 123);
	EXPECT_ANY_THROW(tc.perform(1, 3, 4, -1));
}

//TEST(TCluster, cannotCreateClusterWithNegativeCycleNumber) {
//	EXPECT_ANY_THROW(TCluster tc(24, -7));
//}
//
//TEST(TCluster, cannotCreateClusterWithInvalidModeNumber) {
//	EXPECT_ANY_THROW(TCluster tc(24, 7, -1));
//}
//
//TEST(TCluster, cannotCreateProgramWithInvalidStartTime) {
//	EXPECT_ANY_THROW(TProgram tc("a", -1, 7, 3, 0.89));
//}
//
//TEST(TCluster, cannotCreateProgramWithInvalidCoreNumber) {
//	EXPECT_ANY_THROW(TProgram tc("a", 6, -7, 3, 0.89));
//}
//
//TEST(TCluster, cannotCreateProgramWithInvalidWorkTime) {
//	EXPECT_ANY_THROW(TProgram tc("a", 6, 7, -3, 0.89));
//}
//
//TEST(TCluster, cannotCreateProgramWithInvalidAlphaValue) {
//	EXPECT_ANY_THROW(TProgram tc("a", 6, 7, 3, 1.0001));
//}
//
//TEST(TCluster, cannotCreateClusterWithInvalidCoresNumber) {
//	EXPECT_ANY_THROW(TCluster tc(241, 100));
//}