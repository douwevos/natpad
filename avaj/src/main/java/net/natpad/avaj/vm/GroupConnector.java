package net.natpad.avaj.vm;

import java.util.ArrayList;
import java.util.Collections;
import java.util.HashSet;
import java.util.List;
import java.util.Set;
import java.util.Stack;

import net.natpad.avaj.ast.DeclarationType;
import net.natpad.avaj.ast.IExpression;
import net.natpad.avaj.ast.Literal;
import net.natpad.avaj.classfile.Classfile;
import net.natpad.avaj.classfile.ClassfileCatch;
import net.natpad.avaj.classfile.ClassfileMethod;
import net.natpad.avaj.classfile.ICatchOrFinally;
import net.natpad.avaj.classfile.TryCatch;
import net.natpad.avaj.classfile.TryCatchList;
import net.natpad.avaj.classfile.attribute.AttributeLocalVariableTable;
import net.natpad.avaj.classfile.attribute.LocalVariableTable;
import net.natpad.avaj.classfile.bytecode.MnemonicBlock;
import net.natpad.avaj.classfile.bytecode.Operator;
import net.natpad.avaj.classfile.bytecode.mnemonic.OpDuplicate.DupType;
import net.natpad.avaj.log.LogLevel;
import net.natpad.avaj.log.Logger;
import net.natpad.avaj.vm.intermediate.IIntermediateNode;
import net.natpad.avaj.vm.intermediate.IntermediateFinallyGroup;
import net.natpad.avaj.vm.intermediate.IntermediateForLoop;
import net.natpad.avaj.vm.intermediate.IntermediateGroup;
import net.natpad.avaj.vm.intermediate.IntermediateLoopGroup;
import net.natpad.avaj.vm.intermediate.IntermediateNodeScope;
import net.natpad.avaj.vm.intermediate.IntermediateRunContext;
import net.natpad.avaj.vm.intermediate.IntermediateTryCatchGroup;
import net.natpad.avaj.vm.intermediate.item.IntermediateConditionPusher;
import net.natpad.avaj.vm.intermediate.item.IntermediateConditionSet;
import net.natpad.avaj.vm.intermediate.item.IntermediateConstant;
import net.natpad.avaj.vm.intermediate.item.IntermediateDuplicate;
import net.natpad.avaj.vm.intermediate.item.IntermediateFrameGet;
import net.natpad.avaj.vm.intermediate.item.IntermediateFramePut;
import net.natpad.avaj.vm.intermediate.item.IntermediateGoto;
import net.natpad.avaj.vm.intermediate.item.IntermediateIfThenElse;
import net.natpad.avaj.vm.intermediate.item.IntermediateIncrement;
import net.natpad.avaj.vm.intermediate.item.IntermediateMathBasic;
import net.natpad.avaj.vm.intermediate.item.IntermediateMonitorEnter;
import net.natpad.avaj.vm.intermediate.item.IntermediateMonitorExit;
import net.natpad.avaj.vm.intermediate.item.IntermediateThrow;
import net.natpad.avaj.vm.subcode.IConsumer;
import net.natpad.avaj.vm.subcode.IValueProducer;
import net.natpad.avaj.vm.type.ImportRepository;


public class GroupConnector {
	
	static Logger log = Logger.getInstance(GroupConnector.class);
	
	public final IntermediateRunContext connectContext;
	public final IntermediateGroup modelContainer; 
	public final ImportRepository importRepository;
	public final MnemonicBlock mnemonicBlock;
	private final LocalVariableTable localVariableTable;
	
	public GroupConnector(ImportRepository importRepository, Classfile typeDefinition, ClassfileMethod parsedMethod, MnemonicBlock mnemonicBlock, TryCatchList tryCatchList) {
		this.importRepository = importRepository;
		this.mnemonicBlock = mnemonicBlock;
		
		if (parsedMethod!=null) {
			AttributeLocalVariableTable attributeLocalVariableTable = parsedMethod.getLocalVariableTable();
			if (attributeLocalVariableTable!=null) {
					localVariableTable = attributeLocalVariableTable.getTable();
			} else {
				localVariableTable = null;
			}
		} else {
			localVariableTable = null;
		}
		
		IntermediateGroupCreator blockFactory = new IntermediateGroupCreator(importRepository, typeDefinition, parsedMethod, mnemonicBlock, tryCatchList);
		
		modelContainer = blockFactory.modelContainer;
		log.debug("--------------------------------------------------------------------");
		modelContainer.dump("A:");
		convertGroup(modelContainer);
		log.debug("--------------------------------------------------------------------");
		modelContainer.dump("B:");
		
		connectContext = new IntermediateRunContext(mnemonicBlock, localVariableTable);
	}
	

