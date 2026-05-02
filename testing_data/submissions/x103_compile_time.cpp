#include <utility>

template<int N>
struct Work {
    static constexpr long long value = N * 1234567LL;
};

// Force instantiation of many Work<N>
template<int... Is>
long long expand(std::integer_sequence<int, Is...>) {
    return (Work<Is>::value + ...);
}

int main() {
    constexpr int N = 200000; // increase for more compile time
    auto result = expand(std::make_integer_sequence<int, N>{});
    return result;
}
