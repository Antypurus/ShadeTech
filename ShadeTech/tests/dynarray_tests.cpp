#include <catch2/catch_test_macros.hpp>
#include <core/data_containers/array.h>

// ============================================================
// Test helpers
// ============================================================

namespace {

// Tracks whether move semantics were exercised.
struct MoveTracker {
    int value;
    bool moved_from = false;

    explicit MoveTracker(int v) : value(v) {}
    MoveTracker(const MoveTracker& o) : value(o.value) {}
    MoveTracker(MoveTracker&& o) noexcept : value(o.value) { o.moved_from = true; o.value = -1; }
    bool operator==(const MoveTracker& o) const { return value == o.value; }
    bool operator!=(const MoveTracker& o) const { return !(*this == o); }
};

} // namespace

// ============================================================
// Construction
// ============================================================

TEST_CASE("default constructor creates empty array", "[dynarray][construct]")
{
    SHD::DynArray<int> arr;
    REQUIRE(arr.length() == 0);
}

TEST_CASE("capacity constructor does not add elements", "[dynarray][construct]")
{
    // Pre-allocates storage but size should still be 0.
    SHD::DynArray<int> arr(8);
    REQUIRE(arr.length() == 0);
}

TEST_CASE("capacity constructor allocates sufficient memory for all elements", "[dynarray][construct][bug]")
{
    SHD::DynArray<int> arr(4);
    arr.push_back(10);
    arr.push_back(20);
    arr.push_back(30);
    arr.push_back(40);
    REQUIRE(arr.length() == 4);
    REQUIRE(arr[0] == 10);
    REQUIRE(arr[1] == 20);
    REQUIRE(arr[2] == 30);
    REQUIRE(arr[3] == 40);
}

TEST_CASE("copy constructor produces independent deep copy", "[dynarray][construct]")
{
    SHD::DynArray<int> original;
    original.push_back(1);
    original.push_back(2);
    original.push_back(3);

    SHD::DynArray<int> copy(original);

    REQUIRE(copy.length() == 3);
    REQUIRE(copy[0] == 1);
    REQUIRE(copy[1] == 2);
    REQUIRE(copy[2] == 3);

    // Mutating the copy must not affect the original.
    copy[0] = 99;
    REQUIRE(original[0] == 1);
}

TEST_CASE("move constructor transfers ownership and leaves source empty", "[dynarray][construct]")
{
    SHD::DynArray<int> source;
    source.push_back(7);
    source.push_back(8);

    SHD::DynArray<int> dest(static_cast<SHD::DynArray<int>&&>(source));

    REQUIRE(dest.length() == 2);
    REQUIRE(dest[0] == 7);
    REQUIRE(dest[1] == 8);
    REQUIRE(source.length() == 0);
}

// ============================================================
// Assignment
// ============================================================

TEST_CASE("copy assignment deep copies elements", "[dynarray][assign]")
{
    SHD::DynArray<int> a;
    a.push_back(100);
    a.push_back(200);

    SHD::DynArray<int> b;
    b = a;

    REQUIRE(b.length() == 2);
    REQUIRE(b[0] == 100);
    REQUIRE(b[1] == 200);
}

TEST_CASE("copy assignment is independent of source after copy", "[dynarray][assign]")
{
    SHD::DynArray<int> a;
    a.push_back(1);

    SHD::DynArray<int> b;
    b = a;

    b[0] = 42;
    REQUIRE(a[0] == 1);
}

TEST_CASE("copy assignment self-assignment is a no-op", "[dynarray][assign]")
{
    SHD::DynArray<int> a;
    a.push_back(5);
    a = a; // must not crash or corrupt
    REQUIRE(a.length() == 1);
    REQUIRE(a[0] == 5);
}

TEST_CASE("move assignment transfers correct element count", "[dynarray][assign][bug]")
{
    SHD::DynArray<int> src;
    src.push_back(1);
    src.push_back(2);
    src.push_back(3); // size=3, but capacity will be 4 after doubling

    SHD::DynArray<int> dst;
    dst = static_cast<SHD::DynArray<int>&&>(src);

    REQUIRE(dst.length() == 3);
    REQUIRE(dst[0] == 1);
    REQUIRE(dst[1] == 2);
    REQUIRE(dst[2] == 3);
    REQUIRE(src.length() == 0);
}

TEST_CASE("move assignment self-assignment is a no-op", "[dynarray][assign]")
{
    SHD::DynArray<int> a;
    a.push_back(9);
    a = static_cast<SHD::DynArray<int>&&>(a);
    REQUIRE(a.length() == 1);
}

