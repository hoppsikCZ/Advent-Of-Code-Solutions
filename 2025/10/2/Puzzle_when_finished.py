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
    
class Solver:
    def __init__(self, puzzle: Puzzle):
        self.puzzle = puzzle
        self.positions = list(puzzle.solution)
        self.swithes_used = [0] * len(puzzle.switches)
        self.switch_order = []
        self.max_switch_use = [0] * len(puzzle.switches)
        self.max_achievable = [0] * len(self.positions)
        self.updateMaxSwitchUse()
        
    def updateMaxAchievable(self):
        for position in range(self.puzzle.solution_length):
            self.max_achievable[position] = sum(
                self.max_switch_use[switch_i]
                for switch_i in range(self.puzzle.switch_count)
                if position in self.puzzle.switches[switch_i]
            )

    def updateMaxSwitchUse(self):
        for switch_index in range(self.puzzle.switch_count):
            self.max_switch_use[switch_index] = min(
                val for i, val in enumerate(self.positions)
                if i in self.puzzle.switches[switch_index]
            )
        self.updateMaxAchievable()    
                                      
    def addSwitchIndex(self, index: int, count = 1):
        self.swithes_used[index] += count
        
        self.switch_order.extend([index] * count)
        for point in self.puzzle.switches[index]:
            self.positions[point] -= count
            
        # self.updateMaxSwitchUse()
        
    def isSolvable(self):
        for position in range(self.puzzle.solution_length):
            if self.max_achievable[position] < self.positions[position] or self.positions[position] < 0:
                return False
        return True
    
    def popSwitch(self):
        last = self.switch_order[-1]
        self.swithes_used[last] -= 1
        self.switch_order.pop()
        for point in self.puzzle.switches[last]:
            self.positions[point] += 1
        # self.updateMaxSwitchUse()
        
    def surelyGaus(self):
        for switch_i in range(self.puzzle.switch_count):
            while self.isSolvable():
                if sum(self.positions) == 0:
                    return len(self.switch_order)
                self.addSwitchIndex(switch_i)
            self.popSwitch()
        
        return -1

    def chainSmartLeastComplexBrute(self):
        least_steps = float("inf")
        least_idx = self.findLeastComplexIdx()
        least_val = self.positions[least_idx]
        contains_least = self.findContainingIdx(least_idx)
        not_zeroed = self.findNonZeroedButtons()
        allowed = tuple(set(contains_least) & set(not_zeroed))
        possible = smartCombinations(allowed, least_val)
        for potential in possible:
            snap = self.takeSnapshot()
            for switch in potential:
                self.addSwitchIndex(switch[0], switch[1])
            if min(self.positions) >= 0: 
                if sum(self.positions) == 0:
                    return least_val 
                brute_result = self.chainSmartLeastComplexBrute()  
                if brute_result >= 0:
                    if brute_result < least_steps:
                        least_steps = brute_result
            self.restoreSnapshot(snap)          
                    
        if least_steps:     
            return least_steps + least_val
        else:
            return ERR_SMART_FAIL
            
    def reset(self):
        self.positions = [0] * len(self.puzzle.solution)
        self.swithes_used = [0] * len(self.puzzle.switches)
        
    def takeSnapshot(self):
        return SolverSnapshot(self)
        
    def restoreSnapshot(self, snapshot: SolverSnapshot):
        self.positions = snapshot.position.copy()
        self.swithes_used = snapshot.switches_used.copy()
        self.switch_order = snapshot.switch_order.copy()
        # self.max_switch_use = snapshot.max_switch_use.copy()
        # self.max_achievable = snapshot.max_achievable.copy()
        
    def findLeastIdx(self):
        zeroed = self.findZeroedIndicies()
        least_idx = 0
        least_switches = len(self.puzzle.switches)
        for pos_idx in range(len(self.positions)):
            if pos_idx in zeroed:
                continue
            pos_switches = len(tuple(
                i for i in self.puzzle.switches
                if pos_idx in i
            ))
            if least_switches > pos_switches:
                least_idx = pos_idx
                least_switches = pos_switches
                
        return least_idx
    
    def findLeastComplexIdx(self, positions = tuple()):
        if not positions:
            positions = self.positions
        zeroed = self.findZeroedIndicies(positions)
        least_idx = 0
        least_complex = float('inf')
        for pos_idx, r in enumerate(positions):
            if pos_idx in zeroed:
                continue
            n = len(tuple(
                i for i in self.puzzle.switches
                if pos_idx in i
            ))
            
            complexity = calcComplexity(n, r)
            
            if least_complex > complexity:
                least_idx = pos_idx
                least_complex = complexity
                
        return least_idx
               
        
    def findZeroedIndicies(self, positions = tuple()):
        if not positions:
            positions = self.positions
        return tuple(
            i
            for i, pos in enumerate(positions)
            if pos == 0
        )
        
    def findNonZeroedButtons(self, positions = tuple()):
        zeroed_positions = self.findZeroedIndicies(positions)
        valid_switches = tuple(
            i
            for i in range(len(self.puzzle.switches))
            if set(zeroed_positions).isdisjoint(self.puzzle.switches[i]) 
        )
        return valid_switches
    
    def findContainingMax(self):
        max_self_pos = max(self.positions)
        return tuple(
            i 
            for i, switch in enumerate(self.puzzle.switches) 
            if set(
                max_idx 
                for max_idx, max_val in enumerate(self.positions) 
                if max_val == max_self_pos
                ).issubset(switch)
        )
        
    def findContainingMin(self):
        min_self_pos = min(self.positions)
        return tuple(
            i 
            for i, switch in enumerate(self.puzzle.switches) 
            if set(
                min_idx 
                for min_idx, min_val in enumerate(self.positions) 
                if min_val == min_self_pos
                ).issubset(switch)
        )
            
    def findContainingIdx(self, idx):
        return tuple(
            i for i, switch in enumerate(self.puzzle.switches)
            if idx in switch
        )
    
         
