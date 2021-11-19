// Generated from d:\Dokumente\JustForFun GitHub Clones\spice\compiler\src\grammar\Spice.g4 by ANTLR 4.8
import org.antlr.v4.runtime.Lexer;
import org.antlr.v4.runtime.CharStream;
import org.antlr.v4.runtime.Token;
import org.antlr.v4.runtime.TokenStream;
import org.antlr.v4.runtime.*;
import org.antlr.v4.runtime.atn.*;
import org.antlr.v4.runtime.dfa.DFA;
import org.antlr.v4.runtime.misc.*;

@SuppressWarnings({"all", "warnings", "unchecked", "unused", "cast"})
public class SpiceLexer extends Lexer {
	static { RuntimeMetaData.checkVersion("4.8", RuntimeMetaData.VERSION); }

	protected static final DFA[] _decisionToDFA;
	protected static final PredictionContextCache _sharedContextCache =
		new PredictionContextCache();
	public static final int
		TYPE_DOUBLE=1, TYPE_INT=2, TYPE_STRING=3, TYPE_BOOL=4, TYPE_DYN=5, F=6, 
		P=7, IF=8, ELSE=9, FOR=10, WHILE=11, CONST=12, IMPORT=13, BREAK=14, CONTINUE=15, 
		RETURN=16, AS=17, STRUCT=18, TYPE=19, MAIN=20, PRINTF=21, TRUE=22, FALSE=23, 
		STRING=24, INTEGER=25, DOUBLE=26, IDENTIFIER=27, LBRACE=28, RBRACE=29, 
		LPAREN=30, RPAREN=31, LBRACKET=32, RBRACKET=33, LOGICAL_OR=34, LOGICAL_AND=35, 
		BITWISE_OR=36, BITWISE_AND=37, NOT=38, PLUS_PLUS=39, MINUS_MINUS=40, PLUS_EQUAL=41, 
		MINUS_EQUAL=42, MUL_EQUAL=43, DIV_EQUAL=44, PLUS=45, MINUS=46, MUL=47, 
		DIV=48, GREATER=49, LESS=50, GREATER_EQUAL=51, LESS_EQUAL=52, EQUAL=53, 
		NOT_EQUAL=54, ASSIGN_OP=55, QUESTION_MARK=56, SEMICOLON=57, COLON=58, 
		COMMA=59, DOT=60, COMMENT=61, LINE_COMMENT=62, WS=63;
	public static String[] channelNames = {
		"DEFAULT_TOKEN_CHANNEL", "HIDDEN"
	};

	public static String[] modeNames = {
		"DEFAULT_MODE"
	};

	private static String[] makeRuleNames() {
		return new String[] {
			"TYPE_DOUBLE", "TYPE_INT", "TYPE_STRING", "TYPE_BOOL", "TYPE_DYN", "F", 
			"P", "IF", "ELSE", "FOR", "WHILE", "CONST", "IMPORT", "BREAK", "CONTINUE", 
			"RETURN", "AS", "STRUCT", "TYPE", "MAIN", "PRINTF", "TRUE", "FALSE", 
			"STRING", "INTEGER", "DOUBLE", "IDENTIFIER", "LBRACE", "RBRACE", "LPAREN", 
			"RPAREN", "LBRACKET", "RBRACKET", "LOGICAL_OR", "LOGICAL_AND", "BITWISE_OR", 
			"BITWISE_AND", "NOT", "PLUS_PLUS", "MINUS_MINUS", "PLUS_EQUAL", "MINUS_EQUAL", 
			"MUL_EQUAL", "DIV_EQUAL", "PLUS", "MINUS", "MUL", "DIV", "GREATER", "LESS", 
			"GREATER_EQUAL", "LESS_EQUAL", "EQUAL", "NOT_EQUAL", "ASSIGN_OP", "QUESTION_MARK", 
			"SEMICOLON", "COLON", "COMMA", "DOT", "COMMENT", "LINE_COMMENT", "WS"
		};
	}
	public static final String[] ruleNames = makeRuleNames();

