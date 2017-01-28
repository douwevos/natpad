package net.natpad.jaguar.monkey.request;

import static net.natpad.caterpillar.CatLogger.cat_log_debug;
import net.natpad.caterpillar.CatLogLevel;
import net.natpad.caterpillar.CatLogLevels;
import net.natpad.jaguar.monkey.JagMooseContext;
import net.natpad.jaguar.monkey.JagNodeCompiler;
import net.natpad.jaguar.monkey.JagSrcFolderContent;
import net.natpad.monkey.model.MooIContent;
import net.natpad.monkey.model.MooINode;
import net.natpad.monkey.model.MooModuleContent;
import net.natpad.monkey.model.MooNode;
import net.natpad.monkey.model.MooResourceContent;
import net.natpad.monkey.model.access.MooINodeMatcher;
import net.natpad.monkey.transaction.Transaction;
import net.natpad.worm.Request;
import net.natpad.worm.WormService;

@CatLogLevel(CatLogLevels.CAT_LOG_TRACE)
public class JagRequestApplyTransactionChanges extends Request {

	
	JagMooseContext mooseContext;
	
	public JagRequestApplyTransactionChanges(JagMooseContext context) {
		this.mooseContext = context;
		setTimeOutDelta(150);
	}
	
	
	@Override
	public void runRequest() {
		cat_log_debug("RUNNING !!!");
		
		Transaction transaction = mooseContext.monkeyService.createTransaction();

		transaction.begin();
		
		MooNode rootNode = transaction.root;
		cat_log_debug("rootNode="+rootNode);

		
		
		for(int idx=0; idx<rootNode.count(); idx++) {
			MooINode childNode = rootNode.childAt(idx);
			MooModuleContent moduleContent = childNode.getContent(MooModuleContent.KEY);
			
			if (moduleContent!=null) {
				
				WormService worm_service = mooseContext.getWormService();
				cat_log_debug("refreshing module:"+moduleContent);
				JagRequestRefreshModule refreshModule = new JagRequestRefreshModule(mooseContext, childNode.getUniqueId(), idx);
				worm_service.postRequest(refreshModule);
			}
		}
		
	}
	
	
	
	static class JagSrcFolderMatcher implements MooINodeMatcher {

		private final String srcFolderName;
		
		public JagSrcFolderMatcher(String srcFolderName) {
			this.srcFolderName = srcFolderName;
		}
		
		@Override
		public boolean matches(MooINode node) {
			JagSrcFolderContent srcFolderContent = node.getContent(JagSrcFolderContent.KEY);
			if (srcFolderContent!=null && srcFolderName.equals(node.getName())) {
				return true;
			}
			return false;
		}
		
	}

	
	static class JagCompilerMatcher implements MooINodeMatcher {

		
		public JagCompilerMatcher() {
		}
		
		@Override
		public boolean matches(MooINode node) {
			if (node.getContent(JagNodeCompiler.KEY)!=null) {
				return true;
			}
			return false;
		}
		
	}

	
}


  