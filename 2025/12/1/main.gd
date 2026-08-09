extends SceneTree
var boards: Array[Board]
var present_counts: Array[Array]
var present_spaces: Array[int] = [7, 5, 7, 7, 6, 7]

func _init() -> void:
	var input := load_file()
	var parts := input.split("\n\n")
	parse_boards(parts[-1])
	var result := solve_boards()
	print("P: %s F: %s S: %s" % [result["passed"], result["failed"], result["skipped"]])


func load_file() -> String:
	var file = FileAccess.open("res://input.txt", FileAccess.READ)
	var content := file.get_as_text()
	return content


func parse_boards(board_string: String) -> void:
	var lines := board_string.split("\n", false)
	for line_idx in range(lines.size()):

		var formated := lines[line_idx].replace_chars("x:", 32)
		var parts := formated.split(" ", false)

		boards.append(Board.new(int(parts[0]), int(parts[1])))
		present_counts.append([])

		for part in parts.slice(2):
			present_counts[line_idx].append(int(part))

func solve_boards() -> Dictionary[String, int]:
	var passed := 0
	var fail := 0
	var skipped := 0

	for i in range(boards.size()):
		var count_of_presents := 0
		var count_of_spaces := 0
		for j in range(present_counts[i].size()):
			count_of_presents += present_counts[i][j]
			count_of_spaces += present_counts[i][j] * present_spaces[j]

		if count_of_presents * 9 < boards[i].area:
			passed += 1
		elif count_of_spaces > boards[i].area:
			fail += 1
		else:
			skipped += 1
	
	return { "passed": passed, "failed": fail, "skipped": skipped }
