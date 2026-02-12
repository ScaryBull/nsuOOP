#pragma once

#include <cstddef>
#include <cstdint>
#include <algorithm>
#include <stdexcept>
#include <new>
#include <functional>   // std::hash, std::equal_to
#include <initializer_list>
#include <utility>      // std::pair
#include <emmintrin.h>

// Элементы хранятся в массиве (плоское хранение), коллизии
// разрешаются методом открытой адресации.
template <
	class Key,
	class T,
	class Hash = std::hash<Key>,
	class KeyEqual = std::equal_to<Key>>
class flat_hash_map {
 public:
	using key_type = Key;
	using mapped_type = T;
	using value_type = std::pair<const Key, T>;
	using size_type = std::size_t;
	using hasher = Hash;
	using key_equal = KeyEqual;

	// Итераторы должны быть как минимум однонаправленными (ForwardIterator).
	class iterator;
	class const_iterator;

	// Конструирует пустой контейнер.
	flat_hash_map();

	// Конструирует контейнер с заданным количеством "бакетов"
	// Реальная ёмкость может отличаться, но не должна быть меньше bucket_count.
	explicit flat_hash_map(size_type bucket_count,
												 const Hash& hash = Hash(),
												 const KeyEqual& equal = KeyEqual());

	// Конструирует контейнер, заполняя его элементами из диапазона [first, last).
	template <class InputIt>
	flat_hash_map(InputIt first, InputIt last,
								size_type bucket_count = 0,
								const Hash& hash = Hash(),
								const KeyEqual& equal = KeyEqual());

	// Конструирует контейнер из списка инициализации.
	flat_hash_map(std::initializer_list<value_type> init,
								size_type bucket_count = 0,
								const Hash& hash = Hash(),
								const KeyEqual& equal = KeyEqual());

	flat_hash_map(const flat_hash_map& other);
	flat_hash_map(flat_hash_map&& other) noexcept;

	~flat_hash_map();

	flat_hash_map& operator=(const flat_hash_map& other);
	flat_hash_map& operator=(flat_hash_map&& other) noexcept;
	flat_hash_map& operator=(std::initializer_list<value_type> init);

	// Обменивает содержимое двух контейнеров.
	void swap(flat_hash_map& other) noexcept;

	// Итераторы
	iterator begin() noexcept;
	const_iterator begin() const noexcept;
	const_iterator cbegin() const noexcept;

	iterator end() noexcept;
	const_iterator end() const noexcept;
	const_iterator cend() const noexcept;

	// true, если контейнер пуст.
	bool empty() const noexcept;

	// Количество элементов в контейнере.
	size_type size() const noexcept;

	// Удаляет все элементы из контейнера.
	// Ёмкость (количество бакетов) при этом может остаться прежней.
	void clear() noexcept;

	// Вставка элементов.
	// Возвращает пару (итератор на элемент, флаг "был ли вставлен").
	std::pair<iterator, bool> insert(const value_type& value);
	std::pair<iterator, bool> insert(value_type&& value);

	template <class InputIt>
	void insert(InputIt first, InputIt last);

	void insert(std::initializer_list<value_type> init);

	// Создаёт элемент на месте, если ключ ещё не присутствует.
	template <class... Args>
	std::pair<iterator, bool> emplace(Args&&... args);

	// Удаляет элементы по ключу. Возвращает количество удалённых элементов (0 или 1).
	size_type erase(const key_type& key);

	// Удаляет элемент по итератору. Возвращает итератор на следующий элемент.
	iterator erase(iterator pos);

	// Управление ёмкостью.
	// Гарантирует, что таблица сможет вместить как минимум new_capacity элементов
	// без перераспределения.
	void reserve(size_type new_capacity);

	// Текущее количество "бакетов" (размер внутренней таблицы/массива).
	size_type capacity() const noexcept;

	// Возвращает текущий load factor (size / capacity).
	float load_factor() const noexcept;

	// Устанавливает максимально допустимый load factor.
	void max_load_factor(float ml);

