#include "flat_hash_map.h"
#include <gtest/gtest.h>
#include <string>
#include <vector>

// Хеш-функция с коллизиями
struct CollisionHash {
  std::size_t operator()(int key) const {
    return key % 10;
  }
};

// Тесты конструкторов
TEST(FlatHashMapTest, DefaultConstructor) {
  flat_hash_map<int, std::string> map;
  EXPECT_TRUE(map.empty());
  EXPECT_EQ(map.size(), 0);
  EXPECT_GE(map.capacity(), 16);
}

TEST(FlatHashMapTest, ConstructorWithBucketCount) {
  flat_hash_map<int, int> map(100);
  EXPECT_TRUE(map.empty());
  EXPECT_GE(map.capacity(), 100);
}

TEST(FlatHashMapTest, ConstructorFromRange) {
  std::vector<std::pair<int, std::string>> data = {
    {1, "one"}, {2, "two"}, {3, "three"}
  };
  flat_hash_map<int, std::string> map(data.begin(), data.end());
  
  EXPECT_EQ(map.size(), 3);
  EXPECT_EQ(map[1], "one");
  EXPECT_EQ(map[2], "two");
  EXPECT_EQ(map[3], "three");
}

TEST(FlatHashMapTest, ConstructorFromInitializerList) {
  flat_hash_map<int, std::string> map = {
    {1, "one"}, {2, "two"}, {3, "three"}
  };
  
  EXPECT_EQ(map.size(), 3);
  EXPECT_EQ(map[1], "one");
  EXPECT_EQ(map[2], "two");
}

TEST(FlatHashMapTest, CopyConstructor) {
  flat_hash_map<int, std::string> map1 = {{1, "one"}, {2, "two"}};
  flat_hash_map<int, std::string> map2(map1);
  EXPECT_EQ(map2.size(), 2);
  EXPECT_EQ(map2[1], "one");
  EXPECT_EQ(map2[2], "two");
  map2[3] = "three";
  EXPECT_FALSE(map1.contains(3));
}

TEST(FlatHashMapTest, MoveConstructor) {
  flat_hash_map<int, std::string> map1 = {{1, "one"}, {2, "two"}};
  flat_hash_map<int, std::string> map2(std::move(map1));
  
  EXPECT_EQ(map2.size(), 2);
  EXPECT_EQ(map2[1], "one");
  EXPECT_TRUE(map1.empty());
}

// Тесты операторов присваивания
TEST(FlatHashMapTest, CopyAssignment) {
  flat_hash_map<int, std::string> map1 = {{1, "one"}};
  flat_hash_map<int, std::string> map2;
  map2 = map1;
  
  EXPECT_EQ(map2.size(), 1);
  EXPECT_EQ(map2[1], "one");
}

TEST(FlatHashMapTest, MoveAssignment) {
  flat_hash_map<int, std::string> map1 = {{1, "one"}};
  flat_hash_map<int, std::string> map2;
  map2 = std::move(map1);
  
  EXPECT_EQ(map2.size(), 1);
  EXPECT_TRUE(map1.empty());
}

TEST(FlatHashMapTest, InitializerListAssignment) {
  flat_hash_map<int, std::string> map;
  map = {{1, "one"}, {2, "two"}};
  
  EXPECT_EQ(map.size(), 2);
  EXPECT_EQ(map[1], "one");
}

TEST(FlatHashMapTest, SelfAssignment) {
  flat_hash_map<int, int> map = {{1, 10}, {2, 20}};
  map = map;
  EXPECT_EQ(map.size(), 2);
  EXPECT_EQ(map[1], 10);
}

// Тесты вставки
TEST(FlatHashMapTest, InsertSingleElement) {
  flat_hash_map<int, std::string> map;
  auto [it, inserted] = map.insert({1, "one"});
  
  EXPECT_TRUE(inserted);
  EXPECT_EQ(it->first, 1);
  EXPECT_EQ(it->second, "one");
  EXPECT_EQ(map.size(), 1);
}

