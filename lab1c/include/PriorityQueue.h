#ifndef NSUOOP_PRIORITYQUEUE_H
#define NSUOOP_PRIORITYQUEUE_H

#include <vector>
#include <iostream>
#include <cstddef>
#include <algorithm>

class PriorityQueue {
public:
  using value_type = int;
  using size_type = std::size_t;

  // --- Конструкторы / деструктор ---
  PriorityQueue();                               // max-heap по умолчанию
  explicit PriorityQueue(bool max_heap);         // true = max-heap, false = min-heap
  explicit PriorityQueue(const std::vector<value_type>& data, bool max_heap = true);
  ~PriorityQueue() = default;

  PriorityQueue(const PriorityQueue& other) = default;
  PriorityQueue(PriorityQueue&& other) noexcept = default;

  PriorityQueue& operator=(const PriorityQueue& other) = default;
  PriorityQueue& operator=(PriorityQueue&& other) noexcept = default;

  // --- Основные методы ---
  void push(value_type x);        // Добавить элемент
  void pop();                     // Удалить элемент с наивысшим приоритетом
  const value_type& top() const;  // Получить элемент с наивысшим приоритетом

  bool empty() const noexcept;    // Проверка на пустоту
  size_type size() const noexcept;// Количество элементов
  void clear() noexcept;          // Очистить очередь
  void reserve(size_type n);      // Зарезервировать память под кучу

  bool is_max_heap() const noexcept; // Проверить режим работы (мин или макс куча)

  // --- Операторы сравнения ---
  bool operator==(const PriorityQueue& rhs) const noexcept;
  bool operator!=(const PriorityQueue& rhs) const noexcept;

  friend std::ostream& operator<<(std::ostream& os, const PriorityQueue& pq);

private:
  std::vector<value_type> heap;
  bool is_max_heap_;

  bool compare(value_type a, value_type b) const noexcept;
  void sift_up(size_type idx);
  void sift_down(size_type idx);
};

#endif