import static net.natpad.caterpillar.CatLogger.cat_log_debug;
import net.natpad.caterpillar.CatLogLevel;
import net.natpad.caterpillar.CatLogLevels;

@CatLogLevel(CatLogLevels.CAT_LOG_WARN)
public class CatLoggerTest {

	
	public static void main(String[] args) {
		
		cat_log_debug("testje");
		
		String helper = "douwe";
		
		cat_log_debug("testje2: nummer=%d, location=%p, txt='%s'", 4, helper, helper);
		
		new CatLoggerTest();
	}
	
	public CatLoggerTest() {
		cat_log_debug("show me");
	}
	
}
