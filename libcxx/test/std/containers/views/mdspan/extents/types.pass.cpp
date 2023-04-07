#include <mdspan>
#include <type_traits>
#include <concepts>
#include <cassert>

#include "test_macros.h"

template <class E, class IndexType, size_t... Extents>
void testMdspan() {
  ASSERT_SAME_TYPE(typename E::index_type, IndexType);
  ASSERT_SAME_TYPE(typename E::size_type, std::make_unsigned_t<IndexType>);
  ASSERT_SAME_TYPE(typename E::rank_type, size_t);

  static_assert(sizeof...(Extents) == E::rank());
  static_assert((static_cast<size_t>(Extents == std::dynamic_extent) + ...) == E::rank_dynamic());

  static_assert(std::regular<E>);
  static_assert(std::is_trivially_copyable_v<E>);
}

template <class E, size_t rank, size_t rank_dynamic>
void test_observers() {
  ASSERT_NOEXCEPT(E::rank());
  static_assert(E::rank() == rank);
  ASSERT_NOEXCEPT(E::rank_dynamic());
  static_assert(E::rank_dynamic() == rank_dynamic);
}

template <class T>
void test() {
  testMdspan<std::extents<int, std::dynamic_extent>, int, std::dynamic_extent>();
  testMdspan<std::extents<int, 3>, int, 3>();

  constexpr size_t D = -1;
  constexpr size_t S = 5;

  test_observers<std::extents<int>, 0, 0>();

  test_observers<std::extents<int, S>, 1, 0>();
  test_observers<std::extents<int, D>, 1, 1>();

  test_observers<std::extents<int, S, S>, 2, 0>();
  test_observers<std::extents<int, S, D>, 2, 1>();
  test_observers<std::extents<int, D, S>, 2, 1>();
  test_observers<std::extents<int, D, D>, 2, 2>();

  test_observers<std::extents<int, S, S, S>, 3, 0>();
  test_observers<std::extents<int, S, S, D>, 3, 1>();
  test_observers<std::extents<int, S, D, S>, 3, 1>();
  test_observers<std::extents<int, D, S, S>, 3, 1>();
  test_observers<std::extents<int, S, D, D>, 3, 2>();
  test_observers<std::extents<int, D, S, D>, 3, 2>();
  test_observers<std::extents<int, D, D, S>, 3, 2>();
  test_observers<std::extents<int, D, D, D>, 3, 3>();

  {
    std::extents<int, 1> e;
    static_assert(e.static_extent(0) == 1);
    assert(e.static_extent(0) == 1);
    assert(e.extent(0) == 1);
  }
  {
    std::extents<int, std::dynamic_extent> e{1};
    static_assert(e.static_extent(0) == std::dynamic_extent);
    assert(e.static_extent(0) == std::dynamic_extent);
    assert(e.extent(0) == 1);
  }
  {
    std::extents<int, 1, 2> e;
    static_assert(e.static_extent(0) == 1);
    assert(e.static_extent(0) == 1);
    assert(e.extent(0) == 1);
    static_assert(e.static_extent(1) == 2);
    assert(e.static_extent(1) == 2);
    assert(e.extent(1) == 2);
  }
  {
    std::extents<int, 1, std::dynamic_extent> e{2};
    static_assert(e.static_extent(0) == 1);
    assert(e.static_extent(0) == 1);
    assert(e.extent(0) == 1);
    static_assert(e.static_extent(1) == std::dynamic_extent);
    assert(e.static_extent(1) == std::dynamic_extent);
    assert(e.extent(1) == 2);
  }
  {
    std::extents<int, std::dynamic_extent, 2> e{1};
    static_assert(e.static_extent(0) == std::dynamic_extent);
    assert(e.static_extent(0) == std::dynamic_extent);
    assert(e.extent(0) == 1);
    static_assert(e.static_extent(1) == 2);
    assert(e.static_extent(1) == 2);
    assert(e.extent(1) == 2);
  }
  {
    std::extents<int, std::dynamic_extent, std::dynamic_extent> e{1, 2};
    static_assert(e.static_extent(0) == std::dynamic_extent);
    assert(e.static_extent(0) == std::dynamic_extent);
    assert(e.extent(0) == 1);
    static_assert(e.static_extent(1) == std::dynamic_extent);
    assert(e.static_extent(1) == std::dynamic_extent);
    assert(e.extent(1) == 2);
  }

  std::array a{1, 2, 3};
  std::extents<size_t, std::dynamic_extent, std::dynamic_extent, std::dynamic_extent> e{a};
  std::extents<size_t, 2, std::dynamic_extent, std::dynamic_extent>{a};
  std::extents<size_t, std::dynamic_extent, 2, std::dynamic_extent>{a};
  std::extents<size_t, std::dynamic_extent, std::dynamic_extent, 3>{a};
}

struct A {};

int main() {
  test<int>();
  test<long>();
  test<double>();
  test<A>();
}
