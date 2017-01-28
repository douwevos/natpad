package net.natpad.worm;

class ConditionState {
		
		public final Condition condition;
		public final IConditionObserver selector;
		public int select;
		
		volatile boolean hold;
		
		public ConditionState(Condition condition, IConditionObserver selector) {
			this.condition = condition;
			this.selector = selector;
			select = condition.registerSelector(selector);
		}
		
		public void setHold() {
			int newSelect = condition.getSelect();
//			System.out.println("["+id+"] select="+select+", newSelect="+newSelect+", condition="+condition+", holder="+this);
			if (newSelect!=select) {
				hold = true;
				select = newSelect;
			} else {
				hold = false;
			}
		}
		
		public boolean wasSelected() {
//			System.out.println("["+id+"] hold="+hold+", condition="+condition+", holder="+this);
			return hold;
		}
				
	}