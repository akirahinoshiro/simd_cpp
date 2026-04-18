#include <chrono>
#include <cstdint>
#include <experimental/simd>
#include <iostream>
#include <vector>

namespace stdx = std::experimental;
constexpr std::size_t simd_size = stdx::simd<std::uint8_t>::size();

void add_simd(const std::vector<std::uint8_t> &a, const std::vector<std::uint8_t> &b, std::vector<std::uint8_t> &c)
{
    for (std::size_t i = 0; i < a.size(); i += simd_size)
    {
        stdx::simd<std::uint8_t> va(a.data() + i, stdx::vector_aligned);
        stdx::simd<std::uint8_t> vb(b.data() + i, stdx::vector_aligned);
        stdx::simd<std::uint8_t> vc = va + vb;
        vc.copy_to(c.data() + i, stdx::vector_aligned);
    }
}

void add_scalar(const std::vector<std::uint8_t> &a, const std::vector<std::uint8_t> &b, std::vector<std::uint8_t> &c)
{
    for (std::size_t i = 0; i < a.size(); ++i)
    {
        c[i] = a[i] + b[i];
    }
}

int main()
{
    constexpr std::size_t N = simd_size * 100000000;
    std::vector<std::uint8_t> a(N, 1);
    std::vector<std::uint8_t> b(N, 2);
    std::vector<std::uint8_t> c(N);
    auto start = std::chrono::high_resolution_clock::now();
    add_simd(a, b, c);
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Elapsed time: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " microseconds" << std::endl;
    std::cout << "Elapsed time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " milliseconds" << std::endl;
    std::cout << "Elapsed time: " << std::chrono::duration_cast<std::chrono::seconds>(end - start).count() << " seconds" << std::endl;
    for (auto val : c)
    {
        if (val != 3)
        {
            std::cerr << "Error: expected 3, got " << static_cast<int>(val) << std::endl;
            return 1;
        }
    }
    start = std::chrono::high_resolution_clock::now();
    add_scalar(a, b, c);
    end = std::chrono::high_resolution_clock::now();
    std::cout << "Elapsed time: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " microseconds" << std::endl;
    std::cout << "Elapsed time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " milliseconds" << std::endl;
    std::cout << "Elapsed time: " << std::chrono::duration_cast<std::chrono::seconds>(end - start).count() << " seconds" << std::endl;
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
