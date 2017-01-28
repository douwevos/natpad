package net.natpad.cup.model.parser.generated.runtime;

/**
 * Defines the Scanner interface, which CUP uses in the default
 * implementation of <code>LrParser.scan()</code>.  Integration
 * of scanners implementing <code>Scanner</code> is facilitated.
 *
 * @version last updated 23-Jul-1999
 * @author David MacMahon <davidm@smartsc.com>
 */

/* *************************************************
  Interface Scanner
  
  Declares the next_token() method that should be
  implemented by scanners.  This method is typically
  called by LrParser.scan().  End-of-file can be
  indicated either by returning
  <code>new LrSymbol(LrParser.EOF_sym())</code> or
  <code>null</code>.
 ***************************************************/
public interface LrScanner {
    /** Return the next token, or <code>null</code> on end-of-file. */
    public LrSymbol next_token() throws java.lang.Exception;
}
