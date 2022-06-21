extends Spatial

const MAX_SPEED = 4.0
const BOB_RADIUS = 0.01
const SHOOT_SPEED = 200

var accel: float = 0.0
var speed: float = 0.0
var last_shot: int = 0

onready var camera = $Camera
onready var gun_camera = $ViewportContainer/Viewport/GunCamera
onready var gun = $Gun
onready var laser_sound = $Gun/LaserSound
onready var gun_height:float = $Gun.transform.origin.y
onready var gun_source:Spatial = $Gun/Source
onready var gun_anim:AnimationPlayer = $Gun/AnimationPlayer
onready var footsteps = $Footsteps
var bob: float = 0.0

var beam_class
onready var env = $".."
var beam_target: Vector3

var blocked_by: Dictionary

func _ready():
	beam_class = load("res://scenes/beam.tscn")

func _input(event):
	if not(event is InputEventMouseButton or event is InputEventMouseMotion):
		return

	var mouse_pos:Vector2 = get_viewport().get_mouse_position()

	var raylen = 1000

	var from = camera.project_ray_origin(mouse_pos)
	var to = from + camera.project_ray_normal(mouse_pos) * raylen

	var space_state = get_world().get_direct_space_state()
	var result = space_state.intersect_ray(from, to, [], true, true, true)
	if result:
		beam_target = result.position
		gun.look_at(result.position, Vector3.UP)

func _physics_process(delta):
	gun_camera.global_transform = camera.global_transform

	if Input.is_action_pressed("ui_exit"):
	  get_tree().quit()

	if Input.is_action_pressed("ui_up") && blocked_by.empty():
		if !footsteps.playing:
			footsteps.play()
		accel = 0.075
	else:
		footsteps.stop()
		accel = 0.0
		speed = 0.0

	if Input.is_action_pressed("ui_fire"):
		var now: int = OS.get_ticks_msec()
		if beam_target && (now - last_shot) > SHOOT_SPEED:
			last_shot = now
			var new_beam:Spatial = beam_class.instance()
			new_beam.camera = camera
			env.add_child(new_beam)

			new_beam.transform.origin = gun_source.global_transform.origin
			new_beam.shoot_at(beam_target)
			gun_anim.play("Shoot")
			laser_sound.play()
#			new_beam.rotation = Vector3(gun.rotation.x, gun.rotation.y, gun.rotation.z)


	speed += accel
	if speed > MAX_SPEED:
		speed = MAX_SPEED

	bob += (accel * 1.5)

	gun.transform.origin.y = gun_height + (BOB_RADIUS * sin(bob))
	transform.origin.x -= (speed * delta)

func unblock(alien):
	var _erased:bool = blocked_by.erase(alien)

func block(alien):
	blocked_by[alien] = true