	public void convertGroup(IntermediateGroup groupContainer) {
		
		int round=0;
		while(true) {
			groupContainer.dump("round-"+round+++":");

			eliminateDuplicates(groupContainer);
			detectNonIntegerIncrementOpps(groupContainer);

			if (traceAndConditionModel(groupContainer)) {
				log.debug("did trace and-condition-model");
				continue;
			}
			if (traceLoop(groupContainer)) {
				log.debug("did trace loop");
				continue;
			}

			if (traceForLoop(groupContainer)) {
				log.debug("did trace forloop");
				continue;
			}

			
//			if (traceBooleanCondition(groupContainer)) {
//				continue;
//			}
			if (traceIfThenElseModel(groupContainer)) {
				log.debug("did trace if-then-else");
				continue;
			}
			if (traceCatchBlocks(groupContainer)) {
				log.debug("did trace catch-block");
				continue;
			}
			if (traceSynchronizedBlocks(groupContainer)) {
				log.debug("did trace synchronized-block");
				continue;
			}
			break;
		}
		log.debug("end of connect for group:"+groupContainer);
	}
	

	
	private void eliminateDuplicates(IntermediateGroup groupContainer) {
		for(int modelIndex=0; modelIndex<groupContainer.count(); modelIndex++) {
			IIntermediateNode childGroup = groupContainer.get(modelIndex);
			if (childGroup instanceof IntermediateGroup) {
				eliminateDuplicates((IntermediateGroup) childGroup);
				continue;
			}
			if (childGroup instanceof IntermediateDuplicate) {
				IntermediateDuplicate grpDuplicate = (IntermediateDuplicate) childGroup;
				// TODO: DUP2 its behavior depends on the type of the value pushed onto the stack. At this point we do not yet have a stack.
				if (grpDuplicate.dupType==DupType.DUP2) {
					
					/* detect           Duplicate.dup2
					 *                  FramePut x
					 * replace with:    FramePut x
					 *                  FrameGet x
					 */
					if (modelIndex+1<groupContainer.count()) {
						IIntermediateNode next = groupContainer.get(modelIndex+1);
						if (next instanceof IntermediateFramePut) {
							IntermediateFrameGet replacementFrameGet = new IntermediateFrameGet(grpDuplicate.mnemonicIndex, ((IntermediateFramePut) next).frameIndex);
							groupContainer.remove(modelIndex);
							groupContainer.add(modelIndex+1, replacementFrameGet);
						}
					}
					
					/* detect           FrameGet x
					 *                  Duplicate.dup2
					 * replace with:    FrameGet x
					 *                  FrameGet x
					 */
					if (modelIndex>0) {
						IIntermediateNode next = groupContainer.get(modelIndex-1);
						if (next instanceof IntermediateFrameGet) {
							IntermediateFrameGet replacementFrameGet = new IntermediateFrameGet(grpDuplicate.mnemonicIndex, ((IntermediateFrameGet) next).frameIndex);
							groupContainer.replace(modelIndex, replacementFrameGet);
						}
					}
				}
			}
		}
	}
	
	
	/* detects 'i++' where i is not an integer */
	private void detectNonIntegerIncrementOpps(IntermediateGroup groupContainer) {
		for(int modelIndex=0; modelIndex<groupContainer.count(); modelIndex++) {
			IIntermediateNode node = groupContainer.get(modelIndex);
			if (node instanceof IntermediateGroup) {
				detectNonIntegerIncrementOpps((IntermediateGroup) node);
				continue;
			}
			if (node instanceof IntermediateFrameGet) {
				log.debug("node="+node);
				if (modelIndex+3<groupContainer.count()) {
					IIntermediateNode node1 = groupContainer.get(modelIndex+1);
					IIntermediateNode node2 = groupContainer.get(modelIndex+2);
					IIntermediateNode node3 = groupContainer.get(modelIndex+3);
					if ((node1 instanceof IntermediateConstant) &&
						(node2 instanceof IntermediateMathBasic) &&
						(node3 instanceof IntermediateFramePut)) {

						IntermediateFrameGet grpFrameGet = (IntermediateFrameGet) node;
						IntermediateConstant grpConstant = (IntermediateConstant) node1;
						IntermediateMathBasic grpMathBasic = (IntermediateMathBasic) node2;
						IntermediateFramePut grpFramePut = (IntermediateFramePut) node3;
						
						if (grpFrameGet.frameIndex!=grpFramePut.frameIndex) {
							continue;
						}
						
						
						/* is ADD or SUB */
						boolean isIncrease = true;
						if (grpMathBasic.operator==Operator.SUB) {
							isIncrease = false;
						} else if (grpMathBasic.operator!=Operator.ADD) {
							continue;
						}
						
						/* check if constant is 1 */
						if (grpConstant.defaultExpression instanceof Literal) {
							Literal literal = (Literal) grpConstant.defaultExpression;
							if (literal.value instanceof Number) {
								Number n = (Number) literal.value;
								if (n.doubleValue()!=1.0d) {
									continue;
								}
							} else {
								continue;
							}
						} else {
							continue;
						}
						
						
						/* replace four */
						groupContainer.remove(modelIndex);
						groupContainer.remove(modelIndex);
						groupContainer.remove(modelIndex);
						groupContainer.remove(modelIndex);
						
						IntermediateIncrement grpIncrement = new IntermediateIncrement(-1, grpFrameGet.frameIndex, isIncrease ? 1 : -1);
						groupContainer.add(modelIndex, grpIncrement);

					}
				}
			}
		}
	}

	private boolean traceSynchronizedBlocks(IntermediateGroup groupContainer) {
		for(int modelIndex=0; modelIndex<groupContainer.count(); modelIndex++) {
			IIntermediateNode childGroup = groupContainer.get(modelIndex);
			if (childGroup instanceof IntermediateGroup) {
				IntermediateGroup grp = (IntermediateGroup) childGroup;
				IIntermediateNode grpLast = grp.getLast();
				if (grpLast instanceof IntermediateMonitorEnter) {
					IntermediateMonitorEnter monitorGroup = (IntermediateMonitorEnter) grpLast;
					if (!monitorGroup.isResolved()) {
						log.detail("monitorGroup="+monitorGroup);
						if (modelIndex+1<groupContainer.count()) {
							IIntermediateNode monitorBlockNode = groupContainer.get(modelIndex+1);
							log.detail("monitorBlockNode="+monitorBlockNode);
							if (monitorBlockNode instanceof IntermediateTryCatchGroup) {
								IntermediateTryCatchGroup tryCatchGroup = (IntermediateTryCatchGroup) monitorBlockNode;
								IntermediateFinallyGroup finallyGroup = tryCatchGroup.getFinallyGroup();
								IntermediateGroup subGroup = (IntermediateGroup) finallyGroup.getLast();
								IIntermediateNode last = subGroup.getLast();
								log.detail("last="+last);
								if (last instanceof IntermediateThrow) {
									subGroup.remove(subGroup.count()-1);
								}
								
								
								last = subGroup.getLast();
								if (last instanceof IntermediateMonitorExit) {
									subGroup.remove(subGroup.count()-1);
								}
								
								last = subGroup.getLast();
								if (last instanceof IntermediateFrameGet) {
									subGroup.remove(subGroup.count()-1);
								}
								
								
								
								if (subGroup.count()==0) {
									finallyGroup.remove(finallyGroup.count()-1);
								}
								
								if (tryCatchGroup.isStale()) {
									IntermediateGroup replacement = (IntermediateGroup) (tryCatchGroup.getLast()).makeCopy(false);
									groupContainer.remove(modelIndex+1);
									monitorGroup.add(replacement);
								}
								
								
							} else {
								System.err.println("expected a try-catch-finally-group following monitorenter:");
								monitorGroup.dump("ERR:");
							}
						} else {
							System.err.println("nothing following monitorenter:");
							monitorGroup.dump("ERR:");
						}
						monitorGroup.setResolved(true);
						IntermediateRunContext groupConnectRuntime = new IntermediateRunContext(mnemonicBlock, localVariableTable);
						monitorGroup.connectStageOne(groupConnectRuntime);
						convertGroup(monitorGroup);
						
						groupContainer.dump("JJ:");
						
						return true;
					}
				}
			}
		}
		return false;
	}
	