	// Возвращает максимально допустимый load factor.
	float max_load_factor() const noexcept;

	// Доступ к элементам по ключу.
	// operator[] вставляет элемент с ключом key и значением по умолчанию,
	// если такого ключа ещё нет.
	mapped_type& operator[](const key_type& key);
	mapped_type& operator[](key_type&& key);

	// Бросает std::out_of_range, если ключ не найден.
	mapped_type& at(const key_type& key);
	const mapped_type& at(const key_type& key) const;

	// Поиск элементов.
	iterator find(const key_type& key);
	const_iterator find(const key_type& key) const;

	// true, если элемент с заданным ключом присутствует в контейнере.
	bool contains(const key_type& key) const;

	// Возвращает количество элементов с заданным ключом (0 или 1).
	size_type count(const key_type& key) const;

 private:
  static constexpr uint8_t kEmpty = 0;
  static constexpr uint8_t kDeleted = 1;
  static constexpr size_type kMinCapacity = 16;
  static constexpr size_type kSimdWidth = 16;
  static constexpr float kDefaultMaxLoadFactor = 0.7f;

  uint8_t* tags_;
  char* storage_;
  size_type capacity_;
  size_type size_;
  size_type deleted_count_;
  float max_load_factor_;
  Hash hash_;
  KeyEqual equal_;

  size_type compute_hash(const Key& key) const {
    return hash_(key);
  }

  static int count_trailing_zeros(int mask) {
    unsigned long index;
    _BitScanForward(&index, static_cast<unsigned long>(mask));
    return static_cast<int>(index);
  }

  uint8_t make_tag(size_type hash) const {
    uint8_t tag = static_cast<uint8_t>(hash & 0xFF);
    if (tag < 2) tag = 2;
    return tag;
  }

  value_type* slot_at(size_type index) {
    return reinterpret_cast<value_type*>(storage_ + index * sizeof(value_type));
  }

  const value_type* slot_at(size_type index) const {
    return reinterpret_cast<const value_type*>(storage_ + index * sizeof(value_type));
  }

  void allocate_table(size_type new_capacity) {
    if (new_capacity < kMinCapacity) {
      new_capacity = kMinCapacity;
    }

    size_type cap = kMinCapacity;
    while (cap < new_capacity) cap *= 2;
    new_capacity = cap;

    tags_ = new uint8_t[new_capacity + kSimdWidth];
    std::fill(tags_, tags_ + new_capacity + kSimdWidth, kEmpty);

    storage_ = new char[new_capacity * sizeof(value_type)];
    capacity_ = new_capacity;
    size_ = 0;
    deleted_count_ = 0;
  }

  void deallocate_table() {
    if (tags_) {
      for (size_type i = 0; i < capacity_; ++i) {
        if (tags_[i] != kEmpty && tags_[i] != kDeleted) {
          slot_at(i)->~value_type();
        }
      }
      delete[] tags_;
      delete[] storage_;
      tags_ = nullptr;
      storage_ = nullptr;
    }
  }

  size_type find_slot_for_insert(size_type hash, const Key& key) {
    uint8_t tag = make_tag(hash);
    size_type mask = capacity_ - 1;
    size_type index = hash & mask;
    size_type first_deleted = capacity_;

    __m128i tag_vec = _mm_set1_epi8(tag);
    __m128i empty_vec = _mm_set1_epi8(kEmpty);

    for (size_type probe = 0; probe < capacity_; probe += kSimdWidth) {
      size_type pos = (index + probe) & mask;

      __m128i tags_block = _mm_loadu_si128(reinterpret_cast<const __m128i*>(tags_ + pos));
      __m128i match_mask = _mm_cmpeq_epi8(tags_block, tag_vec);
      int match_bits = _mm_movemask_epi8(match_mask);
      __m128i empty_mask = _mm_cmpeq_epi8(tags_block, empty_vec);
      int empty_bits = _mm_movemask_epi8(empty_mask);

      while (match_bits) {
        int bit_pos = count_trailing_zeros(match_bits);
        size_type candidate = (pos + bit_pos) & mask;
        if (equal_(slot_at(candidate)->first, key)) {
          return candidate; 
        }

        match_bits &= (match_bits - 1);
      }
      for (int i = 0; i < kSimdWidth && pos + i < capacity_; ++i) {
        if (tags_[pos + i] == kDeleted && first_deleted == capacity_) {
          first_deleted = pos + i;
        }
      }

      if (empty_bits) {
        int bit_pos = count_trailing_zeros(empty_bits);
        size_type empty_slot = (pos + bit_pos) & mask;
        return (first_deleted != capacity_) ? first_deleted : empty_slot;
      }
    }

    return (first_deleted != capacity_) ? first_deleted : capacity_;
  }

