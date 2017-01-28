package net.natpad.avaj.vm.intermediate.item;

import net.natpad.avaj.ast.DeclarationType;
import net.natpad.avaj.ast.Identifier;
import net.natpad.avaj.ast.Literal;
import net.natpad.avaj.classfile.bytecode.ConditionType;
import net.natpad.avaj.util.TextUtil;
import net.natpad.avaj.vm.intermediate.AbstractIntermediateItem;
import net.natpad.avaj.vm.intermediate.IIntermediateNode;
import net.natpad.avaj.vm.intermediate.IntermediateRunContext;
import net.natpad.avaj.vm.subcode.Consumer;
import net.natpad.avaj.vm.subcode.DefaultValueProducer;
import net.natpad.avaj.vm.subcode.impl.SubCodeCondition;
import net.natpad.avaj.vm.type.CrossRuleMatchForCondition;
import net.natpad.avaj.vm.type.ValueTypeDefinition;
import net.natpad.avaj.vm.type.ValueTypeDefinitionKind;
import net.natpad.avaj.vm.type.ValueTypeDefinitionList;

public class IntermediateCondition extends AbstractIntermediateItem {

	private final ConditionType type;
	
	public final boolean withZero;
	public final boolean primitive;
	
	public IntermediateCondition(int index, ConditionType type, int jumpIndex, boolean withZero, boolean primitive) {
		super(index);
		scope.setJumpIndex(jumpIndex);
		this.type = type.opposite();
		this.withZero = withZero;
		this.primitive = primitive;
	}
	
	public IIntermediateNode makeCopy(boolean deep) {
		return new IntermediateCondition(mnemonicIndex, type.opposite(), scope.getJumpIndex(), withZero, primitive);
	}
	
	@Override
	public void connectStageOne(IntermediateRunContext modelRuntime) {
		SubCodeCondition subCode = new SubCodeCondition(mnemonicIndex, withZero, type);
		
		Consumer rightConsumer = new Consumer(this);
		Consumer leftConsumer = new Consumer(this);
		subCode.addConsumer(rightConsumer);
		subCode.addConsumer(leftConsumer);
		
		DefaultValueProducer valueProducer = new DefaultValueProducer(this);
		valueProducer.typeSet.add(new ValueTypeDefinition(DeclarationType.DEFAULT_BOOLEAN, ValueTypeDefinitionKind.CONDITION));
		subCode.setValueProducer(valueProducer);
		
		if (withZero) {
			DefaultValueProducer nilProducer = new DefaultValueProducer(this);
			if (primitive) {
				nilProducer.setExpression(new Literal(0));
			} else {
				nilProducer.setExpression(new Identifier("null"));
			}
			modelRuntime.pushValueProvider(nilProducer, 1);
		}
		modelRuntime.addAndRunSubCode(subCode);
		
		// TODO block below should be inserted between the consumer and producer postings
		if (!withZero) {
			ValueTypeDefinitionList leftHandSide = leftConsumer.getTypeSet();
			ValueTypeDefinitionList rightHandSide = rightConsumer.getTypeSet();
			modelRuntime.addValueTypeDefinitionList(leftHandSide);
			modelRuntime.addValueTypeDefinitionList(rightHandSide);
			CrossRuleMatchForCondition crossRule = new CrossRuleMatchForCondition(leftHandSide, rightHandSide);
			leftHandSide.addCrossRule(crossRule);
			rightHandSide.addCrossRule(crossRule);
		}
	}

	@Override
	public boolean isSame(IIntermediateNode otherNode) {
		if (otherNode instanceof IntermediateCondition) {
			IntermediateCondition that = (IntermediateCondition) otherNode;
			return type==that.type && withZero==that.withZero && primitive==that.primitive;
		}
		return false;
	}

	@Override
	public String toString() {
		StringBuilder buf = new StringBuilder();
		buf.append(TextUtil.getName(getClass())).append("[");
		buf.append("type=").append(type);
		buf.append(", primitive=").append(primitive);
		buf.append(", withZero=").append(withZero);
		return buf.append("]").toString();
	}
}
