package net.natpad.avaj.vm.intermediate;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;

import net.natpad.avaj.ast.DeclarationType;
import net.natpad.avaj.ast.Name;
import net.natpad.avaj.classfile.attribute.LocalVariableTable;
import net.natpad.avaj.classfile.bytecode.IMnemonicBlock;
import net.natpad.avaj.log.Logger;
import net.natpad.avaj.vm.subcode.IConsumer;
import net.natpad.avaj.vm.subcode.ISubCode;
import net.natpad.avaj.vm.subcode.IValueProducer;
import net.natpad.avaj.vm.subcode.block.SubCodeBlock;
import net.natpad.avaj.vm.subcode.impl.MethodFrameValue;
import net.natpad.avaj.vm.subcode.impl.ThisOrSuperValue;
import net.natpad.avaj.vm.type.ImportRepository;
import net.natpad.avaj.vm.type.ValueTypeDefinition;
import net.natpad.avaj.vm.type.ValueTypeDefinitionKind;
import net.natpad.avaj.vm.type.ValueTypeDefinitionList;

public class IntermediateRunContext {

	static Logger log = Logger.getInstance(IntermediateRunContext.class);
	
	public final IMnemonicBlock mnemonicBlock;
	public final LocalVariableTable localVariableTable;
	
	public final List<IConsumer> consumerStack = new ArrayList<IConsumer>();

	public final List<IValueProducer> producerStack = new ArrayList<IValueProducer>();
	public final List<IValueProducer> pseudoStack = new ArrayList<IValueProducer>();
	
	private Map<Integer, MethodFrameValue> methodFrameMap = new HashMap<Integer, MethodFrameValue>(); 

	
	public Set<ValueTypeDefinitionList> valueTypeDefinitionLists = new HashSet<ValueTypeDefinitionList>();
	
	public ArrayList<IPostImport> postImportList = new ArrayList<IPostImport>();
	
	public SubCodeBlock subCodeBlock = new SubCodeBlock();
	
	public IntermediateRunContext(IMnemonicBlock mnemonicBlock, LocalVariableTable localVariableTable) {
		this.mnemonicBlock = mnemonicBlock;
		this.localVariableTable = localVariableTable;
	}

	public void setThis(Name importType, Name superType) {
		MethodFrameValue methodFrameValue = new ThisOrSuperValue(0, new DeclarationType(importType, 0));
		methodFrameValue.add(new ValueTypeDefinition(new DeclarationType(importType,0), ValueTypeDefinitionKind.THIS));
		if (superType!=null) {
			methodFrameValue.add(new ValueTypeDefinition(new DeclarationType(superType,0), ValueTypeDefinitionKind.SUPER));
		}
		methodFrameMap.put(0, methodFrameValue);
	}

	
	public void addAndRunSubCode(ISubCode subCode) {
		int consumerCount = 0;
		while(true) {
			IConsumer consumer = subCode.getInputConsumerAt(consumerCount);
			if (consumer!=null) {
				pushValueConsumer(consumer);
				consumerCount++;
			} else {
				break;
			}
		}
		
		IValueProducer outputProducer = subCode.getOutputProducer();
		if (outputProducer!=null) {
			for(int idx=0; idx<consumerCount; idx++) {
				outputProducer.addInputConsumer(subCode.getInputConsumerAt(idx));
			}
			pushValueProvider(outputProducer);
		}
		
		add(subCode);
	}
	
	
	public void connect(ImportRepository importRepository) {
//		modelContainer.connectStageOne(this);
		resolveTypes();
		for(IPostImport postImport : postImportList) {
			postImport.addImport(importRepository);
		}
	}
	
	public IntermediateRunContext createBranch() {
		IntermediateRunContext result = new IntermediateRunContext(mnemonicBlock, localVariableTable);
		result.consumerStack.addAll(consumerStack);
		result.producerStack.addAll(producerStack);
//		result.pseudoStack.addAll(pseudoStack);
		result.methodFrameMap.putAll(methodFrameMap);
		result.postImportList = postImportList;
		result.valueTypeDefinitionLists = valueTypeDefinitionLists;
		return result;
	}
	
