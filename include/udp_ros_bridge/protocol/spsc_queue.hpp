#pragma once
#include <atomic>
#include <array>
#include <cstddef>
#include <cstdint>
#include <type_traits>


template <typename T, size_t Capacity>
class SPSCQueue
{
  static_assert((Capacity & (Capacity - 1)) == 0,
                "Capacity must be power of 2");

public:
  SPSCQueue()
  {
    head_.store(0, std::memory_order_relaxed);
    tail_.store(0, std::memory_order_relaxed);
  }

  // Producer (single thread)
  bool push(const T& item)
  {
    const auto head = head_.load(std::memory_order_relaxed);
    const auto next_head = (head + 1) & (Capacity - 1);

    if (next_head == tail_.load(std::memory_order_acquire))
        return false; // full

    buffer_[head] = item;
    head_.store(next_head, std::memory_order_release);
    return true;
  }

  // Producer move version (better for packets)
  bool push(T&& item)
  {
    const auto head = head_.load(std::memory_order_relaxed);
    const auto next_head = (head + 1) & (Capacity - 1);

    if (next_head == tail_.load(std::memory_order_acquire)) {
        return false;
    }

    buffer_[head] = std::move(item);
    head_.store(next_head, std::memory_order_release);
    return true;
  }

  // Consumer (single thread)
  bool pop(T& item)
  {
    const auto tail = tail_.load(std::memory_order_relaxed);

    if (tail == head_.load(std::memory_order_acquire))
        return false; // empty

    item = std::move(buffer_[tail]);
    tail_.store((tail + 1) & (Capacity - 1),
                std::memory_order_release);
    return true;
  }

  bool empty() const
  {
    return head_.load(std::memory_order_acquire) ==
            tail_.load(std::memory_order_acquire);
  }

  size_t size() const
  {
    const auto h = head_.load(std::memory_order_acquire);
    const auto t = tail_.load(std::memory_order_acquire);
    return (h - t) & (Capacity - 1);
  }

private:
  alignas(64) std::atomic<size_t> head_{0};
  alignas(64) std::atomic<size_t> tail_{0};
  alignas(64) std::array<T, Capacity> buffer_;
};