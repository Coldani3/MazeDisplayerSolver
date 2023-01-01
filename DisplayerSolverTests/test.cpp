#include "pch.h"
#include "../src/Includes/Maths/Coordinate.h"

Coordinate<int> coord1({ 1, 2, 3, 4 });


TEST(TestCaseName, TestName) {
	EXPECT_EQ(1, 1);
	EXPECT_TRUE(true);
}

TEST(CoordinateTests, TestCoordinatesMatchDimension) {
	Coordinate<int> testCoord({ 0, 1, 2, 3 });
	EXPECT_EQ(testCoord.x(), 0);
	EXPECT_EQ(testCoord.y(), 1);
	EXPECT_EQ(testCoord.z(), 2);
	EXPECT_EQ(testCoord.w(), 3);
}

TEST(CoordinateTests, TestEquals) {
	Coordinate<int> coord2 = coord1;

	EXPECT_EQ(coord1, coord2);
}

TEST(CoordinateTests, TestNotEquals) {
	Coordinate<int> coordNotSame({ 1, 1, 1, 1 });

	EXPECT_NE(coord1, coordNotSame);
}

TEST(CoordinateTests, TestCoordAdd) {
	Coordinate<int> coord2({ 3, 2, 1, 0 });
	Coordinate<int> added = coord1 + coord2;

	EXPECT_EQ(added, Coordinate<int>({ 4, 4, 4, 4 }));
}

TEST(CoordinateTests, TestCoordSubtract) {
	Coordinate<int> coord2({ 3, 2, 1, 0 });
	Coordinate<int> subtracted = coord1 - coord2;

	EXPECT_EQ(subtracted, Coordinate<int>({ -2, 0, 2, 4 }));
}