	private boolean traceCatchBlocks(IntermediateGroup groupContainer) {
		for(int modelIndex=0; modelIndex<groupContainer.count(); modelIndex++) {
			IIntermediateNode childGroup = groupContainer.get(modelIndex);
			if (childGroup instanceof IntermediateTryCatchGroup) {
				IntermediateTryCatchGroup tryCatchGroup = (IntermediateTryCatchGroup) childGroup;
				if (tryCatchGroup.arecatchBlocksResolved()) {
					continue;
				}
				TryCatch tryCatch = tryCatchGroup.tryCatch;
				Set<IIntermediateNode> mainSet = collectForward(groupContainer, tryCatchGroup);					
				log.debug("resolving try-catch-block:tryCatchGroup="+tryCatchGroup.hashCode()+", forward-set="+mainSet.size());
				for(ICatchOrFinally catchItem : tryCatch) {
					int handlePc = mnemonicBlock.findByBytecodeOffset(catchItem.getHandlePc());
					IIntermediateNode findNode = groupContainer.findNode(handlePc);
					if (findNode ==null) {
						log.warn(""+catchItem);
						continue;
					}
					IntermediateGroup catchOrFinallyGroup = null;
					if (catchItem instanceof ClassfileCatch) {
						catchOrFinallyGroup = tryCatchGroup.findCatchGroup(((ClassfileCatch) catchItem).exceptionType);
					} else {
						catchOrFinallyGroup = tryCatchGroup.getFinallyGroup();
					}
					
					if (catchOrFinallyGroup==null) {
						log.warn("no group found :::"+catchItem+", tryCatchGroup="+tryCatchGroup);
						tryCatchGroup.dump("---");
						continue;
					}
					
					catchOrFinallyGroup.add(findNode);
					Set<IIntermediateNode> inSet = new HashSet<IIntermediateNode>(mainSet);
					inSet.add(findNode);
					Stack<IIntermediateNode> stack = new Stack<IIntermediateNode>();
					stack.add(findNode);
					while(!stack.isEmpty()) {
						IIntermediateNode groupNode = stack.pop();
						
						log.detail("groupNode.class="+groupNode.getClass().getName());
						IntermediateNodeScope scope = groupNode.getScope();
						log.detail("scope="+scope);
						int exitIndex = scope.getExitIndex();
						if (exitIndex>=0) {
							IIntermediateNode node = groupContainer.findNode(exitIndex);
							if (node!=null && !inSet.contains(node)) {
								if (catchOrFinallyGroup.add(node)) {
									stack.push(node);
								}
							}
						}
						int jumpIndex = scope.getJumpIndex();
						if (jumpIndex>=0) {
							IIntermediateNode node = groupContainer.findNode(jumpIndex);
							if (node!=null && !inSet.contains(node)) {
								if (catchOrFinallyGroup.add(node)) {
									stack.push(node);
								}
							}
						}
					}
					for(IIntermediateNode node : catchOrFinallyGroup) {
						groupContainer.remove(node);
					}
				}
				
				
				convertGroup(tryCatchGroup);
				
				
				/* match code from final block with continuation block */
				int continuationIndex = tryCatchGroup.getScope().getExitIndex();
				log.trace("continuationIndex="+continuationIndex);
				IIntermediateNode foundContinuationNode = groupContainer.findNode(continuationIndex);
				if (log.logEnabbledFor(LogLevel.DETAIL)) {
					StringBuilder buf = new StringBuilder();
					foundContinuationNode.dump(buf, "C::");
					log.detail(buf.toString());
				}
				
				
				
				if (foundContinuationNode instanceof IntermediateGroup) {
					IntermediateGroup continuationGroup = (IntermediateGroup) foundContinuationNode;

					int cntGrpCount = continuationGroup.count();
					
					IntermediateFinallyGroup finallyGroup = tryCatchGroup.getFinallyGroup();
					if (finallyGroup!=null) {
						IIntermediateNode finGrpNode = finallyGroup.get(0);
						if (finGrpNode instanceof IntermediateGroup) {
							IntermediateGroup finGrp = (IntermediateGroup) finGrpNode; 
							int finGrpCount = finGrp.count();
							finGrp.dump("D::");
							
							int fidx = 0;
							int fstart = -1;
							int midx=0;
							for(midx=0; midx<cntGrpCount && fidx<finGrpCount; midx++) {
								IIntermediateNode continuationSubNode = continuationGroup.get(midx);
								if (midx==0) {
									while(fidx<finGrpCount) {
										IIntermediateNode finSubNode = finGrp.get(fidx);
										
										fidx++;
										boolean same = finSubNode.isSame(continuationSubNode);
										log.trace("same="+same+", first match:"+finSubNode+", with="+continuationSubNode);
										if (same) {
											fstart = fidx;
											break;
										}
									}
								} else {
									IIntermediateNode finSubNode = finGrp.get(fidx);
									fidx++;
									if (!finSubNode.isSame(continuationSubNode)) {
										break;
									}
								}
							}
							if (fstart>0 && midx>0) {
								while(midx>0) {
									continuationGroup.remove(0);
									midx--;
								}
							}
						}
					}
					
					
				}
				
				
				tryCatchGroup.setCatchBlocksResolved(true);
				return true;
			}
		}
		return false;
	}

