//
// This file is part of the MQT QCEC library released under the MIT license.
// See README.md or go to https://github.com/cda-tum/qcec for more information.
//

#pragma once

#include "ApplicationScheme.hpp"

#include <map>
#include <ranges>
#include <vector>

namespace ec {
template <class DDType, class Config>
class DiffApplicationScheme final : public ApplicationScheme<DDType, Config> {
public:
  size_t equivalentCount = 0;

  DiffApplicationScheme(TaskManager<DDType, Config>& tm1,
                        TaskManager<DDType, Config>& tm2)
      : ApplicationScheme<DDType, Config>(tm1, tm2), path(myersDiff()),
        index(0) {}

  std::pair<size_t, size_t> operator()() noexcept override {
    index++;
    /*std::cout << "(" << path[index - 1].first << ", " << path[index -
       1].second
              << ")" << "\n";*/
    return path[index - 1];
  }

private:
  std::vector<std::pair<int64_t, int64_t>> path;
  size_t                                   index = 0;
  const std::string                        empty;

  const std::string& getCirc1OpAt(size_t i) {
    auto circ1 = this->taskManager1->getCircuit();
    if (i >= circ1->getNops()) {
      return this->empty;
    }
    return circ1->at(i)->getName();
  }

  const std::string& getCirc2OpAt(size_t i) {
    auto circ2 = this->taskManager2->getCircuit();
    if (i >= circ2->getNops()) {
      return this->empty;
    }
    return circ2->at(i)->getName();
  }

  bool isMatchPoint(size_t x, size_t y) {
    return getCirc1OpAt(x) == getCirc2OpAt(y);
  }

