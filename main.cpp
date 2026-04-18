#include <chrono>
#include <cstdint>
#include <experimental/simd>
#include <iostream>
#include <vector>

namespace stdx = std::experimental;
constexpr std::size_t simd_size = stdx::simd<std::uint64_t>::size();

template <typename T> void add_simd(const std::vector<T> &a, const std::vector<T> &b, std::vector<T> &c) {
    for (std::size_t i = 0; i < a.size(); i += simd_size) {
        stdx::simd<T> va(a.data() + i, stdx::vector_aligned);
        stdx::simd<T> vb(b.data() + i, stdx::vector_aligned);
        stdx::simd<T> vc = va + vb;
        vc.copy_to(c.data() + i, stdx::vector_aligned);
    }
}

template <typename T> void add_scalar(const std::vector<T> &a, const std::vector<T> &b, std::vector<T> &c) {
    for (std::size_t i = 0; i < a.size(); ++i) {
        c.at(i) = a.at(i) + b.at(i);
    }
}

template <typename T> void Measure(std::size_t N) {
    std::vector<T> a(N, 1);
    std::vector<T> b(N, 2);
    std::vector<T> c(N);
    auto start_simd = std::chrono::high_resolution_clock::now();
    add_simd(a, b, c);
    auto end_simd = std::chrono::high_resolution_clock::now();
    auto duration_simd = std::chrono::duration_cast<std::chrono::microseconds>(end_simd - start_simd).count();
    std::cout << "Elapsed time: " << duration_simd << " microseconds" << std::endl;
    for (auto val : c) {
        if (val != 3) {
            std::cerr << "Error: expected 3, got " << static_cast<int>(val) << std::endl;
        }
    }
    auto start_scalar = std::chrono::high_resolution_clock::now();
    add_scalar(a, b, c);
    auto end_scalar = std::chrono::high_resolution_clock::now();
    auto duration_scalar = std::chrono::duration_cast<std::chrono::microseconds>(end_scalar - start_scalar).count();
    std::cout << "Elapsed time: " << duration_scalar << " microseconds" << std::endl;
    std::cout << "performance improvement: " << static_cast<double>(duration_scalar) / duration_simd << "x" << std::endl;
}

int main() {
    constexpr std::size_t N = simd_size * 100000000;
    Measure<std::uint64_t>(N);
}
