package net.natpad.gross.export;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.IOException;
import java.io.PrintStream;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;

import net.natpad.gross.ast.bnf.AstFile;
import net.natpad.gross.ast.bnf.AstLine;
import net.natpad.gross.ast.bnf.AstTypeDescription;
import net.natpad.gross.ast.bnf.BodyArgumentStatement;
import net.natpad.gross.ast.bnf.BodyImplementStatement;
import net.natpad.gross.ast.bnf.BodyMethodStatement;
import net.natpad.gross.ast.bnf.BodyStatement;
import net.natpad.gross.ast.bnf.ForwardTypeDescription;
import net.natpad.gross.ast.bnf.RuleRef;
import net.natpad.gross.ast.bnf.TypeDef;
import net.natpad.gross.ast.bnf.TypeDescription;
import net.natpad.gross.build.CCModel;
import net.natpad.gross.build.CCStateMachine;
import net.natpad.gross.build.NonTerminalExt;
import net.natpad.gross.build.Production;
import net.natpad.gross.build.ProductionPart;
import net.natpad.gross.build.SymbolWithType;
import net.natpad.gross.export.glibc.Argument;
import net.natpad.gross.export.glibc.ClassName;
import net.natpad.gross.export.glibc.InterfaceImplement;
import net.natpad.gross.export.glibc.Method;
import net.natpad.gross.export.glibc.Type;
import net.natpad.gross.runtime.Symbol;

public class AstCreator {

	public final File base;
	public final AstFile astFile;
	public final CCStateMachine stateMachine;
	public final String prefix;
	

	public final HashMap<ClassName, ClassName> typeRepository = new HashMap<ClassName, ClassName>();
	
	public AstCreator(File base, AstFile astFile, CCStateMachine stateMachine, String prefix) {
		this.base = new File(base, "ast");
		this.astFile = astFile;
		this.stateMachine = stateMachine;
		this.prefix = prefix;
		getOrCreateClassName("Cat:StringWo#<caterpillar.h>");
		getOrCreateClassName("GroRun:Fork#<grossruntime.h>");
		getOrCreateClassName("GroRun:ReduceLayout#<grossruntime.h>");
		getOrCreateClassName("G:Object#<glib-object.h>");

	}

	public ClassName getOrCreateClassName(String description) {
		return getOrCreateClassName(new ClassName(description));
	}

