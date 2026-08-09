class_name Present

static var id_pool = 0
var id: int
var size: int
var orientations: Array[Shape]

func _init(orientation: Shape) -> void:
	id = id_pool
	id_pool += 1
	generate_orientations(orientation)


func find_dimensions:


func generate_orientations(orientation: Shape) -> void:
	



class Shape:
	var points: Array[Vector2i]

	func _init(new_points: Array[Vector2i]) -> void:
		points = new_points
	
