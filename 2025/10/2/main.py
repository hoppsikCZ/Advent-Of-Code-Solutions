import time
from datetime import timedelta

from Puzzle import Puzzle, chainSmartLeastComplexBrutePrediction


def parseInput(path) -> list[Puzzle]:
    puzzles = []
    with open("input.txt") as file:
        lines = file.readlines()
        # print(lines)
        for line in lines:
            line_split = line.split(" ")
            solution_strings = line_split[-1][1:-2].split(",")
            # print(solution_strings)
            solution = tuple(map(int, solution_strings))
            switches = (
                tuple(sorted(map(int, switch[1:-1].split(","))))
                for switch in line_split[1:-1]
            )

            puzzles.append(Puzzle(solution, switches))

    return puzzles


START_PERFECT = time.perf_counter()

puzzles = parseInput("input.txt")

result = 0
passed = 0
failed = 0
for i, puzzle in enumerate(puzzles):
    start_time = time.perf_counter()
    start_pretty = td = timedelta(seconds=start_time - START_PERFECT)
    print(f"Solving {i}. ({start_pretty})")
    answer = chainSmartLeastComplexBrutePrediction(
        tuple(puzzle.solution), tuple(puzzle.switches)
    )
    end_time = time.perf_counter()
    end_pretty = td = timedelta(seconds=end_time - START_PERFECT)
    taken_time = end_time - start_time
    taken_pretty = timedelta(seconds=taken_time)
    if answer < 0:
        print(f"Failed {i} with code {answer}. {taken_pretty} ({end_pretty})")
        failed += 1
    else:
        print(f"Solved {i} in {answer} steps. {taken_pretty} ({end_pretty})")
        result += answer
        passed += 1

print(result)

print(f"Passed: {passed} Failed: {failed}")
