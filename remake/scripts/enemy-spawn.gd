extends Spatial

const NEARBY = 4.0

onready var hole = $Hole
onready var alien_scene = preload("res://scenes/alien.tscn")
var spawned: bool = false
var player

func _ready():
	hole.visible = false

func wake(_player):
	if spawned:
		return

	player = _player

	$Scrape.play()
	spawned = true
	$Proximity/CollisionShape.disabled = true
	$AnimationPlayer.play("Grow")
	hole.visible = true

func _popout():
	if player:
		var alien = alien_scene.instance()
		alien.visible = false
		add_child(alien)
		alien.transform.origin = $AlienSpawnPoint.transform.origin
		alien.player = player
		alien.scale = Vector3(0.8, 0.8, 0.8)
		player.block(alien)


func _on_Proximity_body_entered(body):
	var p = body.owner
	wake(p)

	# If another spawn is close by, trigger it too
	var my_pos:Vector3 = global_transform.origin
	for s in get_tree().get_nodes_in_group("spawn"):
		if s != self:
			var d:float = my_pos.distance_to(s.global_transform.origin)
			if d < NEARBY:
				s.wake(p)