	public void resolveBreaks(IntermediateLoopGroup loopGroup) {
		int jumpIndex = loopGroup.getScope().getExitIndex();
		loopGroup.resolveBreaks(jumpIndex);
	}
	
	private boolean traceIfThenElseModel(IntermediateGroup modelContainer) {
//		log.detail(">> if-then-else");
		try {
			return traceIfThenElseModelImpl(modelContainer);
		} finally {
//			log.detail("<< if-then-else");
		}
	}
	
	
	private boolean traceIfThenElseModelImpl(IntermediateGroup modelContainer) {
		boolean changed = false;
		for(int modelIndex=0; modelIndex<modelContainer.count(); modelIndex++) {
			IIntermediateNode nodeCond = modelContainer.get(modelIndex);
			IntermediateNodeScope scope = nodeCond.getScope();
			int condJumpIndex = scope.getJumpIndex();
			int condExitIndex = scope.getExitIndex();
			log.debug("modelIndex="+modelIndex+", condJumpIndex="+condJumpIndex+", condExitIndex="+condExitIndex);
			if (condJumpIndex<0 || condExitIndex<0) {
				continue;
			}

			
			if (condExitIndex>=condJumpIndex) {
				// this is a conditional jump backwards ... thus it should be resolved as a loop
				continue;
			}
			
			
			int nextThenIndex = condExitIndex;
			IntermediateGroup thenGroup = new IntermediateGroup();
			int j=0;
			while(j++<100) {
				IIntermediateNode findNode = modelContainer.findNode(nextThenIndex);
				if (findNode==null) {
					log.warn("WARNING: no node fount at next-then-index:"+nextThenIndex);
					thenGroup = null;
					break;
				}
				
				thenGroup.add(findNode);
				IntermediateNodeScope findScope = findNode.getScope();
				
				nextThenIndex = findScope.getOutIndex();
				if (nextThenIndex==IntermediateNodeScope.INDEX_NONE) {
					thenGroup = null;
					break;
				}
				if ((nextThenIndex>=0 && nextThenIndex>=condJumpIndex) || nextThenIndex==IntermediateNodeScope.INDEX_EXIT){
					break;
				}
			}
			if (j>=100) {
				(new RuntimeException("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAARGH")).printStackTrace();
				System.exit(-1);
			}
			
			if (thenGroup==null || thenGroup.count()==0) {
				continue;
			}
			
			if (nextThenIndex==condJumpIndex) {
				/* this is a if-then-group (so there is no else-group) */
				IntermediateIfThenElse ifThenElse = new IntermediateIfThenElse(nodeCond, thenGroup);
				modelContainer.remove(nodeCond);
				for(IIntermediateNode subNode : thenGroup) {
					modelContainer.remove(subNode);
				}
				modelContainer.add(modelIndex, ifThenElse);
				return true;
			}
			

			int nextElseIndex = condJumpIndex;
			IntermediateGroup elseGroup = new IntermediateGroup();
			while(true) {
				IIntermediateNode findNode = modelContainer.findNode(nextElseIndex);
				if (findNode==null) {
					log.warn("WARNING: no node fount at next-else-index:"+nextElseIndex);
					elseGroup = null;
					break;
				}
				elseGroup.add(findNode);
				IntermediateNodeScope findScope = findNode.getScope();
				
				nextElseIndex = findScope.getOutIndex();
				if (nextElseIndex==IntermediateNodeScope.INDEX_NONE) {
					elseGroup = null;
					break;
				}
				if ((nextElseIndex>=0 && nextElseIndex>=nextThenIndex && nextThenIndex>=0) || nextElseIndex==IntermediateNodeScope.INDEX_EXIT){
					break;
				}
			}

			if (nextThenIndex==IntermediateNodeScope.INDEX_EXIT && nextElseIndex==IntermediateNodeScope.INDEX_EXIT) {
				/* this is a if-then-group (so there is no else-group) */
				
				IntermediateGroup thenGroupCopy = (IntermediateGroup) thenGroup.makeCopy(false);
				IntermediateGroup elseGroupCopy = (IntermediateGroup) elseGroup.makeCopy(false);
				
				IntermediateGroup combinedGroup = stripCombined(thenGroupCopy, elseGroupCopy);
				
				
				IntermediateRunContext thenContext = new IntermediateRunContext(mnemonicBlock, localVariableTable);
				thenGroupCopy.connectStageOne(thenContext);
				
				IntermediateRunContext elseContext = new IntermediateRunContext(mnemonicBlock, localVariableTable);
				elseGroupCopy.connectStageOne(elseContext);

				IIntermediateNode newNode = null;
				if (thenContext.producerStack.size()==1 && elseContext.producerStack.size()==1) {
					IValueProducer valueProducer = thenContext.producerStack.get(0);
					IExpression thenExpr = valueProducer.getExpression(DeclarationType.DEFAULT_BOOLEAN);

					valueProducer = elseContext.producerStack.get(0);
					IExpression elseExpr = valueProducer.getExpression(DeclarationType.DEFAULT_BOOLEAN);

					log.debug("thenExpr="+thenExpr+", elseExpr="+elseExpr);
					
					if ((thenExpr instanceof Literal) && (elseExpr instanceof Literal)) {
						boolean isPusher = true;
						Literal thenLit = (Literal) thenExpr;
						Literal elseLit = (Literal) elseExpr;

						log.debug("thenLit="+thenLit);
						log.debug("elseLit="+elseLit);
						
						boolean thenIsTrue = thenLit.equals(Literal.LITERAL_TRUE);
						boolean thenIsFalse = thenLit.equals(Literal.LITERAL_FALSE);

						if (!thenIsTrue && !thenIsFalse) {
							isPusher = false;
						}

						boolean elseIsTrue = elseLit.equals(Literal.LITERAL_TRUE);
						boolean elseIsFalse = elseLit.equals(Literal.LITERAL_FALSE);
						if (!elseIsTrue && !elseIsFalse) {
							isPusher = false;
						}

						if (isPusher) {
							newNode = new IntermediateConditionPusher(nodeCond, thenGroupCopy, elseGroupCopy, elseIsTrue);
						}

					}
				}
				
				if (newNode==null) {
					newNode = new IntermediateIfThenElse(nodeCond, thenGroupCopy, elseGroupCopy);
				}
				for(IIntermediateNode subNode : thenGroup) {
					modelContainer.remove(subNode);
				}
				for(IIntermediateNode subNode : elseGroup) {
					modelContainer.remove(subNode);
				}
				int findIndex = modelContainer.findIndex(nodeCond);
				modelContainer.remove(findIndex);
				if (combinedGroup!=null && combinedGroup.count()>0) {
					modelContainer.add(findIndex, combinedGroup);
				}
				modelContainer.add(findIndex, newNode);
				return true;
			} else if (nextElseIndex==nextThenIndex) {
				IntermediateIfThenElse ifThenElse = new IntermediateIfThenElse(nodeCond, thenGroup, elseGroup);
				for(IIntermediateNode subNode : thenGroup) {
					modelContainer.remove(subNode);
				}
				for(IIntermediateNode subNode : elseGroup) {
					modelContainer.remove(subNode);
				}
				int findIndex = modelContainer.findIndex(nodeCond);
				modelContainer.remove(findIndex);
				modelContainer.add(findIndex, ifThenElse);
				return true;
			} else {
				if (elseGroup==null || elseGroup.count()==0) {
					/* this is a if-then-group (so there is no else-group) */
					IntermediateIfThenElse ifThenElse = new IntermediateIfThenElse(nodeCond, thenGroup);
					for(IIntermediateNode subNode : thenGroup) {
						modelContainer.remove(subNode);
					}
					int findIndex = modelContainer.findIndex(nodeCond);
					modelContainer.remove(findIndex);
					modelContainer.add(findIndex, ifThenElse);
					return true;
				}

				IntermediateIfThenElse ifThenElse = new IntermediateIfThenElse(nodeCond, thenGroup, elseGroup);
				for(IIntermediateNode subNode : thenGroup) {
					modelContainer.remove(subNode);
				}
				for(IIntermediateNode subNode : elseGroup) {
					modelContainer.remove(subNode);
				}
				int findIndex = modelContainer.findIndex(nodeCond);
				modelContainer.remove(findIndex);
				modelContainer.add(findIndex, ifThenElse);
				return true;
			}
		}	
		return changed;
	}

//	private boolean traceBooleanCondition(Group modelContainer) {
//		boolean changed = false;
//		for(int modelIndex=0; modelIndex<modelContainer.count(); modelIndex++) {
//			IGroupNode group = modelContainer.get(modelIndex);
//			int subJumpIndex = group.getScope().getJumpIndex();
//			if (subJumpIndex==-1 || group.getScope().getExitIndex()==-1) {
//				continue;
//			}
//
//			int elseModelIndex = findModelByIndex(modelContainer, subJumpIndex);
//			if (elseModelIndex==-1) {
//				continue;
//			}
//			
//			int thenModelIndex = findModelByIndex(modelContainer, group.getScope().getExitIndex());
//			if (thenModelIndex==-1) {
//				continue;
//			}
//			
//			IGroupNode elseModel = modelContainer.get(elseModelIndex);
//			IGroupNode thenModel = modelContainer.get(thenModelIndex);
//		
//			int thenOutIndex = thenModel.getScope().getOutIndex();
//			int thenOutModelIndex = findModelByIndex(modelContainer, thenOutIndex);
//			int elseOutIndex = elseModel.getScope().getOutIndex();
//			int elseOutModelIndex = findModelByIndex(modelContainer, elseOutIndex);
//			if ((thenOutModelIndex==elseOutModelIndex)) {
//
//				IGroupNode thenModel2 = thenModel;
//				IGroupNode elseModel2 = elseModel;
//				Group stripCombined = null;
////				if (!thenModel.getScope().isBranchedTo() && !elseModel.getScope().isBranchedTo()) {
////					thenModel2 = thenModel.makeCopy();
////					elseModel2 = elseModel.makeCopy();
////					stripCombined = stripCombined(thenModel2, elseModel2);
////				} else {
////					continue;
////				}
//
//				
//				
//				
//				GroupConnectContext groupConnectRuntime = new GroupConnectContext(modelContainer);
//				thenModel2.connectStageOne(groupConnectRuntime);
//				if (groupConnectRuntime.producerStack.size()!=1) {
//					continue;
//				}
//				IValueProducer valueProducer = groupConnectRuntime.producerStack.get(0);
//				IExpression thenExpr = valueProducer.getExpression(DeclarationDescriptionPrimitive.DEFAULT_BOOLEAN);
//
//				groupConnectRuntime = new GroupConnectContext(modelContainer);
//				elseModel2.connectStageOne(groupConnectRuntime);
//				if (groupConnectRuntime.producerStack.size()!=1) {
//					continue;
//				}
//				valueProducer = groupConnectRuntime.producerStack.get(0);
//				IExpression elseExpr = valueProducer.getExpression(DeclarationDescriptionPrimitive.DEFAULT_BOOLEAN);
//				
//				boolean elseIsTrue = false;
//				if ((thenExpr instanceof Literal) && (elseExpr instanceof Literal)) {
//					Literal thenLit = (Literal) thenExpr;
//					Literal elseLit = (Literal) elseExpr;
//					
//					boolean thenIsTrue = thenLit.equals(Literal.LITERAL_TRUE);
//					boolean thenIsFalse = thenLit.equals(Literal.LITERAL_FALSE);
//
//					if (!thenIsTrue && !thenIsFalse) {
//						continue;
//					}
//
//					elseIsTrue = elseLit.equals(Literal.LITERAL_TRUE);
//					boolean elseIsFalse = elseLit.equals(Literal.LITERAL_FALSE);
//					if (!elseIsTrue && !elseIsFalse) {
//						continue;
//					}
//				}
//				
//				GroupConditionPusher ifThenElse = null;
//				
//				ifThenElse = new GroupConditionPusher(group, thenModel2, elseModel2, elseIsTrue);
//				modelContainer.remove(thenModel);
//				modelContainer.remove(elseModel);
//
//				int findIndex = modelContainer.findIndex(group);
//				modelContainer.remove(findIndex);
//				boolean didAdd = false;
//				boolean didAddCombined = false;
//				if (modelContainer.count()>findIndex) {
//					IGroupNode appendNode = modelContainer.get(findIndex);
//					if (stripCombined!=null && stripCombined.count()>0) {
//						// TODO stripcombined should be prepended at correct position if it contains a single exitIndex and no jumpIndex
//						didAddCombined = appendNode.tryPrepend(stripCombined);
//					}
//					didAdd = didAddCombined && appendNode.tryPrepend(ifThenElse);
//				}
//				if (!didAddCombined && stripCombined!=null) {
//					modelContainer.add(modelIndex, stripCombined);
//				}
//				if (!didAdd) {
//					log.warn("ConditionalExpression group could not be added to the following group");
//					modelContainer.add(modelIndex, ifThenElse);
//				}
//
//
//				return true;
//			}
//		}	
//		return changed;
//	}

