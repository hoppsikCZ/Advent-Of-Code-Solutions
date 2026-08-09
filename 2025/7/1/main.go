package main

import (
	"fmt"
	"os"
	"strings"
)

type Tile int

const (
	Empty Tile = iota
	Origin
	Spliter
	Ray
)

func main() {
	input := getInput("input.txt")
	tiles := generateTileMap(input)

	splits := 0

	for i := 1; i < len(tiles); i++ {
		splits += updateRay(tiles, i)
	}

	fmt.Println(splits)
}

func getInput(fileName string) []string {
	inputBytes, err := os.ReadFile(fileName)
	if err != nil {
		panic(err)
	}

	return strings.Split(string(inputBytes), "\n")
}

func generateTileMap(input []string) [][]Tile {
	tilesMap := make([][]Tile, len(input))

	for x, line := range input {
		tilesMap[x] = make([]Tile, len(line))
		for y, char := range line {
			switch char {
			case '.':
				tilesMap[x][y] = Empty
			case 'S':
				tilesMap[x][y] = Origin
			case '^':
				tilesMap[x][y] = Spliter
			}
		}
	}

	return tilesMap
}

func updateRay(tileMap [][]Tile, row int) int {
	splits := 0
	for i, tile := range tileMap[row] {
		if tileMap[row-1][i] != Ray && tileMap[row-1][i] != Origin {
			continue
		}

		if tile == Ray {
			continue
		}

		if tile == Spliter {
			tileMap[row][i-1] = Ray
			tileMap[row][i+1] = Ray
			splits++
		} else {
			tileMap[row][i] = Ray
		}
	}
	return splits
}
