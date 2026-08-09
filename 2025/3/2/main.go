package main

import (
	"fmt"
	"math"
	"os"
	"strings"
)

func main() {
	input := getInput("input.txt")
	result := 0

	for _, line := range input {
		result += getBatteries(line, 12)
	}

	fmt.Println(result)
}

func getInput(fileName string) []string {
	inputBytes, err := os.ReadFile(fileName)
	if err != nil {
		panic(err)
	}

	return strings.Split(string(inputBytes), "\n")
}

func getBatteries(line string, batCount int) int {
	var val, result, idx, prevIdx int
	for i := batCount - 1; i >= 0; i -= 1 {
		idx, val = getHighestBattery(line[prevIdx : len(line)-i])
		prevIdx += idx + 1
		result += int(math.Pow10(i)) * val
	}

	return result
}

func getHighestBattery(line string) (int, int) {
	var highestIdx, highestVal int
	for i, val := range line {
		valNum := int(val) - '0'
		if valNum > highestVal {
			highestIdx = i
			highestVal = valNum
		}
	}

	return highestIdx, highestVal
}
