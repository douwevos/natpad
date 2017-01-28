
type "GroAst:IRhsPart#../../src/ast/groastirhspart.h"
type "GroAst:IRhsEnlistable#../../src/ast/groastirhsenlistable.h"
type "-GroAstUnaryOp#../groastunaryop.h"
type "CatInteger#<caterpillar.h>"
type "CatArrayWo#<caterpillar.h>"


spec ast type "GroAst:Spec";

term_decl_list ast type "GroAst:TerminalDeclarationList" {
  method groast_terminal_declaration_list_enlist return "CatArrayWo" ｟ 
	CatArrayWo *result = cat_array_wo_new();
	GroAstTerminalDeclarationList *tdl = self;
	while(tdl) {
		GroAstTerminalDeclarationListPrivate *priv = groast_terminal_declaration_list_get_instance_private(tdl);
		cat_array_wo_append(result, (GObject *) priv->td);
		if (priv->tdlist==NULL) {
			break;
		}
		tdl = (GroAstTerminalDeclarationList *) grorun_itoken_get_value(priv->tdlist);
	}
	cat_array_wo_reverse(result);
	return result;
  ｠
};

term_decl ast type "GroAst:TerminalDeclaration" {
  method groast_terminal_declaration_enlist return "CatArrayWo" ｟
	GroAstTerminalDeclarationPrivate *priv = groast_terminal_declaration_get_instance_private(self);
	if (priv->ids==NULL) {
		return NULL;
	}
	return groast_id_list_enlist((GroAstIdList *) grorun_itoken_get_value(priv->ids));
  ｠
}; 

ids ast type "GroAst:IdList" {
  method groast_id_list_enlist return "CatArrayWo" ｟ 
	CatArrayWo *result = cat_array_wo_new();
	GroAstIdList *tdl = self;
	while(tdl) {
		GroAstIdListPrivate *priv = groast_id_list_get_instance_private(tdl);
		cat_array_wo_append(result, (GObject *) priv->id);
		if (priv->ids==NULL) {
			break;
		}
		tdl = (GroAstIdList *) grorun_itoken_get_value(priv->ids);
	}
	cat_array_wo_reverse(result);
	return result;
  ｠
};

production_list ast type "GroAst:ProductionDefinitionList" {
  method groast_production_definition_list_enlist return "CatArrayWo" ｟ 
	CatArrayWo *result = cat_array_wo_new();
	GroAstProductionDefinitionList *tdl = self;
	while(tdl) {
		GroAstProductionDefinitionListPrivate *priv = groast_production_definition_list_get_instance_private(tdl);
		cat_array_wo_append(result, (GObject *) priv->prod);
		if (priv->plist==NULL) {
			break;
		}
		tdl = (GroAstProductionDefinitionList *) grorun_itoken_get_value(priv->plist);
	}
	cat_array_wo_reverse(result);
	return result;
  ｠
}; 

production ast type "GroAst:ProductionDefinition";

rhs_rule_list ast type "GroAst:RhsRuleList" {
  method groast_rhs_rule_list_enlist return "CatArrayWo" ｟ 
	CatArrayWo *result = cat_array_wo_new();
	GroAstRhsRuleList *tdl = self;
	while(tdl) {
		GroAstRhsRuleListPrivate *priv = groast_rhs_rule_list_get_instance_private(tdl);
		cat_array_wo_append(result, (GObject *) priv->rule);
		if (priv->ruleList==NULL) {
			break;
		}
		tdl = (GroAstRhsRuleList *) grorun_itoken_get_value(priv->ruleList);
	}
	cat_array_wo_reverse(result);
	return result;
  ｠
  
  implement "GroAst:IRhsEnlistable" ｟
  	iface->enlist = (GroAstEnlistFunc) groast_rhs_rule_list_enlist;
  ｠
};

rhs_rule ast type "GroAst:RhsRule"  {
  method groast_rhs_rule_enlist return "CatArrayWo" ｟ 
  
	GroAstRhsRulePrivate *priv = groast_rhs_rule_get_instance_private(self);
	if (priv->rulePartList==NULL) {
		return cat_array_wo_new();
	}
	return groast_irhs_enlistable_enlist((GroAstIRhsEnlistable *) grorun_itoken_get_value(priv->rulePartList));
  ｠
  
  implement "GroAst:IRhsEnlistable" ｟
  	iface->enlist = (GroAstEnlistFunc) groast_rhs_rule_enlist;
  ｠
};

