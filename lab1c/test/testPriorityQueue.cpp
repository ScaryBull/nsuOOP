#include <gtest/gtest.h>
#include "PriorityQueue.h"
#include <vector>
#include <stdexcept>


TEST(PriorityQueueTest, DefaultConstructorIsMaxHeap) {
    PriorityQueue pq;
    EXPECT_TRUE(pq.is_max_heap());
    EXPECT_TRUE(pq.empty());
}

TEST(PriorityQueueTest, ConstructorWithFlagSetsMode) {
    PriorityQueue maxPq(true);
    PriorityQueue minPq(false);
    EXPECT_TRUE(maxPq.is_max_heap());
    EXPECT_FALSE(minPq.is_max_heap());
}

TEST(PriorityQueueTest, MaxHeapTopReturnsMax) {
    PriorityQueue pq(true);
    pq.push(10);
    pq.push(30);
    pq.push(20);
    EXPECT_EQ(pq.top(), 30);
}

TEST(PriorityQueueTest, MinHeapTopReturnsMin) {
    PriorityQueue pq(false);
    pq.push(10);
    pq.push(30);
    pq.push(5);
    EXPECT_EQ(pq.top(), 5);
}

TEST(PriorityQueueTest, PopUpdatesMaxHeapCorrectly) {
    PriorityQueue pq;
    pq.push(1);
    pq.push(5);
    pq.push(3);
    EXPECT_EQ(pq.top(), 5);
    pq.pop();
    EXPECT_EQ(pq.top(), 3);
    pq.pop();
    EXPECT_EQ(pq.top(), 1);
    pq.pop();
    EXPECT_TRUE(pq.empty());
}

TEST(PriorityQueueTest, PopUpdatesMinHeapCorrectly) {
    PriorityQueue pq(false);
    pq.push(10);
    pq.push(1);
    pq.push(7);
    EXPECT_EQ(pq.top(), 1);
    pq.pop();
    EXPECT_EQ(pq.top(), 7);
    pq.pop();
    EXPECT_EQ(pq.top(), 10);
    pq.pop();
    EXPECT_TRUE(pq.empty());
}

TEST(PriorityQueueTest, TopOnEmptyThrows) {
    PriorityQueue pq;
    EXPECT_THROW(pq.top(), std::underflow_error);
}

TEST(PriorityQueueTest, PopOnEmptyThrows) {
    PriorityQueue pq;
    EXPECT_THROW(pq.pop(), std::underflow_error);
}

TEST(PriorityQueueTest, ConstructorFromVectorMax) {
    std::vector<int> data = {1, 5, 3, 9, 2};
    PriorityQueue pq(data, true);
    EXPECT_EQ(pq.top(), 9);
    pq.pop();
    EXPECT_EQ(pq.top(), 5);
}

TEST(PriorityQueueTest, ConstructorFromVectorMin) {
    std::vector<int> data = {10, 1, 7, 3};
    PriorityQueue pq(data, false);
    EXPECT_EQ(pq.top(), 1);
    pq.pop();
    EXPECT_EQ(pq.top(), 3);
}

TEST(PriorityQueueTest, EqualityIgnoresHeapStructure) {
    PriorityQueue pq1(true);
    pq1.push(1); pq1.push(2); pq1.push(3);

    PriorityQueue pq2(true);
    pq2.push(3); pq2.push(1); pq2.push(2);

    EXPECT_EQ(pq1, pq2);
}

TEST(PriorityQueueTest, DifferentModesAreNotEqual) {
    PriorityQueue pq1(true);
    PriorityQueue pq2(false);
    pq1.push(42);
    pq2.push(42);
    EXPECT_NE(pq1, pq2);
}

TEST(PriorityQueueTest, NotEqualOperatorWorks) {
    PriorityQueue pq1, pq2;
    pq1.push(1);
    pq2.push(2);
    EXPECT_NE(pq1, pq2);
}

TEST(PriorityQueueTest, ClearMakesQueueEmpty) {
    PriorityQueue pq;
    pq.push(100);
    pq.clear();
    EXPECT_TRUE(pq.empty());
    EXPECT_EQ(pq.size(), 0);
}

TEST(PriorityQueueTest, ReserveDoesNotAffectBehavior) {
    PriorityQueue pq;
    pq.reserve(1000);
    pq.push(42);
    EXPECT_EQ(pq.top(), 42);
    pq.pop();
    EXPECT_TRUE(pq.empty());
}

TEST(PriorityQueueTest, CopyConstructorWorks) {
    PriorityQueue pq1;
    pq1.push(10);
    pq1.push(20);
    PriorityQueue pq2 = pq1;
    EXPECT_EQ(pq1, pq2);
    EXPECT_EQ(pq2.top(), 20);
}

TEST(PriorityQueueTest, CopyAssignmentWorks) {
    PriorityQueue pq1;
    pq1.push(5);
    PriorityQueue pq2;
    pq2 = pq1;
    EXPECT_EQ(pq1, pq2);
    EXPECT_EQ(pq2.top(), 5);
}

TEST(PriorityQueueTest, MoveConstructorWorks) {
    PriorityQueue pq1;
    pq1.push(99);
    PriorityQueue pq2 = std::move(pq1);
    EXPECT_EQ(pq2.top(), 99);
    EXPECT_TRUE(pq1.empty());
    pq1.push(1);
    EXPECT_EQ(pq1.top(), 1);
}

TEST(PriorityQueueTest, MoveAssignmentWorks) {
    PriorityQueue pq1;
    pq1.push(123);
    PriorityQueue pq2;
    pq2 = std::move(pq1);
    EXPECT_EQ(pq2.top(), 123);
    EXPECT_TRUE(pq1.empty());
}

TEST(PriorityQueueTest, HandlesDuplicatesCorrectly) {
    PriorityQueue pq(true);
    pq.push(5);
    pq.push(5);
    pq.push(3);
    EXPECT_EQ(pq.top(), 5);
    pq.pop();
    EXPECT_EQ(pq.top(), 5);
    pq.pop();
    EXPECT_EQ(pq.top(), 3);
}