TEST(FlatHashMapTest, InsertDuplicate) {
  flat_hash_map<int, std::string> map = {{1, "one"}};
  auto [it, inserted] = map.insert({1, "ONE"});
  
  EXPECT_FALSE(inserted);
  EXPECT_EQ(it->second, "one");
  EXPECT_EQ(map.size(), 1);
}

TEST(FlatHashMapTest, InsertMove) {
  flat_hash_map<int, std::string> map;
  std::pair<const int, std::string> value{1, "one"};
  auto [it, inserted] = map.insert(std::move(value));
  
  EXPECT_TRUE(inserted);
  EXPECT_EQ(map[1], "one");
}

TEST(FlatHashMapTest, InsertRange) {
  flat_hash_map<int, int> map;
  std::vector<std::pair<int, int>> data = {{1, 10}, {2, 20}, {3, 30}};
  map.insert(data.begin(), data.end());
  
  EXPECT_EQ(map.size(), 3);
  EXPECT_EQ(map[2], 20);
}

TEST(FlatHashMapTest, InsertInitializerList) {
  flat_hash_map<int, int> map;
  map.insert({{1, 10}, {2, 20}});
  
  EXPECT_EQ(map.size(), 2);
  EXPECT_EQ(map[1], 10);
}

TEST(FlatHashMapTest, Emplace) {
  flat_hash_map<int, std::string> map;
  auto [it, inserted] = map.emplace(1, "one");
  
  EXPECT_TRUE(inserted);
  EXPECT_EQ(it->second, "one");
  EXPECT_EQ(map.size(), 1);
}

TEST(FlatHashMapTest, EmplaceDuplicate) {
  flat_hash_map<int, int> map = {{1, 10}};
  auto [it, inserted] = map.emplace(1, 100);
  
  EXPECT_FALSE(inserted);
  EXPECT_EQ(it->second, 10);
}

// Тесты поиска
TEST(FlatHashMapTest, Find) {
  flat_hash_map<int, std::string> map = {{1, "one"}, {2, "two"}};
  
  auto it = map.find(1);
  ASSERT_NE(it, map.end());
  EXPECT_EQ(it->second, "one");
  
  auto it2 = map.find(999);
  EXPECT_EQ(it2, map.end());
}

TEST(FlatHashMapTest, FindConst) {
  const flat_hash_map<int, std::string> map = {{1, "one"}};
  auto it = map.find(1);
  ASSERT_NE(it, map.end());
  EXPECT_EQ(it->second, "one");
}

TEST(FlatHashMapTest, Contains) {
  flat_hash_map<int, int> map = {{1, 10}, {2, 20}};
  
  EXPECT_TRUE(map.contains(1));
  EXPECT_TRUE(map.contains(2));
  EXPECT_FALSE(map.contains(3));
}

TEST(FlatHashMapTest, Count) {
  flat_hash_map<int, int> map = {{1, 10}};
  
  EXPECT_EQ(map.count(1), 1);
  EXPECT_EQ(map.count(999), 0);
}

// Тесты доступа к элементам
TEST(FlatHashMapTest, OperatorBracket) {
  flat_hash_map<int, std::string> map;
  map[1] = "one";
  map[2] = "two";

  EXPECT_EQ(map[1], "one");
  EXPECT_EQ(map.size(), 2);

  std::string& val = map[3];
  EXPECT_EQ(val, "");
  EXPECT_EQ(map.size(), 3);
}

TEST(FlatHashMapTest, OperatorBracketMove) {
  flat_hash_map<std::string, int> map;
  std::string key = "key";
  map[std::move(key)] = 42;
  
  EXPECT_EQ(map["key"], 42);
}

TEST(FlatHashMapTest, AtMethod) {
  flat_hash_map<int, std::string> map = {{1, "one"}};
  
  EXPECT_EQ(map.at(1), "one");
  EXPECT_THROW(map.at(999), std::out_of_range);
}

TEST(FlatHashMapTest, AtMethodConst) {
  const flat_hash_map<int, std::string> map = {{1, "one"}};
  
  EXPECT_EQ(map.at(1), "one");
  EXPECT_THROW(map.at(999), std::out_of_range);
}

TEST(FlatHashMapTest, AtModify) {
  flat_hash_map<int, int> map = {{1, 10}};
  map.at(1) = 100;
  EXPECT_EQ(map.at(1), 100);
}

