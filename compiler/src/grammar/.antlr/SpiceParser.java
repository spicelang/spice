// Generated from d:\Dokumente\JustForFun GitHub Clones\spice\compiler\src\grammar\Spice.g4 by ANTLR 4.8
import org.antlr.v4.runtime.atn.*;
import org.antlr.v4.runtime.dfa.DFA;
import org.antlr.v4.runtime.*;
import org.antlr.v4.runtime.misc.*;
import org.antlr.v4.runtime.tree.*;
import java.util.List;
import java.util.Iterator;
import java.util.ArrayList;

@SuppressWarnings({"all", "warnings", "unchecked", "unused", "cast"})
public class SpiceParser extends Parser {
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
	public static final int
		RULE_entry = 0, RULE_mainFunctionDef = 1, RULE_functionDef = 2, RULE_procedureDef = 3, 
		RULE_structDef = 4, RULE_forLoop = 5, RULE_whileLoop = 6, RULE_ifStmt = 7, 
		RULE_elseStmt = 8, RULE_stmtLst = 9, RULE_fieldLst = 10, RULE_paramLstDef = 11, 
		RULE_paramLstCall = 12, RULE_stmt = 13, RULE_declStmt = 14, RULE_functionCall = 15, 
		RULE_importStmt = 16, RULE_returnStmt = 17, RULE_breakStmt = 18, RULE_continueStmt = 19, 
		RULE_printfStmt = 20, RULE_assignment = 21, RULE_ternary = 22, RULE_logicalOrExpr = 23, 
		RULE_logicalAndExpr = 24, RULE_bitwiseOrExpr = 25, RULE_bitwiseAndExpr = 26, 
		RULE_equalityExpr = 27, RULE_relationalExpr = 28, RULE_additiveExpr = 29, 
		RULE_multiplicativeExpr = 30, RULE_prefixUnary = 31, RULE_postfixUnary = 32, 
		RULE_atomicExpr = 33, RULE_value = 34, RULE_dataType = 35;
	private static String[] makeRuleNames() {
		return new String[] {
			"entry", "mainFunctionDef", "functionDef", "procedureDef", "structDef", 
			"forLoop", "whileLoop", "ifStmt", "elseStmt", "stmtLst", "fieldLst", 
			"paramLstDef", "paramLstCall", "stmt", "declStmt", "functionCall", "importStmt", 
			"returnStmt", "breakStmt", "continueStmt", "printfStmt", "assignment", 
			"ternary", "logicalOrExpr", "logicalAndExpr", "bitwiseOrExpr", "bitwiseAndExpr", 
			"equalityExpr", "relationalExpr", "additiveExpr", "multiplicativeExpr", 
			"prefixUnary", "postfixUnary", "atomicExpr", "value", "dataType"
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

	@Override
	public String getGrammarFileName() { return "Spice.g4"; }

	@Override
	public String[] getRuleNames() { return ruleNames; }

	@Override
	public String getSerializedATN() { return _serializedATN; }

	@Override
	public ATN getATN() { return _ATN; }

	public SpiceParser(TokenStream input) {
		super(input);
		_interp = new ParserATNSimulator(this,_ATN,_decisionToDFA,_sharedContextCache);
	}

	public static class EntryContext extends ParserRuleContext {
		public List<StmtContext> stmt() {
			return getRuleContexts(StmtContext.class);
		}
		public StmtContext stmt(int i) {
			return getRuleContext(StmtContext.class,i);
		}
		public List<MainFunctionDefContext> mainFunctionDef() {
			return getRuleContexts(MainFunctionDefContext.class);
		}
		public MainFunctionDefContext mainFunctionDef(int i) {
			return getRuleContext(MainFunctionDefContext.class,i);
		}
		public List<FunctionDefContext> functionDef() {
			return getRuleContexts(FunctionDefContext.class);
		}
		public FunctionDefContext functionDef(int i) {
			return getRuleContext(FunctionDefContext.class,i);
		}
		public List<ProcedureDefContext> procedureDef() {
			return getRuleContexts(ProcedureDefContext.class);
		}
		public ProcedureDefContext procedureDef(int i) {
			return getRuleContext(ProcedureDefContext.class,i);
		}
		public EntryContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_entry; }
	}

	public final EntryContext entry() throws RecognitionException {
		EntryContext _localctx = new EntryContext(_ctx, getState());
		enterRule(_localctx, 0, RULE_entry);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(78);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while ((((_la) & ~0x3f) == 0 && ((1L << _la) & ((1L << TYPE_DOUBLE) | (1L << TYPE_INT) | (1L << TYPE_STRING) | (1L << TYPE_BOOL) | (1L << TYPE_DYN) | (1L << F) | (1L << P) | (1L << CONST) | (1L << IMPORT) | (1L << BREAK) | (1L << CONTINUE) | (1L << RETURN) | (1L << PRINTF) | (1L << TRUE) | (1L << FALSE) | (1L << STRING) | (1L << INTEGER) | (1L << DOUBLE) | (1L << IDENTIFIER) | (1L << LPAREN) | (1L << NOT) | (1L << PLUS_PLUS) | (1L << MINUS_MINUS))) != 0)) {
				{
				setState(76);
				_errHandler.sync(this);
				switch ( getInterpreter().adaptivePredict(_input,0,_ctx) ) {
				case 1:
					{
					setState(72);
					stmt();
					}
					break;
				case 2:
					{
					setState(73);
					mainFunctionDef();
					}
					break;
				case 3:
					{
					setState(74);
					functionDef();
					}
					break;
				case 4:
					{
					setState(75);
					procedureDef();
					}
					break;
				}
				}
				setState(80);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class MainFunctionDefContext extends ParserRuleContext {
		public TerminalNode F() { return getToken(SpiceParser.F, 0); }
		public TerminalNode LESS() { return getToken(SpiceParser.LESS, 0); }
		public TerminalNode TYPE_INT() { return getToken(SpiceParser.TYPE_INT, 0); }
		public TerminalNode GREATER() { return getToken(SpiceParser.GREATER, 0); }
		public TerminalNode MAIN() { return getToken(SpiceParser.MAIN, 0); }
		public TerminalNode LPAREN() { return getToken(SpiceParser.LPAREN, 0); }
		public TerminalNode RPAREN() { return getToken(SpiceParser.RPAREN, 0); }
		public TerminalNode LBRACE() { return getToken(SpiceParser.LBRACE, 0); }
		public StmtLstContext stmtLst() {
			return getRuleContext(StmtLstContext.class,0);
		}
		public TerminalNode RBRACE() { return getToken(SpiceParser.RBRACE, 0); }
		public ParamLstDefContext paramLstDef() {
			return getRuleContext(ParamLstDefContext.class,0);
		}
		public MainFunctionDefContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_mainFunctionDef; }
	}

	public final MainFunctionDefContext mainFunctionDef() throws RecognitionException {
		MainFunctionDefContext _localctx = new MainFunctionDefContext(_ctx, getState());
		enterRule(_localctx, 2, RULE_mainFunctionDef);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(81);
			match(F);
			setState(82);
			match(LESS);
			setState(83);
			match(TYPE_INT);
			setState(84);
			match(GREATER);
			setState(85);
			match(MAIN);
			setState(86);
			match(LPAREN);
			setState(88);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if ((((_la) & ~0x3f) == 0 && ((1L << _la) & ((1L << TYPE_DOUBLE) | (1L << TYPE_INT) | (1L << TYPE_STRING) | (1L << TYPE_BOOL) | (1L << TYPE_DYN) | (1L << CONST) | (1L << TRUE) | (1L << FALSE) | (1L << STRING) | (1L << INTEGER) | (1L << DOUBLE) | (1L << IDENTIFIER) | (1L << LPAREN) | (1L << NOT) | (1L << PLUS_PLUS) | (1L << MINUS_MINUS))) != 0)) {
				{
				setState(87);
				paramLstDef();
				}
			}

			setState(90);
			match(RPAREN);
			setState(91);
			match(LBRACE);
			setState(92);
			stmtLst();
			setState(93);
			match(RBRACE);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class FunctionDefContext extends ParserRuleContext {
		public TerminalNode F() { return getToken(SpiceParser.F, 0); }
		public TerminalNode LESS() { return getToken(SpiceParser.LESS, 0); }
		public DataTypeContext dataType() {
			return getRuleContext(DataTypeContext.class,0);
		}
		public TerminalNode GREATER() { return getToken(SpiceParser.GREATER, 0); }
		public TerminalNode IDENTIFIER() { return getToken(SpiceParser.IDENTIFIER, 0); }
		public TerminalNode LPAREN() { return getToken(SpiceParser.LPAREN, 0); }
		public TerminalNode RPAREN() { return getToken(SpiceParser.RPAREN, 0); }
		public TerminalNode LBRACE() { return getToken(SpiceParser.LBRACE, 0); }
		public StmtLstContext stmtLst() {
			return getRuleContext(StmtLstContext.class,0);
		}
		public TerminalNode RBRACE() { return getToken(SpiceParser.RBRACE, 0); }
		public ParamLstDefContext paramLstDef() {
			return getRuleContext(ParamLstDefContext.class,0);
		}
		public FunctionDefContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_functionDef; }
	}

	public final FunctionDefContext functionDef() throws RecognitionException {
		FunctionDefContext _localctx = new FunctionDefContext(_ctx, getState());
		enterRule(_localctx, 4, RULE_functionDef);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(95);
			match(F);
			setState(96);
			match(LESS);
			setState(97);
			dataType();
			setState(98);
			match(GREATER);
			setState(99);
			match(IDENTIFIER);
			setState(100);
			match(LPAREN);
			setState(102);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if ((((_la) & ~0x3f) == 0 && ((1L << _la) & ((1L << TYPE_DOUBLE) | (1L << TYPE_INT) | (1L << TYPE_STRING) | (1L << TYPE_BOOL) | (1L << TYPE_DYN) | (1L << CONST) | (1L << TRUE) | (1L << FALSE) | (1L << STRING) | (1L << INTEGER) | (1L << DOUBLE) | (1L << IDENTIFIER) | (1L << LPAREN) | (1L << NOT) | (1L << PLUS_PLUS) | (1L << MINUS_MINUS))) != 0)) {
				{
				setState(101);
				paramLstDef();
				}
			}

			setState(104);
			match(RPAREN);
			setState(105);
			match(LBRACE);
			setState(106);
			stmtLst();
			setState(107);
			match(RBRACE);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class ProcedureDefContext extends ParserRuleContext {
		public TerminalNode P() { return getToken(SpiceParser.P, 0); }
		public TerminalNode IDENTIFIER() { return getToken(SpiceParser.IDENTIFIER, 0); }
		public TerminalNode LPAREN() { return getToken(SpiceParser.LPAREN, 0); }
		public TerminalNode RPAREN() { return getToken(SpiceParser.RPAREN, 0); }
		public TerminalNode LBRACE() { return getToken(SpiceParser.LBRACE, 0); }
		public StmtLstContext stmtLst() {
			return getRuleContext(StmtLstContext.class,0);
		}
		public TerminalNode RBRACE() { return getToken(SpiceParser.RBRACE, 0); }
		public ParamLstDefContext paramLstDef() {
			return getRuleContext(ParamLstDefContext.class,0);
		}
		public ProcedureDefContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_procedureDef; }
	}

	public final ProcedureDefContext procedureDef() throws RecognitionException {
		ProcedureDefContext _localctx = new ProcedureDefContext(_ctx, getState());
		enterRule(_localctx, 6, RULE_procedureDef);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(109);
			match(P);
			setState(110);
			match(IDENTIFIER);
			setState(111);
			match(LPAREN);
			setState(113);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if ((((_la) & ~0x3f) == 0 && ((1L << _la) & ((1L << TYPE_DOUBLE) | (1L << TYPE_INT) | (1L << TYPE_STRING) | (1L << TYPE_BOOL) | (1L << TYPE_DYN) | (1L << CONST) | (1L << TRUE) | (1L << FALSE) | (1L << STRING) | (1L << INTEGER) | (1L << DOUBLE) | (1L << IDENTIFIER) | (1L << LPAREN) | (1L << NOT) | (1L << PLUS_PLUS) | (1L << MINUS_MINUS))) != 0)) {
				{
				setState(112);
				paramLstDef();
				}
			}

			setState(115);
			match(RPAREN);
			setState(116);
			match(LBRACE);
			setState(117);
			stmtLst();
			setState(118);
			match(RBRACE);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class StructDefContext extends ParserRuleContext {
		public TerminalNode TYPE() { return getToken(SpiceParser.TYPE, 0); }
		public TerminalNode IDENTIFIER() { return getToken(SpiceParser.IDENTIFIER, 0); }
		public TerminalNode STRUCT() { return getToken(SpiceParser.STRUCT, 0); }
		public TerminalNode LBRACE() { return getToken(SpiceParser.LBRACE, 0); }
		public FieldLstContext fieldLst() {
			return getRuleContext(FieldLstContext.class,0);
		}
		public TerminalNode RBRACE() { return getToken(SpiceParser.RBRACE, 0); }
		public StructDefContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_structDef; }
	}

	public final StructDefContext structDef() throws RecognitionException {
		StructDefContext _localctx = new StructDefContext(_ctx, getState());
		enterRule(_localctx, 8, RULE_structDef);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(120);
			match(TYPE);
			setState(121);
			match(IDENTIFIER);
			setState(122);
			match(STRUCT);
			setState(123);
			match(LBRACE);
			setState(124);
			fieldLst();
			setState(125);
			match(RBRACE);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class ForLoopContext extends ParserRuleContext {
		public TerminalNode FOR() { return getToken(SpiceParser.FOR, 0); }
		public List<AssignmentContext> assignment() {
			return getRuleContexts(AssignmentContext.class);
		}
		public AssignmentContext assignment(int i) {
			return getRuleContext(AssignmentContext.class,i);
		}
		public List<TerminalNode> SEMICOLON() { return getTokens(SpiceParser.SEMICOLON); }
		public TerminalNode SEMICOLON(int i) {
			return getToken(SpiceParser.SEMICOLON, i);
		}
		public TerminalNode LBRACE() { return getToken(SpiceParser.LBRACE, 0); }
		public StmtLstContext stmtLst() {
			return getRuleContext(StmtLstContext.class,0);
		}
		public TerminalNode RBRACE() { return getToken(SpiceParser.RBRACE, 0); }
		public ForLoopContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_forLoop; }
	}

	public final ForLoopContext forLoop() throws RecognitionException {
		ForLoopContext _localctx = new ForLoopContext(_ctx, getState());
		enterRule(_localctx, 10, RULE_forLoop);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(127);
			match(FOR);
			setState(128);
			assignment();
			setState(129);
			match(SEMICOLON);
			setState(130);
			assignment();
			setState(131);
			match(SEMICOLON);
			setState(132);
			assignment();
			setState(133);
			match(LBRACE);
			setState(134);
			stmtLst();
			setState(135);
			match(RBRACE);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class WhileLoopContext extends ParserRuleContext {
		public TerminalNode WHILE() { return getToken(SpiceParser.WHILE, 0); }
		public AssignmentContext assignment() {
			return getRuleContext(AssignmentContext.class,0);
		}
		public TerminalNode LBRACE() { return getToken(SpiceParser.LBRACE, 0); }
		public StmtLstContext stmtLst() {
			return getRuleContext(StmtLstContext.class,0);
		}
		public TerminalNode RBRACE() { return getToken(SpiceParser.RBRACE, 0); }
		public WhileLoopContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_whileLoop; }
	}

	public final WhileLoopContext whileLoop() throws RecognitionException {
		WhileLoopContext _localctx = new WhileLoopContext(_ctx, getState());
		enterRule(_localctx, 12, RULE_whileLoop);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(137);
			match(WHILE);
			setState(138);
			assignment();
			setState(139);
			match(LBRACE);
			setState(140);
			stmtLst();
			setState(141);
			match(RBRACE);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class IfStmtContext extends ParserRuleContext {
		public TerminalNode IF() { return getToken(SpiceParser.IF, 0); }
		public AssignmentContext assignment() {
			return getRuleContext(AssignmentContext.class,0);
		}
		public TerminalNode LBRACE() { return getToken(SpiceParser.LBRACE, 0); }
		public StmtLstContext stmtLst() {
			return getRuleContext(StmtLstContext.class,0);
		}
		public TerminalNode RBRACE() { return getToken(SpiceParser.RBRACE, 0); }
		public ElseStmtContext elseStmt() {
			return getRuleContext(ElseStmtContext.class,0);
		}
		public IfStmtContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_ifStmt; }
	}

	public final IfStmtContext ifStmt() throws RecognitionException {
		IfStmtContext _localctx = new IfStmtContext(_ctx, getState());
		enterRule(_localctx, 14, RULE_ifStmt);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(143);
			match(IF);
			setState(144);
			assignment();
			setState(145);
			match(LBRACE);
			setState(146);
			stmtLst();
			setState(147);
			match(RBRACE);
			setState(149);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==ELSE) {
				{
				setState(148);
				elseStmt();
				}
			}

			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class ElseStmtContext extends ParserRuleContext {
		public TerminalNode ELSE() { return getToken(SpiceParser.ELSE, 0); }
		public IfStmtContext ifStmt() {
			return getRuleContext(IfStmtContext.class,0);
		}
		public TerminalNode LBRACE() { return getToken(SpiceParser.LBRACE, 0); }
		public StmtLstContext stmtLst() {
			return getRuleContext(StmtLstContext.class,0);
		}
		public TerminalNode RBRACE() { return getToken(SpiceParser.RBRACE, 0); }
		public ElseStmtContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_elseStmt; }
	}

	public final ElseStmtContext elseStmt() throws RecognitionException {
		ElseStmtContext _localctx = new ElseStmtContext(_ctx, getState());
		enterRule(_localctx, 16, RULE_elseStmt);
		try {
			setState(158);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,6,_ctx) ) {
			case 1:
				enterOuterAlt(_localctx, 1);
				{
				setState(151);
				match(ELSE);
				setState(152);
				ifStmt();
				}
				break;
			case 2:
				enterOuterAlt(_localctx, 2);
				{
				setState(153);
				match(ELSE);
				setState(154);
				match(LBRACE);
				setState(155);
				stmtLst();
				setState(156);
				match(RBRACE);
				}
				break;
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class StmtLstContext extends ParserRuleContext {
		public List<StmtContext> stmt() {
			return getRuleContexts(StmtContext.class);
		}
		public StmtContext stmt(int i) {
			return getRuleContext(StmtContext.class,i);
		}
		public List<ForLoopContext> forLoop() {
			return getRuleContexts(ForLoopContext.class);
		}
		public ForLoopContext forLoop(int i) {
			return getRuleContext(ForLoopContext.class,i);
		}
		public List<WhileLoopContext> whileLoop() {
			return getRuleContexts(WhileLoopContext.class);
		}
		public WhileLoopContext whileLoop(int i) {
			return getRuleContext(WhileLoopContext.class,i);
		}
		public List<IfStmtContext> ifStmt() {
			return getRuleContexts(IfStmtContext.class);
		}
		public IfStmtContext ifStmt(int i) {
			return getRuleContext(IfStmtContext.class,i);
		}
		public StmtLstContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_stmtLst; }
	}

	public final StmtLstContext stmtLst() throws RecognitionException {
		StmtLstContext _localctx = new StmtLstContext(_ctx, getState());
		enterRule(_localctx, 18, RULE_stmtLst);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(166);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while ((((_la) & ~0x3f) == 0 && ((1L << _la) & ((1L << TYPE_DOUBLE) | (1L << TYPE_INT) | (1L << TYPE_STRING) | (1L << TYPE_BOOL) | (1L << TYPE_DYN) | (1L << IF) | (1L << FOR) | (1L << WHILE) | (1L << CONST) | (1L << IMPORT) | (1L << BREAK) | (1L << CONTINUE) | (1L << RETURN) | (1L << PRINTF) | (1L << TRUE) | (1L << FALSE) | (1L << STRING) | (1L << INTEGER) | (1L << DOUBLE) | (1L << IDENTIFIER) | (1L << LPAREN) | (1L << NOT) | (1L << PLUS_PLUS) | (1L << MINUS_MINUS))) != 0)) {
				{
				setState(164);
				_errHandler.sync(this);
				switch (_input.LA(1)) {
				case TYPE_DOUBLE:
				case TYPE_INT:
				case TYPE_STRING:
				case TYPE_BOOL:
				case TYPE_DYN:
				case CONST:
				case IMPORT:
				case BREAK:
				case CONTINUE:
				case RETURN:
				case PRINTF:
				case TRUE:
				case FALSE:
				case STRING:
				case INTEGER:
				case DOUBLE:
				case IDENTIFIER:
				case LPAREN:
				case NOT:
				case PLUS_PLUS:
				case MINUS_MINUS:
					{
					setState(160);
					stmt();
					}
					break;
				case FOR:
					{
					setState(161);
					forLoop();
					}
					break;
				case WHILE:
					{
					setState(162);
					whileLoop();
					}
					break;
				case IF:
					{
					setState(163);
					ifStmt();
					}
					break;
				default:
					throw new NoViableAltException(this);
				}
				}
				setState(168);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class FieldLstContext extends ParserRuleContext {
		public List<DeclStmtContext> declStmt() {
			return getRuleContexts(DeclStmtContext.class);
		}
		public DeclStmtContext declStmt(int i) {
			return getRuleContext(DeclStmtContext.class,i);
		}
		public FieldLstContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_fieldLst; }
	}

	public final FieldLstContext fieldLst() throws RecognitionException {
		FieldLstContext _localctx = new FieldLstContext(_ctx, getState());
		enterRule(_localctx, 20, RULE_fieldLst);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(172);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while ((((_la) & ~0x3f) == 0 && ((1L << _la) & ((1L << TYPE_DOUBLE) | (1L << TYPE_INT) | (1L << TYPE_STRING) | (1L << TYPE_BOOL) | (1L << TYPE_DYN) | (1L << CONST))) != 0)) {
				{
				{
				setState(169);
				declStmt();
				}
				}
				setState(174);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class ParamLstDefContext extends ParserRuleContext {
		public List<DeclStmtContext> declStmt() {
			return getRuleContexts(DeclStmtContext.class);
		}
		public DeclStmtContext declStmt(int i) {
			return getRuleContext(DeclStmtContext.class,i);
		}
		public List<AssignmentContext> assignment() {
			return getRuleContexts(AssignmentContext.class);
		}
		public AssignmentContext assignment(int i) {
			return getRuleContext(AssignmentContext.class,i);
		}
		public List<TerminalNode> COMMA() { return getTokens(SpiceParser.COMMA); }
		public TerminalNode COMMA(int i) {
			return getToken(SpiceParser.COMMA, i);
		}
		public ParamLstDefContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_paramLstDef; }
	}

	public final ParamLstDefContext paramLstDef() throws RecognitionException {
		ParamLstDefContext _localctx = new ParamLstDefContext(_ctx, getState());
		enterRule(_localctx, 22, RULE_paramLstDef);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(177);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,10,_ctx) ) {
			case 1:
				{
				setState(175);
				declStmt();
				}
				break;
			case 2:
				{
				setState(176);
				assignment();
				}
				break;
			}
			setState(186);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==COMMA) {
				{
				{
				setState(179);
				match(COMMA);
				setState(182);
				_errHandler.sync(this);
				switch ( getInterpreter().adaptivePredict(_input,11,_ctx) ) {
				case 1:
					{
					setState(180);
					declStmt();
					}
					break;
				case 2:
					{
					setState(181);
					assignment();
					}
					break;
				}
				}
				}
				setState(188);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class ParamLstCallContext extends ParserRuleContext {
		public List<AssignmentContext> assignment() {
			return getRuleContexts(AssignmentContext.class);
		}
		public AssignmentContext assignment(int i) {
			return getRuleContext(AssignmentContext.class,i);
		}
		public List<TerminalNode> COMMA() { return getTokens(SpiceParser.COMMA); }
		public TerminalNode COMMA(int i) {
			return getToken(SpiceParser.COMMA, i);
		}
		public ParamLstCallContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_paramLstCall; }
	}

	public final ParamLstCallContext paramLstCall() throws RecognitionException {
		ParamLstCallContext _localctx = new ParamLstCallContext(_ctx, getState());
		enterRule(_localctx, 24, RULE_paramLstCall);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(189);
			assignment();
			setState(194);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==COMMA) {
				{
				{
				setState(190);
				match(COMMA);
				setState(191);
				assignment();
				}
				}
				setState(196);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class StmtContext extends ParserRuleContext {
		public TerminalNode SEMICOLON() { return getToken(SpiceParser.SEMICOLON, 0); }
		public DeclStmtContext declStmt() {
			return getRuleContext(DeclStmtContext.class,0);
		}
		public AssignmentContext assignment() {
			return getRuleContext(AssignmentContext.class,0);
		}
		public FunctionCallContext functionCall() {
			return getRuleContext(FunctionCallContext.class,0);
		}
		public ImportStmtContext importStmt() {
			return getRuleContext(ImportStmtContext.class,0);
		}
		public ReturnStmtContext returnStmt() {
			return getRuleContext(ReturnStmtContext.class,0);
		}
		public BreakStmtContext breakStmt() {
			return getRuleContext(BreakStmtContext.class,0);
		}
		public ContinueStmtContext continueStmt() {
			return getRuleContext(ContinueStmtContext.class,0);
		}
		public PrintfStmtContext printfStmt() {
			return getRuleContext(PrintfStmtContext.class,0);
		}
		public StmtContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_stmt; }
	}

	public final StmtContext stmt() throws RecognitionException {
		StmtContext _localctx = new StmtContext(_ctx, getState());
		enterRule(_localctx, 26, RULE_stmt);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(205);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,14,_ctx) ) {
			case 1:
				{
				setState(197);
				declStmt();
				}
				break;
			case 2:
				{
				setState(198);
				assignment();
				}
				break;
			case 3:
				{
				setState(199);
				functionCall();
				}
				break;
			case 4:
				{
				setState(200);
				importStmt();
				}
				break;
			case 5:
				{
				setState(201);
				returnStmt();
				}
				break;
			case 6:
				{
				setState(202);
				breakStmt();
				}
				break;
			case 7:
				{
				setState(203);
				continueStmt();
				}
				break;
			case 8:
				{
				setState(204);
				printfStmt();
				}
				break;
			}
			setState(207);
			match(SEMICOLON);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class DeclStmtContext extends ParserRuleContext {
		public DataTypeContext dataType() {
			return getRuleContext(DataTypeContext.class,0);
		}
		public TerminalNode IDENTIFIER() { return getToken(SpiceParser.IDENTIFIER, 0); }
		public TerminalNode CONST() { return getToken(SpiceParser.CONST, 0); }
		public DeclStmtContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_declStmt; }
	}

	public final DeclStmtContext declStmt() throws RecognitionException {
		DeclStmtContext _localctx = new DeclStmtContext(_ctx, getState());
		enterRule(_localctx, 28, RULE_declStmt);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(210);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==CONST) {
				{
				setState(209);
				match(CONST);
				}
			}

			setState(212);
			dataType();
			setState(213);
			match(IDENTIFIER);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class FunctionCallContext extends ParserRuleContext {
		public List<TerminalNode> IDENTIFIER() { return getTokens(SpiceParser.IDENTIFIER); }
		public TerminalNode IDENTIFIER(int i) {
			return getToken(SpiceParser.IDENTIFIER, i);
		}
		public TerminalNode LPAREN() { return getToken(SpiceParser.LPAREN, 0); }
		public TerminalNode RPAREN() { return getToken(SpiceParser.RPAREN, 0); }
		public List<TerminalNode> DOT() { return getTokens(SpiceParser.DOT); }
		public TerminalNode DOT(int i) {
			return getToken(SpiceParser.DOT, i);
		}
		public ParamLstCallContext paramLstCall() {
			return getRuleContext(ParamLstCallContext.class,0);
		}
		public FunctionCallContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_functionCall; }
	}

	public final FunctionCallContext functionCall() throws RecognitionException {
		FunctionCallContext _localctx = new FunctionCallContext(_ctx, getState());
		enterRule(_localctx, 30, RULE_functionCall);
		int _la;
		try {
			int _alt;
			enterOuterAlt(_localctx, 1);
			{
			setState(219);
			_errHandler.sync(this);
			_alt = getInterpreter().adaptivePredict(_input,16,_ctx);
			while ( _alt!=2 && _alt!=org.antlr.v4.runtime.atn.ATN.INVALID_ALT_NUMBER ) {
				if ( _alt==1 ) {
					{
					{
					setState(215);
					match(IDENTIFIER);
					setState(216);
					match(DOT);
					}
					} 
				}
				setState(221);
				_errHandler.sync(this);
				_alt = getInterpreter().adaptivePredict(_input,16,_ctx);
			}
			setState(222);
			match(IDENTIFIER);
			setState(223);
			match(LPAREN);
			setState(225);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if ((((_la) & ~0x3f) == 0 && ((1L << _la) & ((1L << TYPE_DOUBLE) | (1L << TYPE_INT) | (1L << TYPE_STRING) | (1L << TYPE_BOOL) | (1L << TYPE_DYN) | (1L << CONST) | (1L << TRUE) | (1L << FALSE) | (1L << STRING) | (1L << INTEGER) | (1L << DOUBLE) | (1L << IDENTIFIER) | (1L << LPAREN) | (1L << NOT) | (1L << PLUS_PLUS) | (1L << MINUS_MINUS))) != 0)) {
				{
				setState(224);
				paramLstCall();
				}
			}

			setState(227);
			match(RPAREN);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class ImportStmtContext extends ParserRuleContext {
		public TerminalNode IMPORT() { return getToken(SpiceParser.IMPORT, 0); }
		public TerminalNode STRING() { return getToken(SpiceParser.STRING, 0); }
		public TerminalNode AS() { return getToken(SpiceParser.AS, 0); }
		public TerminalNode IDENTIFIER() { return getToken(SpiceParser.IDENTIFIER, 0); }
		public ImportStmtContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_importStmt; }
	}

	public final ImportStmtContext importStmt() throws RecognitionException {
		ImportStmtContext _localctx = new ImportStmtContext(_ctx, getState());
		enterRule(_localctx, 32, RULE_importStmt);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(229);
			match(IMPORT);
			setState(230);
			match(STRING);
			setState(231);
			match(AS);
			setState(232);
			match(IDENTIFIER);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class ReturnStmtContext extends ParserRuleContext {
		public TerminalNode RETURN() { return getToken(SpiceParser.RETURN, 0); }
		public AssignmentContext assignment() {
			return getRuleContext(AssignmentContext.class,0);
		}
		public ReturnStmtContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_returnStmt; }
	}

	public final ReturnStmtContext returnStmt() throws RecognitionException {
		ReturnStmtContext _localctx = new ReturnStmtContext(_ctx, getState());
		enterRule(_localctx, 34, RULE_returnStmt);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(234);
			match(RETURN);
			setState(235);
			assignment();
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class BreakStmtContext extends ParserRuleContext {
		public TerminalNode BREAK() { return getToken(SpiceParser.BREAK, 0); }
		public TerminalNode INTEGER() { return getToken(SpiceParser.INTEGER, 0); }
		public BreakStmtContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_breakStmt; }
	}

	public final BreakStmtContext breakStmt() throws RecognitionException {
		BreakStmtContext _localctx = new BreakStmtContext(_ctx, getState());
		enterRule(_localctx, 36, RULE_breakStmt);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(237);
			match(BREAK);
			setState(239);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==INTEGER) {
				{
				setState(238);
				match(INTEGER);
				}
			}

			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class ContinueStmtContext extends ParserRuleContext {
		public TerminalNode CONTINUE() { return getToken(SpiceParser.CONTINUE, 0); }
		public TerminalNode INTEGER() { return getToken(SpiceParser.INTEGER, 0); }
		public ContinueStmtContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_continueStmt; }
	}

	public final ContinueStmtContext continueStmt() throws RecognitionException {
		ContinueStmtContext _localctx = new ContinueStmtContext(_ctx, getState());
		enterRule(_localctx, 38, RULE_continueStmt);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(241);
			match(CONTINUE);
			setState(243);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==INTEGER) {
				{
				setState(242);
				match(INTEGER);
				}
			}

			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class PrintfStmtContext extends ParserRuleContext {
		public TerminalNode PRINTF() { return getToken(SpiceParser.PRINTF, 0); }
		public TerminalNode LPAREN() { return getToken(SpiceParser.LPAREN, 0); }
		public TerminalNode STRING() { return getToken(SpiceParser.STRING, 0); }
		public TerminalNode RPAREN() { return getToken(SpiceParser.RPAREN, 0); }
		public List<TerminalNode> COMMA() { return getTokens(SpiceParser.COMMA); }
		public TerminalNode COMMA(int i) {
			return getToken(SpiceParser.COMMA, i);
		}
		public List<AssignmentContext> assignment() {
			return getRuleContexts(AssignmentContext.class);
		}
		public AssignmentContext assignment(int i) {
			return getRuleContext(AssignmentContext.class,i);
		}
		public PrintfStmtContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_printfStmt; }
	}

	public final PrintfStmtContext printfStmt() throws RecognitionException {
		PrintfStmtContext _localctx = new PrintfStmtContext(_ctx, getState());
		enterRule(_localctx, 40, RULE_printfStmt);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(245);
			match(PRINTF);
			setState(246);
			match(LPAREN);
			setState(247);
			match(STRING);
			setState(252);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==COMMA) {
				{
				{
				setState(248);
				match(COMMA);
				setState(249);
				assignment();
				}
				}
				setState(254);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			setState(255);
			match(RPAREN);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class AssignmentContext extends ParserRuleContext {
		public TernaryContext ternary() {
			return getRuleContext(TernaryContext.class,0);
		}
		public TerminalNode ASSIGN_OP() { return getToken(SpiceParser.ASSIGN_OP, 0); }
		public TerminalNode PLUS_EQUAL() { return getToken(SpiceParser.PLUS_EQUAL, 0); }
		public TerminalNode MINUS_EQUAL() { return getToken(SpiceParser.MINUS_EQUAL, 0); }
		public TerminalNode MUL_EQUAL() { return getToken(SpiceParser.MUL_EQUAL, 0); }
		public TerminalNode DIV_EQUAL() { return getToken(SpiceParser.DIV_EQUAL, 0); }
		public DeclStmtContext declStmt() {
			return getRuleContext(DeclStmtContext.class,0);
		}
		public List<TerminalNode> IDENTIFIER() { return getTokens(SpiceParser.IDENTIFIER); }
		public TerminalNode IDENTIFIER(int i) {
			return getToken(SpiceParser.IDENTIFIER, i);
		}
		public List<TerminalNode> DOT() { return getTokens(SpiceParser.DOT); }
		public TerminalNode DOT(int i) {
			return getToken(SpiceParser.DOT, i);
		}
		public AssignmentContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_assignment; }
	}

	public final AssignmentContext assignment() throws RecognitionException {
		AssignmentContext _localctx = new AssignmentContext(_ctx, getState());
		enterRule(_localctx, 42, RULE_assignment);
		int _la;
		try {
			int _alt;
			enterOuterAlt(_localctx, 1);
			{
			setState(269);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,23,_ctx) ) {
			case 1:
				{
				setState(266);
				_errHandler.sync(this);
				switch (_input.LA(1)) {
				case TYPE_DOUBLE:
				case TYPE_INT:
				case TYPE_STRING:
				case TYPE_BOOL:
				case TYPE_DYN:
				case CONST:
					{
					setState(257);
					declStmt();
					}
					break;
				case IDENTIFIER:
					{
					setState(262);
					_errHandler.sync(this);
					_alt = getInterpreter().adaptivePredict(_input,21,_ctx);
					while ( _alt!=2 && _alt!=org.antlr.v4.runtime.atn.ATN.INVALID_ALT_NUMBER ) {
						if ( _alt==1 ) {
							{
							{
							setState(258);
							match(IDENTIFIER);
							setState(259);
							match(DOT);
							}
							} 
						}
						setState(264);
						_errHandler.sync(this);
						_alt = getInterpreter().adaptivePredict(_input,21,_ctx);
					}
					setState(265);
					match(IDENTIFIER);
					}
					break;
				default:
					throw new NoViableAltException(this);
				}
				setState(268);
				_la = _input.LA(1);
				if ( !((((_la) & ~0x3f) == 0 && ((1L << _la) & ((1L << PLUS_EQUAL) | (1L << MINUS_EQUAL) | (1L << MUL_EQUAL) | (1L << DIV_EQUAL) | (1L << ASSIGN_OP))) != 0)) ) {
				_errHandler.recoverInline(this);
				}
				else {
					if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
					_errHandler.reportMatch(this);
					consume();
				}
				}
				break;
			}
			setState(271);
			ternary();
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class TernaryContext extends ParserRuleContext {
		public List<LogicalOrExprContext> logicalOrExpr() {
			return getRuleContexts(LogicalOrExprContext.class);
		}
		public LogicalOrExprContext logicalOrExpr(int i) {
			return getRuleContext(LogicalOrExprContext.class,i);
		}
		public TerminalNode QUESTION_MARK() { return getToken(SpiceParser.QUESTION_MARK, 0); }
		public TerminalNode COLON() { return getToken(SpiceParser.COLON, 0); }
		public TernaryContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_ternary; }
	}

	public final TernaryContext ternary() throws RecognitionException {
		TernaryContext _localctx = new TernaryContext(_ctx, getState());
		enterRule(_localctx, 44, RULE_ternary);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(273);
			logicalOrExpr();
			setState(279);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==QUESTION_MARK) {
				{
				setState(274);
				match(QUESTION_MARK);
				setState(275);
				logicalOrExpr();
				setState(276);
				match(COLON);
				setState(277);
				logicalOrExpr();
				}
			}

			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class LogicalOrExprContext extends ParserRuleContext {
		public List<LogicalAndExprContext> logicalAndExpr() {
			return getRuleContexts(LogicalAndExprContext.class);
		}
		public LogicalAndExprContext logicalAndExpr(int i) {
			return getRuleContext(LogicalAndExprContext.class,i);
		}
		public List<TerminalNode> LOGICAL_OR() { return getTokens(SpiceParser.LOGICAL_OR); }
		public TerminalNode LOGICAL_OR(int i) {
			return getToken(SpiceParser.LOGICAL_OR, i);
		}
		public LogicalOrExprContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_logicalOrExpr; }
	}

	public final LogicalOrExprContext logicalOrExpr() throws RecognitionException {
		LogicalOrExprContext _localctx = new LogicalOrExprContext(_ctx, getState());
		enterRule(_localctx, 46, RULE_logicalOrExpr);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(281);
			logicalAndExpr();
			setState(286);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==LOGICAL_OR) {
				{
				{
				setState(282);
				match(LOGICAL_OR);
				setState(283);
				logicalAndExpr();
				}
				}
				setState(288);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class LogicalAndExprContext extends ParserRuleContext {
		public List<BitwiseOrExprContext> bitwiseOrExpr() {
			return getRuleContexts(BitwiseOrExprContext.class);
		}
		public BitwiseOrExprContext bitwiseOrExpr(int i) {
			return getRuleContext(BitwiseOrExprContext.class,i);
		}
		public List<TerminalNode> LOGICAL_AND() { return getTokens(SpiceParser.LOGICAL_AND); }
		public TerminalNode LOGICAL_AND(int i) {
			return getToken(SpiceParser.LOGICAL_AND, i);
		}
		public LogicalAndExprContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_logicalAndExpr; }
	}

	public final LogicalAndExprContext logicalAndExpr() throws RecognitionException {
		LogicalAndExprContext _localctx = new LogicalAndExprContext(_ctx, getState());
		enterRule(_localctx, 48, RULE_logicalAndExpr);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(289);
			bitwiseOrExpr();
			setState(294);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==LOGICAL_AND) {
				{
				{
				setState(290);
				match(LOGICAL_AND);
				setState(291);
				bitwiseOrExpr();
				}
				}
				setState(296);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class BitwiseOrExprContext extends ParserRuleContext {
		public List<BitwiseAndExprContext> bitwiseAndExpr() {
			return getRuleContexts(BitwiseAndExprContext.class);
		}
		public BitwiseAndExprContext bitwiseAndExpr(int i) {
			return getRuleContext(BitwiseAndExprContext.class,i);
		}
		public List<TerminalNode> BITWISE_OR() { return getTokens(SpiceParser.BITWISE_OR); }
		public TerminalNode BITWISE_OR(int i) {
			return getToken(SpiceParser.BITWISE_OR, i);
		}
		public BitwiseOrExprContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_bitwiseOrExpr; }
	}

	public final BitwiseOrExprContext bitwiseOrExpr() throws RecognitionException {
		BitwiseOrExprContext _localctx = new BitwiseOrExprContext(_ctx, getState());
		enterRule(_localctx, 50, RULE_bitwiseOrExpr);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(297);
			bitwiseAndExpr();
			setState(302);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==BITWISE_OR) {
				{
				{
				setState(298);
				match(BITWISE_OR);
				setState(299);
				bitwiseAndExpr();
				}
				}
				setState(304);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class BitwiseAndExprContext extends ParserRuleContext {
		public List<EqualityExprContext> equalityExpr() {
			return getRuleContexts(EqualityExprContext.class);
		}
		public EqualityExprContext equalityExpr(int i) {
			return getRuleContext(EqualityExprContext.class,i);
		}
		public List<TerminalNode> BITWISE_AND() { return getTokens(SpiceParser.BITWISE_AND); }
		public TerminalNode BITWISE_AND(int i) {
			return getToken(SpiceParser.BITWISE_AND, i);
		}
		public BitwiseAndExprContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_bitwiseAndExpr; }
	}

	public final BitwiseAndExprContext bitwiseAndExpr() throws RecognitionException {
		BitwiseAndExprContext _localctx = new BitwiseAndExprContext(_ctx, getState());
		enterRule(_localctx, 52, RULE_bitwiseAndExpr);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(305);
			equalityExpr();
			setState(310);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==BITWISE_AND) {
				{
				{
				setState(306);
				match(BITWISE_AND);
				setState(307);
				equalityExpr();
				}
				}
				setState(312);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class EqualityExprContext extends ParserRuleContext {
		public List<RelationalExprContext> relationalExpr() {
			return getRuleContexts(RelationalExprContext.class);
		}
		public RelationalExprContext relationalExpr(int i) {
			return getRuleContext(RelationalExprContext.class,i);
		}
		public TerminalNode EQUAL() { return getToken(SpiceParser.EQUAL, 0); }
		public TerminalNode NOT_EQUAL() { return getToken(SpiceParser.NOT_EQUAL, 0); }
		public EqualityExprContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_equalityExpr; }
	}

	public final EqualityExprContext equalityExpr() throws RecognitionException {
		EqualityExprContext _localctx = new EqualityExprContext(_ctx, getState());
		enterRule(_localctx, 54, RULE_equalityExpr);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(313);
			relationalExpr();
			setState(316);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==EQUAL || _la==NOT_EQUAL) {
				{
				setState(314);
				_la = _input.LA(1);
				if ( !(_la==EQUAL || _la==NOT_EQUAL) ) {
				_errHandler.recoverInline(this);
				}
				else {
					if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
					_errHandler.reportMatch(this);
					consume();
				}
				setState(315);
				relationalExpr();
				}
			}

			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class RelationalExprContext extends ParserRuleContext {
		public List<AdditiveExprContext> additiveExpr() {
			return getRuleContexts(AdditiveExprContext.class);
		}
		public AdditiveExprContext additiveExpr(int i) {
			return getRuleContext(AdditiveExprContext.class,i);
		}
		public TerminalNode LESS() { return getToken(SpiceParser.LESS, 0); }
		public TerminalNode GREATER() { return getToken(SpiceParser.GREATER, 0); }
		public TerminalNode LESS_EQUAL() { return getToken(SpiceParser.LESS_EQUAL, 0); }
		public TerminalNode GREATER_EQUAL() { return getToken(SpiceParser.GREATER_EQUAL, 0); }
		public RelationalExprContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_relationalExpr; }
	}

	public final RelationalExprContext relationalExpr() throws RecognitionException {
		RelationalExprContext _localctx = new RelationalExprContext(_ctx, getState());
		enterRule(_localctx, 56, RULE_relationalExpr);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(318);
			additiveExpr();
			setState(321);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if ((((_la) & ~0x3f) == 0 && ((1L << _la) & ((1L << GREATER) | (1L << LESS) | (1L << GREATER_EQUAL) | (1L << LESS_EQUAL))) != 0)) {
				{
				setState(319);
				_la = _input.LA(1);
				if ( !((((_la) & ~0x3f) == 0 && ((1L << _la) & ((1L << GREATER) | (1L << LESS) | (1L << GREATER_EQUAL) | (1L << LESS_EQUAL))) != 0)) ) {
				_errHandler.recoverInline(this);
				}
				else {
					if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
					_errHandler.reportMatch(this);
					consume();
				}
				setState(320);
				additiveExpr();
				}
			}

			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class AdditiveExprContext extends ParserRuleContext {
		public List<MultiplicativeExprContext> multiplicativeExpr() {
			return getRuleContexts(MultiplicativeExprContext.class);
		}
		public MultiplicativeExprContext multiplicativeExpr(int i) {
			return getRuleContext(MultiplicativeExprContext.class,i);
		}
		public List<TerminalNode> PLUS() { return getTokens(SpiceParser.PLUS); }
		public TerminalNode PLUS(int i) {
			return getToken(SpiceParser.PLUS, i);
		}
		public List<TerminalNode> MINUS() { return getTokens(SpiceParser.MINUS); }
		public TerminalNode MINUS(int i) {
			return getToken(SpiceParser.MINUS, i);
		}
		public AdditiveExprContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_additiveExpr; }
	}

	public final AdditiveExprContext additiveExpr() throws RecognitionException {
		AdditiveExprContext _localctx = new AdditiveExprContext(_ctx, getState());
		enterRule(_localctx, 58, RULE_additiveExpr);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(323);
			multiplicativeExpr();
			setState(328);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==PLUS || _la==MINUS) {
				{
				{
				setState(324);
				_la = _input.LA(1);
				if ( !(_la==PLUS || _la==MINUS) ) {
				_errHandler.recoverInline(this);
				}
				else {
					if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
					_errHandler.reportMatch(this);
					consume();
				}
				setState(325);
				multiplicativeExpr();
				}
				}
				setState(330);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class MultiplicativeExprContext extends ParserRuleContext {
		public List<PrefixUnaryContext> prefixUnary() {
			return getRuleContexts(PrefixUnaryContext.class);
		}
		public PrefixUnaryContext prefixUnary(int i) {
			return getRuleContext(PrefixUnaryContext.class,i);
		}
		public List<TerminalNode> MUL() { return getTokens(SpiceParser.MUL); }
		public TerminalNode MUL(int i) {
			return getToken(SpiceParser.MUL, i);
		}
		public List<TerminalNode> DIV() { return getTokens(SpiceParser.DIV); }
		public TerminalNode DIV(int i) {
			return getToken(SpiceParser.DIV, i);
		}
		public MultiplicativeExprContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_multiplicativeExpr; }
	}

	public final MultiplicativeExprContext multiplicativeExpr() throws RecognitionException {
		MultiplicativeExprContext _localctx = new MultiplicativeExprContext(_ctx, getState());
		enterRule(_localctx, 60, RULE_multiplicativeExpr);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(331);
			prefixUnary();
			setState(336);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==MUL || _la==DIV) {
				{
				{
				setState(332);
				_la = _input.LA(1);
				if ( !(_la==MUL || _la==DIV) ) {
				_errHandler.recoverInline(this);
				}
				else {
					if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
					_errHandler.reportMatch(this);
					consume();
				}
				setState(333);
				prefixUnary();
				}
				}
				setState(338);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class PrefixUnaryContext extends ParserRuleContext {
		public PostfixUnaryContext postfixUnary() {
			return getRuleContext(PostfixUnaryContext.class,0);
		}
		public TerminalNode NOT() { return getToken(SpiceParser.NOT, 0); }
		public TerminalNode PLUS_PLUS() { return getToken(SpiceParser.PLUS_PLUS, 0); }
		public TerminalNode MINUS_MINUS() { return getToken(SpiceParser.MINUS_MINUS, 0); }
		public PrefixUnaryContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_prefixUnary; }
	}

	public final PrefixUnaryContext prefixUnary() throws RecognitionException {
		PrefixUnaryContext _localctx = new PrefixUnaryContext(_ctx, getState());
		enterRule(_localctx, 62, RULE_prefixUnary);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(340);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if ((((_la) & ~0x3f) == 0 && ((1L << _la) & ((1L << NOT) | (1L << PLUS_PLUS) | (1L << MINUS_MINUS))) != 0)) {
				{
				setState(339);
				_la = _input.LA(1);
				if ( !((((_la) & ~0x3f) == 0 && ((1L << _la) & ((1L << NOT) | (1L << PLUS_PLUS) | (1L << MINUS_MINUS))) != 0)) ) {
				_errHandler.recoverInline(this);
				}
				else {
					if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
					_errHandler.reportMatch(this);
					consume();
				}
				}
			}

			setState(342);
			postfixUnary();
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class PostfixUnaryContext extends ParserRuleContext {
		public AtomicExprContext atomicExpr() {
			return getRuleContext(AtomicExprContext.class,0);
		}
		public TerminalNode PLUS_PLUS() { return getToken(SpiceParser.PLUS_PLUS, 0); }
		public TerminalNode MINUS_MINUS() { return getToken(SpiceParser.MINUS_MINUS, 0); }
		public PostfixUnaryContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_postfixUnary; }
	}

	public final PostfixUnaryContext postfixUnary() throws RecognitionException {
		PostfixUnaryContext _localctx = new PostfixUnaryContext(_ctx, getState());
		enterRule(_localctx, 64, RULE_postfixUnary);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(344);
			atomicExpr();
			setState(346);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==PLUS_PLUS || _la==MINUS_MINUS) {
				{
				setState(345);
				_la = _input.LA(1);
				if ( !(_la==PLUS_PLUS || _la==MINUS_MINUS) ) {
				_errHandler.recoverInline(this);
				}
				else {
					if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
					_errHandler.reportMatch(this);
					consume();
				}
				}
			}

			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class AtomicExprContext extends ParserRuleContext {
		public ValueContext value() {
			return getRuleContext(ValueContext.class,0);
		}
		public TerminalNode LPAREN() { return getToken(SpiceParser.LPAREN, 0); }
		public AssignmentContext assignment() {
			return getRuleContext(AssignmentContext.class,0);
		}
		public TerminalNode RPAREN() { return getToken(SpiceParser.RPAREN, 0); }
		public AtomicExprContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_atomicExpr; }
	}

	public final AtomicExprContext atomicExpr() throws RecognitionException {
		AtomicExprContext _localctx = new AtomicExprContext(_ctx, getState());
		enterRule(_localctx, 66, RULE_atomicExpr);
		try {
			setState(353);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case TRUE:
			case FALSE:
			case STRING:
			case INTEGER:
			case DOUBLE:
			case IDENTIFIER:
				enterOuterAlt(_localctx, 1);
				{
				setState(348);
				value();
				}
				break;
			case LPAREN:
				enterOuterAlt(_localctx, 2);
				{
				setState(349);
				match(LPAREN);
				setState(350);
				assignment();
				setState(351);
				match(RPAREN);
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class ValueContext extends ParserRuleContext {
		public TerminalNode STRING() { return getToken(SpiceParser.STRING, 0); }
		public TerminalNode TRUE() { return getToken(SpiceParser.TRUE, 0); }
		public TerminalNode FALSE() { return getToken(SpiceParser.FALSE, 0); }
		public TerminalNode INTEGER() { return getToken(SpiceParser.INTEGER, 0); }
		public TerminalNode DOUBLE() { return getToken(SpiceParser.DOUBLE, 0); }
		public TerminalNode IDENTIFIER() { return getToken(SpiceParser.IDENTIFIER, 0); }
		public FunctionCallContext functionCall() {
			return getRuleContext(FunctionCallContext.class,0);
		}
		public ValueContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_value; }
	}

	public final ValueContext value() throws RecognitionException {
		ValueContext _localctx = new ValueContext(_ctx, getState());
		enterRule(_localctx, 68, RULE_value);
		try {
			setState(362);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,36,_ctx) ) {
			case 1:
				enterOuterAlt(_localctx, 1);
				{
				setState(355);
				match(STRING);
				}
				break;
			case 2:
				enterOuterAlt(_localctx, 2);
				{
				setState(356);
				match(TRUE);
				}
				break;
			case 3:
				enterOuterAlt(_localctx, 3);
				{
				setState(357);
				match(FALSE);
				}
				break;
			case 4:
				enterOuterAlt(_localctx, 4);
				{
				setState(358);
				match(INTEGER);
				}
				break;
			case 5:
				enterOuterAlt(_localctx, 5);
				{
				setState(359);
				match(DOUBLE);
				}
				break;
			case 6:
				enterOuterAlt(_localctx, 6);
				{
				setState(360);
				match(IDENTIFIER);
				}
				break;
			case 7:
				enterOuterAlt(_localctx, 7);
				{
				setState(361);
				functionCall();
				}
				break;
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class DataTypeContext extends ParserRuleContext {
		public TerminalNode TYPE_DOUBLE() { return getToken(SpiceParser.TYPE_DOUBLE, 0); }
		public TerminalNode TYPE_INT() { return getToken(SpiceParser.TYPE_INT, 0); }
		public TerminalNode TYPE_STRING() { return getToken(SpiceParser.TYPE_STRING, 0); }
		public TerminalNode TYPE_BOOL() { return getToken(SpiceParser.TYPE_BOOL, 0); }
		public TerminalNode TYPE_DYN() { return getToken(SpiceParser.TYPE_DYN, 0); }
		public DataTypeContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_dataType; }
	}

	public final DataTypeContext dataType() throws RecognitionException {
		DataTypeContext _localctx = new DataTypeContext(_ctx, getState());
		enterRule(_localctx, 70, RULE_dataType);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(364);
			_la = _input.LA(1);
			if ( !((((_la) & ~0x3f) == 0 && ((1L << _la) & ((1L << TYPE_DOUBLE) | (1L << TYPE_INT) | (1L << TYPE_STRING) | (1L << TYPE_BOOL) | (1L << TYPE_DYN))) != 0)) ) {
			_errHandler.recoverInline(this);
			}
			else {
				if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
				_errHandler.reportMatch(this);
				consume();
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static final String _serializedATN =
		"\3\u608b\ua72a\u8133\ub9ed\u417c\u3be7\u7786\u5964\3A\u0171\4\2\t\2\4"+
		"\3\t\3\4\4\t\4\4\5\t\5\4\6\t\6\4\7\t\7\4\b\t\b\4\t\t\t\4\n\t\n\4\13\t"+
		"\13\4\f\t\f\4\r\t\r\4\16\t\16\4\17\t\17\4\20\t\20\4\21\t\21\4\22\t\22"+
		"\4\23\t\23\4\24\t\24\4\25\t\25\4\26\t\26\4\27\t\27\4\30\t\30\4\31\t\31"+
		"\4\32\t\32\4\33\t\33\4\34\t\34\4\35\t\35\4\36\t\36\4\37\t\37\4 \t \4!"+
		"\t!\4\"\t\"\4#\t#\4$\t$\4%\t%\3\2\3\2\3\2\3\2\7\2O\n\2\f\2\16\2R\13\2"+
		"\3\3\3\3\3\3\3\3\3\3\3\3\3\3\5\3[\n\3\3\3\3\3\3\3\3\3\3\3\3\4\3\4\3\4"+
		"\3\4\3\4\3\4\3\4\5\4i\n\4\3\4\3\4\3\4\3\4\3\4\3\5\3\5\3\5\3\5\5\5t\n\5"+
		"\3\5\3\5\3\5\3\5\3\5\3\6\3\6\3\6\3\6\3\6\3\6\3\6\3\7\3\7\3\7\3\7\3\7\3"+
		"\7\3\7\3\7\3\7\3\7\3\b\3\b\3\b\3\b\3\b\3\b\3\t\3\t\3\t\3\t\3\t\3\t\5\t"+
		"\u0098\n\t\3\n\3\n\3\n\3\n\3\n\3\n\3\n\5\n\u00a1\n\n\3\13\3\13\3\13\3"+
		"\13\7\13\u00a7\n\13\f\13\16\13\u00aa\13\13\3\f\7\f\u00ad\n\f\f\f\16\f"+
		"\u00b0\13\f\3\r\3\r\5\r\u00b4\n\r\3\r\3\r\3\r\5\r\u00b9\n\r\7\r\u00bb"+
		"\n\r\f\r\16\r\u00be\13\r\3\16\3\16\3\16\7\16\u00c3\n\16\f\16\16\16\u00c6"+
		"\13\16\3\17\3\17\3\17\3\17\3\17\3\17\3\17\3\17\5\17\u00d0\n\17\3\17\3"+
		"\17\3\20\5\20\u00d5\n\20\3\20\3\20\3\20\3\21\3\21\7\21\u00dc\n\21\f\21"+
		"\16\21\u00df\13\21\3\21\3\21\3\21\5\21\u00e4\n\21\3\21\3\21\3\22\3\22"+
		"\3\22\3\22\3\22\3\23\3\23\3\23\3\24\3\24\5\24\u00f2\n\24\3\25\3\25\5\25"+
		"\u00f6\n\25\3\26\3\26\3\26\3\26\3\26\7\26\u00fd\n\26\f\26\16\26\u0100"+
		"\13\26\3\26\3\26\3\27\3\27\3\27\7\27\u0107\n\27\f\27\16\27\u010a\13\27"+
		"\3\27\5\27\u010d\n\27\3\27\5\27\u0110\n\27\3\27\3\27\3\30\3\30\3\30\3"+
		"\30\3\30\3\30\5\30\u011a\n\30\3\31\3\31\3\31\7\31\u011f\n\31\f\31\16\31"+
		"\u0122\13\31\3\32\3\32\3\32\7\32\u0127\n\32\f\32\16\32\u012a\13\32\3\33"+
		"\3\33\3\33\7\33\u012f\n\33\f\33\16\33\u0132\13\33\3\34\3\34\3\34\7\34"+
		"\u0137\n\34\f\34\16\34\u013a\13\34\3\35\3\35\3\35\5\35\u013f\n\35\3\36"+
		"\3\36\3\36\5\36\u0144\n\36\3\37\3\37\3\37\7\37\u0149\n\37\f\37\16\37\u014c"+
		"\13\37\3 \3 \3 \7 \u0151\n \f \16 \u0154\13 \3!\5!\u0157\n!\3!\3!\3\""+
		"\3\"\5\"\u015d\n\"\3#\3#\3#\3#\3#\5#\u0164\n#\3$\3$\3$\3$\3$\3$\3$\5$"+
		"\u016d\n$\3%\3%\3%\2\2&\2\4\6\b\n\f\16\20\22\24\26\30\32\34\36 \"$&(*"+
		",.\60\62\64\668:<>@BDFH\2\n\4\2+.99\3\2\678\3\2\63\66\3\2/\60\3\2\61\62"+
		"\3\2(*\3\2)*\3\2\3\7\2\u0180\2P\3\2\2\2\4S\3\2\2\2\6a\3\2\2\2\bo\3\2\2"+
		"\2\nz\3\2\2\2\f\u0081\3\2\2\2\16\u008b\3\2\2\2\20\u0091\3\2\2\2\22\u00a0"+
		"\3\2\2\2\24\u00a8\3\2\2\2\26\u00ae\3\2\2\2\30\u00b3\3\2\2\2\32\u00bf\3"+
		"\2\2\2\34\u00cf\3\2\2\2\36\u00d4\3\2\2\2 \u00dd\3\2\2\2\"\u00e7\3\2\2"+
		"\2$\u00ec\3\2\2\2&\u00ef\3\2\2\2(\u00f3\3\2\2\2*\u00f7\3\2\2\2,\u010f"+
		"\3\2\2\2.\u0113\3\2\2\2\60\u011b\3\2\2\2\62\u0123\3\2\2\2\64\u012b\3\2"+
		"\2\2\66\u0133\3\2\2\28\u013b\3\2\2\2:\u0140\3\2\2\2<\u0145\3\2\2\2>\u014d"+
		"\3\2\2\2@\u0156\3\2\2\2B\u015a\3\2\2\2D\u0163\3\2\2\2F\u016c\3\2\2\2H"+
		"\u016e\3\2\2\2JO\5\34\17\2KO\5\4\3\2LO\5\6\4\2MO\5\b\5\2NJ\3\2\2\2NK\3"+
		"\2\2\2NL\3\2\2\2NM\3\2\2\2OR\3\2\2\2PN\3\2\2\2PQ\3\2\2\2Q\3\3\2\2\2RP"+
		"\3\2\2\2ST\7\b\2\2TU\7\64\2\2UV\7\4\2\2VW\7\63\2\2WX\7\26\2\2XZ\7 \2\2"+
		"Y[\5\30\r\2ZY\3\2\2\2Z[\3\2\2\2[\\\3\2\2\2\\]\7!\2\2]^\7\36\2\2^_\5\24"+
		"\13\2_`\7\37\2\2`\5\3\2\2\2ab\7\b\2\2bc\7\64\2\2cd\5H%\2de\7\63\2\2ef"+
		"\7\35\2\2fh\7 \2\2gi\5\30\r\2hg\3\2\2\2hi\3\2\2\2ij\3\2\2\2jk\7!\2\2k"+
		"l\7\36\2\2lm\5\24\13\2mn\7\37\2\2n\7\3\2\2\2op\7\t\2\2pq\7\35\2\2qs\7"+
		" \2\2rt\5\30\r\2sr\3\2\2\2st\3\2\2\2tu\3\2\2\2uv\7!\2\2vw\7\36\2\2wx\5"+
		"\24\13\2xy\7\37\2\2y\t\3\2\2\2z{\7\25\2\2{|\7\35\2\2|}\7\24\2\2}~\7\36"+
		"\2\2~\177\5\26\f\2\177\u0080\7\37\2\2\u0080\13\3\2\2\2\u0081\u0082\7\f"+
		"\2\2\u0082\u0083\5,\27\2\u0083\u0084\7;\2\2\u0084\u0085\5,\27\2\u0085"+
		"\u0086\7;\2\2\u0086\u0087\5,\27\2\u0087\u0088\7\36\2\2\u0088\u0089\5\24"+
		"\13\2\u0089\u008a\7\37\2\2\u008a\r\3\2\2\2\u008b\u008c\7\r\2\2\u008c\u008d"+
		"\5,\27\2\u008d\u008e\7\36\2\2\u008e\u008f\5\24\13\2\u008f\u0090\7\37\2"+
		"\2\u0090\17\3\2\2\2\u0091\u0092\7\n\2\2\u0092\u0093\5,\27\2\u0093\u0094"+
		"\7\36\2\2\u0094\u0095\5\24\13\2\u0095\u0097\7\37\2\2\u0096\u0098\5\22"+
		"\n\2\u0097\u0096\3\2\2\2\u0097\u0098\3\2\2\2\u0098\21\3\2\2\2\u0099\u009a"+
		"\7\13\2\2\u009a\u00a1\5\20\t\2\u009b\u009c\7\13\2\2\u009c\u009d\7\36\2"+
		"\2\u009d\u009e\5\24\13\2\u009e\u009f\7\37\2\2\u009f\u00a1\3\2\2\2\u00a0"+
		"\u0099\3\2\2\2\u00a0\u009b\3\2\2\2\u00a1\23\3\2\2\2\u00a2\u00a7\5\34\17"+
		"\2\u00a3\u00a7\5\f\7\2\u00a4\u00a7\5\16\b\2\u00a5\u00a7\5\20\t\2\u00a6"+
		"\u00a2\3\2\2\2\u00a6\u00a3\3\2\2\2\u00a6\u00a4\3\2\2\2\u00a6\u00a5\3\2"+
		"\2\2\u00a7\u00aa\3\2\2\2\u00a8\u00a6\3\2\2\2\u00a8\u00a9\3\2\2\2\u00a9"+
		"\25\3\2\2\2\u00aa\u00a8\3\2\2\2\u00ab\u00ad\5\36\20\2\u00ac\u00ab\3\2"+
		"\2\2\u00ad\u00b0\3\2\2\2\u00ae\u00ac\3\2\2\2\u00ae\u00af\3\2\2\2\u00af"+
		"\27\3\2\2\2\u00b0\u00ae\3\2\2\2\u00b1\u00b4\5\36\20\2\u00b2\u00b4\5,\27"+
		"\2\u00b3\u00b1\3\2\2\2\u00b3\u00b2\3\2\2\2\u00b4\u00bc\3\2\2\2\u00b5\u00b8"+
		"\7=\2\2\u00b6\u00b9\5\36\20\2\u00b7\u00b9\5,\27\2\u00b8\u00b6\3\2\2\2"+
		"\u00b8\u00b7\3\2\2\2\u00b9\u00bb\3\2\2\2\u00ba\u00b5\3\2\2\2\u00bb\u00be"+
		"\3\2\2\2\u00bc\u00ba\3\2\2\2\u00bc\u00bd\3\2\2\2\u00bd\31\3\2\2\2\u00be"+
		"\u00bc\3\2\2\2\u00bf\u00c4\5,\27\2\u00c0\u00c1\7=\2\2\u00c1\u00c3\5,\27"+
		"\2\u00c2\u00c0\3\2\2\2\u00c3\u00c6\3\2\2\2\u00c4\u00c2\3\2\2\2\u00c4\u00c5"+
		"\3\2\2\2\u00c5\33\3\2\2\2\u00c6\u00c4\3\2\2\2\u00c7\u00d0\5\36\20\2\u00c8"+
		"\u00d0\5,\27\2\u00c9\u00d0\5 \21\2\u00ca\u00d0\5\"\22\2\u00cb\u00d0\5"+
		"$\23\2\u00cc\u00d0\5&\24\2\u00cd\u00d0\5(\25\2\u00ce\u00d0\5*\26\2\u00cf"+
		"\u00c7\3\2\2\2\u00cf\u00c8\3\2\2\2\u00cf\u00c9\3\2\2\2\u00cf\u00ca\3\2"+
		"\2\2\u00cf\u00cb\3\2\2\2\u00cf\u00cc\3\2\2\2\u00cf\u00cd\3\2\2\2\u00cf"+
		"\u00ce\3\2\2\2\u00d0\u00d1\3\2\2\2\u00d1\u00d2\7;\2\2\u00d2\35\3\2\2\2"+
		"\u00d3\u00d5\7\16\2\2\u00d4\u00d3\3\2\2\2\u00d4\u00d5\3\2\2\2\u00d5\u00d6"+
		"\3\2\2\2\u00d6\u00d7\5H%\2\u00d7\u00d8\7\35\2\2\u00d8\37\3\2\2\2\u00d9"+
		"\u00da\7\35\2\2\u00da\u00dc\7>\2\2\u00db\u00d9\3\2\2\2\u00dc\u00df\3\2"+
		"\2\2\u00dd\u00db\3\2\2\2\u00dd\u00de\3\2\2\2\u00de\u00e0\3\2\2\2\u00df"+
		"\u00dd\3\2\2\2\u00e0\u00e1\7\35\2\2\u00e1\u00e3\7 \2\2\u00e2\u00e4\5\32"+
		"\16\2\u00e3\u00e2\3\2\2\2\u00e3\u00e4\3\2\2\2\u00e4\u00e5\3\2\2\2\u00e5"+
		"\u00e6\7!\2\2\u00e6!\3\2\2\2\u00e7\u00e8\7\17\2\2\u00e8\u00e9\7\32\2\2"+
		"\u00e9\u00ea\7\23\2\2\u00ea\u00eb\7\35\2\2\u00eb#\3\2\2\2\u00ec\u00ed"+
		"\7\22\2\2\u00ed\u00ee\5,\27\2\u00ee%\3\2\2\2\u00ef\u00f1\7\20\2\2\u00f0"+
		"\u00f2\7\33\2\2\u00f1\u00f0\3\2\2\2\u00f1\u00f2\3\2\2\2\u00f2\'\3\2\2"+
		"\2\u00f3\u00f5\7\21\2\2\u00f4\u00f6\7\33\2\2\u00f5\u00f4\3\2\2\2\u00f5"+
		"\u00f6\3\2\2\2\u00f6)\3\2\2\2\u00f7\u00f8\7\27\2\2\u00f8\u00f9\7 \2\2"+
		"\u00f9\u00fe\7\32\2\2\u00fa\u00fb\7=\2\2\u00fb\u00fd\5,\27\2\u00fc\u00fa"+
		"\3\2\2\2\u00fd\u0100\3\2\2\2\u00fe\u00fc\3\2\2\2\u00fe\u00ff\3\2\2\2\u00ff"+
		"\u0101\3\2\2\2\u0100\u00fe\3\2\2\2\u0101\u0102\7!\2\2\u0102+\3\2\2\2\u0103"+
		"\u010d\5\36\20\2\u0104\u0105\7\35\2\2\u0105\u0107\7>\2\2\u0106\u0104\3"+
		"\2\2\2\u0107\u010a\3\2\2\2\u0108\u0106\3\2\2\2\u0108\u0109\3\2\2\2\u0109"+
		"\u010b\3\2\2\2\u010a\u0108\3\2\2\2\u010b\u010d\7\35\2\2\u010c\u0103\3"+
		"\2\2\2\u010c\u0108\3\2\2\2\u010d\u010e\3\2\2\2\u010e\u0110\t\2\2\2\u010f"+
		"\u010c\3\2\2\2\u010f\u0110\3\2\2\2\u0110\u0111\3\2\2\2\u0111\u0112\5."+
		"\30\2\u0112-\3\2\2\2\u0113\u0119\5\60\31\2\u0114\u0115\7:\2\2\u0115\u0116"+
		"\5\60\31\2\u0116\u0117\7<\2\2\u0117\u0118\5\60\31\2\u0118\u011a\3\2\2"+
		"\2\u0119\u0114\3\2\2\2\u0119\u011a\3\2\2\2\u011a/\3\2\2\2\u011b\u0120"+
		"\5\62\32\2\u011c\u011d\7$\2\2\u011d\u011f\5\62\32\2\u011e\u011c\3\2\2"+
		"\2\u011f\u0122\3\2\2\2\u0120\u011e\3\2\2\2\u0120\u0121\3\2\2\2\u0121\61"+
		"\3\2\2\2\u0122\u0120\3\2\2\2\u0123\u0128\5\64\33\2\u0124\u0125\7%\2\2"+
		"\u0125\u0127\5\64\33\2\u0126\u0124\3\2\2\2\u0127\u012a\3\2\2\2\u0128\u0126"+
		"\3\2\2\2\u0128\u0129\3\2\2\2\u0129\63\3\2\2\2\u012a\u0128\3\2\2\2\u012b"+
		"\u0130\5\66\34\2\u012c\u012d\7&\2\2\u012d\u012f\5\66\34\2\u012e\u012c"+
		"\3\2\2\2\u012f\u0132\3\2\2\2\u0130\u012e\3\2\2\2\u0130\u0131\3\2\2\2\u0131"+
		"\65\3\2\2\2\u0132\u0130\3\2\2\2\u0133\u0138\58\35\2\u0134\u0135\7\'\2"+
		"\2\u0135\u0137\58\35\2\u0136\u0134\3\2\2\2\u0137\u013a\3\2\2\2\u0138\u0136"+
		"\3\2\2\2\u0138\u0139\3\2\2\2\u0139\67\3\2\2\2\u013a\u0138\3\2\2\2\u013b"+
		"\u013e\5:\36\2\u013c\u013d\t\3\2\2\u013d\u013f\5:\36\2\u013e\u013c\3\2"+
		"\2\2\u013e\u013f\3\2\2\2\u013f9\3\2\2\2\u0140\u0143\5<\37\2\u0141\u0142"+
		"\t\4\2\2\u0142\u0144\5<\37\2\u0143\u0141\3\2\2\2\u0143\u0144\3\2\2\2\u0144"+
		";\3\2\2\2\u0145\u014a\5> \2\u0146\u0147\t\5\2\2\u0147\u0149\5> \2\u0148"+
		"\u0146\3\2\2\2\u0149\u014c\3\2\2\2\u014a\u0148\3\2\2\2\u014a\u014b\3\2"+
		"\2\2\u014b=\3\2\2\2\u014c\u014a\3\2\2\2\u014d\u0152\5@!\2\u014e\u014f"+
		"\t\6\2\2\u014f\u0151\5@!\2\u0150\u014e\3\2\2\2\u0151\u0154\3\2\2\2\u0152"+
		"\u0150\3\2\2\2\u0152\u0153\3\2\2\2\u0153?\3\2\2\2\u0154\u0152\3\2\2\2"+
		"\u0155\u0157\t\7\2\2\u0156\u0155\3\2\2\2\u0156\u0157\3\2\2\2\u0157\u0158"+
		"\3\2\2\2\u0158\u0159\5B\"\2\u0159A\3\2\2\2\u015a\u015c\5D#\2\u015b\u015d"+
		"\t\b\2\2\u015c\u015b\3\2\2\2\u015c\u015d\3\2\2\2\u015dC\3\2\2\2\u015e"+
		"\u0164\5F$\2\u015f\u0160\7 \2\2\u0160\u0161\5,\27\2\u0161\u0162\7!\2\2"+
		"\u0162\u0164\3\2\2\2\u0163\u015e\3\2\2\2\u0163\u015f\3\2\2\2\u0164E\3"+
		"\2\2\2\u0165\u016d\7\32\2\2\u0166\u016d\7\30\2\2\u0167\u016d\7\31\2\2"+
		"\u0168\u016d\7\33\2\2\u0169\u016d\7\34\2\2\u016a\u016d\7\35\2\2\u016b"+
		"\u016d\5 \21\2\u016c\u0165\3\2\2\2\u016c\u0166\3\2\2\2\u016c\u0167\3\2"+
		"\2\2\u016c\u0168\3\2\2\2\u016c\u0169\3\2\2\2\u016c\u016a\3\2\2\2\u016c"+
		"\u016b\3\2\2\2\u016dG\3\2\2\2\u016e\u016f\t\t\2\2\u016fI\3\2\2\2\'NPZ"+
		"hs\u0097\u00a0\u00a6\u00a8\u00ae\u00b3\u00b8\u00bc\u00c4\u00cf\u00d4\u00dd"+
		"\u00e3\u00f1\u00f5\u00fe\u0108\u010c\u010f\u0119\u0120\u0128\u0130\u0138"+
		"\u013e\u0143\u014a\u0152\u0156\u015c\u0163\u016c";
	public static final ATN _ATN =
		new ATNDeserializer().deserialize(_serializedATN.toCharArray());
	static {
		_decisionToDFA = new DFA[_ATN.getNumberOfDecisions()];
		for (int i = 0; i < _ATN.getNumberOfDecisions(); i++) {
			_decisionToDFA[i] = new DFA(_ATN.getDecisionState(i), i);
		}
	}
}