class SolverSnapshot:
    def __init__(self, solver: Solver):
        self.position = solver.positions.copy()
        self.switches_used = solver.swithes_used.copy()
        self.switch_order = solver.switch_order.copy()
        # self.max_switch_use = solver.max_switch_use.copy()
        # self.max_achievable = solver.max_achievable.copy()
        
def smartCombinations(iterable: tuple[int, ...], r: int):
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
            counts[i + 1:] = [sum(counts[i + 1:]) + 1] + [0] * (-i - 2)
            yield tuple((pool[k], counts[k]) for k in range(n) if counts[k] > 0)
            break
        
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
            counts[i + 1:] = [sum(counts[i + 1:]) + 1] + [0] * (-i - 2)
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
        min(
            pos for pos_idx, pos in enumerate(positions)
            if pos_idx in switch
            ) 
        for idx, switch in enumerate(switches)
        if idx in selected 
    )

def chainSmartLeastComplexBrutePrediction(positions: tuple[int, ...], switches: tuple[tuple[int, ...], ...]):
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
                    brute_result = chainSmartLeastComplexBrutePrediction(tuple(new_positions), switches)  
                    if brute_result >= 0:
                        if brute_result < steps:
                            steps = brute_result                 
    if steps:     
        return steps + least_val
    else:
        return ERR_SMART_FAIL
  
# @cache  
def findLeastComplexIdx(positions: tuple[int, ...], switches: tuple[tuple[int, ...], ...]):
    zeroed = findZeroedIndicies(positions)
    least_idx = 0
    least_complex = float('inf')
    for pos_idx, r in enumerate(positions):
        if pos_idx in zeroed:
            continue
        n = len(tuple(
            i for i in switches
            if pos_idx in i
        ))
        
        complexity = calcComplexity(n, r)
        
        if least_complex > complexity:
            least_idx = pos_idx
            least_complex = complexity
            
    return least_idx

# @cache
def findContainingIdx(idx: int, switches: tuple[tuple[int, ...], ...]):
    return tuple(
        i for i, switch in enumerate(switches)
        if idx in switch
    )

# @cache
def findZeroedIndicies(positions: tuple[int, ...]):
    return tuple(
        i
        for i, pos in enumerate(positions)
        if pos == 0
    )

# @cache 
def findNonZeroedButtons(positions: tuple[int, ...], switches: tuple[tuple[int, ...], ...]):
    zeroed_positions = findZeroedIndicies(positions)
    valid_switches = tuple(
        i
        for i in range(len(switches))
        if set(zeroed_positions).isdisjoint(switches[i]) 
    )
    return valid_switches

def findMinimum(postions: tuple[int, ...], switches: tuple[tuple[int, ...], ...]):
    # for i in range(len(postions)):
    #     buttons = findContainingIdx(i, switches)
    #     affected = 
    return max(postions)