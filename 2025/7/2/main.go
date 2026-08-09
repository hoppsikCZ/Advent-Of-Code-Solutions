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

	_, originY := findOrigin(tiles)
	rays := make([][]int, len(tiles))
	rays[0] = make([]int, len(tiles[0]))
	rays[0][originY] = 1

	for i := 1; i < len(tiles); i++ {
		rays[i] = make([]int, len(tiles[i]))
		updateRay(tiles, rays, i)
	}

	result := 0

	for i := 0; i < len(rays[len(rays)-1]); i++ {
		result += rays[len(rays)-1][i]
	}

	fmt.Println(result)

	// for i := 0; i < len(rays); i++ {
	// 	row := 0
	// 	for j := 0; j < len(rays[i]); j++ {
	// 		switch tiles[i][j] {
	// 		case Empty:
	// 			fmt.Print(". ")
	// 		case Origin:
	// 			fmt.Print("S ")
	// 		case Spliter:
	// 			fmt.Print("^ ")
	// 		}
	// 	}

	// 	fmt.Print("\n")

	// 	changed := true
	// 	for changed {
	// 		changed = false
	// 		for j := 0; j < len(rays[i]); j++ {
	// 			if len(strconv.Itoa(rays[i][j])) <= row {
	// 				fmt.Print("  ")
	// 			} else {
	// 				fmt.Print(rays[i][j]%int(math.Pow10(row+1))/int(math.Pow10(row)), " ")
	// 				changed = true
	// 			}
	// 		}
	// 		row++
	// 		fmt.Print("\n")
	// 	}
	// }

	//fmt.Println(simulateRay(tiles, originX, originY, len(tiles)))
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

func updateRay(tileMap [][]Tile, rays [][]int, row int) {
	for i, tile := range tileMap[row] {
		prevRays := rays[row-1][i]
		if prevRays == 0 {
			continue
		}

		if tile == Spliter {
			rays[row][i-1] += prevRays
			rays[row][i+1] += prevRays
		} else {
			rays[row][i] += prevRays
		}
	}
}

func findOrigin(tileMap [][]Tile) (int, int) {
	for posX, line := range tileMap {
		for posY, tile := range line {
			if tile == Origin {
				return posX, posY
			}
		}
	}

	return 0, 0
}

/*func simulateRay(tileMap [][]Tile, posX int, posY int, boundX int) int {
	for posX < boundX-1 {
		nextTile := tileMap[posX+1][posY]
		switch nextTile {
		case Empty:
			posX++
		case Spliter:
			return simulateRay(tileMap, posX+1, posY-1, boundX) + simulateRay(tileMap, posX+1, posY+1, boundX)
		}
	}

	return 1
}*/