	public ClassName getOrCreateClassName(ClassName className) {
		ClassName repClassName = typeRepository.get(className);
		if (repClassName==null) {
			repClassName = className;
			typeRepository.put(className, className);
		}
		System.out.println("retrieved : "+repClassName+" "+repClassName.createFilename());
		return repClassName;
	}

	
	public void createAll() throws IOException {
		base.mkdirs();
		
		if (astFile.typeDefs!=null) {
			List<TypeDef> predefinedTypes = astFile.typeDefs.enlist();
			if (predefinedTypes!=null) {
				for (TypeDef typeDef : predefinedTypes) {
					getOrCreateClassName(typeDef.description);
				}
			}
		}
		
		List<AstLine> astLines = astFile.astLines.enlist();
		HashMap<RuleRef, Type> lhsTypeMap = new HashMap<RuleRef, Type>();
		for(AstLine line : astLines) {
			Type type = null;
			ClassName cname = getOrCreateClassName(line.typeDescription.typeDef.description);
			if (line.typeDescription instanceof AstTypeDescription) {
				type = new AstType(cname);
				AstTypeDescription astTDescr = (AstTypeDescription) line.typeDescription;
				if (astTDescr.optAsName!=null) {
					ClassName asClassName = getOrCreateClassName(astTDescr.optAsName);
					((AstType) type).setAsName(asClassName);
				}
				if (astTDescr.body!=null) {
					List<BodyStatement> enlist = astTDescr.body.enlist();
					for(BodyStatement stat : enlist) {
						if (stat instanceof BodyMethodStatement) {
							BodyMethodStatement bms = (BodyMethodStatement) stat;
							ClassName rt = null;
							if (bms.returnType != null) {
								rt = getOrCreateClassName(bms.returnType);
							}
							Method m = new Method(bms.methodName, rt);
							m.content = bms.codeText;
							m.isPrivate = false;
							type.addMethod(m);
						} else if (stat instanceof BodyImplementStatement) {
							BodyImplementStatement bis = (BodyImplementStatement) stat;
							
							ClassName ifaceClassName = getOrCreateClassName(bis.ifaceTypeName);
							InterfaceImplement ifaceImplement = new InterfaceImplement(ifaceClassName);
							ifaceImplement.initText = bis.ifaceInitText;
							type.addInterface(ifaceImplement);
						}
					}
				}
				
			} else if (line.typeDescription instanceof ForwardTypeDescription) {
				type = new ForwardType(cname);
			} else if (line.typeDescription instanceof TypeDescription) {
				type = new Type(cname, cname.createFilename());
			}

			List<RuleRef> enlist = line.ruleRefList.enlist();
			for(RuleRef ruleRef : enlist) {
				System.out.println("lhs-map:"+ruleRef+", type="+type.className);
				lhsTypeMap.put(ruleRef, type);
			}
		}


		HashMap<Production, MethodInvocation> prodMethodMap = new HashMap<Production, MethodInvocation>();
		ClassName astActionsClassName = getOrCreateClassName(new ClassName(prefix, "AstActions", "ast_actions"));
		Type astActionType = new Type(astActionsClassName, astActionsClassName.createFilename());
		astActionType.addImport(getOrCreateClassName("GroRun:Fork"));

		String methodName = astActionsClassName.fullLow()+"_new";
		Method constructorMethod = new Method(methodName, astActionsClassName);
		astActionType.addMethod(constructorMethod);
		constructorMethod.isConstructor = true;
		
		
		for(AstLine line : astLines) {

			List<RuleRef> enlist = line.ruleRefList.enlist();
			for(RuleRef ruleRef : enlist) {
				Type ruleType = lhsTypeMap.get(ruleRef);
					
				CCModel model = stateMachine.model;
				NonTerminalExt lhs = model.getNonTerminalByName(ruleRef.lhsName);
				if (lhs==null) {
					if ((ruleType instanceof ForwardType) || (ruleType instanceof AstType)) {
						throw new RuntimeException("non terminal with name "+ruleRef.lhsName+" not found");
					}
					continue;
				}
				for(Production prod : lhs) {
					if (ruleRef.label==null) {
						RuleRef tstRuleRef = new RuleRef(ruleRef.lhsName, prod.label);
						Type tstType = lhsTypeMap.get(tstRuleRef);
						if (tstType!=null && tstType!=ruleType) {
							continue;
						}
					} else if (!ruleRef.label.equals(prod.label)) {
						continue;
					}
					
					System.err.println("adding to:rule-ref:"+ruleRef+", "+prod+" ruleType="+ruleType);
					
					if (ruleType instanceof ForwardType) {
						generateFwdrs(prod, prodMethodMap);
					} else if (ruleType instanceof AstType) {
						generateProduction(lhsTypeMap, line, (AstType) ruleType, prod, prodMethodMap, astActionType);
					}
				}
			}
			
		}
		
		
		HashSet<AstType> typesdone = new HashSet<AstType>();
		for(Type type : lhsTypeMap.values()) {
			if (type instanceof AstType) {
				AstType astType = (AstType) type;
				if (typesdone.add(astType)) {
					astType.generateAstGetters();
					astType.createCFile(base);
					astType.createHeaderFile(base);
					if (astType.addedAtLeasOneMethod) {
						astActionType.addImport(astType.className);
					}
				}
			}
		}
		
		
		ClassName actionHandlerClassName = getOrCreateClassName(new ClassName("GroRun","IActionHandler", "iaction_handler"));
		InterfaceImplement actionInterfaceImplement = new InterfaceImplement(actionHandlerClassName);
		
		String pr = "static GroRunIToken *l_extract(GroRunFork *fork, GroRunReduceLayout *reduce_layout, int offset) {\n"+
				"	GroRunIToken *result = NULL;\n"+
				"	int rloff = grorun_reduce_layout_calculate_stack_offset(reduce_layout, offset);\n"+
				"	GroRunStackEntry *fe = grorun_fork_get_entry(fork, rloff);\n"+
				"	if (fe!=NULL) {\n"+
				"		result = grorun_stack_entry_get_lookahead(fe);\n"+
				"	}\n"+
				"	return result;\n"+
				"}\n";
		
		actionInterfaceImplement.prefixContent = pr;
		
		astActionType.addInterface(actionInterfaceImplement);
		
		
		Method actionMethod = new Method("run_action", getOrCreateClassName("G:Object:object"));
		actionMethod.arguments.add(new Argument(getOrCreateClassName("GroRun:Fork"), "fork"));
		actionMethod.arguments.add(new Argument(getOrCreateClassName("GroRun:ReduceLayout:reduce_layout"), "reduce_layout"));
		actionMethod.arguments.add(new Argument(ClassName.INT, "action_id"));
		actionInterfaceImplement.addMethod("runAction", actionMethod);

		
		ByteArrayOutputStream baos = new ByteArrayOutputStream();
		PrintStream amOut = new PrintStream(baos);
		amOut.println("	GObject *result = NULL;");
		amOut.println("	int prod_id = grorun_reduce_layout_get_production_id(reduce_layout);");
		amOut.println("	switch(prod_id) {");

		for(Production prod : prodMethodMap.keySet()) {
			amOut.println();
			MethodInvocation methodInvocation = prodMethodMap.get(prod);
			amOut.println("		case "+prod.id.intValue()+" : { // "+prod);
			StringBuilder bufInvoke = new StringBuilder();
			String fwdLabel = null;
			int argCount = 0;
			for(int idx=0; idx<prod.rhsCount(); idx++) {
				ProductionPart rhs = prod.rhsAt(idx);
				if (rhs.label!=null) {
					amOut.println("			GroRunIToken *"+rhs.label+" = l_extract(fork, reduce_layout, "+idx+");");
					fwdLabel = rhs.label;
					if (bufInvoke.length()!=0) {
						bufInvoke.append(", ");
					}
					bufInvoke.append(rhs.label);
					argCount++;
				}
			}
			if (methodInvocation.method.methodName.equals("FORWARD")) {
				amOut.println("			result = grorun_itoken_get_value("+fwdLabel+");");
				amOut.println("			if (result) {g_object_ref(result); }");
			} else {
				List<Argument> methodArguments = methodInvocation.method.arguments;
				for(int argIdx=argCount; argIdx<methodArguments.size(); argIdx++) {
					Argument argument = methodArguments.get(argIdx);
					BodyArgumentStatement bodyArgumentStatement = methodInvocation.addArgs.get(argument.label);
					if (bufInvoke.length()!=0) {
						bufInvoke.append(", ");
					}
					bufInvoke.append(bodyArgumentStatement.value);
				}
				amOut.println("			result = (GObject *) "+methodInvocation.method.methodName+"("+bufInvoke.toString()+");");
			}
			amOut.println("		} break;");
		}

		amOut.println("	}");
		amOut.println("	return result;");
		amOut.flush();
		actionMethod.content = baos.toString();
		
		astActionType.createHeaderFile(base);
		astActionType.createCFile(base);		
	}

	
	private void generateFwdrs(Production prod, HashMap<Production, MethodInvocation> prodMethodMap) {
		
		if (prod.rhsCount()==0) {
			return;
		}
		boolean hasLabel = false;
		for(int rhsIdx=0; rhsIdx<prod.rhsCount(); rhsIdx++) {
			ProductionPart rhsAt = prod.rhsAt(rhsIdx);
			if (rhsAt.label!=null) {
				if (hasLabel) {
					throw new RuntimeException("production has two labels can not forward:"+prod);
				}
				hasLabel = true;
			}
		}
		
		System.out.println("prod:"+prod+" FORWARD");
		
		if (hasLabel) {
			Method m = new Method("FORWARD", null);
			MethodInvocation mi = new MethodInvocation();
			mi.method = m;
			prodMethodMap.put(prod, mi);
		}
		
	}