rhs_rule_part_list ast type "GroAst:RhsRulePartList" {
  method groast_rhs_rule_part_list_enlist return "CatArrayWo" ｟ 
	CatArrayWo *result = cat_array_wo_new();
	GroAstRhsRulePartList *tdl = self;
	while(tdl) {
		GroAstRhsRulePartListPrivate *priv = groast_rhs_rule_part_list_get_instance_private(tdl);
		cat_array_wo_append(result, (GObject *) priv->rulePart);
		if (priv->rulePartList==NULL) {
			break;
		}
		tdl = (GroAstRhsRulePartList *) grorun_itoken_get_value(priv->rulePartList);
	}
	cat_array_wo_reverse(result);
	return result;
  ｠
  
  implement "GroAst:IRhsEnlistable" ｟
  	iface->enlist = (GroAstEnlistFunc) groast_rhs_rule_part_list_enlist;
  ｠
};

rhs_rule_part forward type "GroAst:IRhsPart";

name forward type "CatStringWo";

rhs_entry type "GroAst:IRhsPart";
rhs_sub_list ast type "GroAst:RhsSubList" as "GroAst:IRhsPart" {
  method groast_rhs_sub_list_enlist return "CatArrayWo" ｟ 
	CatArrayWo *result = cat_array_wo_new();
	GroAstRhsSubList *tdl = self;
	while(tdl) {
		GroAstRhsSubListPrivate *priv = groast_rhs_sub_list_get_instance_private(tdl);
		cat_array_wo_append(result, (GObject *) priv->sub);
		if (priv->subList==NULL) {
			break;
		}
		tdl = (GroAstRhsSubList *) grorun_itoken_get_value(priv->subList);
	}
	cat_array_wo_reverse(result);
	return result;
  ｠
  
  implement "GroAst:IRhsEnlistable" ｟
  	iface->enlist = (GroAstEnlistFunc) groast_rhs_sub_list_enlist;
  ｠
};

rhs_sub ast type "GroAst:RhsSub" as "GroAst:IRhsPart" {
  method groast_rhs_sub_enlist return "CatArrayWo" ｟ 
	CatArrayWo *result = cat_array_wo_new();
	GroAstRhsSub *tdl = self;
	while(tdl) {
		GroAstRhsSubPrivate *priv = groast_rhs_sub_get_instance_private(tdl);
		cat_array_wo_append(result, (GObject *) priv->entry);
		if (priv->sub==NULL) {
			break;
		}
		tdl = (GroAstRhsSub *) grorun_itoken_get_value(priv->sub);
	}
	cat_array_wo_reverse(result);
	return result;
  ｠
  
  implement "GroAst:IRhsEnlistable" ｟
  	iface->enlist = (GroAstEnlistFunc) groast_rhs_sub_enlist;
  ｠
};

rhs_entry_complete type "GroAst:IRhsPart";

rhs_entry_complete ≣ b forward type "GroAst:IRhsPart";
rhs_entry_complete ≣ s ast type "GroAst:RhsUnaryPart" as "GroAst:IRhsPart" {
	argument "-GroAstUnaryOp", "op", "GRO_AST_UN_STAR" 
};

rhs_entry_complete ≣ p 
	ast type "GroAst:RhsUnaryPart" as "GroAst:IRhsPart" {
		argument "-GroAstUnaryOp", "op", "GRO_AST_UN_PLUS" 
	};

rhs_entry ≣ o 
rhs_entry_complete ≣ e 
	ast type "GroAst:RhsUnaryPart" as "GroAst:IRhsPart" {
		argument "-GroAstUnaryOp", "op", "GRO_AST_UN_EXCL" 
	};


rhs_entry ≣ s forward type "GroAst:IRhsPart";
rhs_entry ≣ b ast type "GroAst:SymbolPart";

opt_label  forward type "CatStringWo";




type_definition_list type "GroAst:TypeDefinitionList";
type_definition_list  ≣ l ast type "GroAst:TypeDefinitionList" {
  method groast_type_definition_list_enlist return "CatArrayWo" ｟ 
	CatArrayWo *result = cat_array_wo_new();
	GroAstTypeDefinitionList *tdl = self;
	while(tdl) {
		GroAstTypeDefinitionListPrivate *priv = groast_type_definition_list_get_instance_private(tdl);
		cat_array_wo_append(result, (GObject *) priv->typeDef);
		if (priv->tdList==NULL) {
			break;
		}
		tdl = (GroAstTypeDefinitionList *) grorun_itoken_get_value(priv->tdList);
	}
	cat_array_wo_reverse(result);
	return result;
  ｠
};

type_definition ast type "GroAst:TypeDefinition";


code_definition_list type "GroAst:CodeDefinitionList";
code_definition_list  ≣ l ast type "GroAst:CodeDefinitionList";
code_definition ast type "GroAst:CodeDefinition";

prod_rule_ref ast type "GroAst:ProdRuleRef";

code_text forward type "CatStringWo";


imports_code forward type "CatStringWo";