// ============================================================
// push_back
// ============================================================

TEST_CASE("push_back lvalue increments length and stores element", "[dynarray][push_back]")
{
    SHD::DynArray<int> arr;
    int v = 42;
    arr.push_back(v);
    REQUIRE(arr.length() == 1);
    REQUIRE(arr[0] == 42);
}

TEST_CASE("push_back rvalue invokes move constructor on element", "[dynarray][push_back]")
{
    SHD::DynArray<MoveTracker> arr;
    MoveTracker t(55);
    arr.push_back(static_cast<MoveTracker&&>(t));
    REQUIRE(arr.length() == 1);
    REQUIRE(arr[0].value == 55);
    REQUIRE(t.moved_from);
}

TEST_CASE("push_back preserves insertion order", "[dynarray][push_back]")
{
    SHD::DynArray<int> arr;
    for (int i = 0; i < 10; ++i)
        arr.push_back(i * 10);

    REQUIRE(arr.length() == 10);
    for (usize i = 0; i < 10; ++i)
        REQUIRE(arr[i] == static_cast<int>(i) * 10);
}

TEST_CASE("push_back growth strategy doubles capacity starting from 1", "[dynarray][push_back]")
{
    // Verify that the array can hold many elements via repeated doubling
    // without crashing (exercises the resize path repeatedly).
    SHD::DynArray<int> arr;
    const int N = 33; // crosses several doubling thresholds
    for (int i = 0; i < N; ++i)
        arr.push_back(i);

    REQUIRE(arr.length() == static_cast<usize>(N));
    for (int i = 0; i < N; ++i)
        REQUIRE(arr[i] == i);
}

// ============================================================
// Element access
// ============================================================

TEST_CASE("operator[] returns correct element at each index", "[dynarray][access]")
{
    SHD::DynArray<int> arr;
    arr.push_back(10);
    arr.push_back(20);
    arr.push_back(30);

    REQUIRE(arr[0] == 10);
    REQUIRE(arr[1] == 20);
    REQUIRE(arr[2] == 30);
}

TEST_CASE("operator[] element is mutable", "[dynarray][access]")
{
    SHD::DynArray<int> arr;
    arr.push_back(1);
    arr[0] = 99;
    REQUIRE(arr[0] == 99);
}

TEST_CASE("range-for iteration yields elements in insertion order", "[dynarray][access]")
{
    SHD::DynArray<int> arr;
    arr.push_back(3);
    arr.push_back(1);
    arr.push_back(4);

    int expected[] = {3, 1, 4};
    int idx = 0;
    for (auto& v : arr)
        REQUIRE(v == expected[idx++]);

    REQUIRE(idx == 3);
}

// ============================================================
// clear
// ============================================================

TEST_CASE("clear resets length to zero", "[dynarray][clear]")
{
    SHD::DynArray<int> arr;
    arr.push_back(1);
    arr.push_back(2);
    arr.clear();
    REQUIRE(arr.length() == 0);
}

TEST_CASE("push_back works correctly after clear", "[dynarray][clear]")
{
    SHD::DynArray<int> arr;
    arr.push_back(1);
    arr.push_back(2);
    arr.clear();

    arr.push_back(99);
    REQUIRE(arr.length() == 1);
    REQUIRE(arr[0] == 99);
}

// ============================================================
// Equality
// ============================================================

TEST_CASE("equal arrays compare as equal", "[dynarray][equality]")
{
    SHD::DynArray<int> a, b;
    a.push_back(1); a.push_back(2);
    b.push_back(1); b.push_back(2);
    REQUIRE(a == b);
}

TEST_CASE("arrays of different lengths compare as not equal", "[dynarray][equality]")
{
    SHD::DynArray<int> a, b;
    a.push_back(1);
    b.push_back(1); b.push_back(2);
    REQUIRE_FALSE(a == b);
}

TEST_CASE("arrays with same length but different elements compare as not equal", "[dynarray][equality]")
{
    SHD::DynArray<int> a, b;
    a.push_back(1); a.push_back(2);
    b.push_back(1); b.push_back(9);
    REQUIRE_FALSE(a == b);
}

TEST_CASE("two empty arrays compare as equal", "[dynarray][equality]")
{
    SHD::DynArray<int> a, b;
    REQUIRE(a == b);
}

// ============================================================
// resize (internal capacity growth)
// ============================================================