  size_type find_slot_for_lookup(size_type hash, const Key& key) const {
    uint8_t tag = make_tag(hash);
    size_type mask = capacity_ - 1;
    size_type index = hash & mask;

    __m128i tag_vec = _mm_set1_epi8(tag);
    __m128i empty_vec = _mm_set1_epi8(kEmpty);

    for (size_type probe = 0; probe < capacity_; probe += kSimdWidth) {
      size_type pos = (index + probe) & mask;

      __m128i tags_block = _mm_loadu_si128(reinterpret_cast<const __m128i*>(tags_ + pos));
      __m128i match_mask = _mm_cmpeq_epi8(tags_block, tag_vec);
      int match_bits = _mm_movemask_epi8(match_mask);

      __m128i empty_mask = _mm_cmpeq_epi8(tags_block, empty_vec);
      int empty_bits = _mm_movemask_epi8(empty_mask);

      while (match_bits) {
        int bit_pos = count_trailing_zeros(match_bits);
        size_type candidate = (pos + bit_pos) & mask;
        if (equal_(slot_at(candidate)->first, key)) {
          return candidate;
        }
        match_bits &= (match_bits - 1);
      }

      if (empty_bits) {
        return capacity_;
      }
    }

    return capacity_;
  }

  bool needs_rehash() const {
    return static_cast<float>(size_ + 1) > capacity_ * max_load_factor_;
  }

  void rehash(size_type new_capacity) {
    uint8_t* old_tags = tags_;
    char* old_storage = storage_;
    size_type old_capacity = capacity_;
    allocate_table(new_capacity);
    for (size_type i = 0; i < old_capacity; ++i) {
      if (old_tags[i] != kEmpty && old_tags[i] != kDeleted) {
        value_type* old_value = reinterpret_cast<value_type*>(old_storage + i * sizeof(value_type));
        size_type hash = compute_hash(old_value->first);
        size_type new_index = find_slot_for_insert(hash, old_value->first);

        tags_[new_index] = make_tag(hash);
        new (slot_at(new_index)) value_type(std::move(*old_value));
        old_value->~value_type();
        ++size_;
      }
    }

    delete[] old_tags;
    delete[] old_storage;
  }

  size_type next_occupied(size_type index) const {
    while (index < capacity_ && (tags_[index] == kEmpty || tags_[index] == kDeleted)) {
      ++index;
    }
    return index;
  }
};

template <class Key, class T, class Hash, class KeyEqual>
class flat_hash_map<Key, T, Hash, KeyEqual>::iterator {
 public:
  using difference_type = std::ptrdiff_t;
  using value_type = flat_hash_map::value_type;
  using pointer = value_type*;
  using reference = value_type&;
  using iterator_category = std::forward_iterator_tag;

  iterator() : map_(nullptr), index_(0) {}
  iterator(flat_hash_map* map, size_type index) : map_(map), index_(index) {}

  reference operator*() const { return *map_->slot_at(index_); }
  pointer operator->() const { return map_->slot_at(index_); }

  iterator& operator++() {
    ++index_;
    index_ = map_->next_occupied(index_);
    return *this;
  }

  iterator operator++(int) {
    iterator tmp = *this;
    ++(*this);
    return tmp;
  }