	private static String[] makeLiteralNames() {
		return new String[] {
			null, "'double'", "'int'", "'string'", "'bool'", "'dyn'", "'f'", "'p'", 
			"'if'", "'else'", "'for'", "'while'", "'const'", "'import'", "'break'", 
			"'continue'", "'return'", "'as'", "'struct'", "'type'", "'main'", "'printf'", 
			"'true'", "'false'", null, null, null, null, "'{'", "'}'", "'('", "')'", 
			"'['", "']'", "'||'", "'&&'", "'|'", "'&'", "'!'", "'++'", "'--'", "'+='", 
			"'-='", "'*='", "'/='", "'+'", "'-'", "'*'", "'/'", "'>'", "'<'", "'>='", 
			"'<='", "'=='", "'!='", "'='", "'?'", "';'", "':'", "','", "'.'"
		};
	}
	private static final String[] _LITERAL_NAMES = makeLiteralNames();
	private static String[] makeSymbolicNames() {
		return new String[] {
			null, "TYPE_DOUBLE", "TYPE_INT", "TYPE_STRING", "TYPE_BOOL", "TYPE_DYN", 
			"F", "P", "IF", "ELSE", "FOR", "WHILE", "CONST", "IMPORT", "BREAK", "CONTINUE", 
			"RETURN", "AS", "STRUCT", "TYPE", "MAIN", "PRINTF", "TRUE", "FALSE", 
			"STRING", "INTEGER", "DOUBLE", "IDENTIFIER", "LBRACE", "RBRACE", "LPAREN", 
			"RPAREN", "LBRACKET", "RBRACKET", "LOGICAL_OR", "LOGICAL_AND", "BITWISE_OR", 
			"BITWISE_AND", "NOT", "PLUS_PLUS", "MINUS_MINUS", "PLUS_EQUAL", "MINUS_EQUAL", 
			"MUL_EQUAL", "DIV_EQUAL", "PLUS", "MINUS", "MUL", "DIV", "GREATER", "LESS", 
			"GREATER_EQUAL", "LESS_EQUAL", "EQUAL", "NOT_EQUAL", "ASSIGN_OP", "QUESTION_MARK", 
			"SEMICOLON", "COLON", "COMMA", "DOT", "COMMENT", "LINE_COMMENT", "WS"
		};
	}
	private static final String[] _SYMBOLIC_NAMES = makeSymbolicNames();
	public static final Vocabulary VOCABULARY = new VocabularyImpl(_LITERAL_NAMES, _SYMBOLIC_NAMES);

	/**
	 * @deprecated Use {@link #VOCABULARY} instead.
	 */
	@Deprecated
	public static final String[] tokenNames;
	static {
		tokenNames = new String[_SYMBOLIC_NAMES.length];
		for (int i = 0; i < tokenNames.length; i++) {
			tokenNames[i] = VOCABULARY.getLiteralName(i);
			if (tokenNames[i] == null) {
				tokenNames[i] = VOCABULARY.getSymbolicName(i);
			}

			if (tokenNames[i] == null) {
				tokenNames[i] = "<INVALID>";
			}
		}
	}

	@Override
	@Deprecated
	public String[] getTokenNames() {
		return tokenNames;
	}

	@Override

	public Vocabulary getVocabulary() {
		return VOCABULARY;
	}


	public SpiceLexer(CharStream input) {
		super(input);
		_interp = new LexerATNSimulator(this,_ATN,_decisionToDFA,_sharedContextCache);
	}

	@Override
	public String getGrammarFileName() { return "Spice.g4"; }

	@Override
	public String[] getRuleNames() { return ruleNames; }

	@Override
	public String getSerializedATN() { return _serializedATN; }

	@Override
	public String[] getChannelNames() { return channelNames; }

