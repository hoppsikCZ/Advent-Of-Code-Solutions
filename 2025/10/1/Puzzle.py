from itertools import combinations

class Puzzle:
    def __init__(self, solution, solution_length, switches):
        self.solution = solution
        self.switches = switches
        self.length = solution_length
        
    def __str__(self):
        string = f"{self.solution:0{self.length}b} ["
        for i, switch in enumerate(self.switches):
            if i != 0:
                string += ", "
        
            string += f"{switch:0{self.length}b}"
            
        string += "]"
        
        return string
    
    def find_optimal_solution(self):
        for i in range(1, len(self.switches) + 1):
            for comb in combinations(self.switches, i):
                end_state = 0
                for operation in comb:
                    end_state ^= operation
                    
                if end_state == self.solution:
                    return len(comb)
            
        print("ERROR: Failed to find solution!")
        return 0