// Тесты удаления
TEST(FlatHashMapTest, EraseByKey) {
  flat_hash_map<int, std::string> map = {{1, "one"}, {2, "two"}, {3, "three"}};
  
  size_t removed = map.erase(2);
  EXPECT_EQ(removed, 1);
  EXPECT_EQ(map.size(), 2);
  EXPECT_FALSE(map.contains(2));
  EXPECT_TRUE(map.contains(1));
  EXPECT_TRUE(map.contains(3));
}

TEST(FlatHashMapTest, EraseNonExistent) {
  flat_hash_map<int, int> map = {{1, 10}};
  
  size_t removed = map.erase(999);
  EXPECT_EQ(removed, 0);
  EXPECT_EQ(map.size(), 1);
}

TEST(FlatHashMapTest, EraseByIterator) {
  flat_hash_map<int, std::string> map = {{1, "one"}, {2, "two"}, {3, "three"}};
  
  auto it = map.find(2);
  ASSERT_NE(it, map.end());
  
  auto next_it = map.erase(it);
  EXPECT_EQ(map.size(), 2);
  EXPECT_FALSE(map.contains(2));
}

TEST(FlatHashMapTest, EraseAll) {
  flat_hash_map<int, int> map = {{1, 10}, {2, 20}, {3, 30}};
  
  map.erase(1);
  map.erase(2);
  map.erase(3);
  
  EXPECT_TRUE(map.empty());
  EXPECT_EQ(map.size(), 0);
}

// Тесты итераторов
TEST(FlatHashMapTest, IteratorTraversal) {
  flat_hash_map<int, int> map = {{1, 10}, {2, 20}, {3, 30}};
  
  std::vector<int> keys;
  for (auto& [key, value] : map) {
    keys.push_back(key);
  }
  
  EXPECT_EQ(keys.size(), 3);
  std::sort(keys.begin(), keys.end());
  EXPECT_EQ(keys, std::vector<int>({1, 2, 3}));
}

TEST(FlatHashMapTest, ConstIterator) {
  const flat_hash_map<int, std::string> map = {{1, "one"}, {2, "two"}};
  
  int count = 0;
  for (auto it = map.begin(); it != map.end(); ++it) {
    ++count;
  }
  EXPECT_EQ(count, 2);
}

TEST(FlatHashMapTest, CBeginCEnd) {
  flat_hash_map<int, int> map = {{1, 10}};
  
  auto it = map.cbegin();
  EXPECT_EQ(it->first, 1);
  ++it;
  EXPECT_EQ(it, map.cend());
}

TEST(FlatHashMapTest, EmptyIterator) {
  flat_hash_map<int, int> map;
  EXPECT_EQ(map.begin(), map.end());
}

TEST(FlatHashMapTest, IteratorModify) {
  flat_hash_map<int, int> map = {{1, 10}};
  
  for (auto& [key, value] : map) {
    value = 100;
  }
  
  EXPECT_EQ(map[1], 100);
}

// Тесты управления ёмкостью
TEST(FlatHashMapTest, EmptyAndSize) {
  flat_hash_map<int, int> map;
  EXPECT_TRUE(map.empty());
  EXPECT_EQ(map.size(), 0);
  
  map[1] = 10;
  EXPECT_FALSE(map.empty());
  EXPECT_EQ(map.size(), 1);
}

TEST(FlatHashMapTest, Clear) {
  flat_hash_map<int, std::string> map = {{1, "one"}, {2, "two"}};
  
  map.clear();
  EXPECT_TRUE(map.empty());
  EXPECT_EQ(map.size(), 0);
  EXPECT_GT(map.capacity(), 0);
}

TEST(FlatHashMapTest, Reserve) {
  flat_hash_map<int, int> map;
  size_t old_capacity = map.capacity();
  
  map.reserve(1000);
  EXPECT_GE(map.capacity(), 1000);
  EXPECT_GE(map.capacity(), old_capacity);
}

TEST(FlatHashMapTest, LoadFactor) {
  flat_hash_map<int, int> map;
  EXPECT_EQ(map.load_factor(), 0.0f);
  
  map[1] = 10;
  float lf = map.load_factor();
  EXPECT_GT(lf, 0.0f);
  EXPECT_LE(lf, 1.0f);
}