  std::vector<std::pair<int64_t, int64_t>>
  myersRecursive(int64_t x, int64_t y, int64_t w, int64_t h) {
    const int64_t m               = w;
    const int64_t n               = h;
    const int64_t max             = m + n;
    auto          best_x_forward  = std::vector<int64_t>(max * 2 + 1, -1);
    auto          best_x_backward = std::vector<int64_t>(max * 2 + 1, -1);
    best_x_forward[max + 1]       = 0;
    best_x_backward[max + 1]      = 0;
    int64_t snake_start_x         = -1;
    int64_t snake_start_y         = -1;
    int64_t snake_end_x           = -1;
    int64_t snake_end_y           = -1;

    for (int64_t operations = 0; operations <= max; operations++) {
      for (int64_t diagonal = -(operations - 2 * std::max(0L, operations - n));
           diagonal <= (operations - 2 * std::max(0L, operations - m));
           diagonal += 2) {
        if (diagonal == -operations) {
          best_x_forward[max + diagonal] = best_x_forward[max + diagonal + 1];
        } else if (diagonal == operations) {
          best_x_forward[max + diagonal] =
              best_x_forward[max + diagonal - 1] + 1;
        } else if (best_x_forward[max + diagonal - 1] >
                   best_x_forward[max + diagonal + 1]) {
          best_x_forward[max + diagonal] =
              best_x_forward[max + diagonal - 1] + 1;
        } else {
          best_x_forward[max + diagonal] = best_x_forward[max + diagonal + 1];
        }

        int64_t snake_start = best_x_forward[max + diagonal];
        while (best_x_forward[max + diagonal] < m &&
               best_x_forward[max + diagonal] - diagonal < n &&
               isMatchPoint(x + best_x_forward[max + diagonal],
                            y + best_x_forward[max + diagonal] - diagonal)) {
          best_x_forward[max + diagonal]++;
        }

        if ((-max < -diagonal + m - n) && (-diagonal + m - n < max) &&
            (best_x_forward[max + diagonal] != -1) &&
            (best_x_backward[max - diagonal + m - n] != -1) &&
            (best_x_forward[max + diagonal] +
                 best_x_backward[max - diagonal + m - n] >=
             m)) {
          snake_start_x = snake_start;
          snake_start_y = snake_start - diagonal;
          snake_end_x   = best_x_forward[max + diagonal];
          snake_end_y   = best_x_forward[max + diagonal] - diagonal;
          break;
        }
      }

      if (snake_start_x != -1) {
        break;
      }

      for (int64_t diagonal = -(operations - 2 * std::max(0L, operations - n));
           diagonal <= (operations - 2 * std::max(0L, operations - m));
           diagonal += 2) {
        if (diagonal == -operations) {
          best_x_backward[max + diagonal] = best_x_backward[max + diagonal + 1];
        } else if (diagonal == operations) {
          best_x_backward[max + diagonal] =
              best_x_backward[max + diagonal - 1] + 1;
        } else if (best_x_backward[max + diagonal - 1] >
                   best_x_backward[max + diagonal + 1]) {
          best_x_backward[max + diagonal] =
              best_x_backward[max + diagonal - 1] + 1;
        } else {
          best_x_backward[max + diagonal] = best_x_backward[max + diagonal + 1];
        }

        int64_t snake_end = best_x_backward[max + diagonal];
        while (0 < m - best_x_backward[max + diagonal] &&
               0 < n - best_x_backward[max + diagonal] + diagonal &&
               isMatchPoint(x + m - best_x_backward[max + diagonal] - 1,
                            y + n - best_x_backward[max + diagonal] + diagonal -
                                1)) {
          best_x_backward[max + diagonal]++;
        }

        if ((-max < -diagonal + m - n) && (-diagonal + m - n < max) &&
            (best_x_forward[max + diagonal] != -1) &&
            (best_x_backward[max - diagonal + m - n] != -1) &&
            (best_x_forward[max - diagonal + m - n] +
                 best_x_backward[max + diagonal] >=
             m)) {
          snake_start_x = m - best_x_backward[max + diagonal];
          snake_start_y = n - best_x_backward[max + diagonal] + diagonal;
          snake_end_x   = m - snake_end;
          snake_end_y   = n - snake_end + diagonal;
          break;
        }
      }

      if (snake_start_x != -1) {
        break;
      }
    }

    auto result = std::vector<std::pair<int64_t, int64_t>>();

    w = snake_start_x;
    h = snake_start_y;
    if (w == 0 || h == 0) {
      if (w != 0 || h != 0) {
        result.insert(result.end(), std::pair(w, h));
      }
    } else if (w == m && h == n) {
      if (w > h) {
        result.insert(result.end(), std::pair(h, h));
        result.insert(result.end(), std::pair(w - h, 0));
      } else {
        result.insert(result.end(), std::pair(w, w));
        result.insert(result.end(), std::pair(0, h - w));
      }
    } else {
      auto pre = myersRecursive(x, y, w, h);
      result.insert(result.end(), pre.begin(), pre.end());
    }

    if (snake_end_x - snake_start_x > 0) {
      result.insert(result.end(), std::pair(snake_end_x - snake_start_x,
                                            snake_end_x - snake_start_x));
    }

    w = m - snake_end_x;
    h = n - snake_end_y;
    if (w == 0 || h == 0) {
      if (w != 0 || h != 0) {
        result.insert(result.end(), std::pair(w, h));
      }
    } else if (w == m && h == n) {
      if (w > h) {
        result.insert(result.end(), std::pair(w - h, 0));
        result.insert(result.end(), std::pair(h, h));
      } else {
        result.insert(result.end(), std::pair(0, h - w));
        result.insert(result.end(), std::pair(w, w));
      }
    } else {
      auto post = myersRecursive(x + snake_end_x, y + snake_end_y, w, h);
      result.insert(result.end(), post.begin(), post.end());
    }

    return result;
  }

  std::vector<std::pair<size_t, size_t>>
  patienceSort(const std::map<size_t, size_t>& indexMatches) {
    struct PileElement {
      size_t aIndex;
      size_t bIndex;
      size_t prev;
    };

    std::vector<std::vector<PileElement>> piles;

    for (auto [aIndex, bIndex] : indexMatches) {
      if (piles.size() == 0) {
        piles.push_back({PileElement{aIndex, bIndex, 0}});
        continue;
      }

      for (size_t i = 0; i <= piles.size(); i++) {
        if (i == piles.size()) {
          piles.push_back(
              {PileElement{aIndex, bIndex, piles[i - 1].size() - 1}});
          break;
        }

        if (piles[i][piles[i].size() - 1].bIndex > bIndex) {
          piles[i].push_back(
              PileElement{aIndex, bIndex, piles[i - 1].size() - 1});
          break;
        }
      }
    }

    std::vector<std::pair<size_t, size_t>> increasingMatches;
    size_t prevPileElement = piles[piles.size() - 1].size() - 1;

    for (size_t i = piles.size(); i > 0; i--) {
      PileElement e = piles[i - 1][prevPileElement];
      increasingMatches.insert(increasingMatches.begin(),
                               std::pair(e.aIndex, e.bIndex));
      prevPileElement = e.prev;
    }

    return increasingMatches;
  }

