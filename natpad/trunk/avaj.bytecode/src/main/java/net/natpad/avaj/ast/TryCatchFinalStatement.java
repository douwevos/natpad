package net.natpad.avaj.ast;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import net.natpad.avaj.ast.formater.Writer;


public class TryCatchFinalStatement implements IStatement {

	
	public final Block tryBlock;
	public final Block finalBlock;
	private final CatchBlock catchBlocks[];
	public int statementLineNr = -1;

	public TryCatchFinalStatement(Block tryBlock, Block finalBlock, CatchBlock ... catchBlocks) {
		this.tryBlock = tryBlock;
		this.finalBlock = finalBlock;
		this.catchBlocks = catchBlocks;
	}
	
	
	@Override
	public void setAtLeastLineNumber(int statementLineNr) {
		this.statementLineNr = statementLineNr;
	}
	
	public List<CatchBlock> getCatchBocks() {
		ArrayList<CatchBlock> result = new ArrayList<CatchBlock>();
		result.addAll(Arrays.asList(catchBlocks));
		return result;
	}

	@Override
	public void writeStatement(Writer out) {
		out.setAtLeastLineNr(statementLineNr);
		out.print("try ");
		tryBlock.writeStatement(out);
		if (catchBlocks!=null && catchBlocks.length>0) {
			for(CatchBlock cblock : catchBlocks) {
				cblock.writeStatement(out);
			}
		}
		if (finalBlock!=null) {
			finalBlock.writeStatement(out);
		}
		out.print("\n");
	}
}
