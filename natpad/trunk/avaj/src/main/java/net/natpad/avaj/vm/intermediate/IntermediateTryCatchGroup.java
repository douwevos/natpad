package net.natpad.avaj.vm.intermediate;

import java.util.ArrayList;
import java.util.List;

import net.natpad.avaj.ast.DeclarationType;
import net.natpad.avaj.classfile.TryCatch;
import net.natpad.avaj.log.Logger;
import net.natpad.avaj.vm.subcode.impl.SubCodeCatch;
import net.natpad.avaj.vm.subcode.impl.SubCodeFinally;
import net.natpad.avaj.vm.subcode.impl.SubCodeTryCatchFinal;

public class IntermediateTryCatchGroup extends IntermediateGroup {

	public final TryCatch tryCatch;

	
	private boolean catchBlocksResolved = false;
	private List<ItermediateCatchGroup> catchGroups = new ArrayList<ItermediateCatchGroup>();
	private IntermediateFinallyGroup finallyGroup;

	
	public IntermediateTryCatchGroup(TryCatch tryCatch) {
		this.tryCatch = tryCatch;
	}
	
	public void add(IntermediateGroup catchOrFinalGroup) {
		if (catchOrFinalGroup instanceof ItermediateCatchGroup) {
			catchGroups.add((ItermediateCatchGroup) catchOrFinalGroup);
		} else {
			finallyGroup = (IntermediateFinallyGroup) catchOrFinalGroup;
		}
	}
	
	
	public ItermediateCatchGroup findCatchGroup(DeclarationType exceptionType) {
		for(ItermediateCatchGroup catchGroup : catchGroups) {
			if (catchGroup.exceptionType == exceptionType) {
				return catchGroup;
			}
		}
		return null;
	}
	
	public IntermediateFinallyGroup getFinallyGroup() {
		return finallyGroup;
	}

	
	
	public boolean isStale() {
		return catchGroups.isEmpty() && (finallyGroup==null || finallyGroup.count()==0);
	}
	
	public void setCatchBlocksResolved(boolean val) {
		catchBlocksResolved = val;
	}
	
	public boolean arecatchBlocksResolved() {
		return catchBlocksResolved;
	}
	
	
	@Override
	public void connectStageOne(IntermediateRunContext modelRuntime) {
		IntermediateRunContext mainContext = modelRuntime.createBranch();
		super.connectStageOne(mainContext);

		
		SubCodeCatch catchSubCodes[] = null;
		if (catchGroups!=null && catchGroups.size()>0) {
			catchSubCodes = new SubCodeCatch[catchGroups.size()];
			int idx = 0;
			for(ItermediateCatchGroup catchGroup : catchGroups) {
				catchGroup.connectStageOne(modelRuntime.createBranch());
				catchSubCodes[idx] = catchGroup.subCodeCatch;
				idx++;
			}
		}
		
		SubCodeFinally subCodeFinally = null;
		if (finallyGroup!=null) {
			finallyGroup.connectStageOne(modelRuntime.createBranch());
			subCodeFinally = finallyGroup.subCodeFinally;
		}
		log.detail("subCodeFinally="+subCodeFinally+", finallyGroup="+finallyGroup);
		
		SubCodeTryCatchFinal subCode = new SubCodeTryCatchFinal(mainContext.subCodeBlock, subCodeFinally, catchSubCodes);
		modelRuntime.add(subCode);
	}
	
	
	@Override
	public IIntermediateNode makeCopy(boolean deep) {
		IntermediateTryCatchGroup result = new IntermediateTryCatchGroup(tryCatch);
		for(IIntermediateNode child : this) {
			if (deep) {
				add(child.makeCopy(true));
			} else {
				add(child);
			}
		}

		if (deep) {
			for(ItermediateCatchGroup catchGroup : catchGroups) {
				result.add((IntermediateGroup) catchGroup.makeCopy(true));
			}
			if (finallyGroup!=null) {
				result.finallyGroup = (IntermediateFinallyGroup) finallyGroup.makeCopy(true);
			}
		} else {
			result.catchGroups.addAll(catchGroups);
			result.finallyGroup = finallyGroup;
		}
		
		result.setCatchBlocksResolved(catchBlocksResolved);
		return result;
	}
	
	public void dump(StringBuilder dumpBuffer, String prefix) {
		dumpBuffer.append(prefix).append(toString()).append(Logger.ln);
		prefix+="   ";
		dumpBuffer.append(prefix).append("try").append(Logger.ln);
		String prefixSub = prefix+"   ";
		
		for(IIntermediateNode node : groupList) {
			node.dump(dumpBuffer, prefixSub);
		}

		for(ItermediateCatchGroup catchGroup : catchGroups) {
			dumpBuffer.append(prefix).append("catch").append(Logger.ln);
			catchGroup.dump(dumpBuffer, prefixSub);
		}

		if (finallyGroup!=null) {
			dumpBuffer.append(prefix).append("finally").append(Logger.ln);
			finallyGroup.dump(dumpBuffer, prefixSub);
		}
	}


	
}
