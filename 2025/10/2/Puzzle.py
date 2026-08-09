import math
from functools import cache

# DON'T TOUCH
ERR_BRUTE_COMPLEXITY = -1
ERR_BRUTE_IMPOSSIBLE = -2

ERR_SMART_FAIL = -3
ERR_DEPTH_EXCEEDED = -4


class Puzzle:
    def __init__(self, solution: tuple[int, ...], switches):
        self.solution = solution
        self.switches = tuple(sorted(tuple(switches), key=len, reverse=True))
        self.switch_count = len(self.switches)
        self.solution_length = len(self.solution)

    def __str__(self):
        return str(self.solution) + " " + str(self.switches)


def smartCombinationsLimits(iterable: tuple[int, ...], r: int, limits: tuple[int, ...]):
    pool = tuple(iterable)
    n = len(pool)
    if not n or r < 0:
        return

    counts = [r] + [0] * (n - 1)
    yield tuple((pool[i], counts[i]) for i in range(n) if counts[i] > 0)
    head = -n
    while True:
        for i in range(-1, head - 1, -1):
            if counts[i] == r:
                if i == -1:
                    return
                elif i != head:
                    head = i
                    break

            if i == -1 or counts[i] == 0:
                continue

            counts[i] -= 1
            counts[i + 1 :] = [sum(counts[i + 1 :]) + 1] + [0] * (-i - 2)
            limits_good = True
            for j, val in enumerate(limits):
                if counts[j] > val:
                    limits_good = False
                    break
            if limits_good:
                yield tuple((pool[k], counts[k]) for k in range(n) if counts[k] > 0)
            break


def calcComplexity(n, r):
    NoC = math.comb(n + r - 1, r)
    complexity = NoC * r
    return complexity


def getLimits(selected, positions, switches):
    return tuple(
        min(pos for pos_idx, pos in enumerate(positions) if pos_idx in switch)
        for idx, switch in enumerate(switches)
        if idx in selected
    )


def chainSmartLeastComplexBrutePrediction(
    positions: tuple[int, ...], switches: tuple[tuple[int, ...], ...]
):
    steps = float("inf")
    least_idx = findLeastComplexIdx(positions, switches)
    contains_least = findContainingIdx(least_idx, switches)
    not_zeroed = findNonZeroedButtons(positions, switches)
    allowed = tuple(set(contains_least) & set(not_zeroed))
    least_val = positions[least_idx]
    limits = getLimits(allowed, positions, switches)
    possible = smartCombinationsLimits(allowed, least_val, limits)
    for potential in possible:
        new_positions = list(positions)
        for switch_idx in potential:
            for pos in switches[switch_idx[0]]:
                new_positions[pos] -= switch_idx[1]
        if min(new_positions) == 0:
            if sum(new_positions) == 0:
                return least_val
            switches_left = findNonZeroedButtons(positions, switches)
            if switches_left:
                if findMinimum(tuple(new_positions), switches) <= steps:
                    brute_result = chainSmartLeastComplexBrutePrediction(
                        tuple(new_positions), switches
                    )
                    if brute_result >= 0:
                        if brute_result < steps:
                            steps = brute_result
    if steps:
        return steps + least_val
    else:
        return ERR_SMART_FAIL


def findLeastComplexIdx(
    positions: tuple[int, ...], switches: tuple[tuple[int, ...], ...]
):
    zeroed = findZeroedIndicies(positions)
    least_idx = 0
    least_complex = float("inf")
    for pos_idx, r in enumerate(positions):
        if pos_idx in zeroed:
            continue
        n = len(tuple(i for i in switches if pos_idx in i))

        complexity = calcComplexity(n, r)

        if least_complex > complexity:
            least_idx = pos_idx
            least_complex = complexity

    return least_idx


def findContainingIdx(idx: int, switches: tuple[tuple[int, ...], ...]):
    return tuple(i for i, switch in enumerate(switches) if idx in switch)


def findZeroedIndicies(positions: tuple[int, ...]):
    return tuple(i for i, pos in enumerate(positions) if pos == 0)


def findNonZeroedButtons(
    positions: tuple[int, ...], switches: tuple[tuple[int, ...], ...]
):
    zeroed_positions = findZeroedIndicies(positions)
    valid_switches = tuple(
        i for i in range(len(switches)) if set(zeroed_positions).isdisjoint(switches[i])
    )
    return valid_switches


def findMinimum(postions: tuple[int, ...], switches: tuple[tuple[int, ...], ...]):
    # for i in range(len(postions)):
    #     buttons = findContainingIdx(i, switches)
    #     affected =
    return max(postions)