	private void generateProduction(HashMap<RuleRef, Type> lhsTypeMap, AstLine line, AstType astType, Production prod, HashMap<Production, MethodInvocation> prodMethodMap, Type astActionType) throws IOException {
		if (prod.rhsCount()==0) {
			return;
		}
		ClassName tokenClassName = getOrCreateClassName("GroRun:IToken:itoken");
		ArrayList<Argument> arguments = new ArrayList<Argument>();
		int tokenCount = 0;
		for(int rhsIdx=0; rhsIdx<prod.rhsCount(); rhsIdx++) {
			ProductionPart rhsAt = prod.rhsAt(rhsIdx);
			if (rhsAt.label!=null) {
				ClassName argType = extractTypeForName(rhsAt.symbol.name, lhsTypeMap);
				
				astType.addField(rhsAt.label, tokenClassName, true);
				astType.addImport(argType);
				Argument argument = new Argument(argType, rhsAt.label);
				arguments.add(argument);
				tokenCount++;
			}
		}
		
		
		MethodInvocation mi = new MethodInvocation();
		AstTypeDescription astTpDescr = (AstTypeDescription) line.typeDescription;
		if (astTpDescr.body!=null) {
			List<BodyStatement> statements = astTpDescr.body.enlist();
			for (BodyStatement bodyStatement : statements) {
				if (bodyStatement instanceof BodyArgumentStatement) {
					BodyArgumentStatement bas = (BodyArgumentStatement) bodyStatement;
					ClassName argType = getOrCreateClassName(bas.typeName);

					astType.addField(bas.label, argType, true);
					astType.addImport(argType);

					Argument addArg = new Argument(argType, bas.label);
					arguments.add(addArg);
					mi.addAdditionalArgument(bas);
				}
			}
		}

		boolean constructorPresent = false;
		for(ArrayList<Argument> existing : astType.methodMap.keySet()) {
			if (existing.equals(arguments)) {
				Method method = astType.methodMap.get(existing);
				mi.method = method;
				prodMethodMap.put(prod, mi);
				constructorPresent = true;
				break;
			}
		}
		if (!constructorPresent) {
			
			String methodName = astType.className.fullLow()+"_new_"+(astType.methodMap.size()+1);
			if (methodName.equals("groast_rhs_entry_complete_new_2")) {
				System.out.println("ast-line="+line);
				System.out.println("prod="+prod);
				System.out.println("astType.classname="+astType.className);
				throw new RuntimeException("jahoor");
			}
			Method method = new Method(methodName, astType.className);
			astType.methodMap.put(arguments, method);
			mi.method = method;
			prodMethodMap.put(prod, mi);
			method.isConstructor = true;
			astType.addMethod(method);
			
			int tidx = 0;
			for(Argument arg : arguments) {
				astType.tokenFields.put(arg.label, arg.type);
				if (tidx<tokenCount) {
					Argument param = new Argument(tokenClassName, arg.label);
					method.arguments.add(param);
				} else {
					Argument param = new Argument(arg.type, arg.label);
					method.arguments.add(param);
				}
				tidx++;
			}
			astType.addedAtLeasOneMethod = true;
		}
		
	}

	
	
