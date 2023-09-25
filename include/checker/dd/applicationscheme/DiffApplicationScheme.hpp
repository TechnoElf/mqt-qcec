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
    findPath();
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

  bool isMatchPoint(size_t x, size_t y) {
    auto circ1 = this->taskManager1.getCircuit();
    auto circ2 = this->taskManager2.getCircuit();

    if (x >= circ1->getNops() || y >= circ2->getNops()) {
      return false;
    }

    return circ1->at(x)->getName() == circ2->at(y)->getName();
  }

  std::vector<std::vector<std::optional<std::pair<int64_t, int64_t>>>>
  constructEditGraph() {
    const int64_t m = this->taskManager1.getCircuit()->getNops();
    const int64_t n = this->taskManager2.getCircuit()->getNops();

    auto parent =
        std::vector<std::vector<std::optional<std::pair<int64_t, int64_t>>>>(m +
                                                                             1);
    for (const int64_t i : std::views::iota(0L, m + 1)) {
      parent[i].resize(static_cast<size_t>(n + 1));
    }

    auto endpoints = std::vector<std::pair<int64_t, int64_t>>();
    endpoints.emplace_back(-1, 0);
    auto curEndpoints = std::vector<std::pair<int64_t, int64_t>>();

    for ([[maybe_unused]] const ulong d : std::views::iota(0L, m + n + 1)) {
      curEndpoints.swap(endpoints);
      endpoints.clear();

      for (auto& e : curEndpoints) {
        if (0 <= e.first + 1 && e.first + 1 <= m && 0 <= e.second &&
            e.second <= n) {
          auto [x, y]         = std::pair(e.first + 1, e.second);
          auto [prevX, prevY] = e;

          while (isMatchPoint(x, y)) {
            if (parent[x][y]) {
              break;
            }
            if (0 <= x && x <= m && 0 <= y && y <= n) {
              parent[x][y] = std::pair(prevX, prevY);
            }
            std::tie(prevX, prevY) = std::pair(x, y);
            std::tie(x, y)         = std::pair(x + 1, y + 1);
          }

          if (!parent[x][y]) {
            if (0 <= x && x <= m && 0 <= y && y <= n) {
              parent[x][y] = std::pair(prevX, prevY);
            }
            if (x >= m && y >= n) {
              return parent;
            }
            endpoints.emplace_back(x, y);
          }
        }

        if (0 <= e.first && e.first <= m && 0 <= e.second + 1 &&
            e.second + 1 <= n) {
          auto [x, y]         = std::pair(e.first, e.second + 1);
          auto [prevX, prevY] = e;

          while (isMatchPoint(x, y)) {
            if (parent[x][y]) {
              break;
            }
            if (0 <= x && x <= m && 0 <= y && y <= n) {
              parent[x][y] = std::pair(prevX, prevY);
            }
            std::tie(prevX, prevY) = std::pair(x, y);
            std::tie(x, y)         = std::pair(x + 1, y + 1);
          }

          if (!parent[x][y]) {
            if (0 <= x && x <= m && 0 <= y && y <= n) {
              parent[x][y] = std::pair(prevX, prevY);
            }
            if (x >= m && y >= n) {
              return parent;
            }
            endpoints.emplace_back(x, y);
          }
        }
      }
    }

    return parent;
  }

  void findPath() {
    const int64_t m = this->taskManager1.getCircuit()->getNops();
    const int64_t n = this->taskManager2.getCircuit()->getNops();

    auto editGraph = constructEditGraph();

    path.clear();
    auto [x, y] = std::pair(m, n);
    while (editGraph[static_cast<size_t>(x)][static_cast<size_t>(y)]) {
      std::tie(x, y) =
          editGraph[static_cast<size_t>(x)][static_cast<size_t>(y)].value();
      if (x < 0 || y < 0) {
        break;
      }
      path.insert(path.begin(), std::pair(x, y));
    }
  }
};
} // namespace ec
