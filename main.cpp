#include <array>
#include <iostream>
#include <numeric>
#include <vector>

bool sumsAreEqual(const std::vector<int32_t>& numbers) {
    const int32_t topRowSum = numbers[0] + numbers[1] + numbers[2];
    const int32_t middleRowSum = numbers[3] + numbers[4] + numbers[5];
    const int32_t bottomRowSum = numbers[6] + numbers[7] + numbers[8];

    if (topRowSum != middleRowSum or middleRowSum != bottomRowSum) {
        return false;
    }

    const int32_t leftColumnSum = numbers[0] + numbers[3] + numbers[6];
    const int32_t middleColumnSum = numbers[1] + numbers[4] + numbers[7];
    const int32_t rightColumnSum = numbers[2] + numbers[5] + numbers[8];

    if (bottomRowSum != leftColumnSum or leftColumnSum != middleColumnSum or middleColumnSum != rightColumnSum) {
        return false;
    }

    const int32_t nwSeSum = numbers[0] + numbers[4] + numbers[8];
    const int32_t swNeSum = numbers[6] + numbers[4] + numbers[2];

    return rightColumnSum == nwSeSum and nwSeSum == swNeSum;
}

std::vector<int32_t> generateSquareNumbers(const int32_t limit) {
    std::vector<int32_t> squareNumbers(limit + 1);
    std::iota(squareNumbers.begin(), squareNumbers.end(), 0);

    std::transform(squareNumbers.begin(), squareNumbers.end(), squareNumbers.begin(),
                   [](const int32_t num) { return num * num; });

    return squareNumbers;
}

std::vector<std::vector<int32_t>> generateUnfilteredIndices() {
    std::vector<std::vector<int32_t>> unfilteredIndices;

    std::vector<int32_t> indices(9);
    std::iota(indices.begin(), indices.end(), 0);

    do {
        unfilteredIndices.push_back(indices);
    } while (std::next_permutation(indices.begin(), indices.end()));

    return unfilteredIndices;
}

bool shouldKeepPermutation(const std::vector<int>& indices, const std::array<std::array<int, 9>, 7>& permutationsToIgnore) {
    return std::all_of(permutationsToIgnore.begin(), permutationsToIgnore.end(), [&](const std::array<int, 9>& permutationToIgnore) {
        return std::mismatch(indices.begin(), indices.end(), permutationToIgnore.begin()).first == indices.end();
    });
}

std::vector<std::vector<int>> filterIndices(const std::vector<std::vector<int>>& unfilteredIndices, const std::array<std::array<int, 9>, 7>& permutationsToIgnore) {
    std::vector<std::vector<int>> indices;
    for (const auto& indices_set : unfilteredIndices) {
        if (shouldKeepPermutation(indices_set, permutationsToIgnore)) {
            indices.push_back(indices_set);
        }
    }
    return indices;
}

std::vector<std::vector<int>> generateCombinations(const std::vector<int>& squareNumbers, const int combinationSize) {
    std::vector<bool> bitmask(squareNumbers.size(), false);
    std::fill_n(bitmask.begin(), combinationSize, true);

    std::vector<std::vector<int>> combinations;

    do {
        std::vector<int> temp;
        for (size_t i = 0; i < squareNumbers.size(); ++i) {
            if (bitmask[i]) {
                temp.push_back(squareNumbers[i]);
            }
        }
        combinations.push_back(temp);
    } while (std::next_permutation(bitmask.begin(), bitmask.end()));

    return combinations;
}

int main() {
    constexpr int32_t limit = 20;

    const std::vector<int32_t> squareNumbers = generateSquareNumbers(limit);

     // generate 9! permutations
     // then for every iteration, plug in a different combination of indices
     // for the squares vector

    const std::vector<std::vector<int>> unfilteredIndices = generateUnfilteredIndices();

    const std::array<std::array<int, 9>, 7> permutationsToIgnore = {{
        // rotation
        {6, 3, 0, 7, 4, 1, 8, 5, 2},
        {8, 7, 6, 5, 4, 3, 2, 1, 0},
        {2, 5, 8, 1, 4, 7, 0, 3, 6},
        // flip nw-se
        {0, 3, 6, 1, 4, 7, 2, 5, 8},
        // flip sw-ne
        {8, 5, 2, 7, 4, 1, 6, 3, 0},
        // flip n-s
        {2, 1, 0, 5, 4, 3, 8, 7, 6},
        // flip w-e
        {6, 7, 8, 3, 4, 5, 0, 1, 2},
    }};

    // for every unfiltered_index in unfiltered_indices, remove any permutations that match the permutations_to_ignore array
    std::vector<std::vector<int>> indices = filterIndices(unfilteredIndices, permutationsToIgnore);

    std::vector<std::vector<int>> result = generateCombinations(squareNumbers, 9);

//     combinations
//         .par_bridge()
//         .for_each(|combination: Vec<&int>| {
//             indices.iter().for_each(|index| {
//                 let numbers: Vec<&int> = index.iter().map(|&i| combination[i as usize]).collect();
//
//                 if sums_are_equal(&numbers) {
//                     println!("{:?}", numbers);
//                 }
//             });
//         });
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