  bool operator==(const iterator& other) const {
    return index_ == other.index_;
  }

  bool operator!=(const iterator& other) const {
    return !(*this == other);
  }

 private:
  friend class flat_hash_map;
  flat_hash_map* map_;
  size_type index_;
};

template <class Key, class T, class Hash, class KeyEqual>
class flat_hash_map<Key, T, Hash, KeyEqual>::const_iterator {
 public:
  using difference_type = std::ptrdiff_t;
  using value_type = flat_hash_map::value_type;
  using pointer = const value_type*;
  using reference = const value_type&;
  using iterator_category = std::forward_iterator_tag;

  const_iterator() : map_(nullptr), index_(0) {}
  const_iterator(const flat_hash_map* map, size_type index) : map_(map), index_(index) {}
  const_iterator(const iterator& it) : map_(it.map_), index_(it.index_) {}

  reference operator*() const { return *map_->slot_at(index_); }
  pointer operator->() const { return map_->slot_at(index_); }

  const_iterator& operator++() {
    ++index_;
    index_ = map_->next_occupied(index_);
    return *this;
  }

  const_iterator operator++(int) {
    const_iterator tmp = *this;
    ++(*this);
    return tmp;
  }

  bool operator==(const const_iterator& other) const {
    return index_ == other.index_;
  }

  bool operator!=(const const_iterator& other) const {
    return !(*this == other);
  }

 private:
  friend class flat_hash_map;
  const flat_hash_map* map_;
  size_type index_;
};

template <class Key, class T, class Hash, class KeyEqual>
flat_hash_map<Key, T, Hash, KeyEqual>::flat_hash_map()
    : tags_(nullptr), storage_(nullptr), capacity_(0), size_(0), 
      deleted_count_(0), max_load_factor_(kDefaultMaxLoadFactor),
      hash_(Hash()), equal_(KeyEqual()) {
  allocate_table(kMinCapacity);
}

template <class Key, class T, class Hash, class KeyEqual>
flat_hash_map<Key, T, Hash, KeyEqual>::flat_hash_map(
    size_type bucket_count, const Hash& hash, const KeyEqual& equal)
    : tags_(nullptr), storage_(nullptr), capacity_(0), size_(0),
      deleted_count_(0), max_load_factor_(kDefaultMaxLoadFactor),
      hash_(hash), equal_(equal) {
  allocate_table(bucket_count);
}

template <class Key, class T, class Hash, class KeyEqual>
template <class InputIt>
flat_hash_map<Key, T, Hash, KeyEqual>::flat_hash_map(
    InputIt first, InputIt last, size_type bucket_count,
    const Hash& hash, const KeyEqual& equal)
    : tags_(nullptr), storage_(nullptr), capacity_(0), size_(0),
      deleted_count_(0), max_load_factor_(kDefaultMaxLoadFactor),
      hash_(hash), equal_(equal) {
  allocate_table(bucket_count);
  insert(first, last);
}

template <class Key, class T, class Hash, class KeyEqual>
flat_hash_map<Key, T, Hash, KeyEqual>::flat_hash_map(
    std::initializer_list<value_type> init, size_type bucket_count,
    const Hash& hash, const KeyEqual& equal)
    : tags_(nullptr), storage_(nullptr), capacity_(0), size_(0),
      deleted_count_(0), max_load_factor_(kDefaultMaxLoadFactor),
      hash_(hash), equal_(equal) {
  allocate_table(bucket_count > init.size() ? bucket_count : init.size());
  insert(init);
}

template <class Key, class T, class Hash, class KeyEqual>
flat_hash_map<Key, T, Hash, KeyEqual>::flat_hash_map(const flat_hash_map& other)
    : tags_(nullptr), storage_(nullptr), capacity_(0), size_(0),
      deleted_count_(0), max_load_factor_(other.max_load_factor_),
      hash_(other.hash_), equal_(other.equal_) {
  allocate_table(other.capacity_);
  for (size_type i = 0; i < other.capacity_; ++i) {
    if (other.tags_[i] != kEmpty && other.tags_[i] != kDeleted) {
      tags_[i] = other.tags_[i];
      new (slot_at(i)) value_type(*other.slot_at(i));
      ++size_;
    }
  }
  deleted_count_ = other.deleted_count_;
}