TEST_CASE("resize preserves existing elements", "[dynarray][resize]")
{
    SHD::DynArray<int> arr;
    arr.push_back(10);
    arr.push_back(20);

    // resize changes capacity; length is unchanged (resize is a capacity op).
    arr.resize(16);

    REQUIRE(arr.length() == 2);
    REQUIRE(arr[0] == 10);
    REQUIRE(arr[1] == 20);
}

TEST_CASE("pop_back removes last element and decrements length", "[dynarray][pop_back]")
{
    SHD::DynArray<int> arr;
    arr.push_back(1);
    arr.push_back(2);
    arr.push_back(3);
    arr.pop_back();
    REQUIRE(arr.length() == 2);
    REQUIRE(arr[0] == 1);
    REQUIRE(arr[1] == 2);
}

// ============================================================
// TDD stubs — features not yet implemented
// These tests are skipped until the corresponding API is added.
// Once an API is implemented, remove the SKIP() call and verify.
// ============================================================
TEST_CASE("empty returns true for a default-constructed array", "[dynarray][tdd]")
{
    SKIP("empty() not yet implemented");
    // SHD::DynArray<int> arr;
    // REQUIRE(arr.empty());
    // arr.push_back(1);
    // REQUIRE_FALSE(arr.empty());
}

TEST_CASE("front returns reference to first element", "[dynarray][tdd]")
{
    SKIP("front() not yet implemented");
    // SHD::DynArray<int> arr;
    // arr.push_back(42);
    // arr.push_back(99);
    // REQUIRE(arr.front() == 42);
    // arr.front() = 0;
    // REQUIRE(arr[0] == 0);
}

TEST_CASE("back returns reference to last element", "[dynarray][tdd]")
{
    SKIP("back() not yet implemented");
    // SHD::DynArray<int> arr;
    // arr.push_back(1);
    // arr.push_back(2);
    // arr.push_back(3);
    // REQUIRE(arr.back() == 3);
    // arr.back() = 100;
    // REQUIRE(arr[2] == 100);
}

TEST_CASE("reserve grows capacity without changing length", "[dynarray][tdd]")
{
    SKIP("reserve() not yet implemented");
    // SHD::DynArray<int> arr;
    // arr.push_back(1);
    // arr.reserve(64);
    // REQUIRE(arr.length() == 1); // length unchanged
    // // Subsequent push_backs up to 64 elements must not trigger a resize.
    // for (int i = 0; i < 63; ++i)
    //     arr.push_back(i);
    // REQUIRE(arr.length() == 64);
}

TEST_CASE("contains returns true when element is present", "[dynarray][tdd]")
{
    SKIP("contains() not yet implemented");
    // SHD::DynArray<int> arr;
    // arr.push_back(10);
    // arr.push_back(20);
    // arr.push_back(30);
    // REQUIRE(arr.contains(20));
    // REQUIRE_FALSE(arr.contains(99));
}

TEST_CASE("insert places element at specified index and shifts remainder", "[dynarray][tdd]")
{
    SKIP("insert() not yet implemented");
    // SHD::DynArray<int> arr;
    // arr.push_back(1);
    // arr.push_back(3);
    // arr.insert(1, 2); // insert 2 between 1 and 3
    // REQUIRE(arr.length() == 3);
    // REQUIRE(arr[0] == 1);
    // REQUIRE(arr[1] == 2);
    // REQUIRE(arr[2] == 3);
}

TEST_CASE("erase removes element at index and shifts remainder left", "[dynarray][tdd]")
{
    SKIP("erase() not yet implemented");
    // SHD::DynArray<int> arr;
    // arr.push_back(10);
    // arr.push_back(20);
    // arr.push_back(30);
    // arr.erase(1); // remove 20
    // REQUIRE(arr.length() == 2);
    // REQUIRE(arr[0] == 10);
    // REQUIRE(arr[1] == 30);
}

TEST_CASE("const operator[] allows read access on a const array", "[dynarray][tdd]")
{
    SKIP("const operator[] not yet implemented");
    // SHD::DynArray<int> arr;
    // arr.push_back(7);
    // const SHD::DynArray<int>& carr = arr;
    // REQUIRE(carr[0] == 7);
}

TEST_CASE("const begin and end enable range-for on a const array", "[dynarray][tdd]")
{
    SKIP("const begin()/end() not yet implemented");
    // SHD::DynArray<int> arr;
    // arr.push_back(1);
    // arr.push_back(2);
    // const SHD::DynArray<int>& carr = arr;
    // int sum = 0;
    // for (const auto& v : carr)
    //     sum += v;
    // REQUIRE(sum == 3);
}