	public static class MethodInvocation {
		public Method method;
		
		public HashMap<String, BodyArgumentStatement> addArgs = new HashMap<String, BodyArgumentStatement>();

		public void addAdditionalArgument(BodyArgumentStatement bas) {
			addArgs.put(bas.label, bas);
			
		}
	}
	
	private ClassName extractTypeForName(String name, HashMap<RuleRef, Type> lhsTypeMap) {
		ClassName result = null;
		Type astType = lhsTypeMap.get(new RuleRef(name, null));
		System.err.println("extract name="+name+", lhs-type="+astType);
		if (astType!=null) {
			if (astType instanceof AstType) {
				return ((AstType) astType).getAsName();
			}
			return astType.className;
		}
		if (result==null) {
			Symbol symbolByName = stateMachine.model.getSymbolByName(name);
			if (symbolByName instanceof SymbolWithType) {
				SymbolWithType swt = (SymbolWithType) symbolByName;
				String type = swt.getType();
				if (type!=null && type.length()>0) {
					result = getOrCreateClassName(new ClassName(type));
				}
			}
		}
		if (result == null) {
			result = getOrCreateClassName(new ClassName("G:Object"));
		}
		return result;
	}




	
	static class AstType extends Type {

		
		public ClassName asName;
		public boolean addedAtLeasOneMethod;
		public HashMap<ArrayList<Argument>, Method> methodMap = new HashMap<ArrayList<Argument>, Method>();
		public HashMap<String, ClassName> tokenFields = new HashMap<String, ClassName>(); 