	private IntermediateGroup stripCombined(IntermediateGroup groupTwo, IntermediateGroup groupOne) {
		if (groupOne.count()!=groupTwo.count()) {
			return null;
		}
		CombineItem combineItem = new CombineItem();
		combineItem.groupOne = groupOne;
		combineItem.groupTwo = groupTwo;
		
		List<CombineItem> stack = new ArrayList<CombineItem>();
		
		while(true) {
			if (combineItem.lastIndex==-1 || combineItem.doBreak) {
				if (stack.isEmpty()) {
					return combineItem.output;
				}
				CombineItem lastCombineItem = stack.remove(stack.size()-1);
				lastCombineItem.doBreak = combineItem.doBreak;
				lastCombineItem.prepend(combineItem.output);
				combineItem = lastCombineItem;
			} else {
				IIntermediateNode nodeOne = combineItem.groupOne.get(combineItem.lastIndex);
				IIntermediateNode nodeTwo = combineItem.groupTwo.get(combineItem.lastIndex);
				if (nodeOne instanceof IntermediateGroup && nodeTwo instanceof IntermediateGroup) {
					CombineItem newCombineItem = new CombineItem();
					newCombineItem.groupOne = (IntermediateGroup) nodeOne;
					newCombineItem.groupTwo = (IntermediateGroup) nodeTwo;
					if (newCombineItem.groupOne.count()==newCombineItem.groupTwo.count()) {
						stack.add(combineItem);
						newCombineItem.lastIndex = newCombineItem.groupOne.count();
						combineItem = newCombineItem;
					} else {
						combineItem.doBreak = true;
					}
				} else {
					if (nodeOne.isSame(nodeTwo)) {
						combineItem.groupOne.remove(combineItem.lastIndex);
						combineItem.groupTwo.remove(combineItem.lastIndex);
						combineItem.prependCombined(nodeOne, nodeTwo);
					} else {
						combineItem.doBreak = true;
						combineItem.lastIndex = 0;
					}
				}
			}
			combineItem.lastIndex--;
		}
	}
	
