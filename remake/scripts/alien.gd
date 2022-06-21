extends Spatial

var hp: int = 1

onready var tree:AnimationTree = $AnimationTree
var state_machine: AnimationNodeStateMachinePlayback
var player

func _ready():
	state_machine = tree.get("parameters/playback")

func attack() -> void:
	$VoiceSound.stop()
	$BiteSound.play()

func dead() -> void:
	queue_free()

func make_noise() -> void:
	$VoiceSound.play()

func _on_AlienArea_area_entered(area):
	var beam = area.get_parent()
	beam.hit_something()

	if state_machine.get_current_node() != "Hurt":
		state_machine.travel("Hurt")
		$SquishSound.play()
		$VoiceSound.stop()

		hp -= 1
		if hp <= 0:
			$AlienArea/CollisionShape.disabled = true
			if player:
				player.unblock(self)
			$BeamSound.play()
			state_machine.travel("Die")

