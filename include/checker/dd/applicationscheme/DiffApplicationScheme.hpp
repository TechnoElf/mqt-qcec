//
// This file is part of the MQT QCEC library released under the MIT license.
// See README.md or go to https://github.com/cda-tum/qcec for more information.
//

#pragma once

#include "ApplicationScheme.hpp"

#include <ranges>

namespace ec {
template <class DDType, class Config>
class DiffApplicationScheme final : public ApplicationScheme<DDType, Config> {
public:
  DiffApplicationScheme(TaskManager<DDType, Config>& tm1,
                        TaskManager<DDType, Config>& tm2)
      : ApplicationScheme<DDType, Config>(tm1, tm2), index(0), path() {
    path = diff();
  }

  std::pair<size_t, size_t> operator()() noexcept override {
    int64_t dx = path[index + 1].first - path[index].first;
    int64_t dy = path[index + 1].second - path[index].second;
    index += 1;

    return {dx, dy};
  }

private:
  std::vector<std::pair<int64_t, int64_t>> path;
  size_t                                   index;

  void printVec(const std::vector<int64_t>& vec) {
    std::cout << "[";
    if (vec.size() > 0) {
      std::cout << vec[0];
      for (size_t i = 1; i < vec.size(); i++) {
        std::cout << ", " << vec[i];
      }
    }
    std::cout << "]";
  }

  void printVec(const std::vector<std::pair<int64_t, int64_t>>& vec) {
    std::cout << "[";
    if (vec.size() > 0) {
      std::cout << "\n"
                << "  (" << vec[0].first << ", " << vec[0].second << ")";
      for (size_t i = 1; i < vec.size(); i++) {
        std::cout << ", "
                  << "\n"
                  << "  (" << vec[i].first << ", " << vec[i].second << ")";
      }
      std::cout << "\n";
    }
    std::cout << "]";
  }

  bool isMatchPoint(size_t x, size_t y) {
    auto circ1 = this->taskManager1.getCircuit();
    auto circ2 = this->taskManager2.getCircuit();

    if (x >= circ1->getNops() || y >= circ2->getNops()) {
      return false;
    }

    return circ1->at(x)->getName() == circ2->at(y)->getName();
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

    for (int64_t operations = 0; operations < max; operations++) {
      for (int64_t diagonal = -operations; diagonal <= operations;
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

      for (int64_t diagonal = -operations; diagonal <= operations;
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

  std::vector<std::pair<int64_t, int64_t>> diff() {
    std::vector<std::pair<int64_t, int64_t>> result =
        myersRecursive(0, 0, this->taskManager1.getCircuit()->getNops(),
                       this->taskManager2.getCircuit()->getNops());

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

    /*
    for (size_t i = 0; i < result.size() - 1; ++i) {
        if ((result[i].first == 0 && result[i + 1].second == 0) || (result[i +
    1].first == 0 && result[i].second == 0)) { result[i] =
    std::pair(result[i].first + result[i + 1].first, result[i].second + result[i
    + 1].second); result.erase(result.begin() + i + 1); i--;
        }
    }
    */

    return result;
  }
};
} // namespace ec
