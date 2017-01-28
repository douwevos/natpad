package net.avaj.classfile.source;

import java.io.IOException;

import net.natpad.avaj.ast.Block;
import net.natpad.avaj.ast.LogicalExpression;
import net.natpad.avaj.ast.Method;
import net.natpad.avaj.ast.ReturnStatement;
import net.natpad.avaj.ast.Type;
import net.natpad.avaj.ast.formater.Writer;
import net.natpad.avaj.classfile.bytecode.Operator;

import org.junit.Assert;
import org.junit.Test;

public class ExpressionsExampleUT {

	Writer out = new Writer();

	@Test
	public void testFields() {
		try {
			Helper helper = new Helper("ExpressionsExample.class");
			Type type = helper.type;
			type.write(out);
			System.out.println(""+out);

			
			Method methodIntBitwiseOr = helper.findMethodByName("testIntBitwiseOr");
			Block statementBlock = methodIntBitwiseOr.statementBlock;
			ReturnStatement returnStat = (ReturnStatement) statementBlock.get(0);
			LogicalExpression logExpr = (LogicalExpression) returnStat.expression;
			Assert.assertEquals(logExpr.operator, Operator.BITWISE_OR);
			
			Assert.assertEquals(logExpr.get(0), methodIntBitwiseOr.methodHeader.getArgs()[0].getVariable());
			Assert.assertEquals(logExpr.get(1), methodIntBitwiseOr.methodHeader.getArgs()[1].getVariable());
			
			
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	
}