	static class CombineItem {
		private IntermediateGroup groupOne, groupTwo;
		int lastIndex;
		IntermediateGroup output;
		boolean doBreak = false;
		
		public void prependCombined(IIntermediateNode nodeOne, IIntermediateNode nodeTwo) {
			if (nodeOne==null || nodeTwo==null) {
				return;
			}
			if (output==null) {
				output = new IntermediateGroup();
			}
			output.add(0, nodeOne);
		}

		public void prepend(IntermediateGroup group) {
			if (group==null) {
				return;
			}
			if (output==null) {
				output = new IntermediateGroup();
			}
			output.add(0, group);
		}
	}
	

	private boolean traceAndConditionModel(IntermediateGroup modelContainer) {
		
		boolean changed = false;
		
		int startIndex = -1;
		int jumpIndex = -1;
		boolean keep = true;
		for(int modelIndex=0; modelIndex<modelContainer.count(); modelIndex++) {
			IIntermediateNode group = modelContainer.get(modelIndex);
			log.trace("running group:"+group);
			keep = false;
			int childJumpIndex = group.getScope().getJumpIndex();
			int childExitIndex = group.getScope().getExitIndex();
			log.trace("modelIndex="+modelIndex+", jump-index="+childJumpIndex+", exit-index="+childExitIndex);
			if (childJumpIndex!=-1 && childExitIndex!=-1) {
				if (startIndex == -1) {
					startIndex = modelIndex;
					jumpIndex = childJumpIndex;
					keep = true;
				} else {
					if (jumpIndex == childJumpIndex || jumpIndex == childExitIndex) { 
						keep = true;
					}
				}
			}

			log.trace("modelIndex="+modelIndex+", keep="+keep+", startIndex="+startIndex+", jumpIndex="+jumpIndex);
			
			if (!keep && startIndex!=-1) {
				if (modelIndex<=startIndex+1) {
					modelIndex = startIndex;
					startIndex = -1;
					continue;
				}
				int elseBlockIndex = findModelByIndex(modelContainer, jumpIndex);
				if (elseBlockIndex==-1) {
					modelIndex = startIndex; 
					startIndex = -1;
					continue;
				}

				int count = (modelIndex)-startIndex;
				IIntermediateNode condGrpNodes[] = new IIntermediateNode[count];
				for(int i=count-1; i>=0; i--) {
					condGrpNodes[i] = modelContainer.remove(i+startIndex);
				}
				
				modelContainer.add(startIndex, new IntermediateConditionSet(jumpIndex, condGrpNodes));
				modelIndex = startIndex;
				startIndex=-1;
				changed=true;
				return true;
			}
		
		}
		return changed;
	}

	
	int loopTime;
	
	
	public boolean traceForLoop(IntermediateGroup modelContainer) {
		boolean changed = false;
		for(int modelIndex=1; modelIndex<modelContainer.count(); modelIndex++) {
			IIntermediateNode group = modelContainer.get(modelIndex);
			if (group instanceof IntermediateLoopGroup) {
				IntermediateLoopGroup loopGroup = (IntermediateLoopGroup) group;
				int initFrameIndex = -1;
				IntermediateGroup newInitGroup = null;
				
				/* search through  init nodes */
				IIntermediateNode initNode = modelContainer.get(modelIndex-1);
				if (initNode instanceof IntermediateGroup) {
					IntermediateGroup initGroup = (IntermediateGroup) initNode;
					int count = initGroup.count();
					int offset = count-1;
					while(offset>=0) {
						initNode = initGroup.get(offset);
						if (initNode instanceof IntermediateGoto) {
							offset--;
							continue;
						} else if (initNode instanceof IntermediateFramePut) {
							IntermediateFramePut fp = (IntermediateFramePut) initNode;
							initFrameIndex = fp.frameIndex;
						}
						break;
					}
					
					if (initFrameIndex>=0) {
						IntermediateRunContext irc = new IntermediateRunContext(mnemonicBlock, localVariableTable);
						for(int idx=0; idx<offset; idx++) {
							initNode = initGroup.get(idx);
							System.out.println("initNode="+initNode);
							initNode.connectStageOne(irc);
						}
						IValueProducer topProducer = irc.peekProducer(0);
						List<IIntermediateNode> nodes = collectIntermediateNodesForProducer(topProducer);
						nodes.add(initGroup.get(offset));
						newInitGroup = new IntermediateGroup(nodes);
					}
					
				}
				
				
				
				
				int incFrameIndex = -1;
				/* search through condition nodes*/
				IIntermediateNode last = loopGroup.get(loopGroup.count()-2);
				IIntermediateNode newIncrementNode = null;
				if (last instanceof IntermediateGroup) {
					IntermediateGroup incrementGroup = (IntermediateGroup) last;
					int count = incrementGroup.count();
					int offset = count-1;
					while(offset>=0) {
						initNode = incrementGroup.get(offset);
						if (initNode instanceof IntermediateGoto) {
							offset--;
							continue;
						} else if (initNode instanceof IntermediateIncrement) {
							IntermediateIncrement fp = (IntermediateIncrement) initNode;
							incFrameIndex = fp.frameIndex;
							newIncrementNode = fp;
						}
						break;
					}
					
				}
				
				if (incFrameIndex==initFrameIndex && incFrameIndex>=0) {
				
					IIntermediateNode newConditionNode = loopGroup.getLast();
					
					
					
					IIntermediateNode newLoopNode = null;
					if (loopGroup.count()==2) {
						newLoopNode = loopGroup.get(0);
					} else {
						IntermediateGroup group2 = new IntermediateGroup();
						for(int idx=0; idx<loopGroup.count()-1; idx++) {
							group2.add(loopGroup.get(idx));
						}
						newLoopNode = group2;
					}
					
					last = loopGroup.get(loopGroup.count()-2);
					if (last instanceof IntermediateGroup) {
						IntermediateGroup incrementGroup = (IntermediateGroup) last;
						incrementGroup.remove(newIncrementNode);
					}

					
					
					IntermediateForLoop intermediateForLoop = new IntermediateForLoop(newInitGroup, newConditionNode, newIncrementNode, newLoopNode);
					StringBuilder dumpBuffer = new StringBuilder();
					intermediateForLoop.dump(dumpBuffer, ">>");
					System.out.println(dumpBuffer.toString());
//					System.exit(-1);
					
					
					initNode = modelContainer.get(modelIndex-1);
					if (initNode instanceof IntermediateGroup) {
						IntermediateGroup initGroup = (IntermediateGroup) initNode;
						for(int idx=0; idx<newInitGroup.count(); idx++) {
							initGroup.remove(newInitGroup.get(idx));
						}
					}

					modelContainer.remove(modelIndex);
					modelContainer.add(modelIndex, intermediateForLoop);
					
					
//					new IntermediateForLoop(initNode, conditionNode, incrementNode, loopNode);
				}
				
//				
//				IntermediateLoopGroup loopGroup = (IntermediateLoopGroup) group;
//				IntermediateRunContext runContext = new IntermediateRunContext(modelContainer, mnemonicBlock, localVariableTable);
//				
//				
//				IntermediateLoopGroup loopGroupTest = (IntermediateLoopGroup) loopGroup.makeCopy(true);
//				
////				loopGroupTest.dump("loopGroupTest-pre:");
////				
////				loopGroupTest.removeTail();
////				loopGroupTest.dump("loopGroupTest-post:");
//				
//
//				loopGroupTest.consumeCondition = false;
//				
//				loopGroupTest.connectStageOne(runContext);
//				if (runContext.producerStack.size()>0) {
//					IValueProducer topProducer = runContext.producerStack.get(runContext.producerStack.size()-1);
//					
//					System.out.println("top-producer:"+topProducer.getIntermediateNode());
//					List<IIntermediateNode> nodes = collectIntermediateNodesForProducer(topProducer);
//					IntermediateGroup intermediateGroup = new IntermediateGroup(nodes);
//					intermediateGroup.dump("top::");
//					
//					IIntermediateNode last = loopGroupTest.getLast();
//					if (last instanceof IntermediateGroup) {
//						IntermediateGroup lastgrp = (IntermediateGroup) last;
//						
//					}
//					
//					
//					
//				}
			}
		}
		return changed;
	}
	
	
	private List<IIntermediateNode> collectIntermediateNodesForProducer(IValueProducer topProducer) {
		System.out.println("topProducer="+topProducer);
		ArrayList<IIntermediateNode> result = new ArrayList<IIntermediateNode>();
		ArrayList<IConsumer> stack = new ArrayList<IConsumer>();
		if (topProducer!=null) {
			IIntermediateNode intermediateNode = topProducer.getIntermediateNode();
			if (intermediateNode!=null && !result.contains(intermediateNode)) {
				result.add(intermediateNode);
			}
		}
		stack.addAll(topProducer.getInputConsumers());
		while(!stack.isEmpty()) {
			IConsumer consumer = stack.remove(0);
			IIntermediateNode intermediateNode = consumer.getIntermediateNode();
			System.out.println("consumer="+consumer+", stack.si="+stack.size()+", intermediateNode="+intermediateNode);
			if (intermediateNode!=null  && !result.contains(intermediateNode)) {
				result.add(intermediateNode);
			}
			
			IValueProducer producer = consumer.getProducer();
			if (producer!=null) {
				intermediateNode = producer.getIntermediateNode();
				System.out.println("consumer.producer="+producer+" ,intermediateNode="+intermediateNode);
				if (intermediateNode!=null && !result.contains(intermediateNode)) {
					result.add(intermediateNode);
				}
				List<IConsumer> consumers = producer.getInputConsumers();
				if (consumers!=null) {
					for(int idx=0; idx<consumers.size(); idx++) {
						IConsumer iConsumer = consumers.get(idx);
						System.out.println("consumer["+idx+"] = "+iConsumer);
					}
					stack.addAll(producer.getInputConsumers());
				}
			}
		}
		Collections.reverse(result);
		return result;
	}