  std::vector<std::pair<int64_t, int64_t>>
  patienceRecursive(int64_t x, int64_t y, int64_t w, int64_t h) {
    std::map<std::string, size_t> aCount;
    std::map<std::string, size_t> aIndex;
    std::map<std::string, size_t> bCount;
    std::map<std::string, size_t> bIndex;

    for (int64_t i = 0; i < w; i++) {
      aCount[getCirc1OpAt(x + i)] += 1;
      aIndex[getCirc1OpAt(x + i)] += i;
    }

    for (int64_t i = 0; i < h; i++) {
      bCount[getCirc2OpAt(y + i)] += 1;
      bIndex[getCirc2OpAt(y + i)] += i;
    }

    std::map<size_t, size_t> indexMatches;

    for (auto [element, count] : aCount) {
      if (count == 1 && bCount[element] == 1) {
        indexMatches[aIndex[element]] = bIndex[element];
      }
    }

    if (indexMatches.empty()) {
      return myersRecursive(x, y, w, h);
    }

    std::vector<std::pair<size_t, size_t>> increasingMatches =
        patienceSort(indexMatches);

    increasingMatches.emplace_back(std::pair(w, h));

    std::vector<std::pair<int64_t, int64_t>> diff;

    size_t prevAIndex = 0;
    size_t prevBIndex = 0;

    for (auto [a_index, b_index] : increasingMatches) {
      size_t aDelta = a_index - prevAIndex;
      size_t bDelta = b_index - prevBIndex;

      /*
      if (aDelta > 0) {
        diff.emplace_back(std::pair(aDelta, 0));
      }

      if (bDelta > 0) {
        diff.emplace_back(std::pair(0, bDelta));
      }
       */

      if (aDelta > 0 && bDelta > 0) {
        std::vector<std::pair<int64_t, int64_t>> sub =
            patienceRecursive(prevAIndex, prevBIndex, aDelta, bDelta);
        diff.insert(diff.end(), sub.begin(), sub.end());
      } else if (aDelta > 0) {
        diff.emplace_back(std::pair(aDelta, 0));
      } else if (bDelta > 0) {
        diff.emplace_back(std::pair(0, bDelta));
      }

      if (a_index == static_cast<size_t>(w) &&
          b_index == static_cast<size_t>(h)) {
        break;
      }

      if (diff[diff.size() - 1].first == diff[diff.size() - 1].second) {
        diff[diff.size() - 1].first += 1;
        diff[diff.size() - 1].second += 1;
      } else {
        diff.emplace_back(std::pair(1, 1));
      }

      prevAIndex = a_index + 1;
      prevBIndex = b_index + 1;
    }

    return diff;
  }

