#include <chrono>
#include <cstdint>
#include <experimental/simd>
#include <iostream>
#include <vector>

namespace stdx = std::experimental;

void add(const std::vector<std::uint8_t> &a, const std::vector<std::uint8_t> &b, std::vector<std::uint8_t> &c)
{
    stdx::simd<std::uint8_t> va(a.data(), stdx::vector_aligned);
    stdx::simd<std::uint8_t> vb(b.data(), stdx::vector_aligned);
    stdx::simd<std::uint8_t> vc = va + vb;
    vc.copy_to(c.data(), stdx::vector_aligned);
}

int main()
{
    constexpr std::size_t N = 16;
    std::vector<std::uint8_t> a(N, 1);
    std::vector<std::uint8_t> b(N, 2);
    std::vector<std::uint8_t> c(N);
    auto start = std::chrono::high_resolution_clock::now();
    add(a, b, c);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Elapsed time: " << duration.count() << " microseconds" << std::endl;
    for (auto val : c)
    {
        if (val != 3)
        {
            std::cerr << "Error: expected 3, got " << static_cast<int>(val) << std::endl;
            return 1;
        }
    }

    return 0;
}
