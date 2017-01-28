package net.natpad.avaj.vm.type;

import java.util.ArrayList;
import java.util.HashSet;
import java.util.Set;

import net.natpad.avaj.ast.DeclarationType;
import net.natpad.avaj.classfile.bytecode.Type;
import net.natpad.avaj.log.Logger;
import net.natpad.avaj.util.TextUtil;

@SuppressWarnings("serial")
public class ValueTypeDefinitionList extends ArrayList<ValueTypeDefinition> {

	static Logger log = Logger.getInstance(ICrossRule.class);

	
	/* TODO: due to cross-typeset-rules this type_set might change. Either always create a new instance or make sure this single instance is unmodifiable  */
	public static final ValueTypeDefinitionList NULL_TYPE_SET = new ValueTypeDefinitionList();
	
	/* TODO: see NULL_TYPE_SET */
	public static final ValueTypeDefinitionList HIDDEN_EXCEPTION_TYPE_SET = new ValueTypeDefinitionList();
	
	private final ArrayList<ICrossRule> crossRules = new ArrayList<ICrossRule>();
	
	int hashcode;
	
	public ValueTypeDefinitionList() {
		hashcode = (int) (Math.random()*100000);
	}
	
	public ValueTypeDefinitionList(DeclarationType typeDescription, ValueTypeDefinitionKind defKind) {
		hashcode = (int) (Math.random()*100000);
		add(new ValueTypeDefinition(typeDescription, defKind));
	}


	
	public void addCrossRule(ICrossRule crossRule) {
		crossRules.add(crossRule);
		log.detail("add-cross-rule this="+this);
	}

	
	static enum CrossRuleState {
		UNRESOLVED,
		RUNNING,
		RESOLVED
	}
	
	CrossRuleState crossRulesState = CrossRuleState.UNRESOLVED;
	
	
	public static int primitiveTypeGetPrio(Type type) {
		switch(type) {
			case BOOLEAN : return 0;
			case BYTE : return 10; 
			case SHORT : return 20; 
			case INT : return 30; 
			case CHAR : return 35; 
			case FLOAT : return 40; 
			case LONG : return 50; 
			case DOUBLE : return 60; 
			case VOID : return 1000; 
		}
		return 2000;
	}
	
	public static boolean primitiveTypeIsPrefered(Type main, Type other) {
		return primitiveTypeGetPrio(other)>primitiveTypeGetPrio(main);
		
	}
	
	
	public boolean applyCrossRules() {
		if (crossRulesState == CrossRuleState.RESOLVED) {
			return true;
		}
		boolean result = true;

		log.debug("cross-rules.size="+crossRules.size()+", this="+this);

		int cridx = 0;
		for(ICrossRule crossRule : crossRules) {
			boolean didApply = crossRule.apply();
			log.detail("["+cridx+"] cross-rule="+crossRule+", didApply="+didApply);
			cridx++;
			if (!didApply) {
				result = false;
			}
		}
		if (result) {
			crossRulesState = CrossRuleState.RESOLVED;
		} else {
			switch(crossRulesState) {
				case RESOLVED : {
					result = true;
				} break;
				case RUNNING : {
					result = true;
					crossRulesState = CrossRuleState.RESOLVED;
				} break;
				case UNRESOLVED : {
					crossRulesState = CrossRuleState.RUNNING;
				}
			}
		}
		
		return result;
	}
	
	
	DeclarationType getLocalBestTypeDef() {
		DeclarationType result = null;
		ValueTypeDefinitionKind bestKind = ValueTypeDefinitionKind.NULL;

		for(ValueTypeDefinition valTypeDef : this) {
			if (valTypeDef.isIncluded) {
				if (result==null) {
					result = valTypeDef.typeDefinition;
					bestKind = valTypeDef.kind;
				} else {
					if (bestKind.priority<valTypeDef.kind.priority) {
						result = valTypeDef.typeDefinition;
						bestKind = valTypeDef.kind;
					} else if (bestKind.priority==valTypeDef.kind.priority) {

						if (result.isPrimitive() && valTypeDef.typeDefinition.isPrimitive()) {
							if (!primitiveTypeIsPrefered(result.type, valTypeDef.typeDefinition.type)) {
								result = valTypeDef.typeDefinition;
								bestKind = valTypeDef.kind;
							}
						}
					}
				}
			}
		}
		
		return result;
	}
	
	Set<DeclarationType> enlistForKind(ValueTypeDefinitionKind kind) {
		Set<DeclarationType> result = new HashSet<DeclarationType>();
		for(ValueTypeDefinition valTypeDef : this) {
			if (valTypeDef.isIncluded) {
				if(valTypeDef.kind==kind) {
					result.add(valTypeDef.typeDefinition);
				}
			}
		}
		return result;
	}

	Set<DeclarationType> enlistIncluded() {
		Set<DeclarationType> result = new HashSet<DeclarationType>();
		for(ValueTypeDefinition valTypeDef : this) {
			if (valTypeDef.isIncluded) {
				result.add(valTypeDef.typeDefinition);
			}
		}
		return result;
	}

	
	public boolean allCrossRulesApplied() {
		return crossRulesState==CrossRuleState.RESOLVED;
	}
	

	public boolean crossRulesRunning() {
		return crossRulesState==CrossRuleState.RUNNING;
	}

	
	public void copyTo(ValueTypeDefinitionList destination) {
		for(ValueTypeDefinition valTypeDef : this) {
			ValueTypeDefinition cloned = valTypeDef.cloneDefinition();
			destination.add(cloned);
		}
	}

	public DeclarationType getBestType() {
		DeclarationType result = getLocalBestTypeDef();
		if (result == null) {
			result = DeclarationType.DEFAULT_OBJECT;
		}
		return result;
	}
	
	
	
	@Override
	public boolean equals(Object other) {
		if (this==other) {
			return true;
		}
//		if (other instanceof ValueTypeDefinitionList) {
//			return super.equals(other);
//		}
		return false;
	}
	
	@Override
	public int hashCode() {
		return hashcode;
	}

	@Override
	public String toString() {
		
		String out = "";
		if (!isEmpty()) {
			for(ValueTypeDefinition valTypeDef : this) {
				if (out.length()>0) {
					out+=", ";
				}
				out += valTypeDef.typeDefinition;
				out += ":"+valTypeDef.kind;
			}
		}
		return TextUtil.getName(getClass())+"["+hashCode()+", types="+out+", crossrules="+crossRules.size()+"]";
	}

	public void excludeSet(Set<?> set, boolean opposite) {
		
		for(ValueTypeDefinition valTypeDef : this) {
			if (valTypeDef.isIncluded) {
				if (set.contains(valTypeDef.typeDefinition)) {
					if (!opposite) {
						valTypeDef.isIncluded = false;
					}
				} else {
					if (opposite) {
						valTypeDef.isIncluded = false;
					}
				}
			}
		}
		
	}



}
