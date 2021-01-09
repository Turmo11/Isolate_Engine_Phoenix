//Remember that constructor is mandatory

class Engine{
	construct new(){}
	foreign static Log(message)
	foreign Log(message)
}

Engine.Log("Welcome to my world...welcome to Wren!")


var engine_instance1 = Engine.new()
var engine_instance2 = Engine.new()
var engine_instance3 = Engine.new()
var engine_instance4 = Engine.new()

engine_instance1.Log("Yay! (child1)")
engine_instance2.Log("Oh oh... (child2)")
engine_instance3.Log("You should be sleeping man. (child3)")
engine_instance4.Log("Wre-what? (child4)")