		public AstType(ClassName className) {
			super(className, className.createFilename());
			asName = className;
			addImport(new ClassName("GroRun:Fork"));
		}
		
		
		public void setAsName(ClassName asName) {
			this.asName = asName;
		}
		
		public ClassName getAsName() {
			return asName;
		}
		
		
		public void generateAstGetters() {
			ClassName tokenClassName = new ClassName("GroRun:IToken:itoken");

			/* generating getters */
			for(String fieldLabel : tokenFields.keySet()) {
				Field field = fieldList.get(fieldLabel);
				ClassName fieldClassName = tokenFields.get(fieldLabel);
				if (field.type.equals(tokenClassName)) {
					Method methodTk = new Method(className.fullLow()+"_tk_"+fieldLabel, tokenClassName);
					StringBuilder buf = new StringBuilder();
					System.out.println("fieldLabel="+fieldLabel);
					if (field.isPrivate) {
						buf.append("	"+className.camelPrefix+className.camelPostfix+"Private *priv = "+className.fullLow()+"_get_instance_private(self);\n");
						buf.append("	return priv->"+fieldLabel+";");
					} else {
						buf.append("	return self->"+fieldLabel+";");
					}
					methodTk.content = buf.toString();
					addMethod(methodTk);
					Method methodGet = new Method(className.fullLow()+"_get_"+fieldLabel, fieldClassName);
					buf = new StringBuilder();
					if (field.isPrivate) {
						buf.append("	"+className.camelPrefix+className.camelPostfix+"Private *priv = "+className.fullLow()+"_get_instance_private(self);\n");
						buf.append("	return ("+fieldClassName.fullTypePtr()+") grorun_itoken_get_value(priv->"+fieldLabel+");");
					} else {
						buf.append("	return ("+fieldClassName.fullTypePtr()+") grorun_itoken_get_value(self->"+fieldLabel+");");
					}
					methodGet.content = buf.toString();
					addMethod(methodGet);
				} else {
					Method methodGet = new Method(className.fullLow()+"_get_"+fieldLabel, fieldClassName);
					StringBuilder buf = new StringBuilder();
					if (field.isPrivate) {
						buf.append("	"+className.camelPrefix+className.camelPostfix+"Private *priv = "+className.fullLow()+"_get_instance_private(self);\n");
						buf.append("	return ("+fieldClassName.fullTypePtr()+") priv->"+fieldLabel+";");
					} else {
						buf.append("	return ("+fieldClassName.fullTypePtr()+") self->"+fieldLabel+";");
					}
					methodGet.content = buf.toString();
					addMethod(methodGet);
					
				}
	
	
			}
		}
		
	}
	
	
	static class ForwardType extends Type {
		
		public ForwardType(ClassName className) {
			super(className, className.createFilename());
		}
		
		@Override
		public String toString() {
			return "ForwardType["+className+"]";
		}
	}
	
}
