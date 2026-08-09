from Puzzle import Puzzle

def parseInput(path):
    puzzles = []
    with open("input.txt") as file:
        lines = file.readlines()
        for line in lines:
            line_split = line.split(' ')
            solution_string = line_split[0][1:-1]
            solution = int(solution_string.replace('.', '0').replace('#', '1'), 2)
            switches = []
            length = len(solution_string)
            for switch in line_split[1:-1]:
                switch_value = 0
                bits = switch[1:-1].split(',')
                for bit in bits:
                    switch_value += pow(2, length - int(bit) - 1)
                    
                switches.append(switch_value)
            puzzles.append(Puzzle(solution, length, switches))
    
    return puzzles

puzzles = parseInput("input.txt")

result = 0
for puzzle in puzzles:  
    result += puzzle.find_optimal_solution()
    
print(result)