template <class Key, class T, class Hash, class KeyEqual>
flat_hash_map<Key, T, Hash, KeyEqual>::flat_hash_map(flat_hash_map&& other) noexcept
    : tags_(other.tags_), storage_(other.storage_), capacity_(other.capacity_),
      size_(other.size_), deleted_count_(other.deleted_count_),
      max_load_factor_(other.max_load_factor_),
      hash_(std::move(other.hash_)), equal_(std::move(other.equal_)) {
  other.tags_ = nullptr;
  other.storage_ = nullptr;
  other.capacity_ = 0;
  other.size_ = 0;
  other.deleted_count_ = 0;
}

template <class Key, class T, class Hash, class KeyEqual>
flat_hash_map<Key, T, Hash, KeyEqual>::~flat_hash_map() {
  deallocate_table();
}

template <class Key, class T, class Hash, class KeyEqual>
auto flat_hash_map<Key, T, Hash, KeyEqual>::operator=(const flat_hash_map& other)
    -> flat_hash_map& {
  if (this != &other) {
    flat_hash_map tmp(other);
    swap(tmp);
  }
  return *this;
}

template <class Key, class T, class Hash, class KeyEqual>
auto flat_hash_map<Key, T, Hash, KeyEqual>::operator=(flat_hash_map&& other) noexcept
    -> flat_hash_map& {
  if (this != &other) {
    deallocate_table();
    tags_ = other.tags_;
    storage_ = other.storage_;
    capacity_ = other.capacity_;
    size_ = other.size_;
    deleted_count_ = other.deleted_count_;
    max_load_factor_ = other.max_load_factor_;
    hash_ = std::move(other.hash_);
    equal_ = std::move(other.equal_);
    
    other.tags_ = nullptr;
    other.storage_ = nullptr;
    other.capacity_ = 0;
    other.size_ = 0;
    other.deleted_count_ = 0;
  }
  return *this;
}

template <class Key, class T, class Hash, class KeyEqual>
auto flat_hash_map<Key, T, Hash, KeyEqual>::operator=(
    std::initializer_list<value_type> init) -> flat_hash_map& {
  clear();
  reserve(init.size());
  insert(init);
  return *this;
}

template <class Key, class T, class Hash, class KeyEqual>
void flat_hash_map<Key, T, Hash, KeyEqual>::swap(flat_hash_map& other) noexcept {
  std::swap(tags_, other.tags_);
  std::swap(storage_, other.storage_);
  std::swap(capacity_, other.capacity_);
  std::swap(size_, other.size_);
  std::swap(deleted_count_, other.deleted_count_);
  std::swap(max_load_factor_, other.max_load_factor_);
  std::swap(hash_, other.hash_);
  std::swap(equal_, other.equal_);
}

template <class Key, class T, class Hash, class KeyEqual>
auto flat_hash_map<Key, T, Hash, KeyEqual>::begin() noexcept -> iterator {
  return iterator(this, next_occupied(0));
}

template <class Key, class T, class Hash, class KeyEqual>
auto flat_hash_map<Key, T, Hash, KeyEqual>::begin() const noexcept -> const_iterator {
  return const_iterator(this, next_occupied(0));
}

template <class Key, class T, class Hash, class KeyEqual>
auto flat_hash_map<Key, T, Hash, KeyEqual>::cbegin() const noexcept -> const_iterator {
  return const_iterator(this, next_occupied(0));
}

template <class Key, class T, class Hash, class KeyEqual>
auto flat_hash_map<Key, T, Hash, KeyEqual>::end() noexcept -> iterator {
  return iterator(this, capacity_);
}

template <class Key, class T, class Hash, class KeyEqual>
auto flat_hash_map<Key, T, Hash, KeyEqual>::end() const noexcept -> const_iterator {
  return const_iterator(this, capacity_);
}

