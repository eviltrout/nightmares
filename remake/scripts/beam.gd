extends Spatial

const BEAM_SPEED = 35.0
const LIFE_TIME = 5.0

var source: Vector3
var target: Vector3
var speed: float
var t: float = 0.0
var life: float
var camera: Camera

var exploding: bool = false

onready var explosion = $Explosion
onready var beam = $beam

func shoot_at(_target: Vector3):
	life = LIFE_TIME
	target = _target

	var local_target = to_local(target)
	look_at(target, Vector3.UP)

	source = transform.origin
	speed = local_target.distance_to(transform.origin) / BEAM_SPEED

func _process(delta):
	if beam.visible:
		transform.origin = lerp(source, target, t)
		t += (delta / speed)

	life -= delta
	if life < 0:
		queue_free()

func hit_something():
	if beam.visible:
		if camera:
			camera.shake()

		beam.visible = false
		life = 1.0

func _on_Area_body_entered(body):
	hit_something()

	if body is StaticBody:
		if body.is_in_group("glass"):
			$GlassHit.play()
		else:
			$MetalHit.play()
	exploding = true
	explosion.translation = to_local(target)
	explosion.emitting = true

