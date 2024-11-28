/*
 * Copyright (c) 2024 Morwenn.
 * SPDX-License-Identifier: MIT
 */
#include <array>
#include <deque>
#include <iterator>
#include <numeric>
#include <ranges>
#include <vector>
#include <catch2/catch_test_macros.hpp>
#include <gfx/timsort.hpp>
#include "test_helpers.hpp"

TEST_CASE( "support for std::ranges::views::zip" )
{
    SECTION( "zip two small collections" ) {
        // issue #40
        std::vector<int> vec = {4, 2, 3, 1};
        std::array<char, 4> arr = {'A', 'C', 'B', 'D'};
        auto zipped = std::views::zip(vec, arr);

        gfx::timsort(
            zipped, {},
            [](std::tuple<int&, char&> const& pair) {
                return std::get<0>(pair);
            }
        );
        CHECK( std::ranges::is_sorted(vec) );
        CHECK( std::ranges::is_sorted(arr, std::ranges::greater{}) );
    }

    SECTION( "zip two big collections" ) {
        std::vector<int> vec(3000);
        std::deque<long long int> deq(3000);
        std::iota(vec.begin(), vec.end(), -500);
        std::ranges::reverse(vec);
        std::iota(deq.begin(), deq.end(), -500);

        auto zipped = std::views::zip(vec, deq);
        test_helpers::shuffle(zipped);

        gfx::timsort(zipped);
        CHECK( std::ranges::is_sorted(vec) );
        CHECK( std::ranges::is_sorted(deq, std::ranges::greater{}) );
    }
}