template <class Key, class T, class Hash, class KeyEqual>
auto flat_hash_map<Key, T, Hash, KeyEqual>::cend() const noexcept -> const_iterator {
  return const_iterator(this, capacity_);
}

template <class Key, class T, class Hash, class KeyEqual>
bool flat_hash_map<Key, T, Hash, KeyEqual>::empty() const noexcept {
  return size_ == 0;
}

template <class Key, class T, class Hash, class KeyEqual>
auto flat_hash_map<Key, T, Hash, KeyEqual>::size() const noexcept -> size_type {
  return size_;
}

template <class Key, class T, class Hash, class KeyEqual>
void flat_hash_map<Key, T, Hash, KeyEqual>::clear() noexcept {
  for (size_type i = 0; i < capacity_; ++i) {
    if (tags_[i] != kEmpty && tags_[i] != kDeleted) {
      slot_at(i)->~value_type();
    }
    tags_[i] = kEmpty;
  }

  size_ = 0;
  deleted_count_ = 0;
}

template <class Key, class T, class Hash, class KeyEqual>
auto flat_hash_map<Key, T, Hash, KeyEqual>::insert(const value_type& value)
    -> std::pair<iterator, bool> {
  if (needs_rehash()) {
    rehash(capacity_ * 2);
  }

  size_type hash = compute_hash(value.first);
  size_type index = find_slot_for_insert(hash, value.first);

  if (index == capacity_) {
    return {end(), false};
  }

  if (tags_[index] != kEmpty && tags_[index] != kDeleted) {
    return {iterator(this, index), false};
  }

  if (tags_[index] == kDeleted) {
    --deleted_count_;
  }

  tags_[index] = make_tag(hash);
  new (slot_at(index)) value_type(value);
  ++size_;

  return {iterator(this, index), true};
}

template <class Key, class T, class Hash, class KeyEqual>
auto flat_hash_map<Key, T, Hash, KeyEqual>::insert(value_type&& value)
    -> std::pair<iterator, bool> {
  if (needs_rehash()) {
    rehash(capacity_ * 2);
  }

  size_type hash = compute_hash(value.first);
  size_type index = find_slot_for_insert(hash, value.first);

  if (index == capacity_) {
    return {end(), false};
  }

  if (tags_[index] != kEmpty && tags_[index] != kDeleted) {
    return {iterator(this, index), false};
  }

  if (tags_[index] == kDeleted) {
    --deleted_count_;
  }

  tags_[index] = make_tag(hash);
  new (slot_at(index)) value_type(std::move(value));
  ++size_;

  return {iterator(this, index), true};
}

template <class Key, class T, class Hash, class KeyEqual>
template <class InputIt>
void flat_hash_map<Key, T, Hash, KeyEqual>::insert(InputIt first, InputIt last) {
  for (; first != last; ++first) {
    insert(*first);
  }
}

template <class Key, class T, class Hash, class KeyEqual>
void flat_hash_map<Key, T, Hash, KeyEqual>::insert(
    std::initializer_list<value_type> init) {
  insert(init.begin(), init.end());
}

template <class Key, class T, class Hash, class KeyEqual>
template <class... Args>
auto flat_hash_map<Key, T, Hash, KeyEqual>::emplace(Args&&... args)
    -> std::pair<iterator, bool> {
  value_type temp(std::forward<Args>(args)...);
  return insert(std::move(temp));
}

template <class Key, class T, class Hash, class KeyEqual>
auto flat_hash_map<Key, T, Hash, KeyEqual>::erase(const key_type& key) -> size_type {
  size_type hash = compute_hash(key);
  size_type index = find_slot_for_lookup(hash, key);

  if (index == capacity_) {
    return 0;
  }

  slot_at(index)->~value_type();
  tags_[index] = kDeleted;
  --size_;
  ++deleted_count_;
  if (deleted_count_ > capacity_ / 4) {
    rehash(capacity_);
  }

  return 1;
}

