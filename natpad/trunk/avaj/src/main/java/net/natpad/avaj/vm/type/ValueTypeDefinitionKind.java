package net.natpad.avaj.vm.type;

public enum ValueTypeDefinitionKind {

	CATCHED_EXCEPTION(100), 
	ARGUMENT(100),
	LOCALVALUETABLE(100),
	CONDITION(90), 
	NULL(85), 
	CONSTANT(80), 
	NEW(70), 
	SUPER(60), 
	THIS(65), 
	RETURN(50), 
	CAST(30), 
	INSTANCEOF(30); 
	
	public final int priority;
	
	private ValueTypeDefinitionKind(int priority) {
		this.priority = priority;
	}
}
