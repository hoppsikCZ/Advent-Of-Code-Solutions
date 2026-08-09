package main

import (
	"fmt"
	"os"
	"strings"
)

func main() {
	input := getInput("input.txt")
	result := 0

	for _, line := range input {
		result += getBatteries(line)
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

func getBatteries(line string) int {
	firstIdx, firstBat := getHighestBattery(line[:len(line)-1])

	secondLine := line[firstIdx+1:]

	_, secondBat := getHighestBattery(secondLine)

	fmt.Println(firstBat*10 + secondBat)

	return firstBat*10 + secondBat
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
