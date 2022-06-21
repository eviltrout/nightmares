extends Camera

const DAMPENING = 1.0
const POWER = 2.0
const MAX_X = 0.01
const MAX_Y = 0.01

var trauma: float = 0.0

func shake(amount: float = 0.5):
	trauma += amount

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	if trauma > 1.0:
		trauma = 1.0
	trauma -= (DAMPENING * delta)
	if trauma < 0.0:
		trauma = 0.0

	var power:float = pow(trauma, POWER)
	rotation.x = MAX_X * power * rand_range(-1, 1)
	rotation.z = MAX_Y * power * rand_range(-1, 1)