	@Override
	public String[] getModeNames() { return modeNames; }

	@Override
	public ATN getATN() { return _ATN; }

	public static final String _serializedATN =
		"\3\u608b\ua72a\u8133\ub9ed\u417c\u3be7\u7786\u5964\2A\u01a3\b\1\4\2\t"+
		"\2\4\3\t\3\4\4\t\4\4\5\t\5\4\6\t\6\4\7\t\7\4\b\t\b\4\t\t\t\4\n\t\n\4\13"+
		"\t\13\4\f\t\f\4\r\t\r\4\16\t\16\4\17\t\17\4\20\t\20\4\21\t\21\4\22\t\22"+
		"\4\23\t\23\4\24\t\24\4\25\t\25\4\26\t\26\4\27\t\27\4\30\t\30\4\31\t\31"+
		"\4\32\t\32\4\33\t\33\4\34\t\34\4\35\t\35\4\36\t\36\4\37\t\37\4 \t \4!"+
		"\t!\4\"\t\"\4#\t#\4$\t$\4%\t%\4&\t&\4\'\t\'\4(\t(\4)\t)\4*\t*\4+\t+\4"+
		",\t,\4-\t-\4.\t.\4/\t/\4\60\t\60\4\61\t\61\4\62\t\62\4\63\t\63\4\64\t"+
		"\64\4\65\t\65\4\66\t\66\4\67\t\67\48\t8\49\t9\4:\t:\4;\t;\4<\t<\4=\t="+
		"\4>\t>\4?\t?\4@\t@\3\2\3\2\3\2\3\2\3\2\3\2\3\2\3\3\3\3\3\3\3\3\3\4\3\4"+
		"\3\4\3\4\3\4\3\4\3\4\3\5\3\5\3\5\3\5\3\5\3\6\3\6\3\6\3\6\3\7\3\7\3\b\3"+
		"\b\3\t\3\t\3\t\3\n\3\n\3\n\3\n\3\n\3\13\3\13\3\13\3\13\3\f\3\f\3\f\3\f"+
		"\3\f\3\f\3\r\3\r\3\r\3\r\3\r\3\r\3\16\3\16\3\16\3\16\3\16\3\16\3\16\3"+
		"\17\3\17\3\17\3\17\3\17\3\17\3\20\3\20\3\20\3\20\3\20\3\20\3\20\3\20\3"+
		"\20\3\21\3\21\3\21\3\21\3\21\3\21\3\21\3\22\3\22\3\22\3\23\3\23\3\23\3"+
		"\23\3\23\3\23\3\23\3\24\3\24\3\24\3\24\3\24\3\25\3\25\3\25\3\25\3\25\3"+
		"\26\3\26\3\26\3\26\3\26\3\26\3\26\3\27\3\27\3\27\3\27\3\27\3\30\3\30\3"+
		"\30\3\30\3\30\3\30\3\31\3\31\3\31\3\31\3\31\5\31\u0101\n\31\7\31\u0103"+
		"\n\31\f\31\16\31\u0106\13\31\3\31\3\31\3\32\7\32\u010b\n\32\f\32\16\32"+
		"\u010e\13\32\3\32\6\32\u0111\n\32\r\32\16\32\u0112\3\32\7\32\u0116\n\32"+
		"\f\32\16\32\u0119\13\32\3\32\5\32\u011c\n\32\3\33\7\33\u011f\n\33\f\33"+
		"\16\33\u0122\13\33\3\33\6\33\u0125\n\33\r\33\16\33\u0126\3\33\3\33\6\33"+
		"\u012b\n\33\r\33\16\33\u012c\3\34\3\34\7\34\u0131\n\34\f\34\16\34\u0134"+
		"\13\34\3\35\3\35\3\36\3\36\3\37\3\37\3 \3 \3!\3!\3\"\3\"\3#\3#\3#\3$\3"+
		"$\3$\3%\3%\3&\3&\3\'\3\'\3(\3(\3(\3)\3)\3)\3*\3*\3*\3+\3+\3+\3,\3,\3,"+
		"\3-\3-\3-\3.\3.\3/\3/\3\60\3\60\3\61\3\61\3\62\3\62\3\63\3\63\3\64\3\64"+
		"\3\64\3\65\3\65\3\65\3\66\3\66\3\66\3\67\3\67\3\67\38\38\39\39\3:\3:\3"+
		";\3;\3<\3<\3=\3=\3>\3>\3>\3>\7>\u0188\n>\f>\16>\u018b\13>\3>\3>\3>\3>"+
		"\3>\3?\3?\3?\3?\7?\u0196\n?\f?\16?\u0199\13?\3?\3?\3@\6@\u019e\n@\r@\16"+
		"@\u019f\3@\3@\3\u0189\2A\3\3\5\4\7\5\t\6\13\7\r\b\17\t\21\n\23\13\25\f"+
		"\27\r\31\16\33\17\35\20\37\21!\22#\23%\24\'\25)\26+\27-\30/\31\61\32\63"+
		"\33\65\34\67\359\36;\37= ?!A\"C#E$G%I&K\'M(O)Q*S+U,W-Y.[/]\60_\61a\62"+
		"c\63e\64g\65i\66k\67m8o9q:s;u<w=y>{?}@\177A\3\2\t\6\2\f\f\17\17$$^^\3"+
		"\2\63;\3\2\62;\5\2C\\aac|\6\2\62;C\\aac|\4\2\f\f\17\17\5\2\13\f\17\17"+
		"\"\"\2\u01b0\2\3\3\2\2\2\2\5\3\2\2\2\2\7\3\2\2\2\2\t\3\2\2\2\2\13\3\2"+
		"\2\2\2\r\3\2\2\2\2\17\3\2\2\2\2\21\3\2\2\2\2\23\3\2\2\2\2\25\3\2\2\2\2"+
		"\27\3\2\2\2\2\31\3\2\2\2\2\33\3\2\2\2\2\35\3\2\2\2\2\37\3\2\2\2\2!\3\2"+
		"\2\2\2#\3\2\2\2\2%\3\2\2\2\2\'\3\2\2\2\2)\3\2\2\2\2+\3\2\2\2\2-\3\2\2"+
		"\2\2/\3\2\2\2\2\61\3\2\2\2\2\63\3\2\2\2\2\65\3\2\2\2\2\67\3\2\2\2\29\3"+
		"\2\2\2\2;\3\2\2\2\2=\3\2\2\2\2?\3\2\2\2\2A\3\2\2\2\2C\3\2\2\2\2E\3\2\2"+
		"\2\2G\3\2\2\2\2I\3\2\2\2\2K\3\2\2\2\2M\3\2\2\2\2O\3\2\2\2\2Q\3\2\2\2\2"+
		"S\3\2\2\2\2U\3\2\2\2\2W\3\2\2\2\2Y\3\2\2\2\2[\3\2\2\2\2]\3\2\2\2\2_\3"+
		"\2\2\2\2a\3\2\2\2\2c\3\2\2\2\2e\3\2\2\2\2g\3\2\2\2\2i\3\2\2\2\2k\3\2\2"+
		"\2\2m\3\2\2\2\2o\3\2\2\2\2q\3\2\2\2\2s\3\2\2\2\2u\3\2\2\2\2w\3\2\2\2\2"+
		"y\3\2\2\2\2{\3\2\2\2\2}\3\2\2\2\2\177\3\2\2\2\3\u0081\3\2\2\2\5\u0088"+
		"\3\2\2\2\7\u008c\3\2\2\2\t\u0093\3\2\2\2\13\u0098\3\2\2\2\r\u009c\3\2"+
		"\2\2\17\u009e\3\2\2\2\21\u00a0\3\2\2\2\23\u00a3\3\2\2\2\25\u00a8\3\2\2"+
		"\2\27\u00ac\3\2\2\2\31\u00b2\3\2\2\2\33\u00b8\3\2\2\2\35\u00bf\3\2\2\2"+
		"\37\u00c5\3\2\2\2!\u00ce\3\2\2\2#\u00d5\3\2\2\2%\u00d8\3\2\2\2\'\u00df"+
		"\3\2\2\2)\u00e4\3\2\2\2+\u00e9\3\2\2\2-\u00f0\3\2\2\2/\u00f5\3\2\2\2\61"+
		"\u00fb\3\2\2\2\63\u011b\3\2\2\2\65\u0120\3\2\2\2\67\u012e\3\2\2\29\u0135"+
		"\3\2\2\2;\u0137\3\2\2\2=\u0139\3\2\2\2?\u013b\3\2\2\2A\u013d\3\2\2\2C"+
		"\u013f\3\2\2\2E\u0141\3\2\2\2G\u0144\3\2\2\2I\u0147\3\2\2\2K\u0149\3\2"+
		"\2\2M\u014b\3\2\2\2O\u014d\3\2\2\2Q\u0150\3\2\2\2S\u0153\3\2\2\2U\u0156"+
		"\3\2\2\2W\u0159\3\2\2\2Y\u015c\3\2\2\2[\u015f\3\2\2\2]\u0161\3\2\2\2_"+
		"\u0163\3\2\2\2a\u0165\3\2\2\2c\u0167\3\2\2\2e\u0169\3\2\2\2g\u016b\3\2"+
		"\2\2i\u016e\3\2\2\2k\u0171\3\2\2\2m\u0174\3\2\2\2o\u0177\3\2\2\2q\u0179"+
		"\3\2\2\2s\u017b\3\2\2\2u\u017d\3\2\2\2w\u017f\3\2\2\2y\u0181\3\2\2\2{"+
		"\u0183\3\2\2\2}\u0191\3\2\2\2\177\u019d\3\2\2\2\u0081\u0082\7f\2\2\u0082"+
		"\u0083\7q\2\2\u0083\u0084\7w\2\2\u0084\u0085\7d\2\2\u0085\u0086\7n\2\2"+
		"\u0086\u0087\7g\2\2\u0087\4\3\2\2\2\u0088\u0089\7k\2\2\u0089\u008a\7p"+
		"\2\2\u008a\u008b\7v\2\2\u008b\6\3\2\2\2\u008c\u008d\7u\2\2\u008d\u008e"+
		"\7v\2\2\u008e\u008f\7t\2\2\u008f\u0090\7k\2\2\u0090\u0091\7p\2\2\u0091"+
		"\u0092\7i\2\2\u0092\b\3\2\2\2\u0093\u0094\7d\2\2\u0094\u0095\7q\2\2\u0095"+
		"\u0096\7q\2\2\u0096\u0097\7n\2\2\u0097\n\3\2\2\2\u0098\u0099\7f\2\2\u0099"+
		"\u009a\7{\2\2\u009a\u009b\7p\2\2\u009b\f\3\2\2\2\u009c\u009d\7h\2\2\u009d"+
		"\16\3\2\2\2\u009e\u009f\7r\2\2\u009f\20\3\2\2\2\u00a0\u00a1\7k\2\2\u00a1"+
		"\u00a2\7h\2\2\u00a2\22\3\2\2\2\u00a3\u00a4\7g\2\2\u00a4\u00a5\7n\2\2\u00a5"+
		"\u00a6\7u\2\2\u00a6\u00a7\7g\2\2\u00a7\24\3\2\2\2\u00a8\u00a9\7h\2\2\u00a9"+
		"\u00aa\7q\2\2\u00aa\u00ab\7t\2\2\u00ab\26\3\2\2\2\u00ac\u00ad\7y\2\2\u00ad"+
		"\u00ae\7j\2\2\u00ae\u00af\7k\2\2\u00af\u00b0\7n\2\2\u00b0\u00b1\7g\2\2"+
		"\u00b1\30\3\2\2\2\u00b2\u00b3\7e\2\2\u00b3\u00b4\7q\2\2\u00b4\u00b5\7"+
		"p\2\2\u00b5\u00b6\7u\2\2\u00b6\u00b7\7v\2\2\u00b7\32\3\2\2\2\u00b8\u00b9"+
		"\7k\2\2\u00b9\u00ba\7o\2\2\u00ba\u00bb\7r\2\2\u00bb\u00bc\7q\2\2\u00bc"+
		"\u00bd\7t\2\2\u00bd\u00be\7v\2\2\u00be\34\3\2\2\2\u00bf\u00c0\7d\2\2\u00c0"+
		"\u00c1\7t\2\2\u00c1\u00c2\7g\2\2\u00c2\u00c3\7c\2\2\u00c3\u00c4\7m\2\2"+
		"\u00c4\36\3\2\2\2\u00c5\u00c6\7e\2\2\u00c6\u00c7\7q\2\2\u00c7\u00c8\7"+
		"p\2\2\u00c8\u00c9\7v\2\2\u00c9\u00ca\7k\2\2\u00ca\u00cb\7p\2\2\u00cb\u00cc"+
		"\7w\2\2\u00cc\u00cd\7g\2\2\u00cd \3\2\2\2\u00ce\u00cf\7t\2\2\u00cf\u00d0"+
		"\7g\2\2\u00d0\u00d1\7v\2\2\u00d1\u00d2\7w\2\2\u00d2\u00d3\7t\2\2\u00d3"+
		"\u00d4\7p\2\2\u00d4\"\3\2\2\2\u00d5\u00d6\7c\2\2\u00d6\u00d7\7u\2\2\u00d7"+
		"$\3\2\2\2\u00d8\u00d9\7u\2\2\u00d9\u00da\7v\2\2\u00da\u00db\7t\2\2\u00db"+
		"\u00dc\7w\2\2\u00dc\u00dd\7e\2\2\u00dd\u00de\7v\2\2\u00de&\3\2\2\2\u00df"+
		"\u00e0\7v\2\2\u00e0\u00e1\7{\2\2\u00e1\u00e2\7r\2\2\u00e2\u00e3\7g\2\2"+
		"\u00e3(\3\2\2\2\u00e4\u00e5\7o\2\2\u00e5\u00e6\7c\2\2\u00e6\u00e7\7k\2"+
		"\2\u00e7\u00e8\7p\2\2\u00e8*\3\2\2\2\u00e9\u00ea\7r\2\2\u00ea\u00eb\7"+
		"t\2\2\u00eb\u00ec\7k\2\2\u00ec\u00ed\7p\2\2\u00ed\u00ee\7v\2\2\u00ee\u00ef"+
		"\7h\2\2\u00ef,\3\2\2\2\u00f0\u00f1\7v\2\2\u00f1\u00f2\7t\2\2\u00f2\u00f3"+
		"\7w\2\2\u00f3\u00f4\7g\2\2\u00f4.\3\2\2\2\u00f5\u00f6\7h\2\2\u00f6\u00f7"+
		"\7c\2\2\u00f7\u00f8\7n\2\2\u00f8\u00f9\7u\2\2\u00f9\u00fa\7g\2\2\u00fa"+
		"\60\3\2\2\2\u00fb\u0104\7$\2\2\u00fc\u0103\n\2\2\2\u00fd\u0100\7^\2\2"+
		"\u00fe\u0101\13\2\2\2\u00ff\u0101\7\2\2\3\u0100\u00fe\3\2\2\2\u0100\u00ff"+
		"\3\2\2\2\u0101\u0103\3\2\2\2\u0102\u00fc\3\2\2\2\u0102\u00fd\3\2\2\2\u0103"+
		"\u0106\3\2\2\2\u0104\u0102\3\2\2\2\u0104\u0105\3\2\2\2\u0105\u0107\3\2"+
		"\2\2\u0106\u0104\3\2\2\2\u0107\u0108\7$\2\2\u0108\62\3\2\2\2\u0109\u010b"+
		"\7/\2\2\u010a\u0109\3\2\2\2\u010b\u010e\3\2\2\2\u010c\u010a\3\2\2\2\u010c"+
		"\u010d\3\2\2\2\u010d\u0110\3\2\2\2\u010e\u010c\3\2\2\2\u010f\u0111\t\3"+
		"\2\2\u0110\u010f\3\2\2\2\u0111\u0112\3\2\2\2\u0112\u0110\3\2\2\2\u0112"+
		"\u0113\3\2\2\2\u0113\u0117\3\2\2\2\u0114\u0116\t\4\2\2\u0115\u0114\3\2"+
		"\2\2\u0116\u0119\3\2\2\2\u0117\u0115\3\2\2\2\u0117\u0118\3\2\2\2\u0118"+
		"\u011c\3\2\2\2\u0119\u0117\3\2\2\2\u011a\u011c\7\62\2\2\u011b\u010c\3"+
		"\2\2\2\u011b\u011a\3\2\2\2\u011c\64\3\2\2\2\u011d\u011f\7/\2\2\u011e\u011d"+
		"\3\2\2\2\u011f\u0122\3\2\2\2\u0120\u011e\3\2\2\2\u0120\u0121\3\2\2\2\u0121"+
		"\u0124\3\2\2\2\u0122\u0120\3\2\2\2\u0123\u0125\t\4\2\2\u0124\u0123\3\2"+
		"\2\2\u0125\u0126\3\2\2\2\u0126\u0124\3\2\2\2\u0126\u0127\3\2\2\2\u0127"+
		"\u0128\3\2\2\2\u0128\u012a\13\2\2\2\u0129\u012b\t\4\2\2\u012a\u0129\3"+
		"\2\2\2\u012b\u012c\3\2\2\2\u012c\u012a\3\2\2\2\u012c\u012d\3\2\2\2\u012d"+
		"\66\3\2\2\2\u012e\u0132\t\5\2\2\u012f\u0131\t\6\2\2\u0130\u012f\3\2\2"+
		"\2\u0131\u0134\3\2\2\2\u0132\u0130\3\2\2\2\u0132\u0133\3\2\2\2\u01338"+
		"\3\2\2\2\u0134\u0132\3\2\2\2\u0135\u0136\7}\2\2\u0136:\3\2\2\2\u0137\u0138"+
		"\7\177\2\2\u0138<\3\2\2\2\u0139\u013a\7*\2\2\u013a>\3\2\2\2\u013b\u013c"+
		"\7+\2\2\u013c@\3\2\2\2\u013d\u013e\7]\2\2\u013eB\3\2\2\2\u013f\u0140\7"+
		"_\2\2\u0140D\3\2\2\2\u0141\u0142\7~\2\2\u0142\u0143\7~\2\2\u0143F\3\2"+
		"\2\2\u0144\u0145\7(\2\2\u0145\u0146\7(\2\2\u0146H\3\2\2\2\u0147\u0148"+
		"\7~\2\2\u0148J\3\2\2\2\u0149\u014a\7(\2\2\u014aL\3\2\2\2\u014b\u014c\7"+
		"#\2\2\u014cN\3\2\2\2\u014d\u014e\7-\2\2\u014e\u014f\7-\2\2\u014fP\3\2"+
		"\2\2\u0150\u0151\7/\2\2\u0151\u0152\7/\2\2\u0152R\3\2\2\2\u0153\u0154"+
		"\7-\2\2\u0154\u0155\7?\2\2\u0155T\3\2\2\2\u0156\u0157\7/\2\2\u0157\u0158"+
		"\7?\2\2\u0158V\3\2\2\2\u0159\u015a\7,\2\2\u015a\u015b\7?\2\2\u015bX\3"+
		"\2\2\2\u015c\u015d\7\61\2\2\u015d\u015e\7?\2\2\u015eZ\3\2\2\2\u015f\u0160"+
		"\7-\2\2\u0160\\\3\2\2\2\u0161\u0162\7/\2\2\u0162^\3\2\2\2\u0163\u0164"+
		"\7,\2\2\u0164`\3\2\2\2\u0165\u0166\7\61\2\2\u0166b\3\2\2\2\u0167\u0168"+
		"\7@\2\2\u0168d\3\2\2\2\u0169\u016a\7>\2\2\u016af\3\2\2\2\u016b\u016c\7"+
		"@\2\2\u016c\u016d\7?\2\2\u016dh\3\2\2\2\u016e\u016f\7>\2\2\u016f\u0170"+
		"\7?\2\2\u0170j\3\2\2\2\u0171\u0172\7?\2\2\u0172\u0173\7?\2\2\u0173l\3"+
		"\2\2\2\u0174\u0175\7#\2\2\u0175\u0176\7?\2\2\u0176n\3\2\2\2\u0177\u0178"+
		"\7?\2\2\u0178p\3\2\2\2\u0179\u017a\7A\2\2\u017ar\3\2\2\2\u017b\u017c\7"+
		"=\2\2\u017ct\3\2\2\2\u017d\u017e\7<\2\2\u017ev\3\2\2\2\u017f\u0180\7."+
		"\2\2\u0180x\3\2\2\2\u0181\u0182\7\60\2\2\u0182z\3\2\2\2\u0183\u0184\7"+
		"\61\2\2\u0184\u0185\7,\2\2\u0185\u0189\3\2\2\2\u0186\u0188\13\2\2\2\u0187"+
		"\u0186\3\2\2\2\u0188\u018b\3\2\2\2\u0189\u018a\3\2\2\2\u0189\u0187\3\2"+
		"\2\2\u018a\u018c\3\2\2\2\u018b\u0189\3\2\2\2\u018c\u018d\7,\2\2\u018d"+
		"\u018e\7\61\2\2\u018e\u018f\3\2\2\2\u018f\u0190\b>\2\2\u0190|\3\2\2\2"+
		"\u0191\u0192\7\61\2\2\u0192\u0193\7\61\2\2\u0193\u0197\3\2\2\2\u0194\u0196"+
		"\n\7\2\2\u0195\u0194\3\2\2\2\u0196\u0199\3\2\2\2\u0197\u0195\3\2\2\2\u0197"+
		"\u0198\3\2\2\2\u0198\u019a\3\2\2\2\u0199\u0197\3\2\2\2\u019a\u019b\b?"+
		"\2\2\u019b~\3\2\2\2\u019c\u019e\t\b\2\2\u019d\u019c\3\2\2\2\u019e\u019f"+
		"\3\2\2\2\u019f\u019d\3\2\2\2\u019f\u01a0\3\2\2\2\u01a0\u01a1\3\2\2\2\u01a1"+
		"\u01a2\b@\3\2\u01a2\u0080\3\2\2\2\21\2\u0100\u0102\u0104\u010c\u0112\u0117"+
		"\u011b\u0120\u0126\u012c\u0132\u0189\u0197\u019f\4\b\2\2\2\3\2";
	public static final ATN _ATN =
		new ATNDeserializer().deserialize(_serializedATN.toCharArray());
	static {
		_decisionToDFA = new DFA[_ATN.getNumberOfDecisions()];
		for (int i = 0; i < _ATN.getNumberOfDecisions(); i++) {
			_decisionToDFA[i] = new DFA(_ATN.getDecisionState(i), i);
		}
	}
}