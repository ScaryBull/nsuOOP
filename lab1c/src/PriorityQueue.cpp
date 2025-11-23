#include "PriorityQueue.h"


PriorityQueue::PriorityQueue()
    : is_max_heap_(true) {}

PriorityQueue::PriorityQueue(bool max_heap)
    : is_max_heap_(max_heap) {}

PriorityQueue::PriorityQueue(const std::vector<value_type>& data, bool max_heap)
    : heap(data), is_max_heap_(max_heap) {
    for (size_type i = heap.size() / 2; i > 0; --i) {
        sift_down(i - 1);
    }
}

bool PriorityQueue::compare(value_type a, value_type b) const noexcept {
    if (is_max_heap_) {
        return a > b;
    } else {
        return a < b;
    }
}

void PriorityQueue::sift_up(size_type idx) {
    while (idx > 0) {
        size_type parent = (idx - 1) / 2;
        if (compare(heap[parent], heap[idx])) {
            break;
        }
        std::swap(heap[parent], heap[idx]);
        idx = parent;
    }
}

void PriorityQueue::sift_down(size_type idx) {
    size_type n = heap.size();
    while (true) {
        size_type left = 2 * idx + 1;
        size_type right = 2 * idx + 2;
        size_type ext = idx;

        if (left < n && !compare(heap[ext], heap[left])) {
            ext = left;
        }
        if (right < n && !compare(heap[ext], heap[right])) {
            ext = right;
        }

        if (ext == idx)
            break;
        std::swap(heap[idx], heap[ext]);
        idx = ext;
    }
}

void PriorityQueue::push(value_type x) {
    heap.push_back(x);
    sift_up(heap.size() - 1);
}

void PriorityQueue::pop() {
    if (empty()) {
        throw std::underflow_error("pop() on empty PriorityQueue");
    }
    if (heap.size() == 1) {
        heap.pop_back();
        return;
    }
    heap[0] = heap.back();
    heap.pop_back();
    sift_down(0);
}

const PriorityQueue::value_type& PriorityQueue::top() const {
    if (empty()) {
        throw std::underflow_error("top() on empty PriorityQueue");
    }
    return heap[0];
}

bool PriorityQueue::empty() const noexcept {
    return heap.empty();
}

PriorityQueue::size_type PriorityQueue::size() const noexcept {
    return heap.size();
}

void PriorityQueue::clear() noexcept {
    heap.clear();
}

void PriorityQueue::reserve(size_type n) {
    heap.reserve(n);
}

bool PriorityQueue::is_max_heap() const noexcept {
    return is_max_heap_;
}

bool PriorityQueue::operator==(const PriorityQueue& rhs) const noexcept {
    if (is_max_heap_ != rhs.is_max_heap_)
        return false;
    if (heap.size() != rhs.heap.size())
        return false;

    auto a = heap;
    auto b = rhs.heap;
    std::sort(a.begin(), a.end());
    std::sort(b.begin(), b.end());
    return a == b;
}

bool PriorityQueue::operator!=(const PriorityQueue& rhs) const noexcept {
    return !(*this == rhs);
}

std::ostream& operator<<(std::ostream& os, const PriorityQueue& pq) {
    os << "[";
    for (size_t i = 0; i < pq.heap.size(); ++i) {
        if (i > 0)
            os << ", ";
        os << pq.heap[i];
    }
    os << "]";
    return os;
}