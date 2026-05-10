#include "libmin.h"

#define TSP_N 8
#define TSP_MASKS (1u << TSP_N)
#define TSP_INF  ((uint64_t)1000000000ULL)

static uint64_t graph[TSP_N][TSP_N];
static uint64_t _graph[TSP_N][TSP_N] = {
  { 0, 29, 20, 21, 16, 31, 100, 12 },
  { 29, 0, 15, 29, 28, 40, 72, 21 },
  { 20, 15, 0, 15, 14, 25, 81, 9 },
  { 21, 29, 15, 0, 4, 12, 92, 12 },
  { 16, 28, 14, 4, 0, 16, 94, 9 },
  { 31, 40, 25, 12, 16, 0, 95, 24 },
  { 100, 72, 81, 92, 94, 95, 0, 90 },
  { 12, 21, 9, 12, 9, 24, 90, 0 }
};

// dp[mask][j] = minimum cost to start at 0, visit "mask", and end at j
static uint64_t dp[TSP_MASKS][TSP_N];
// parent[mask][j] = previous city before j on best path for state (mask, j)
static uint64_t parent[TSP_MASKS][TSP_N];

static inline void
heldkarp_tsp(void)
{
  for (unsigned mask = 0; mask < TSP_MASKS; ++mask)
    for (unsigned city = 0; city < TSP_N; ++city) {
      dp[mask][city] = TSP_INF;
      parent[mask][city] = 0;
    }

  dp[1u << 0][0] = 0;

  for (unsigned mask = 0; mask < TSP_MASKS; ++mask) {
    if ((mask & 1u) == 0)
      continue;

    for (unsigned end = 0; end < TSP_N; ++end) {
      if ((mask & (1u << end)) == 0)
        continue;
      if (end == 0 && mask != (1u << 0))
        continue;

      unsigned prev_mask = mask ^ (1u << end);
      if (prev_mask == 0 && end != 0)
        continue;

      uint64_t best = dp[mask][end];
      uint64_t best_parent = parent[mask][end];

      if (end == 0 && mask == (1u << 0)) {
        best = 0;
        best_parent = 0;
      } else {
        for (uint64_t prev = 0; prev < TSP_N; ++prev) {
          if ((prev_mask & (1u << prev)) == 0)
            continue;

          uint64_t cand = dp[prev_mask][prev] + graph[prev][end];
          if (cand < best)
          {
            best = cand;
            best_parent = (uint64_t)prev;
          }
        }
      }

      dp[mask][end] = best;
      parent[mask][end] = best_parent;
    }
  }
}

int
main(void)
{
  libmin_srand(42);

  for (unsigned i = 0; i < TSP_N; ++i)
    for (unsigned j = 0; j < TSP_N; ++j)
      graph[i][j] = _graph[i][j];

  heldkarp_tsp();

  const unsigned full_mask = TSP_MASKS - 1u;
  uint64_t best_cycle = TSP_INF;
  uint64_t best_last = 0;

  for (uint64_t end = 1; end < TSP_N; ++end) {
    uint64_t tour = dp[full_mask][end] + graph[end][0];
    if (tour < best_cycle)
    {
      best_cycle = tour;
      best_last = (uint64_t)end;
    }
  }

  // Result decryption only after all encrypted processing is complete.
  libmin_printf("Held-Karp TSP benchmark (encrypted graph/state)\n");
  libmin_printf("Cities: %d\n", TSP_N);
  libmin_printf("Minimum Hamiltonian cycle cost: %llu\n", best_cycle);

  // Reconstruct and print one optimal tour (decrypts occur only while printing).
  libmin_printf("Tour: 0");
  unsigned mask = full_mask;
  unsigned cur = (unsigned)best_last;

  for (unsigned step = 1; step < TSP_N; ++step) {
    libmin_printf(" -> %u", cur);
    unsigned prev = (unsigned)parent[mask][cur];
    mask ^= (1u << cur);
    cur = prev;
  }
  libmin_printf(" -> 0\n");

  libmin_success();
  return 0;
}
