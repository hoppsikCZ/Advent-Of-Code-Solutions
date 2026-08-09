from itertools import combinations, combinations_with_replacement
import math

# DON'T TOUCH
ERR_BRUTE_COMPLEXITY = -1
ERR_BRUTE_IMPOSSIBLE = -2

ERR_SMART_FAIL = -3
ERR_DEPTH_EXCEEDED = -4

# CONFIG
MAX_BRUTE_BUFFER = 0
COMPLEXITY_CUTOFF = 100000000
CHAIN_SIMPLE_STEP = 50


class Puzzle:
    def __init__(self, solution: tuple[int, ...], switches):
        self.solution = solution
        self.switches = tuple(sorted(tuple(switches), key=len, reverse=True))
        
    def __str__(self):
        return str(self.solution) + " " + str(self.switches)
    
class Solver:
    def __init__(self, puzzle: Puzzle):
        self.puzzle = puzzle
        self.position = list(puzzle.solution)
        self.swithes_used = [0] * len(puzzle.switches)
        self.switch_order = []

                                      
    def addSwitchIndex(self, index: int, count = 1):
        self.swithes_used[index] += count
        
        self.switch_order.extend([index] * count)
        for point in self.puzzle.switches[index]:
            self.position[point] -= count
    
            
    def popSwitch(self):
        last = self.switch_order[-1]
        self.swithes_used[last] -= 1
        self.switch_order.pop()
        for point in self.puzzle.switches[last]:
            self.position[point] += 1
            
    def reset(self):
        self.position = [0] * len(self.puzzle.solution)
        self.swithes_used = [0] * len(self.puzzle.switches)
        
    def takeSnapshot(self):
        return SolverSnapshot(self)
        
    def restoreSnapshot(self, snapshot: SolverSnapshot):
        self.position = snapshot.position.copy()
        self.swithes_used = snapshot.switches_used.copy()
        self.switch_order = snapshot.switch_order.copy()
        
    def findLeastIdx(self):
        zeroed = self.findZeroedIndicies()
        least_idx = 0
        least_switches = len(self.puzzle.switches)
        for pos_idx in range(len(self.position)):
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
    
    def findLeastComplexIdx(self):
        zeroed = self.findZeroedIndicies()
        least_idx = 0
        least_complex = float('inf')
        for pos_idx, r in enumerate(self.position):
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
               
        
    def findZeroedIndicies(self):
        return tuple(
            i
            for i, pos in enumerate(self.position)
            if pos == 0
        )
        
    def findNonZeroedButtons(self):
        zeroed_positions = self.findZeroedIndicies()
        valid_switches = tuple(
            i
            for i in range(len(self.puzzle.switches))
            if set(zeroed_positions).isdisjoint(self.puzzle.switches[i]) 
        )
        return valid_switches
    
    def findContainingMax(self):
        max_self_pos = max(self.position)
        return tuple(
            i 
            for i, switch in enumerate(self.puzzle.switches) 
            if set(
                max_idx 
                for max_idx, max_val in enumerate(self.position) 
                if max_val == max_self_pos
                ).issubset(switch)
        )
        
    def findContainingMin(self):
        min_self_pos = min(self.position)
        return tuple(
            i 
            for i, switch in enumerate(self.puzzle.switches) 
            if set(
                min_idx 
                for min_idx, min_val in enumerate(self.position) 
                if min_val == min_self_pos
                ).issubset(switch)
        )
            
    def findContainingIdx(self, idx):
        return tuple(
            i for i, switch in enumerate(self.puzzle.switches)
            if idx in switch
        )
    
    def bruteForce(self, switches = tuple()):
        if not switches:
            switches = range(len(self.puzzle.switches))
        i = max(self.position) - 1
        while True:
            i += 1
            if i > sum(self.position):
                return ERR_BRUTE_IMPOSSIBLE
            
            test = combinations_with_replacement(switches, i)
            NoC = math.comb(len(switches) + i - 1, i)
            complexity = NoC * i
            print(f"Bruteforcing with: n={len(switches)}, r={i}, NoC={NoC}, complexity={complexity}")
            if complexity > COMPLEXITY_CUTOFF:
                return ERR_BRUTE_COMPLEXITY
                        
            for solve in test:
                snap = self.takeSnapshot()
                for switch in solve:
                    self.addSwitchIndex(switch)

                if self.position == [0] * len(self.position): 
                    return i
                self.restoreSnapshot(snap)
                
    def smartBruteForce(self, switches = tuple()):
        if not switches:
            switches = range(len(self.puzzle.switches))
        i = max(self.position) - 1
        while True:
            i += 1
            if i > sum(self.position):
                return ERR_BRUTE_IMPOSSIBLE
            
            NoC = math.comb(len(switches) + i - 1, i)
            complexity = NoC * i
            print(f"\r\033[KBruteforcing with: n={len(switches)}, r={i}, NoC={NoC}, complexity={complexity}", end="")
            test = smartCombinations(switches, i)
            #print(tuple(test))
            if complexity > COMPLEXITY_CUTOFF:
                print("")
                return ERR_BRUTE_COMPLEXITY
                        
            for solve in test:
                snap = self.takeSnapshot()
                for switch in solve:
                    self.addSwitchIndex(switch[0], switch[1])

                if self.position == [0] * len(self.position):
                    print("") 
                    return i
                self.restoreSnapshot(snap)
    
    # Shelved
    def prioritySolve(self):
        i = 0
        blacklist = []
        
        while True:
            i += 1
            
            optimal = [i for i, x in enumerate(self.position) if x == max(self.position)]
            # print(optimal)
            best = tuple()
            best_idx = -1
            for idx, switch in enumerate(self.puzzle.switches):
                #print(switch)
                if set(switch).issubset(optimal) and len(switch) > len(best) and not idx in blacklist:
                    best = switch
                    best_idx = idx
                
            #Nejprve se musi checknout, jestli neni kombinace, ktera snizi vsechny, ale potrebnou o vic, pak az se muze jit na blacklist.
                    
            if len(best) == 0 or best_idx == -1:
                if len(self.switch_order) == 0:
                    return -1
                blacklist.append(self.switch_order[-1])
                self.popSwitch()
                print("Blacklist:" + str(blacklist))
                i -= 1
            else:
                self.addSwitchIndex(best_idx)    
                print(self.position)
                # if max(self.position) == 0:
                #     return i
                all_zero = True  
                # for num in self.position:
                #     if num != 0:
                #         all_zero = False
                #         break
                if self.position == [0] * len(self.position):
                    return i
                      
    # Final
    def maxBrute(self):
        if MAX_BRUTE_BUFFER > min(self.position):
            return self.bruteForce()
        
        contains_max = self.findContainingMax()
        allowed = tuple(
            i 
            for i in contains_max
            if len(self.puzzle.switches[i]) == len(max(map(lambda sw_idx: self.puzzle.switches[sw_idx], contains_max), key=len))
        )
        i = 0
        blacklist = []
        while i < sum(self.puzzle.solution):
                    i += 1
                    test = combinations_with_replacement(allowed, i)
                    best_position_sum = sum(self.position)
                    best_snap = self.takeSnapshot()
                    for solve in test:
                        snap = self.takeSnapshot()
                        for switch in solve:
                            self.addSwitchIndex(switch)
                    
                        if min(self.position) == MAX_BRUTE_BUFFER: 
                            if sum(self.position) < best_position_sum:
                                best_position_sum = sum(self.position)
                                best_snap = self.takeSnapshot()
                                
                        self.restoreSnapshot(snap)
                                
                    if min(best_snap.position) == MAX_BRUTE_BUFFER:
                        self.restoreSnapshot(best_snap)
                        while True:
                            brute_result = self.smartBruteForce(self.findNonZeroedButtons())
                            if brute_result == ERR_BRUTE_IMPOSSIBLE:
                                return ERR_BRUTE_IMPOSSIBLE
                            elif brute_result == ERR_BRUTE_IMPOSSIBLE:
                                return ERR_BRUTE_COMPLEXITY
                            else:
                                return len(self.switch_order)
        return -1

    def smartMaxBrute(self):
        contains_max = self.findContainingMax()
        not_zeroed = self.findNonZeroedButtons()
        allowed = tuple(set(contains_max) & set(not_zeroed))
        # print(self.position)
        # print(self.puzzle.switches)
        #print(allowed)
        
        # possible = sorted(smartCombinations(allowed, max(self.position)), key=lambda x: (
        #     len(x),
        #     tuple(len(self.puzzle.switches[i[0]]) * i[1] for i in x)
        # ), reverse=True)
        possible = smartCombinations(allowed, max(self.position))
        for potential in possible:
            # print(potential)
            snap = self.takeSnapshot()
            for switch in potential:
                self.addSwitchIndex(switch[0], switch[1])
        
            if min(self.position) == 0:
                #print(self.position)
                switches_left = self.findNonZeroedButtons()
                if switches_left: 
                    brute_result = self.smartBruteForce(switches_left)  
                    if brute_result >= 0:
                        return len(self.switch_order)
                    elif brute_result == ERR_BRUTE_COMPLEXITY:
                        return ERR_SMART_FAIL            
            self.restoreSnapshot(snap)          
                        
        return ERR_SMART_FAIL
        
    def chainSmartMaxBrute(self):
        contains_max = self.findContainingMax()
        not_zeroed = self.findNonZeroedButtons()
        allowed = tuple(set(contains_max) & set(not_zeroed))
        max_val = max(self.position)
        possible = smartCombinations(allowed, max_val)
        for potential in possible:
            # print(potential)
            snap = self.takeSnapshot()
            for switch in potential:
                self.addSwitchIndex(switch[0], switch[1])
        
            if min(self.position) == 0:
                if sum(self.position) == 0:
                    return max_val
                #print(self.position)
                switches_left = self.findNonZeroedButtons()
                if switches_left: 
                    brute_result = self.chainSmartMaxBrute()  
                    if brute_result >= 0:
                        return max_val + brute_result          
            self.restoreSnapshot(snap)          
                        
        return ERR_SMART_FAIL
    
    def chainSmartMinBrute(self):
        contains_min = self.findContainingMin()
        not_zeroed = self.findNonZeroedButtons()
        allowed = tuple(set(contains_min) & set(not_zeroed))
        min_val = min(self.position)
        possible = smartCombinations(allowed, min_val)
        for potential in possible:
            # print(potential)
            snap = self.takeSnapshot()
            for switch in potential:
                self.addSwitchIndex(switch[0], switch[1])
        
            if min(self.position) == 0:
                if sum(self.position) == 0:
                    return min_val
                #print(self.position)
                switches_left = self.findNonZeroedButtons()
                if switches_left: 
                    brute_result = self.chainSmartMinBrute()  
                    if brute_result >= 0:
                        return min_val + brute_result           
            self.restoreSnapshot(snap)          
                        
        return ERR_SMART_FAIL
    
    def chainSmartLeastBrute(self):
        least_idx = self.findLeastIdx()
        contains_least = self.findContainingIdx(least_idx)
        not_zeroed = self.findNonZeroedButtons()
        allowed = tuple(set(contains_least) & set(not_zeroed))
        least_val = self.position[least_idx]
        possible = smartCombinations(allowed, least_val)
        for potential in possible:
            # print(potential)
            snap = self.takeSnapshot()
            for switch in potential:
                self.addSwitchIndex(switch[0], switch[1])
        
            if min(self.position) == 0:
                if sum(self.position) == 0:
                    return least_val
                #print(self.position)
                switches_left = self.findNonZeroedButtons()
                if switches_left: 
                    brute_result = self.chainSmartLeastBrute()  
                    if brute_result >= 0:
                        return least_val + brute_result           
            self.restoreSnapshot(snap)          
                        
        return ERR_SMART_FAIL
        
    def chainSmartLeastComplexBrute(self, depth = 0, best_solution = float('inf')):
        steps = None
        # print(self.position)
        # print(self.puzzle.switches)
        least_idx = self.findLeastComplexIdx()
        contains_least = self.findContainingIdx(least_idx)
        not_zeroed = self.findNonZeroedButtons()
        allowed = tuple(set(contains_least) & set(not_zeroed))
        least_val = self.position[least_idx]
        if least_val + depth > best_solution:
            return ERR_DEPTH_EXCEEDED
        # if calcComplexity(len(allowed), least_val) > COMPLEXITY_CUTOFF:
        #     return ERR_BRUTE_COMPLEXITY
        # print(f"\033[1A\033[KChaining with: n={len(allowed)}, r={least_val}, complexity={calcComplexity(len(allowed), least_val)}")
        possible = smartCombinations(allowed, least_val)
        for potential in possible:
            # print(potential)
            snap = self.takeSnapshot()
            for switch in potential:
                self.addSwitchIndex(switch[0], switch[1])
        
            if min(self.position) == 0:
                if sum(self.position) == 0:
                    return least_val
                #print(self.position)
                switches_left = self.findNonZeroedButtons()
                if switches_left: 
                    if depth == 0 and steps:
                        best_solution = steps
                    brute_result = self.chainSmartLeastComplexBrute(least_val + depth, best_solution)  
                    if brute_result >= 0:
                        if not steps:
                            steps = brute_result
                        elif brute_result < steps:
                            steps = brute_result
                    elif brute_result == ERR_BRUTE_COMPLEXITY:
                        return ERR_BRUTE_COMPLEXITY
            self.restoreSnapshot(snap)          
                    
        if steps:     
            return steps + least_val
        else:
            return ERR_SMART_FAIL
        
    def chainSmartLeastComplexBrutePrediction(self, depth = 0, best_solution = float('inf')):
        steps = None
        least_idx = self.findLeastComplexIdx()
        contains_least = self.findContainingIdx(least_idx)
        not_zeroed = self.findNonZeroedButtons()
        allowed = tuple(set(contains_least) & set(not_zeroed))
        least_val = self.position[least_idx]
        possible = smartCombinations(allowed, least_val)
        for potential in possible:
            snap = self.takeSnapshot()
            for switch in potential:
                self.addSwitchIndex(switch[0], switch[1])
            if min(self.position) == 0:
                if sum(self.position) == 0:
                    return least_val
                switches_left = self.findNonZeroedButtons()
                if switches_left: 
                    if least_val + depth + max(self.position) <= best_solution:
                        brute_result = self.chainSmartLeastComplexBrutePrediction(least_val + depth, best_solution)  
                        if brute_result >= 0:
                            if not steps:
                                steps = brute_result
                            elif brute_result < steps:
                                steps = brute_result
                            if depth == 0:
                                new_best = steps + least_val
                                print(f"Found a solution: {best_solution} -> {new_best}")
                                best_solution = new_best            
                    elif steps:
                        return steps + least_val
            self.restoreSnapshot(snap)          
                    
        if steps:     
            return steps + least_val
        else:
            return ERR_SMART_FAIL
        
    def chainSimpleBrute(self):
        steps = None
        # print(self.position)
        # print(self.puzzle.switches)
        least_idx = self.findLeastComplexIdx()
        contains_least = self.findContainingIdx(least_idx)
        not_zeroed = self.findNonZeroedButtons()
        allowed = tuple(set(contains_least) & set(not_zeroed))
        least_val = self.position[least_idx]
        # print(f"Chaining with: n={len(allowed)}, r={least_val}, complexity={calcComplexity(len(allowed), least_val)}")
        step_size = least_val if least_val < CHAIN_SIMPLE_STEP else CHAIN_SIMPLE_STEP
        possible = smartCombinations(allowed, step_size)
        for potential in possible:
            # print(potential)
            snap = self.takeSnapshot()
            for switch in potential:
                self.addSwitchIndex(switch[0], switch[1])
            if min(self.position) >= 0:
                if sum(self.position) == 0:
                    return step_size
                #print(self.position)
                switches_left = self.findNonZeroedButtons()
                if switches_left: 
                    brute_result = self.chainSimpleBrute()  
                    if brute_result >= 0:
                        if not steps:
                            steps = brute_result
                        elif brute_result < steps:
                            steps = brute_result
            self.restoreSnapshot(snap)          
                    
        if steps:     
            return steps + step_size
        else:
            return ERR_SMART_FAIL
        
    def chainSimpleTree(self, buttons):
        steps = None
        # print(self.position)
        # print(self.puzzle.switches)
        least_idx = self.findLeastIdx()
        contains_least = self.findContainingIdx(least_idx)
        allowed = tuple(set(contains_least) & set(buttons))
        # print(f"Chaining with: n={len(allowed)}, r={least_val}, complexity={calcComplexity(len(allowed), least_val)}")
        for potential in allowed:
            # print(potential)
            snap = self.takeSnapshot()
            self.addSwitchIndex(potential)
            if min(self.position) >= 0:
                if sum(self.position) == 0:
                    return 1
                #print(self.position)
                switches_left = self.findNonZeroedButtons()
                if switches_left: 
                    brute_result = self.chainSimpleTree(switches_left)  
                    if brute_result >= 0:
                        if not steps:
                            steps = brute_result
                        elif brute_result < steps:
                            steps = brute_result
            self.restoreSnapshot(snap)          
                    
        if steps:     
            return steps + 1
        else:
            return ERR_SMART_FAIL
    

                
class SolverSnapshot:
    def __init__(self, solver: Solver):
        self.position = solver.position.copy()
        self.switches_used = solver.swithes_used.copy()
        self.switch_order = solver.switch_order.copy()
        
def smartCombinations(iterable, r):
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
        
def smartCombinationsNew(iterable, r):
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
            counts[i + 1] += 1
            for cleaner in range(i + 2, 0):
                counts[i + 1] += counts[cleaner]
                counts[cleaner] = 0
                
            yield tuple((pool[k], counts[k]) for k in range(n) if counts[k] > 0)
            break
        
def calcComplexity(n, r):
    NoC = math.comb(n + r - 1, r)
    complexity = NoC * r
    return complexity