	private void resolveTypes() {
		log.detail("resolve-types: valueTypeDefinitionLists.size="+valueTypeDefinitionLists.size());
		ArrayList<ValueTypeDefinitionList> stack = new ArrayList<ValueTypeDefinitionList>(valueTypeDefinitionLists);
//		for(int i=0; i<stack.size(); i++) {
//			log.debug("stack["+i+"]="+stack.get(i));
//		}
		while(!stack.isEmpty()) {
			ValueTypeDefinitionList valueTypeDefinitionList = stack.remove(0);
			log.trace("resolve-types: valueTypeDefinitionList="+valueTypeDefinitionList);
			boolean result = valueTypeDefinitionList.applyCrossRules();
			if (result==false) {
				stack.add(valueTypeDefinitionList);
			}
		}
		
		
	}

	public IValueProducer peekProducer(int index) {
		int offset = producerStack.size()-1 - index;
		if (offset>=0) {
			return producerStack.get(offset);
		}
		return null;
	}

	
	public IValueProducer popProducer() {
		IValueProducer producer = producerStack.remove(producerStack.size()-1);
		return producer;
	}
	
	public void pushValueProvider(IValueProducer producer) {
		log.debug("pushing provider:"+producer);
		producerStack.add(producer);
	}

	public void pushValueProvider(IValueProducer producer, int index) {
//		log.debug("pushing provider:"+producer);
		int offset = producerStack.size()-index;
		if (offset>=0) {
			producerStack.add(offset, producer);
		} else {
			System.err.println("invalid stack:producer="+producer+", index="+index+", offset="+offset);
		}
	}

	
	public void pushPseudoProducer(IValueProducer producer) {
		pseudoStack.add(producer);
	}

	public void pushValueConsumer(IConsumer consumer) {
		log.debug("pushing consumer:"+consumer+", producerStack.size="+producerStack.size());
		if (producerStack.isEmpty()) {
			consumerStack.add(consumer);
//			consumer.getExpression(null);
		} else {
			IValueProducer producer = popProducer();
			log.debug("consumer:"+consumer+" connected with producer:"+producer);
			producer.setConsumer(consumer);
			consumer.consume(producer);
		}
	}
	
	public void dumpStacks() {
		if (producerStack.isEmpty()) {
			log.debug("producer-stack:empty");
		} else {
			log.debug("producer-stack:"+producerStack.size());
			for(int idx=0; idx<producerStack.size(); idx++) {
				log.debug("["+idx+"] "+producerStack.get(idx));
			}
		}
	}

	

	public MethodFrameValue getMethodFrameValue(int frameIndex) {
		return methodFrameMap.get(frameIndex);
	}
	
	public MethodFrameValue createMethodFrameValue(int frameIndex) {
		MethodFrameValue result = methodFrameMap.get(frameIndex);
		if (result==null) {
			result = new MethodFrameValue(frameIndex, null);
			methodFrameMap.put(frameIndex, result);
		}
		return result;
	}

	public void add(int frameIndex, MethodFrameValue methodFrameValue) {
		methodFrameMap.put(frameIndex, methodFrameValue);
		
	}

	

	public void addValueTypeDefinitionList(ValueTypeDefinitionList valueTypeDefList) {
		boolean add = valueTypeDefinitionLists.add(valueTypeDefList);
		log.debug("#### valueTypeDefList="+valueTypeDefList+", result="+add+", size="+valueTypeDefinitionLists.size());
	}
	
	public void add(ISubCode subCode) {
		log.debug(" >> adding subcode:"+subCode);
		subCodeBlock.append(subCode);
	}
	
	
	public void addPostImporter(IPostImport postImport) {
		postImportList.add(postImport);
	}


}