template <class Key, class T, class Hash, class KeyEqual>
auto flat_hash_map<Key, T, Hash, KeyEqual>::erase(iterator pos) -> iterator {
  size_type index = pos.index_;
  slot_at(index)->~value_type();
  tags_[index] = kDeleted;
  --size_;
  ++deleted_count_;

  if (deleted_count_ > capacity_ / 4) {
    rehash(capacity_);
    return begin();
  }

  return iterator(this, next_occupied(index + 1));
}

template <class Key, class T, class Hash, class KeyEqual>
void flat_hash_map<Key, T, Hash, KeyEqual>::reserve(size_type new_capacity) {
  if (new_capacity > capacity_) {
    rehash(new_capacity);
  }
}

template <class Key, class T, class Hash, class KeyEqual>
auto flat_hash_map<Key, T, Hash, KeyEqual>::capacity() const noexcept -> size_type {
  return capacity_;
}

template <class Key, class T, class Hash, class KeyEqual>
float flat_hash_map<Key, T, Hash, KeyEqual>::load_factor() const noexcept {
  return capacity_ > 0 ? static_cast<float>(size_) / capacity_ : 0.0f;
}

template <class Key, class T, class Hash, class KeyEqual>
void flat_hash_map<Key, T, Hash, KeyEqual>::max_load_factor(float ml) {
  max_load_factor_ = ml;
}

template <class Key, class T, class Hash, class KeyEqual>
float flat_hash_map<Key, T, Hash, KeyEqual>::max_load_factor() const noexcept {
  return max_load_factor_;
}

template <class Key, class T, class Hash, class KeyEqual>
auto flat_hash_map<Key, T, Hash, KeyEqual>::operator[](const key_type& key)
    -> mapped_type& {
  auto [it, inserted] = insert({key, T()});
  return it->second;
}

template <class Key, class T, class Hash, class KeyEqual>
auto flat_hash_map<Key, T, Hash, KeyEqual>::operator[](key_type&& key)
    -> mapped_type& {
  auto [it, inserted] = insert({std::move(key), T()});
  return it->second;
}

template <class Key, class T, class Hash, class KeyEqual>
auto flat_hash_map<Key, T, Hash, KeyEqual>::at(const key_type& key) -> mapped_type& {
  size_type hash = compute_hash(key);
  size_type index = find_slot_for_lookup(hash, key);
  if (index == capacity_) {
    throw std::out_of_range("flat_hash_map::at: key not found");
  }

  return slot_at(index)->second;
}

template <class Key, class T, class Hash, class KeyEqual>
auto flat_hash_map<Key, T, Hash, KeyEqual>::at(const key_type& key) const
    -> const mapped_type& {
  size_type hash = compute_hash(key);
  size_type index = find_slot_for_lookup(hash, key);
  if (index == capacity_) {
    throw std::out_of_range("flat_hash_map::at: key not found");
  }

  return slot_at(index)->second;
}

template <class Key, class T, class Hash, class KeyEqual>
auto flat_hash_map<Key, T, Hash, KeyEqual>::find(const key_type& key) -> iterator {
  size_type hash = compute_hash(key);
  size_type index = find_slot_for_lookup(hash, key);
  return index == capacity_ ? end() : iterator(this, index);
}

template <class Key, class T, class Hash, class KeyEqual>
auto flat_hash_map<Key, T, Hash, KeyEqual>::find(const key_type& key) const
    -> const_iterator {
  size_type hash = compute_hash(key);
  size_type index = find_slot_for_lookup(hash, key);
  return index == capacity_ ? end() : const_iterator(this, index);
}

template <class Key, class T, class Hash, class KeyEqual>
bool flat_hash_map<Key, T, Hash, KeyEqual>::contains(const key_type& key) const {
  return find(key) != end();
}

template <class Key, class T, class Hash, class KeyEqual>
auto flat_hash_map<Key, T, Hash, KeyEqual>::count(const key_type& key) const
    -> size_type {
  return contains(key) ? 1 : 0;
}