  std::vector<std::pair<int64_t, int64_t>> myersDiff() {
    std::vector<std::pair<int64_t, int64_t>> result =
        myersRecursive(0, 0, this->taskManager1->getCircuit()->getNops(),
                       this->taskManager2->getCircuit()->getNops());

    for (size_t i = 0; i < result.size() - 1; ++i) {
      if (result[i].first == 0 && result[i + 1].first == 0) {
        result[i] = std::pair(0, result[i].second + result[i + 1].second);
        result.erase(result.begin() + i + 1);
        i--;
      } else if (result[i].second == 0 && result[i + 1].second == 0) {
        result[i] = std::pair(result[i].first + result[i + 1].first, 0);
        result.erase(result.begin() + i + 1);
        i--;
      } else if (result[i].first == result[i].second &&
                 result[i + 1].first == result[i + 1].second) {
        result[i] = std::pair(result[i].first + result[i + 1].first,
                              result[i].second + result[i + 1].second);
        result.erase(result.begin() + i + 1);
        i--;
      }
    }

    for (size_t i = 0; i < path.size(); ++i) {
      if (path[i].first == path[i].second) {
        equivalentCount += path[i].first * 2;
      }
    }

    for (size_t i = 0; i < result.size() - 1; ++i) {
      if ((result[i].first == 0 && result[i + 1].first == 0) ||
          (result[i].second == 0 && result[i + 1].second == 0)) {
        result[i] = std::pair(result[i].first + result[i + 1].first,
                              result[i].second + result[i + 1].second);
        result.erase(result.begin() + i + 1);
        i--;
      }
    }

    for (size_t i = 0; i < result.size() - 1; i++) {
      if ((result[i].first == 0 && result[i + 1].second == 0) ||
          (result[i].second == 0 && result[i + 1].first == 0)) {
        int64_t a = result[i].first + result[i + 1].first;
        int64_t b = result[i].second + result[i + 1].second;
        if (a > 1 && b > 1) {
          int64_t max = std::max(a, b);
          result[i] = {a % max, 0};
          result[i + 1] = {0, b % max};

          for (size_t j = 0; j < max; j++) {
            result.insert(result.begin() + static_cast<int64_t>(i + 2 + 2 * j),
                          {a / max, 0});
            result.insert(result.begin() + static_cast<int64_t>(i + 3 + 2 * j),
                          {0, b / max});
          }

          i += 1 + 2 * static_cast<size_t>(max - 1);
        }
      }
    }

    for (size_t i = 0; i < result.size(); i++) {
      int64_t a = result[i].first;
      int64_t b = result[i].second;
      if (a > 1 && b > 1) {
        int64_t max = std::max(a, b);
        result[i] = {a % max, b % max};

        for (size_t j = 0; j < max; j++) {
          result.insert(result.begin() + static_cast<int64_t>(i + 1 + j),
                        {a / max, b / max});
        }

        i += 1 + static_cast<size_t>(max - 1);
      }
    }

    /*for (size_t i = 0; i < result.size(); i++) {
      if (result[i].first > 5 || result[i].second > 5) {
        result.insert(result.begin() + static_cast<int64_t>(i + 1),
    {(result[i].first + 1) / 2, (result[i].second + 1) / 2}); result[i] =
    {result[i].first / 2, result[i].second / 2}; i++;
      }
    }

    for (size_t i = 0; i < result.size(); i++) {
      if (result[i].first > 5 || result[i].second > 5) {
        result.insert(result.begin() + static_cast<int64_t>(i + 1),
    {(result[i].first + 1) / 2, (result[i].second + 1) / 2}); result[i] =
    {result[i].first / 2, result[i].second / 2}; i++;
      }
    }*/

    /*for (size_t i = 0; i < result.size() / 2; i++) {
      result[i].second ^= result[result.size() - i].second;
      result[result.size() - i].second ^= result[i].second;
      result[i].second ^= result[result.size() - i].second;

      result[i].first ^= result[result.size() - i].first;
      result[result.size() - i].first ^= result[i].first;
      result[i].first ^= result[result.size() - i].first;
    }*/

    for (size_t i = 0; i < result.size() - 1; i++) {
      if ((result[i].first == 0 && result[i + 1].second == 0) ||
          (result[i].second == 0 && result[i + 1].first == 0) ||
          (result[i].first == 0 && result[i + 1].first == 0) ||
          (result[i].second == 0 && result[i + 1].second == 0)) {
        result[i] = std::pair(result[i].first + result[i + 1].first,
                              result[i].second + result[i + 1].second);
        result.erase(result.begin() + i + 1);
        i--;
      }
    }

    for (size_t i = 0; i < result.size(); i++) {
      if (result[i].first == 0 && result[i].second == 0) {
        result.erase(result.begin() + i);
        i--;
      }
    }

    return result;
  }

  std::vector<std::pair<int64_t, int64_t>> patienceDiff() {
    std::vector<std::pair<int64_t, int64_t>> result =
        patienceRecursive(0, 0, this->taskManager1->getCircuit()->getNops(),
                          this->taskManager2->getCircuit()->getNops());

    return result;
  }
};
} // namespace ec
