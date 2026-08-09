class_name Board

var width: int
var height: int
var area: int:
	get:
		return width * height

var data: Array[int]

func _init(c_width: int, c_height: int) -> void:
	width = c_width
	height = c_height

	for _i in range(height):
		data.append(0)