TEST(FlatHashMapTest, MaxLoadFactor) {
  flat_hash_map<int, int> map;
  EXPECT_GT(map.max_load_factor(), 0.0f);
  
  map.max_load_factor(0.5f);
  EXPECT_EQ(map.max_load_factor(), 0.5f);
}

TEST(FlatHashMapTest, AutomaticRehash) {
  flat_hash_map<int, int> map(16);
  size_t old_capacity = map.capacity();

  for (int i = 0; i < 100; ++i) {
    map[i] = i * 10;
  }
  
  EXPECT_GT(map.capacity(), old_capacity);
  EXPECT_EQ(map.size(), 100);

  for (int i = 0; i < 100; ++i) {
    EXPECT_EQ(map[i], i * 10);
  }
}

// Тесты swap
TEST(FlatHashMapTest, Swap) {
  flat_hash_map<int, std::string> map1 = {{1, "one"}};
  flat_hash_map<int, std::string> map2 = {{2, "two"}, {3, "three"}};
  
  map1.swap(map2);
  
  EXPECT_EQ(map1.size(), 2);
  EXPECT_EQ(map1[2], "two");
  
  EXPECT_EQ(map2.size(), 1);
  EXPECT_EQ(map2[1], "one");
}

// Тесты с коллизиями
TEST(FlatHashMapTest, CollisionHandling) {
  flat_hash_map<int, int, CollisionHash> map;
  map[10] = 100;
  map[20] = 200;
  map[30] = 300;
  map[11] = 110;
  
  EXPECT_EQ(map.size(), 4);
  EXPECT_EQ(map[10], 100);
  EXPECT_EQ(map[20], 200);
  EXPECT_EQ(map[30], 300);
  EXPECT_EQ(map[11], 110);
}

TEST(FlatHashMapTest, ManyCollisions) {
  flat_hash_map<int, int, CollisionHash> map;

  for (int i = 0; i < 50; i += 10) {
    map[i] = i * 10;
  }
  
  EXPECT_EQ(map.size(), 5);
  for (int i = 0; i < 50; i += 10) {
    EXPECT_EQ(map[i], i * 10);
  }
}

// Граничные случаи
TEST(FlatHashMapTest, LargeDataSet) {
  flat_hash_map<int, int> map;
  
  const int N = 10000;
  for (int i = 0; i < N; ++i) {
    map[i] = i * 2;
  }
  
  EXPECT_EQ(map.size(), N);
  
  for (int i = 0; i < N; ++i) {
    EXPECT_EQ(map[i], i * 2);
  }
}

TEST(FlatHashMapTest, StringKeys) {
  flat_hash_map<std::string, int> map;
  
  map["hello"] = 1;
  map["world"] = 2;
  map["test"] = 3;
  
  EXPECT_EQ(map["hello"], 1);
  EXPECT_TRUE(map.contains("world"));
  EXPECT_FALSE(map.contains("missing"));
}

TEST(FlatHashMapTest, ComplexValueType) {
  flat_hash_map<int, std::vector<int>> map;
  
  map[1] = {1, 2, 3};
  map[2] = {4, 5, 6};
  
  EXPECT_EQ(map[1].size(), 3);
  EXPECT_EQ(map[1][0], 1);
  EXPECT_EQ(map[2][2], 6);
}

TEST(FlatHashMapTest, InsertEraseInsert) {
  flat_hash_map<int, int> map;
  
  map[1] = 10;
  map.erase(1);
  map[1] = 20;
  
  EXPECT_EQ(map[1], 20);
  EXPECT_EQ(map.size(), 1);
}

TEST(FlatHashMapTest, ManyDeletesRehash) {
  flat_hash_map<int, int> map;
  for (int i = 0; i < 100; ++i) {
    map[i] = i;
  }

  for (int i = 0; i < 50; ++i) {
    map.erase(i);
  }
  
  EXPECT_EQ(map.size(), 50);

  for (int i = 50; i < 100; ++i) {
    EXPECT_EQ(map[i], i);
  }
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