	public boolean traceLoop(IntermediateGroup modelContainer) {
		boolean changed = false;
		
		if (loopTime++>300) {
			(new RuntimeException("sdfguhruoigheuiqwrhgtiuhgfuisdhfuighdsfuighisdugd")).printStackTrace();
			modelContainer.dump("EEEEE");
			System.exit(-1);
		}
		for(int modelIndex=0; modelIndex<modelContainer.count(); modelIndex++) {
			IIntermediateNode group = modelContainer.get(modelIndex);
			IntermediateNodeScope childScope = group.getScope();
			int jumpIndex = childScope.getJumpIndex();
			log.trace("modelIndex="+modelIndex+", jumpIndex="+jumpIndex+", child="+group);
			if (jumpIndex==-1) {
				continue;
			}
			int last = childScope.getExitIndex();
			log.trace("jumpIndex="+jumpIndex+", last="+last);
			if (jumpIndex!=-1 && last!=-1 && jumpIndex<last) {
				/* the jump is backwards */
				
				int jumpModelIndex = findModelByIndex(modelContainer, jumpIndex);
				if (modelIndex==modelContainer.count()-1 && jumpModelIndex==0) {
					/* TODO: why is this ?? */
					continue;
				}
				
				log.trace("modelIndex="+modelIndex+", jumpModelIndex="+jumpModelIndex);
				if (jumpModelIndex==modelIndex) {
					IIntermediateNode conditionModel = modelContainer.remove(modelIndex);
					
					IntermediateLoopGroup loopContainer = new IntermediateLoopGroup(conditionModel);
					resolveBreaks(loopContainer);
					convertGroup(loopContainer);
					modelContainer.add(jumpModelIndex, loopContainer);
					changed = true;
				} else if (jumpModelIndex>0) {

					IntermediateLoopGroup loopContainer = new IntermediateLoopGroup();
					
					IIntermediateNode conditionModel = modelContainer.remove(modelIndex);
					for(int subCodeIdx=modelIndex-1; subCodeIdx>=jumpModelIndex; subCodeIdx--) {
						loopContainer.add(0, modelContainer.remove(subCodeIdx));
					}
					resolveBreaks(loopContainer);
					convertGroup(loopContainer);
					loopContainer.add(loopContainer.count(), conditionModel);
					modelContainer.add(jumpModelIndex, loopContainer);
					changed = true;
				}
			}
		}
		return changed;
	}

	public int findModelByIndex(IntermediateGroup modelContainer, int mnemonicIndex) {
		int index = 0;
		for(IIntermediateNode model : modelContainer) {
			if (model.contains(mnemonicIndex)) {
				return index;
			}
			index++;
		}
		return -1;
	}
	

	public Set<IIntermediateNode> collectForward(IntermediateGroup groupContainer, IIntermediateNode startGroup) {
		Stack<IIntermediateNode> stack = new Stack<IIntermediateNode>();
		stack.add(startGroup);
		Set<IIntermediateNode> result = new HashSet<IIntermediateNode>();
		result.add(startGroup);
		while(!stack.isEmpty()) {
			IIntermediateNode group = stack.pop();
			log.debug("group="+group);
			int exitIndex = group.getScope().getExitIndex();
			if (exitIndex>=0) {
				IIntermediateNode node = groupContainer.findNode(exitIndex);
				log.debug("node="+node+", exitIndex="+exitIndex);
				if (node!=null) {
					if (result.add(node)) {
						stack.push(node);
					}
				}
			}
			int jumpIndex = group.getScope().getJumpIndex();
			if (jumpIndex>=0) {
				IIntermediateNode node = groupContainer.findNode(jumpIndex);
				log.debug("node="+node+", jumpIndex="+jumpIndex);
				if (node!=null) {
					if (result.add(node)) {
						stack.push(node);
					}
				}
			}
		}
		return result;
	}
	
}
