// A Bison parser, made by GNU Bison 3.3.1.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015, 2018-2019 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.

// Undocumented macros, especially those whose name start with YY_,
// are private implementation details.  Do not rely on them.





// //                    "%code requires" blocks.
#line 7 "D:/ProjectsVCS/MyProjects/SPIR-V-Blast/lang/blast.y" // lalr1.cc:431

#include <map>
#include <unordered_map>
#include <any>
#include <list>
#include <vector>
#include <string>
#include <memory>
#include <algorithm>

enum class IdentifierType
{
	Undefined,
	Attribute,
	Structure,
	Function,
	Parameter,
	Variable
};

enum class LiteralType
{
	String,
	UNumber,
	INumber,
	DNumber
};

enum class AttributeParamType
{
	None,
	Identifier,
	Literal,
};

enum class ExpressionType
{
	None,
	/*Basic*/
	Literal,
	Identifier,
	/*Ariphmetic*/
	Add,
	Negate,
	Multiply,
	Divide,
	Modulo,
	PreIncrement,
	PostIncrement,
	PreDecrement,
	PostDecrement,
	/*Logical*/
	Equal,
	NonEqual,
	And,
	Or,
	Less,
	More,
	LessOrEqual,
	MoreOrEqual,
	/*Control Switch - condition and loops */
	If,
	For,
	While,
	DoWhile,
	Switch,
	/*Control Switch - switch cases*/
	SwitchCase,
	SwitchDefault,
	/*Control Switch - common*/
	FunctionCall,
	Return,
	Break,
	Continue,
	/*References*/
	Reference, Dereference,
	/*Misc*/
	Comma,
};

struct Identifier
{
	IdentifierType type;
	std::string name;
	std::size_t id;
};

struct BaseVariable
{
	struct Type type;
	Identifier ident;
}

struct Attribute
{
	Identifier ident {};
	AttributeParamType type;
	Identifier paramIdent {};
	Literal paramLiteral {};
};

struct Attributable
{
	std::vector<Attribute> attributes;
}

struct Function : Attributable
{
	std::string name;
	struct Expression body;
};

struct FunctionParameter : BaseVariable
{
	std::size_t position; //position in parameters list
};

struct Struct : Attributable
{
	std::vector<struct StructMember> members;
};

struct StructMember : Attributable, BaseVariable
{
	std::size_t position; //member position in struct
};

struct Literal
{
	LiteralType type;
	union
	{
		uint64_t unum;
		int64_t inum;
		double dnum;
		std::string string;
	} literal;
};

typedef  std::list<struct Expression> ExpressionParams;

struct Expression
{
	ExpressionType type;
	Identifier ident {};
	Literal literal {};
	ExpressionParams params;
};

struct lexcontext;


#line 195 "D:/ProjectsVCS/MyProjects/SPIR-V-Blast/src/gen/blast_parser.hpp" // lalr1.cc:431


# include <cstdlib> // std::abort
# include <iostream>
# include <stdexcept>
# include <string>
# include <vector>

#if defined __cplusplus
# define YY_CPLUSPLUS __cplusplus
#else
# define YY_CPLUSPLUS 199711L
#endif

// Support move semantics when possible.
#if 201103L <= YY_CPLUSPLUS
# define YY_MOVE           std::move
# define YY_MOVE_OR_COPY   move
# define YY_MOVE_REF(Type) Type&&
# define YY_RVREF(Type)    Type&&
# define YY_COPY(Type)     Type
#else
# define YY_MOVE
# define YY_MOVE_OR_COPY   copy
# define YY_MOVE_REF(Type) Type&
# define YY_RVREF(Type)    const Type&
# define YY_COPY(Type)     const Type&
#endif

// Support noexcept when possible.
#if 201103L <= YY_CPLUSPLUS
# define YY_NOEXCEPT noexcept
# define YY_NOTHROW
#else
# define YY_NOEXCEPT
# define YY_NOTHROW throw ()
#endif

// Support constexpr when possible.
#if 201703 <= YY_CPLUSPLUS
# define YY_CONSTEXPR constexpr
#else
# define YY_CONSTEXPR
#endif



#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

#line 3 "D:/ProjectsVCS/MyProjects/SPIR-V-Blast/lang/blast.y" // lalr1.cc:431
namespace gen {
#line 306 "D:/ProjectsVCS/MyProjects/SPIR-V-Blast/src/gen/blast_parser.hpp" // lalr1.cc:431

  /// A point in a source file.
  class position
  {
  public:
    /// Construct a position.
    explicit position (std::string* f = YY_NULLPTR,
                       unsigned l = 1u,
                       unsigned c = 1u)
      : filename (f)
      , line (l)
      , column (c)
    {}


    /// Initialization.
    void initialize (std::string* fn = YY_NULLPTR,
                     unsigned l = 1u,
                     unsigned c = 1u)
    {
      filename = fn;
      line = l;
      column = c;
    }

    /** \name Line and Column related manipulators
     ** \{ */
    /// (line related) Advance to the COUNT next lines.
    void lines (int count = 1)
    {
      if (count)
        {
          column = 1u;
          line = add_ (line, count, 1);
        }
    }

    /// (column related) Advance to the COUNT next columns.
    void columns (int count = 1)
    {
      column = add_ (column, count, 1);
    }
    /** \} */

    /// File name to which this position refers.
    std::string* filename;
    /// Current line number.
    unsigned line;
    /// Current column number.
    unsigned column;

  private:
    /// Compute max (min, lhs+rhs).
    static unsigned add_ (unsigned lhs, int rhs, int min)
    {
      return static_cast<unsigned> (std::max (min,
                                              static_cast<int> (lhs) + rhs));
    }
  };

  /// Add \a width columns, in place.
  inline position&
  operator+= (position& res, int width)
  {
    res.columns (width);
    return res;
  }

  /// Add \a width columns.
  inline position
  operator+ (position res, int width)
  {
    return res += width;
  }

  /// Subtract \a width columns, in place.
  inline position&
  operator-= (position& res, int width)
  {
    return res += -width;
  }

  /// Subtract \a width columns.
  inline position
  operator- (position res, int width)
  {
    return res -= width;
  }

  /// Compare two position objects.
  inline bool
  operator== (const position& pos1, const position& pos2)
  {
    return (pos1.line == pos2.line
            && pos1.column == pos2.column
            && (pos1.filename == pos2.filename
                || (pos1.filename && pos2.filename
                    && *pos1.filename == *pos2.filename)));
  }

  /// Compare two position objects.
  inline bool
  operator!= (const position& pos1, const position& pos2)
  {
    return !(pos1 == pos2);
  }

  /** \brief Intercept output stream redirection.
   ** \param ostr the destination output stream
   ** \param pos a reference to the position to redirect
   */
  template <typename YYChar>
  std::basic_ostream<YYChar>&
  operator<< (std::basic_ostream<YYChar>& ostr, const position& pos)
  {
    if (pos.filename)
      ostr << *pos.filename << ':';
    return ostr << pos.line << '.' << pos.column;
  }

  /// Two points in a source file.
  class location
  {
  public:

    /// Construct a location from \a b to \a e.
    location (const position& b, const position& e)
      : begin (b)
      , end (e)
    {}

    /// Construct a 0-width location in \a p.
    explicit location (const position& p = position ())
      : begin (p)
      , end (p)
    {}

    /// Construct a 0-width location in \a f, \a l, \a c.
    explicit location (std::string* f,
                       unsigned l = 1u,
                       unsigned c = 1u)
      : begin (f, l, c)
      , end (f, l, c)
    {}


    /// Initialization.
    void initialize (std::string* f = YY_NULLPTR,
                     unsigned l = 1u,
                     unsigned c = 1u)
    {
      begin.initialize (f, l, c);
      end = begin;
    }

    /** \name Line and Column related manipulators
     ** \{ */
  public:
    /// Reset initial location to final location.
    void step ()
    {
      begin = end;
    }

    /// Extend the current location to the COUNT next columns.
    void columns (int count = 1)
    {
      end += count;
    }

    /// Extend the current location to the COUNT next lines.
    void lines (int count = 1)
    {
      end.lines (count);
    }
    /** \} */


  public:
    /// Beginning of the located region.
    position begin;
    /// End of the located region.
    position end;
  };

  /// Join two locations, in place.
  inline location& operator+= (location& res, const location& end)
  {
    res.end = end.end;
    return res;
  }

  /// Join two locations.
  inline location operator+ (location res, const location& end)
  {
    return res += end;
  }

  /// Add \a width columns to the end position, in place.
  inline location& operator+= (location& res, int width)
  {
    res.columns (width);
    return res;
  }

  /// Add \a width columns to the end position.
  inline location operator+ (location res, int width)
  {
    return res += width;
  }

  /// Subtract \a width columns to the end position, in place.
  inline location& operator-= (location& res, int width)
  {
    return res += -width;
  }

  /// Subtract \a width columns to the end position.
  inline location operator- (location res, int width)
  {
    return res -= width;
  }

  /// Compare two location objects.
  inline bool
  operator== (const location& loc1, const location& loc2)
  {
    return loc1.begin == loc2.begin && loc1.end == loc2.end;
  }

  /// Compare two location objects.
  inline bool
  operator!= (const location& loc1, const location& loc2)
  {
    return !(loc1 == loc2);
  }

  /** \brief Intercept output stream redirection.
   ** \param ostr the destination output stream
   ** \param loc a reference to the location to redirect
   **
   ** Avoid duplicate information.
   */
  template <typename YYChar>
  std::basic_ostream<YYChar>&
  operator<< (std::basic_ostream<YYChar>& ostr, const location& loc)
  {
    unsigned end_col = 0 < loc.end.column ? loc.end.column - 1 : 0;
    ostr << loc.begin;
    if (loc.end.filename
        && (!loc.begin.filename
            || *loc.begin.filename != *loc.end.filename))
      ostr << '-' << loc.end.filename << ':' << loc.end.line << '.' << end_col;
    else if (loc.begin.line < loc.end.line)
      ostr << '-' << loc.end.line << '.' << end_col;
    else if (loc.begin.column < end_col)
      ostr << '-' << end_col;
    return ostr;
  }


  /// A Bison parser.
  class BlastParser
  {
  public:
#ifndef YYSTYPE
    /// Symbol semantic values.
    typedef int semantic_type;
#else
    typedef YYSTYPE semantic_type;
#endif
    /// Symbol locations.
    typedef location location_type;

    /// Syntax errors thrown from user actions.
    struct syntax_error : std::runtime_error
    {
      syntax_error (const location_type& l, const std::string& m)
        : std::runtime_error (m)
        , location (l)
      {}

      syntax_error (const syntax_error& s)
        : std::runtime_error (s.what ())
        , location (s.location)
      {}

      ~syntax_error () YY_NOEXCEPT YY_NOTHROW;

      location_type location;
    };

    /// Tokens.
    struct token
    {
      enum yytokentype
      {
        END = 0,
        DO = 258,
        WHILE = 259,
        IF = 260,
        SWITCH = 261,
        FOR = 262,
        DEFAULT = 263,
        CASE = 264,
        RETURN = 265,
        BREAK = 266,
        CONTINUE = 267,
        CONST = 268,
        SPECCONST = 269,
        NOVAR = 270,
        RUNTIME = 271,
        VOID = 272,
        INT = 273,
        FLOAT = 274,
        BOOL = 275,
        MATRIX = 276,
        VECTOR = 277,
        IMAGE = 278,
        SAMPLER = 279,
        SAMPLED_IMAGE = 280,
        STRUCT = 281,
        IDENTIFIER = 282,
        USER_DEFINED_TYPE = 283,
        NUMLITERAL = 284,
        STRINGLITERAL = 285,
        OR = 286,
        AND = 287,
        EQ = 288,
        NE = 289,
        LESS = 290,
        MORE = 291,
        LESS_EQ = 292,
        MORE_EQ = 293,
        INC = 294,
        DEC = 295,
        PL_EQ = 296,
        MI_EQ = 297,
        UMINUS = 304,
        UPLUS = 305,
        PTR_DR = 306,
        ADDR = 307,
        POST_INC = 310,
        POST_DEC = 311
      };
    };

    /// (External) token type, as returned by yylex.
    typedef token::yytokentype token_type;

    /// Symbol type: an internal symbol number.
    typedef int symbol_number_type;

    /// The symbol type number to denote an empty symbol.
    enum { empty_symbol = -2 };

    /// Internal symbol number for tokens (subsumed by symbol_number_type).
    typedef unsigned char token_number_type;

    /// A complete symbol.
    ///
    /// Expects its Base type to provide access to the symbol type
    /// via type_get ().
    ///
    /// Provide access to semantic value and location.
    template <typename Base>
    struct basic_symbol : Base
    {
      /// Alias to Base.
      typedef Base super_type;

      /// Default constructor.
      basic_symbol ()
        : value ()
        , location ()
      {}

#if 201103L <= YY_CPLUSPLUS
      /// Move constructor.
      basic_symbol (basic_symbol&& that);
#endif

      /// Copy constructor.
      basic_symbol (const basic_symbol& that);
      /// Constructor for valueless symbols.
      basic_symbol (typename Base::kind_type t,
                    YY_MOVE_REF (location_type) l);

      /// Constructor for symbols with semantic value.
      basic_symbol (typename Base::kind_type t,
                    YY_RVREF (semantic_type) v,
                    YY_RVREF (location_type) l);

      /// Destroy the symbol.
      ~basic_symbol ()
      {
        clear ();
      }

      /// Destroy contents, and record that is empty.
      void clear ()
      {
        Base::clear ();
      }

      /// Whether empty.
      bool empty () const YY_NOEXCEPT;

      /// Destructive move, \a s is emptied into this.
      void move (basic_symbol& s);

      /// The semantic value.
      semantic_type value;

      /// The location.
      location_type location;

    private:
#if YY_CPLUSPLUS < 201103L
      /// Assignment operator.
      basic_symbol& operator= (const basic_symbol& that);
#endif
    };

    /// Type access provider for token (enum) based symbols.
    struct by_type
    {
      /// Default constructor.
      by_type ();

#if 201103L <= YY_CPLUSPLUS
      /// Move constructor.
      by_type (by_type&& that);
#endif

      /// Copy constructor.
      by_type (const by_type& that);

      /// The symbol type as needed by the constructor.
      typedef token_type kind_type;

      /// Constructor from (external) token numbers.
      by_type (kind_type t);

      /// Record that this symbol is empty.
      void clear ();

      /// Steal the symbol type from \a that.
      void move (by_type& that);

      /// The (internal) type number (corresponding to \a type).
      /// \a empty when empty.
      symbol_number_type type_get () const YY_NOEXCEPT;

      /// The token.
      token_type token () const YY_NOEXCEPT;

      /// The symbol type.
      /// \a empty_symbol when empty.
      /// An int, not token_number_type, to be able to store empty_symbol.
      int type;
    };

    /// "External" symbols: returned by the scanner.
    struct symbol_type : basic_symbol<by_type>
    {};

    /// Build a parser object.
    BlastParser (lexcontext &ctx_yyarg);
    virtual ~BlastParser ();

    /// Parse.  An alias for parse ().
    /// \returns  0 iff parsing succeeded.
    int operator() ();

    /// Parse.
    /// \returns  0 iff parsing succeeded.
    virtual int parse ();

#if YYDEBUG
    /// The current debugging stream.
    std::ostream& debug_stream () const YY_ATTRIBUTE_PURE;
    /// Set the current debugging stream.
    void set_debug_stream (std::ostream &);

    /// Type for debugging levels.
    typedef int debug_level_type;
    /// The current debugging level.
    debug_level_type debug_level () const YY_ATTRIBUTE_PURE;
    /// Set the current debugging level.
    void set_debug_level (debug_level_type l);
#endif

    /// Report a syntax error.
    /// \param loc    where the syntax error is found.
    /// \param msg    a description of the syntax error.
    virtual void error (const location_type& loc, const std::string& msg);

    /// Report a syntax error.
    void error (const syntax_error& err);



  private:
    /// This class is not copyable.
    BlastParser (const BlastParser&);
    BlastParser& operator= (const BlastParser&);

    /// State numbers.
    typedef int state_type;

    /// Generate an error message.
    /// \param yystate   the state where the error occurred.
    /// \param yyla      the lookahead token.
    virtual std::string yysyntax_error_ (state_type yystate,
                                         const symbol_type& yyla) const;

    /// Compute post-reduction state.
    /// \param yystate   the current state
    /// \param yysym     the nonterminal to push on the stack
    state_type yy_lr_goto_state_ (state_type yystate, int yysym);

    /// Whether the given \c yypact_ value indicates a defaulted state.
    /// \param yyvalue   the value to check
    static bool yy_pact_value_is_default_ (int yyvalue);

    /// Whether the given \c yytable_ value indicates a syntax error.
    /// \param yyvalue   the value to check
    static bool yy_table_value_is_error_ (int yyvalue);

    static const short yypact_ninf_;
    static const signed char yytable_ninf_;

    /// Convert a scanner token number \a t to a symbol number.
    static token_number_type yytranslate_ (int t);

    // Tables.
  // YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
  // STATE-NUM.
  static const short yypact_[];

  // YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
  // Performed when YYTABLE does not specify something else to do.  Zero
  // means the default is an error.
  static const unsigned char yydefact_[];

  // YYPGOTO[NTERM-NUM].
  static const short yypgoto_[];

  // YYDEFGOTO[NTERM-NUM].
  static const short yydefgoto_[];

  // YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
  // positive, shift that token.  If negative, reduce the rule whose
  // number is the opposite.  If YYTABLE_NINF, syntax error.
  static const short yytable_[];

  static const short yycheck_[];

  // YYSTOS[STATE-NUM] -- The (internal number of the) accessing
  // symbol of state STATE-NUM.
  static const unsigned char yystos_[];

  // YYR1[YYN] -- Symbol number of symbol that rule YYN derives.
  static const unsigned char yyr1_[];

  // YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.
  static const unsigned char yyr2_[];


#if YYDEBUG
    /// For a symbol, its name in clear.
    static const char* const yytname_[];

  // YYRLINE[YYN] -- Source line where rule number YYN was defined.
  static const unsigned short yyrline_[];
    /// Report on the debug stream that the rule \a r is going to be reduced.
    virtual void yy_reduce_print_ (int r);
    /// Print the state stack on the debug stream.
    virtual void yystack_print_ ();

    /// Debugging level.
    int yydebug_;
    /// Debug stream.
    std::ostream* yycdebug_;

    /// \brief Display a symbol type, value and location.
    /// \param yyo    The output stream.
    /// \param yysym  The symbol.
    template <typename Base>
    void yy_print_ (std::ostream& yyo, const basic_symbol<Base>& yysym) const;
#endif

    /// \brief Reclaim the memory associated to a symbol.
    /// \param yymsg     Why this token is reclaimed.
    ///                  If null, print nothing.
    /// \param yysym     The symbol.
    template <typename Base>
    void yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const;

  private:
    /// Type access provider for state based symbols.
    struct by_state
    {
      /// Default constructor.
      by_state () YY_NOEXCEPT;

      /// The symbol type as needed by the constructor.
      typedef state_type kind_type;

      /// Constructor.
      by_state (kind_type s) YY_NOEXCEPT;

      /// Copy constructor.
      by_state (const by_state& that) YY_NOEXCEPT;

      /// Record that this symbol is empty.
      void clear () YY_NOEXCEPT;

      /// Steal the symbol type from \a that.
      void move (by_state& that);

      /// The (internal) type number (corresponding to \a state).
      /// \a empty_symbol when empty.
      symbol_number_type type_get () const YY_NOEXCEPT;

      /// The state number used to denote an empty symbol.
      enum { empty_state = -1 };

      /// The state.
      /// \a empty when empty.
      state_type state;
    };

    /// "Internal" symbol: element of the stack.
    struct stack_symbol_type : basic_symbol<by_state>
    {
      /// Superclass.
      typedef basic_symbol<by_state> super_type;
      /// Construct an empty symbol.
      stack_symbol_type ();
      /// Move or copy construction.
      stack_symbol_type (YY_RVREF (stack_symbol_type) that);
      /// Steal the contents from \a sym to build this.
      stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) sym);
#if YY_CPLUSPLUS < 201103L
      /// Assignment, needed by push_back by some old implementations.
      /// Moves the contents of that.
      stack_symbol_type& operator= (stack_symbol_type& that);
#endif
    };

    /// A stack with random access from its top.
    template <typename T, typename S = std::vector<T> >
    class stack
    {
    public:
      // Hide our reversed order.
      typedef typename S::reverse_iterator iterator;
      typedef typename S::const_reverse_iterator const_iterator;
      typedef typename S::size_type size_type;

      stack (size_type n = 200)
        : seq_ (n)
      {}

      /// Random access.
      ///
      /// Index 0 returns the topmost element.
      T&
      operator[] (size_type i)
      {
        return seq_[size () - 1 - i];
      }

      /// Random access.
      ///
      /// Index 0 returns the topmost element.
      T&
      operator[] (int i)
      {
        return operator[] (size_type (i));
      }

      /// Random access.
      ///
      /// Index 0 returns the topmost element.
      const T&
      operator[] (size_type i) const
      {
        return seq_[size () - 1 - i];
      }

      /// Random access.
      ///
      /// Index 0 returns the topmost element.
      const T&
      operator[] (int i) const
      {
        return operator[] (size_type (i));
      }

      /// Steal the contents of \a t.
      ///
      /// Close to move-semantics.
      void
      push (YY_MOVE_REF (T) t)
      {
        seq_.push_back (T ());
        operator[] (0).move (t);
      }

      /// Pop elements from the stack.
      void
      pop (int n = 1) YY_NOEXCEPT
      {
        for (; 0 < n; --n)
          seq_.pop_back ();
      }

      /// Pop all elements from the stack.
      void
      clear () YY_NOEXCEPT
      {
        seq_.clear ();
      }

      /// Number of elements on the stack.
      size_type
      size () const YY_NOEXCEPT
      {
        return seq_.size ();
      }

      /// Iterator on top of the stack (going downwards).
      const_iterator
      begin () const YY_NOEXCEPT
      {
        return seq_.rbegin ();
      }

      /// Bottom of the stack.
      const_iterator
      end () const YY_NOEXCEPT
      {
        return seq_.rend ();
      }

      /// Present a slice of the top of a stack.
      class slice
      {
      public:
        slice (const stack& stack, int range)
          : stack_ (stack)
          , range_ (range)
        {}

        const T&
        operator[] (int i) const
        {
          return stack_[range_ - i];
        }

      private:
        const stack& stack_;
        int range_;
      };

    private:
      stack (const stack&);
      stack& operator= (const stack&);
      /// The wrapped container.
      S seq_;
    };


    /// Stack type.
    typedef stack<stack_symbol_type> stack_type;

    /// The stack.
    stack_type yystack_;

    /// Push a new state on the stack.
    /// \param m    a debug message to display
    ///             if null, no trace is output.
    /// \param sym  the symbol
    /// \warning the contents of \a s.value is stolen.
    void yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym);

    /// Push a new look ahead token on the state on the stack.
    /// \param m    a debug message to display
    ///             if null, no trace is output.
    /// \param s    the state
    /// \param sym  the symbol (for its value and location).
    /// \warning the contents of \a sym.value is stolen.
    void yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym);

    /// Pop \a n symbols from the stack.
    void yypop_ (int n = 1);

    /// Constants.
    enum
    {
      yyeof_ = 0,
      yylast_ = 1147,     ///< Last index in yytable_.
      yynnts_ = 61,  ///< Number of nonterminal symbols.
      yyfinal_ = 3, ///< Termination state number.
      yyterror_ = 1,
      yyerrcode_ = 256,
      yyntokens_ = 83  ///< Number of tokens.
    };


    // User arguments.
    lexcontext &ctx;
  };


#line 3 "D:/ProjectsVCS/MyProjects/SPIR-V-Blast/lang/blast.y" // lalr1.cc:431
} // gen
#line 1127 "D:/ProjectsVCS/MyProjects/SPIR-V-Blast/src/gen/blast_parser.hpp" // lalr1.cc:431






// Unqualified %code blocks.
#line 163 "D:/ProjectsVCS/MyProjects/SPIR-V-Blast/lang/blast.y" // lalr1.cc:435


struct scope
{
	std::unordered_map<std::string, Identifier> identifiers;
};

struct lexcontext
{
	std::vector<Attribute> tempAttributes;
	std::vector<scope> scopes;
public:
private:
	void addTempAttribute(const std::string &name, AttributeParamType type, std::any param);
	void operator++();
	void operator--();
};


#line 1155 "D:/ProjectsVCS/MyProjects/SPIR-V-Blast/src/gen/blast_parser.hpp" // lalr1.cc:435


#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> // FIXME: INFRINGES ON USER NAME SPACE.
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

// Whether we are compiled with exception support.
#ifndef YY_EXCEPTIONS
# if defined __GNUC__ && !defined __EXCEPTIONS
#  define YY_EXCEPTIONS 0
# else
#  define YY_EXCEPTIONS 1
# endif
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K].location)
/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

# ifndef YYLLOC_DEFAULT
#  define YYLLOC_DEFAULT(Current, Rhs, N)                               \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).begin  = YYRHSLOC (Rhs, 1).begin;                   \
          (Current).end    = YYRHSLOC (Rhs, N).end;                     \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).begin = (Current).end = YYRHSLOC (Rhs, 0).end;      \
        }                                                               \
    while (false)
# endif


// Suppress unused-variable warnings by "using" E.
#define YYUSE(E) ((void) (E))

// Enable debugging if requested.
#if YYDEBUG

// A pseudo ostream that takes yydebug_ into account.
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Symbol)         \
  do {                                          \
    if (yydebug_)                               \
    {                                           \
      *yycdebug_ << Title << ' ';               \
      yy_print_ (*yycdebug_, Symbol);           \
      *yycdebug_ << '\n';                       \
    }                                           \
  } while (false)

# define YY_REDUCE_PRINT(Rule)          \
  do {                                  \
    if (yydebug_)                       \
      yy_reduce_print_ (Rule);          \
  } while (false)

# define YY_STACK_PRINT()               \
  do {                                  \
    if (yydebug_)                       \
      yystack_print_ ();                \
  } while (false)

#else // !YYDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YYUSE (Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void> (0)
# define YY_STACK_PRINT()                static_cast<void> (0)

#endif // !YYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyla.clear ())

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)

#line 3 "D:/ProjectsVCS/MyProjects/SPIR-V-Blast/lang/blast.y" // lalr1.cc:510
namespace gen {
#line 1250 "D:/ProjectsVCS/MyProjects/SPIR-V-Blast/src/gen/blast_parser.hpp" // lalr1.cc:510

  /// Build a parser object.
  BlastParser::BlastParser (lexcontext &ctx_yyarg)
    :
#if YYDEBUG
      yydebug_ (false),
      yycdebug_ (&std::cerr),
#endif
      ctx (ctx_yyarg)
  {}

  BlastParser::~BlastParser ()
  {}

  BlastParser::syntax_error::~syntax_error () YY_NOEXCEPT YY_NOTHROW
  {}

  /*---------------.
  | Symbol types.  |
  `---------------*/

  // basic_symbol.
#if 201103L <= YY_CPLUSPLUS
  template <typename Base>
  BlastParser::basic_symbol<Base>::basic_symbol (basic_symbol&& that)
    : Base (std::move (that))
    , value (std::move (that.value))
    , location (std::move (that.location))
  {}
#endif

  template <typename Base>
  BlastParser::basic_symbol<Base>::basic_symbol (const basic_symbol& that)
    : Base (that)
    , value (that.value)
    , location (that.location)
  {}


  /// Constructor for valueless symbols.
  template <typename Base>
  BlastParser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, YY_MOVE_REF (location_type) l)
    : Base (t)
    , value ()
    , location (l)
  {}

  template <typename Base>
  BlastParser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, YY_RVREF (semantic_type) v, YY_RVREF (location_type) l)
    : Base (t)
    , value (YY_MOVE (v))
    , location (YY_MOVE (l))
  {}

  template <typename Base>
  bool
  BlastParser::basic_symbol<Base>::empty () const YY_NOEXCEPT
  {
    return Base::type_get () == empty_symbol;
  }

  template <typename Base>
  void
  BlastParser::basic_symbol<Base>::move (basic_symbol& s)
  {
    super_type::move (s);
    value = YY_MOVE (s.value);
    location = YY_MOVE (s.location);
  }

  // by_type.
  BlastParser::by_type::by_type ()
    : type (empty_symbol)
  {}

#if 201103L <= YY_CPLUSPLUS
  BlastParser::by_type::by_type (by_type&& that)
    : type (that.type)
  {
    that.clear ();
  }
#endif

  BlastParser::by_type::by_type (const by_type& that)
    : type (that.type)
  {}

  BlastParser::by_type::by_type (token_type t)
    : type (yytranslate_ (t))
  {}

  void
  BlastParser::by_type::clear ()
  {
    type = empty_symbol;
  }

  void
  BlastParser::by_type::move (by_type& that)
  {
    type = that.type;
    that.clear ();
  }

  int
  BlastParser::by_type::type_get () const YY_NOEXCEPT
  {
    return type;
  }


  // by_state.
  BlastParser::by_state::by_state () YY_NOEXCEPT
    : state (empty_state)
  {}

  BlastParser::by_state::by_state (const by_state& that) YY_NOEXCEPT
    : state (that.state)
  {}

  void
  BlastParser::by_state::clear () YY_NOEXCEPT
  {
    state = empty_state;
  }

  void
  BlastParser::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  BlastParser::by_state::by_state (state_type s) YY_NOEXCEPT
    : state (s)
  {}

  BlastParser::symbol_number_type
  BlastParser::by_state::type_get () const YY_NOEXCEPT
  {
    if (state == empty_state)
      return empty_symbol;
    else
      return yystos_[state];
  }

  BlastParser::stack_symbol_type::stack_symbol_type ()
  {}

  BlastParser::stack_symbol_type::stack_symbol_type (YY_RVREF (stack_symbol_type) that)
    : super_type (YY_MOVE (that.state), YY_MOVE (that.value), YY_MOVE (that.location))
  {
#if 201103L <= YY_CPLUSPLUS
    // that is emptied.
    that.state = empty_state;
#endif
  }

  BlastParser::stack_symbol_type::stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) that)
    : super_type (s, YY_MOVE (that.value), YY_MOVE (that.location))
  {
    // that is emptied.
    that.type = empty_symbol;
  }

#if YY_CPLUSPLUS < 201103L
  BlastParser::stack_symbol_type&
  BlastParser::stack_symbol_type::operator= (stack_symbol_type& that)
  {
    state = that.state;
    value = that.value;
    location = that.location;
    // that is emptied.
    that.state = empty_state;
    return *this;
  }
#endif

  template <typename Base>
  void
  BlastParser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);

    // User destructor.
    YYUSE (yysym.type_get ());
  }

#if YYDEBUG
  template <typename Base>
  void
  BlastParser::yy_print_ (std::ostream& yyo,
                                     const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YYUSE (yyoutput);
    symbol_number_type yytype = yysym.type_get ();
#if defined __GNUC__ && ! defined __clang__ && ! defined __ICC && __GNUC__ * 100 + __GNUC_MINOR__ <= 408
    // Avoid a (spurious) G++ 4.8 warning about "array subscript is
    // below array bounds".
    if (yysym.empty ())
      std::abort ();
#endif
    yyo << (yytype < yyntokens_ ? "token" : "nterm")
        << ' ' << yytname_[yytype] << " ("
        << yysym.location << ": ";
    YYUSE (yytype);
    yyo << ')';
  }
#endif

  void
  BlastParser::yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym)
  {
    if (m)
      YY_SYMBOL_PRINT (m, sym);
    yystack_.push (YY_MOVE (sym));
  }

  void
  BlastParser::yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym)
  {
#if 201103L <= YY_CPLUSPLUS
    yypush_ (m, stack_symbol_type (s, std::move (sym)));
#else
    stack_symbol_type ss (s, sym);
    yypush_ (m, ss);
#endif
  }

  void
  BlastParser::yypop_ (int n)
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  BlastParser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  BlastParser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  BlastParser::debug_level_type
  BlastParser::debug_level () const
  {
    return yydebug_;
  }

  void
  BlastParser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  BlastParser::state_type
  BlastParser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - yyntokens_] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - yyntokens_];
  }

  bool
  BlastParser::yy_pact_value_is_default_ (int yyvalue)
  {
    return yyvalue == yypact_ninf_;
  }

  bool
  BlastParser::yy_table_value_is_error_ (int yyvalue)
  {
    return yyvalue == yytable_ninf_;
  }

  int
  BlastParser::operator() ()
  {
    return parse ();
  }

  int
  BlastParser::parse ()
  {
    // State.
    int yyn;
    /// Length of the RHS of the rule being reduced.
    int yylen = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;

    /// The locations where the error started and ended.
    stack_symbol_type yyerror_range[3];

    /// The return value of parse ().
    int yyresult;

#if YY_EXCEPTIONS
    try
#endif // YY_EXCEPTIONS
      {
    YYCDEBUG << "Starting parse\n";


    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, YY_MOVE (yyla));

  /*-----------------------------------------------.
  | yynewstate -- push a new symbol on the stack.  |
  `-----------------------------------------------*/
  yynewstate:
    YYCDEBUG << "Entering state " << yystack_[0].state << '\n';

    // Accept?
    if (yystack_[0].state == yyfinal_)
      YYACCEPT;

    goto yybackup;


  /*-----------.
  | yybackup.  |
  `-----------*/
  yybackup:
    // Try to take a decision without lookahead.
    yyn = yypact_[yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyla.empty ())
      {
        YYCDEBUG << "Reading a token: ";
#if YY_EXCEPTIONS
        try
#endif // YY_EXCEPTIONS
          {
            yyla.type = yytranslate_ (yylex (&yyla.value, &yyla.location, ctx));
          }
#if YY_EXCEPTIONS
        catch (const syntax_error& yyexc)
          {
            YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
            error (yyexc);
            goto yyerrlab1;
          }
#endif // YY_EXCEPTIONS
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.type_get ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.type_get ())
      goto yydefault;

    // Reduce or error.
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
        if (yy_table_value_is_error_ (yyn))
          goto yyerrlab;
        yyn = -yyn;
        goto yyreduce;
      }

    // Count tokens shifted since error; after three, turn off error status.
    if (yyerrstatus_)
      --yyerrstatus_;

    // Shift the lookahead token.
    yypush_ ("Shifting", yyn, YY_MOVE (yyla));
    goto yynewstate;


  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;


  /*-----------------------------.
  | yyreduce -- do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_ (yystack_[yylen].state, yyr1_[yyn]);
      /* If YYLEN is nonzero, implement the default value of the
         action: '$$ = $1'.  Otherwise, use the top of the stack.

         Otherwise, the following line sets YYLHS.VALUE to garbage.
         This behavior is undocumented and Bison users should not rely
         upon it.  */
      if (yylen)
        yylhs.value = yystack_[yylen - 1].value;
      else
        yylhs.value = yystack_[0].value;

      // Default location.
      {
        stack_type::slice range (yystack_, yylen);
        YYLLOC_DEFAULT (yylhs.location, range, yylen);
        yyerror_range[1].location = yylhs.location;
      }

      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
#if YY_EXCEPTIONS
      try
#endif // YY_EXCEPTIONS
        {
          switch (yyn)
            {

#line 1691 "D:/ProjectsVCS/MyProjects/SPIR-V-Blast/src/gen/blast_parser.hpp" // lalr1.cc:919
            default:
              break;
            }
        }
#if YY_EXCEPTIONS
      catch (const syntax_error& yyexc)
        {
          YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
          error (yyexc);
          YYERROR;
        }
#endif // YY_EXCEPTIONS
      YY_SYMBOL_PRINT ("-> $$ =", yylhs);
      yypop_ (yylen);
      yylen = 0;
      YY_STACK_PRINT ();

      // Shift the result of the reduction.
      yypush_ (YY_NULLPTR, YY_MOVE (yylhs));
    }
    goto yynewstate;


  /*--------------------------------------.
  | yyerrlab -- here on detecting error.  |
  `--------------------------------------*/
  yyerrlab:
    // If not already recovering from an error, report this error.
    if (!yyerrstatus_)
      {
        ++yynerrs_;
        error (yyla.location, yysyntax_error_ (yystack_[0].state, yyla));
      }


    yyerror_range[1].location = yyla.location;
    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.type_get () == yyeof_)
          YYABORT;
        else if (!yyla.empty ())
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyla.clear ();
          }
      }

    // Else will try to reuse lookahead token after shifting the error token.
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:
    /* Pacify compilers when the user code never invokes YYERROR and
       the label yyerrorlab therefore never appears in user code.  */
    if (false)
      YYERROR;

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    goto yyerrlab1;


  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    {
      stack_symbol_type error_token;
      for (;;)
        {
          yyn = yypact_[yystack_[0].state];
          if (!yy_pact_value_is_default_ (yyn))
            {
              yyn += yyterror_;
              if (0 <= yyn && yyn <= yylast_ && yycheck_[yyn] == yyterror_)
                {
                  yyn = yytable_[yyn];
                  if (0 < yyn)
                    break;
                }
            }

          // Pop the current state because it cannot handle the error token.
          if (yystack_.size () == 1)
            YYABORT;

          yyerror_range[1].location = yystack_[0].location;
          yy_destroy_ ("Error: popping", yystack_[0]);
          yypop_ ();
          YY_STACK_PRINT ();
        }

      yyerror_range[2].location = yyla.location;
      YYLLOC_DEFAULT (error_token.location, yyerror_range, 2);

      // Shift the error token.
      error_token.state = yyn;
      yypush_ ("Shifting", YY_MOVE (error_token));
    }
    goto yynewstate;


  /*-------------------------------------.
  | yyacceptlab -- YYACCEPT comes here.  |
  `-------------------------------------*/
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;


  /*-----------------------------------.
  | yyabortlab -- YYABORT comes here.  |
  `-----------------------------------*/
  yyabortlab:
    yyresult = 1;
    goto yyreturn;


  /*-----------------------------------------------------.
  | yyreturn -- parsing is finished, return the result.  |
  `-----------------------------------------------------*/
  yyreturn:
    if (!yyla.empty ())
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    while (1 < yystack_.size ())
      {
        yy_destroy_ ("Cleanup: popping", yystack_[0]);
        yypop_ ();
      }

    return yyresult;
  }
#if YY_EXCEPTIONS
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack\n";
        // Do not try to display the values of the reclaimed symbols,
        // as their printers might throw an exception.
        if (!yyla.empty ())
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
#endif // YY_EXCEPTIONS
  }

  void
  BlastParser::error (const syntax_error& yyexc)
  {
    error (yyexc.location, yyexc.what ());
  }

  // Generate an error message.
  std::string
  BlastParser::yysyntax_error_ (state_type, const symbol_type&) const
  {
    return YY_("syntax error");
  }


  const short BlastParser::yypact_ninf_ = -213;

  const signed char BlastParser::yytable_ninf_ = -63;

  const short
  BlastParser::yypact_[] =
  {
    -213,    25,    44,  -213,  -213,  -213,  -213,   -11,  -213,    15,
    -213,   155,  -213,  -213,  -213,  -213,  -213,  -213,   -12,    48,
      52,   103,  -213,   274,    11,    12,    18,    60,   390,    55,
      58,  -213,  -213,  -213,    13,    13,    13,    13,    13,    13,
      13,    13,    13,  -213,  -213,    64,   233,  -213,  -213,  -213,
    -213,  -213,  -213,  -213,   705,  -213,    61,    -3,  -213,    90,
      62,    79,  -213,  -213,  -213,  -213,   114,   115,  -213,  -213,
    -213,  -213,  -213,  -213,  -213,  -213,  -213,  -213,  -213,  -213,
    -213,  -213,  -213,  -213,  -213,   150,    13,    13,    13,    78,
      81,  -213,   750,  -213,  -213,    -9,    -9,    -9,    -9,    -9,
      -9,    -9,    -9,   -38,   837,  -213,  -213,    13,    13,    13,
      13,    13,    13,    13,    13,  -213,  -213,    13,    13,    13,
      13,    13,    13,    13,    13,    13,    13,    13,    13,    13,
      13,    13,    13,    13,    13,   111,    13,    13,    13,  -213,
    -213,   130,    85,    86,  -213,    -3,   105,  -213,    21,  -213,
      30,    30,   -54,   104,   429,   475,    97,   521,   101,  -213,
    -213,   102,  -213,  -213,    13,  -213,   878,   918,  1053,  1053,
    1075,  1075,  1075,  1075,   837,   837,   795,   837,   837,   837,
     837,   837,   837,   837,   958,   998,  1038,   298,   298,    -9,
      -9,    -9,  -213,   -37,   354,  -213,  -213,   131,   133,  -213,
    -213,   154,  -213,  -213,  -213,  -213,   -31,  -213,   156,   141,
     143,  -213,  -213,  -213,    13,   274,   274,    13,  -213,    13,
      -4,    13,   837,    13,  -213,  -213,    13,   162,  -213,   -18,
      70,  -213,  -213,   161,   163,   567,  -213,  -213,   613,   659,
     144,   170,  -213,  -213,  -213,   126,   837,   837,   837,   157,
     176,  -213,  -213,   164,   168,   135,   274,  -213,   274,   169,
      13,    13,  -213,   179,  -213,  -213,  -213,  -213,   315,  -213,
    -213,   274,   145,   837,   837,   173,  -213,  -213,   274,  -213,
    -213
  };

  const unsigned char
  BlastParser::yydefact_[] =
  {
       4,     0,   154,     1,     3,     5,     6,     0,     7,   130,
       9,   154,     8,    17,   126,   127,   128,   129,     0,     0,
       0,     0,   153,   154,     0,     0,     0,     0,     0,     0,
       0,    70,   114,   115,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    39,    24,     0,   154,    22,    23,    29,
      30,    31,    32,    33,     0,    71,     0,   130,   154,   156,
       0,     0,   144,   145,   146,   147,     0,     0,   150,   151,
     152,   134,   123,   132,   131,   133,   135,   136,   137,   138,
     140,   139,   141,   142,   143,     0,     0,   154,   154,     0,
     154,    36,     0,    38,    37,   109,   110,   106,   104,   103,
     105,   107,   108,     0,    68,    18,    21,     0,     0,     0,
       0,     0,     0,     0,     0,   111,   112,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    34,
      25,     0,     0,   154,    64,   130,     0,   155,   130,    10,
       0,     0,   121,     0,     0,     0,     0,     0,     0,    57,
      44,     0,    46,    35,     0,    72,    95,    96,    97,    98,
      99,   100,   101,   102,    84,    85,     0,    78,    86,    87,
      88,    89,    91,    90,    93,    92,    94,    79,    80,    81,
      82,    83,    73,     0,     0,    77,    76,   119,   116,    61,
      63,     0,   159,   157,   158,    12,     0,    13,    16,     0,
       0,   124,   125,   122,     0,   154,   154,     0,    52,     0,
       0,    49,    69,     0,    74,    75,     0,     0,    67,     0,
     130,    11,    15,     0,     0,     0,    42,    40,     0,     0,
       0,     0,    54,    55,    56,     0,    48,   113,   120,   117,
       0,    65,    14,     0,     0,     0,   154,    53,     0,     0,
      51,     0,    66,     0,   148,    45,    41,    27,    60,    28,
      59,     0,     0,    50,   118,     0,    26,    58,   154,   149,
      43
  };

  const short
  BlastParser::yypgoto_[] =
  {
    -213,  -213,  -213,  -213,  -213,  -213,  -213,  -213,  -213,   -13,
    -213,    -7,  -213,  -213,   -22,  -213,  -212,  -213,  -213,  -213,
    -213,  -213,  -213,  -213,  -213,  -213,  -213,  -213,  -213,  -213,
     -51,  -213,  -213,  -213,    80,  -213,    87,   -19,  -213,    59,
    -213,    -6,  -213,  -213,  -213,  -213,  -213,   -98,  -213,  -213,
    -213,  -213,  -213,  -213,  -213,  -213,  -213,  -213,     0,  -213,
    -213
  };

  const short
  BlastParser::yydefgoto_[] =
  {
      -1,     1,     2,     4,     5,     6,     7,   149,   206,   207,
      12,    44,    45,    46,    47,   268,    48,    49,    50,    51,
      52,    53,   161,   245,   272,    89,   160,   220,   243,   244,
     270,     8,   142,   143,   144,   229,   103,    54,    55,    56,
     198,   208,   152,   213,    21,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    57,    22,
      60
  };

  const short
  BlastParser::yytable_[] =
  {
      13,    85,     9,    20,   240,   241,   211,   164,   164,    92,
      14,    15,    16,    17,   230,    95,    96,    97,    98,    99,
     100,   101,   102,   104,   106,     3,   212,   250,    14,    15,
      16,    17,   115,   116,    14,    15,    16,    17,   165,   224,
      31,    18,    32,    33,    -2,   231,   269,    62,    63,    64,
      65,   141,   209,   210,    34,    35,   276,   251,   145,   269,
     135,   136,   137,   138,    10,    58,    11,   154,   155,   157,
      36,    37,    38,    39,   242,    59,    40,    41,    19,    61,
      86,    87,    42,    14,    15,    16,    17,    88,   166,   167,
     168,   169,   170,   171,   172,   173,    19,   205,   174,   175,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     186,   187,   188,   189,   190,   191,   104,   194,   195,   196,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    90,
      93,    71,   202,    94,   203,   204,   140,   146,    31,   201,
      32,    33,   105,   145,   147,   222,   156,   158,   148,   162,
     150,   151,    34,    35,   153,   159,   -47,   197,    23,    24,
      25,    26,    27,   199,   -62,    28,    29,    30,    36,    37,
      38,    39,   217,   214,    40,    41,   219,   221,   227,   226,
      42,   228,    31,   232,    32,    33,   233,   192,   234,   249,
     253,   258,   254,   236,   237,   235,    34,    35,   238,   259,
     239,   260,   246,   262,   247,   261,   264,   248,   275,   263,
     265,   279,    36,    37,    38,    39,   271,   252,    40,    41,
     277,   278,   193,   200,    42,     0,     0,     0,     0,     0,
      43,     0,    11,   -20,   266,     0,    23,    24,    25,    26,
      27,   273,   274,    28,    29,    30,     0,     0,     0,     0,
       0,   267,     0,     0,     0,     0,   280,     0,     0,     0,
      31,     0,    32,    33,   267,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    34,    35,     0,    23,    24,    25,
      26,    27,     0,     0,    28,    29,    30,     0,     0,     0,
      36,    37,    38,    39,     0,     0,    40,    41,     0,     0,
       0,    31,    42,    32,    33,     0,     0,     0,    43,     0,
      11,   -19,     0,     0,     0,    34,    35,     0,    23,    24,
      25,    26,    27,     0,     0,    28,    29,    30,     0,     0,
       0,    36,    37,    38,    39,     0,     0,    40,    41,   115,
     116,     0,    31,    42,    32,    33,     0,     0,     0,    43,
       0,    11,     0,     0,     0,     0,    34,    35,   132,   133,
     134,     0,     0,     0,     0,     0,     0,   135,   136,   137,
     138,     0,    36,    37,    38,    39,     0,     0,    40,    41,
       0,     0,     0,     0,    42,   107,   108,   109,   110,   111,
      43,   112,     0,   113,   114,   115,   116,   117,   118,     0,
     119,     0,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,    31,     0,    32,
      33,     0,     0,   135,   136,   137,   138,     0,     0,     0,
       0,    34,    35,   225,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    36,    37,    38,
      39,     0,     0,    40,    41,     0,     0,     0,     0,    42,
     107,   108,   109,   110,   111,    91,   112,     0,   113,   114,
     115,   116,   117,   118,     0,   119,     0,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,     0,     0,     0,     0,     0,     0,   135,   136,
     137,   138,     0,     0,     0,   215,   107,   108,   109,   110,
     111,     0,   112,     0,   113,   114,   115,   116,   117,   118,
       0,   119,     0,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,     0,     0,
       0,     0,     0,     0,   135,   136,   137,   138,     0,     0,
       0,   216,   107,   108,   109,   110,   111,     0,   112,     0,
     113,   114,   115,   116,   117,   118,     0,   119,     0,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,     0,     0,     0,     0,     0,     0,
     135,   136,   137,   138,     0,     0,     0,   218,   107,   108,
     109,   110,   111,     0,   112,     0,   113,   114,   115,   116,
     117,   118,     0,   119,     0,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
       0,     0,     0,     0,     0,     0,   135,   136,   137,   138,
       0,     0,     0,   255,   107,   108,   109,   110,   111,     0,
     112,     0,   113,   114,   115,   116,   117,   118,     0,   119,
       0,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,     0,     0,     0,     0,
       0,     0,   135,   136,   137,   138,     0,     0,     0,   256,
     107,   108,   109,   110,   111,     0,   112,     0,   113,   114,
     115,   116,   117,   118,     0,   119,     0,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,     0,     0,     0,     0,     0,     0,   135,   136,
     137,   138,     0,     0,     0,   257,   107,   108,   109,   110,
     111,     0,   112,     0,   113,   114,   115,   116,   117,   118,
       0,   119,     0,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,     0,     0,
       0,     0,     0,     0,   135,   136,   137,   138,     0,     0,
     139,   107,   108,   109,   110,   111,     0,   112,     0,   113,
     114,   115,   116,   117,   118,     0,   119,     0,   120,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,     0,     0,     0,     0,     0,     0,   135,
     136,   137,   138,     0,     0,   163,   107,   108,   109,   110,
     111,     0,   112,     0,   113,   114,   115,   116,   117,   118,
       0,   119,   223,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,     0,     0,
       0,     0,     0,     0,   135,   136,   137,   138,   107,   108,
     109,   110,   111,     0,   112,     0,   113,   114,   115,   116,
     117,   118,     0,   119,     0,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
       0,     0,     0,     0,     0,     0,   135,   136,   137,   138,
     108,   109,   110,   111,     0,   112,     0,   113,   114,   115,
     116,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   127,   128,   129,   130,   131,   132,   133,
     134,     0,     0,     0,     0,     0,     0,   135,   136,   137,
     138,   109,   110,   111,     0,   112,     0,   113,   114,   115,
     116,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   127,   128,   129,   130,   131,   132,   133,
     134,     0,     0,     0,     0,     0,     0,   135,   136,   137,
     138,   109,   110,   111,     0,   112,     0,   113,   114,   115,
     116,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   128,   129,   130,   131,   132,   133,
     134,     0,     0,     0,     0,     0,     0,   135,   136,   137,
     138,   109,   110,   111,     0,   112,     0,   113,   114,   115,
     116,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   129,   130,   131,   132,   133,
     134,     0,     0,     0,     0,     0,     0,   135,   136,   137,
     138,   109,   110,   111,     0,   112,     0,   113,   114,   115,
     116,     0,     0,     0,     0,     0,     0,     0,   111,     0,
     112,     0,   113,   114,   115,   116,   130,   131,   132,   133,
     134,     0,     0,     0,     0,     0,     0,   135,   136,   137,
     138,   130,   131,   132,   133,   134,   115,   116,     0,     0,
       0,     0,   135,   136,   137,   138,     0,     0,     0,     0,
       0,     0,     0,   130,   131,   132,   133,   134,     0,     0,
       0,     0,     0,     0,   135,   136,   137,   138
  };

  const short
  BlastParser::yycheck_[] =
  {
       7,    23,     2,     9,     8,     9,    60,    45,    45,    28,
      13,    14,    15,    16,    45,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    46,     0,    80,    45,    13,    14,
      15,    16,    41,    42,    13,    14,    15,    16,    76,    76,
      27,    26,    29,    30,     0,    76,   258,    17,    18,    19,
      20,    57,   150,   151,    41,    42,   268,    75,    58,   271,
      69,    70,    71,    72,    75,    77,    77,    86,    87,    88,
      57,    58,    59,    60,    78,    27,    63,    64,    81,    27,
      69,    69,    69,    13,    14,    15,    16,    69,   107,   108,
     109,   110,   111,   112,   113,   114,    81,    76,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    69,
      75,    28,    27,    75,    29,    30,    75,    47,    27,   145,
      29,    30,    78,   143,    82,   164,    87,    88,    69,    90,
      36,    36,    41,    42,     4,    77,    75,    27,     3,     4,
       5,     6,     7,    78,    78,    10,    11,    12,    57,    58,
      59,    60,    75,    69,    63,    64,    75,    75,    45,    48,
      69,    27,    27,    27,    29,    30,    45,    76,    45,    27,
      29,    47,    29,   215,   216,   214,    41,    42,   217,    29,
     219,    75,   221,    27,   223,    48,    38,   226,    29,    45,
      75,    38,    57,    58,    59,    60,    47,   230,    63,    64,
     271,    76,   135,   143,    69,    -1,    -1,    -1,    -1,    -1,
      75,    -1,    77,    78,   256,    -1,     3,     4,     5,     6,
       7,   260,   261,    10,    11,    12,    -1,    -1,    -1,    -1,
      -1,   258,    -1,    -1,    -1,    -1,   278,    -1,    -1,    -1,
      27,    -1,    29,    30,   271,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    41,    42,    -1,     3,     4,     5,
       6,     7,    -1,    -1,    10,    11,    12,    -1,    -1,    -1,
      57,    58,    59,    60,    -1,    -1,    63,    64,    -1,    -1,
      -1,    27,    69,    29,    30,    -1,    -1,    -1,    75,    -1,
      77,    78,    -1,    -1,    -1,    41,    42,    -1,     3,     4,
       5,     6,     7,    -1,    -1,    10,    11,    12,    -1,    -1,
      -1,    57,    58,    59,    60,    -1,    -1,    63,    64,    41,
      42,    -1,    27,    69,    29,    30,    -1,    -1,    -1,    75,
      -1,    77,    -1,    -1,    -1,    -1,    41,    42,    60,    61,
      62,    -1,    -1,    -1,    -1,    -1,    -1,    69,    70,    71,
      72,    -1,    57,    58,    59,    60,    -1,    -1,    63,    64,
      -1,    -1,    -1,    -1,    69,    31,    32,    33,    34,    35,
      75,    37,    -1,    39,    40,    41,    42,    43,    44,    -1,
      46,    -1,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    27,    -1,    29,
      30,    -1,    -1,    69,    70,    71,    72,    -1,    -1,    -1,
      -1,    41,    42,    79,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    57,    58,    59,
      60,    -1,    -1,    63,    64,    -1,    -1,    -1,    -1,    69,
      31,    32,    33,    34,    35,    75,    37,    -1,    39,    40,
      41,    42,    43,    44,    -1,    46,    -1,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    -1,    -1,    -1,    -1,    -1,    -1,    69,    70,
      71,    72,    -1,    -1,    -1,    76,    31,    32,    33,    34,
      35,    -1,    37,    -1,    39,    40,    41,    42,    43,    44,
      -1,    46,    -1,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    -1,    -1,
      -1,    -1,    -1,    -1,    69,    70,    71,    72,    -1,    -1,
      -1,    76,    31,    32,    33,    34,    35,    -1,    37,    -1,
      39,    40,    41,    42,    43,    44,    -1,    46,    -1,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    -1,    -1,    -1,    -1,    -1,    -1,
      69,    70,    71,    72,    -1,    -1,    -1,    76,    31,    32,
      33,    34,    35,    -1,    37,    -1,    39,    40,    41,    42,
      43,    44,    -1,    46,    -1,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      -1,    -1,    -1,    -1,    -1,    -1,    69,    70,    71,    72,
      -1,    -1,    -1,    76,    31,    32,    33,    34,    35,    -1,
      37,    -1,    39,    40,    41,    42,    43,    44,    -1,    46,
      -1,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    -1,    -1,    -1,    -1,
      -1,    -1,    69,    70,    71,    72,    -1,    -1,    -1,    76,
      31,    32,    33,    34,    35,    -1,    37,    -1,    39,    40,
      41,    42,    43,    44,    -1,    46,    -1,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    -1,    -1,    -1,    -1,    -1,    -1,    69,    70,
      71,    72,    -1,    -1,    -1,    76,    31,    32,    33,    34,
      35,    -1,    37,    -1,    39,    40,    41,    42,    43,    44,
      -1,    46,    -1,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    -1,    -1,
      -1,    -1,    -1,    -1,    69,    70,    71,    72,    -1,    -1,
      75,    31,    32,    33,    34,    35,    -1,    37,    -1,    39,
      40,    41,    42,    43,    44,    -1,    46,    -1,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    -1,    -1,    -1,    -1,    -1,    -1,    69,
      70,    71,    72,    -1,    -1,    75,    31,    32,    33,    34,
      35,    -1,    37,    -1,    39,    40,    41,    42,    43,    44,
      -1,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    -1,    -1,
      -1,    -1,    -1,    -1,    69,    70,    71,    72,    31,    32,
      33,    34,    35,    -1,    37,    -1,    39,    40,    41,    42,
      43,    44,    -1,    46,    -1,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      -1,    -1,    -1,    -1,    -1,    -1,    69,    70,    71,    72,
      32,    33,    34,    35,    -1,    37,    -1,    39,    40,    41,
      42,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    55,    56,    57,    58,    59,    60,    61,
      62,    -1,    -1,    -1,    -1,    -1,    -1,    69,    70,    71,
      72,    33,    34,    35,    -1,    37,    -1,    39,    40,    41,
      42,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    55,    56,    57,    58,    59,    60,    61,
      62,    -1,    -1,    -1,    -1,    -1,    -1,    69,    70,    71,
      72,    33,    34,    35,    -1,    37,    -1,    39,    40,    41,
      42,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    56,    57,    58,    59,    60,    61,
      62,    -1,    -1,    -1,    -1,    -1,    -1,    69,    70,    71,
      72,    33,    34,    35,    -1,    37,    -1,    39,    40,    41,
      42,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    57,    58,    59,    60,    61,
      62,    -1,    -1,    -1,    -1,    -1,    -1,    69,    70,    71,
      72,    33,    34,    35,    -1,    37,    -1,    39,    40,    41,
      42,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    35,    -1,
      37,    -1,    39,    40,    41,    42,    58,    59,    60,    61,
      62,    -1,    -1,    -1,    -1,    -1,    -1,    69,    70,    71,
      72,    58,    59,    60,    61,    62,    41,    42,    -1,    -1,
      -1,    -1,    69,    70,    71,    72,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    58,    59,    60,    61,    62,    -1,    -1,
      -1,    -1,    -1,    -1,    69,    70,    71,    72
  };

  const unsigned char
  BlastParser::yystos_[] =
  {
       0,    84,    85,     0,    86,    87,    88,    89,   114,   141,
      75,    77,    93,    94,    13,    14,    15,    16,    26,    81,
     124,   127,   142,     3,     4,     5,     6,     7,    10,    11,
      12,    27,    29,    30,    41,    42,    57,    58,    59,    60,
      63,    64,    69,    75,    94,    95,    96,    97,    99,   100,
     101,   102,   103,   104,   120,   121,   122,   141,    77,    27,
     143,    27,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    28,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   139,   140,    97,    69,    69,    69,   108,
      69,    75,   120,    75,    75,   120,   120,   120,   120,   120,
     120,   120,   120,   119,   120,    78,    97,    31,    32,    33,
      34,    35,    37,    39,    40,    41,    42,    43,    44,    46,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    69,    70,    71,    72,    75,
      75,   124,   115,   116,   117,   141,    47,    82,    69,    90,
      36,    36,   125,     4,   120,   120,   122,   120,   122,    77,
     109,   105,   122,    75,    45,    76,   120,   120,   120,   120,
     120,   120,   120,   120,   120,   120,   120,   120,   120,   120,
     120,   120,   120,   120,   120,   120,   120,   120,   120,   120,
     120,   120,    76,   119,   120,   120,   120,    27,   123,    78,
     117,   124,    27,    29,    30,    76,    91,    92,   124,   130,
     130,    60,    80,   126,    69,    76,    76,    75,    76,    75,
     110,    75,   120,    47,    76,    79,    48,    45,    27,   118,
      45,    76,    27,    45,    45,   120,    97,    97,   120,   120,
       8,     9,    78,   111,   112,   106,   120,   120,   120,    27,
      45,    75,    92,    29,    29,    76,    76,    76,    47,    29,
      75,    48,    27,    45,    38,    75,    97,    94,    98,    99,
     113,    47,   107,   120,   120,    29,    99,   113,    76,    38,
      97
  };

  const unsigned char
  BlastParser::yyr1_[] =
  {
       0,    83,    84,    85,    85,    86,    86,    86,    87,    88,
      89,    90,    90,    91,    91,    92,    92,    93,    94,    95,
      95,    96,    96,    97,    97,    97,    98,    98,    98,    99,
      99,    99,    99,    99,    99,    99,    99,    99,    99,    99,
     100,   100,   101,   102,   103,   104,   105,   105,   106,   106,
     107,   107,   108,   108,   109,   110,   110,   110,   111,   112,
     113,   114,   115,   116,   116,   117,   118,   118,   119,   119,
     120,   120,   120,   120,   120,   120,   120,   120,   120,   120,
     120,   120,   120,   120,   120,   120,   120,   120,   120,   120,
     120,   120,   120,   120,   120,   120,   120,   120,   120,   120,
     120,   120,   120,   120,   120,   120,   120,   120,   120,   120,
     120,   120,   120,   120,   121,   121,   122,   123,   123,   123,
     123,   124,   125,   125,   126,   126,   127,   127,   127,   127,
     127,   128,   128,   128,   129,   130,   130,   130,   130,   131,
     131,   131,   131,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   141,   142,   143,   143,   143,   143
  };

  const unsigned char
  BlastParser::yyr2_[] =
  {
       0,     2,     1,     2,     0,     1,     1,     1,     2,     2,
       4,     3,     2,     1,     3,     2,     1,     1,     3,     1,
       0,     2,     1,     1,     1,     2,     2,     1,     1,     1,
       1,     1,     1,     1,     2,     3,     2,     2,     2,     1,
       5,     7,     5,     9,     3,     7,     1,     0,     1,     0,
       1,     0,     3,     5,     3,     2,     2,     0,     4,     3,
       1,     5,     1,     2,     1,     4,     3,     1,     1,     3,
       1,     1,     3,     3,     4,     4,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     5,     1,     1,     3,     3,     5,     1,
       3,     3,     2,     0,     1,     1,     1,     1,     1,     1,
       0,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     6,     8,
       1,     1,     1,     2,     0,     3,     1,     3,     3,     3
  };


#if YYDEBUG
  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a yyntokens_, nonterminals.
  const char*
  const BlastParser::yytname_[] =
  {
  "END", "error", "$undefined", "\"do\"", "\"while\"", "\"if\"",
  "\"switch\"", "\"for\"", "\"default\"", "\"case\"", "\"return\"",
  "\"break\"", "\"continue\"", "\"const\"", "\"specconst\"", "\"novar\"",
  "\"runtime\"", "\"void\"", "\"int\"", "\"float\"", "\"bool\"", "\"mat\"",
  "\"vec\"", "\"img\"", "\"smpl\"", "\"simg\"", "\"struct\"", "IDENTIFIER",
  "USER_DEFINED_TYPE", "NUMLITERAL", "STRINGLITERAL", "\"||\"", "\"&&\"",
  "\"==\"", "\"!=\"", "LESS", "'<'", "MORE", "'>'", "\"<=\"", "\">=\"",
  "\"++\"", "\"--\"", "\"+=\"", "\"-=\"", "','", "'?'", "':'", "'='",
  "\"*=\"", "\"/=\"", "\"%=\"", "\"&=\"", "\"^=\"", "\"|=\"", "'|'", "'^'",
  "'&'", "'+'", "'-'", "'*'", "'/'", "'%'", "'!'", "'~'", "UMINUS",
  "UPLUS", "PTR_DR", "ADDR", "'('", "'['", "\".\"", "\"->\"", "POST_INC",
  "POST_DEC", "';'", "')'", "'{'", "'}'", "']'", "\"[]\"", "\"[[\"",
  "\"]]\"", "$accept", "shader", "shader_unit_rec", "shader_unit",
  "function", "function_prototype", "function_decl", "function_parameters",
  "function_parameter_list", "function_parameter", "function_body",
  "braced_body", "body", "statement_rec", "statement", "statement_nb_rec",
  "statement_nb", "if_statement", "while_statement", "for_statement",
  "switch_statement", "do_while_statement", "for_init", "for_condition",
  "for_action", "switch_init", "switch_body", "switch_case_rec",
  "switch_case", "switch_default_case", "case_body", "struct",
  "struct_body", "struct_members_continious", "struct_member",
  "struct_member_continious", "comma_expression", "expression", "literal",
  "var_def", "var_def_continious", "type", "type_suffix",
  "type_suffix_variant", "type_mod", "type_variant", "custom_type",
  "simple_type", "complex_type_variant", "void_type", "int_type",
  "float_type", "bool_type", "vector_type", "matrix_type", "image_type",
  "sampler_type", "sampled_image_type", "attribute_rec", "attribute",
  "attribute_body", YY_NULLPTR
  };


  const unsigned short
  BlastParser::yyrline_[] =
  {
       0,   214,   214,   216,   217,   219,   220,   221,   225,   227,
     229,   231,   232,   234,   235,   237,   238,   240,   244,   246,
     247,   249,   250,   252,   253,   254,   256,   257,   258,   260,
     261,   262,   263,   264,   265,   266,   267,   268,   269,   270,
     275,   276,   277,   278,   279,   280,   282,   283,   285,   286,
     288,   289,   291,   292,   294,   296,   297,   298,   300,   301,
     303,   307,   309,   311,   312,   314,   316,   317,   321,   322,
     324,   325,   326,   327,   328,   329,   330,   331,   332,   333,
     334,   335,   336,   337,   338,   339,   340,   341,   342,   343,
     344,   345,   346,   347,   348,   349,   350,   351,   352,   353,
     354,   355,   356,   357,   358,   359,   360,   361,   362,   363,
     364,   365,   366,   367,   371,   372,   376,   378,   379,   380,
     381,   385,   387,   388,   390,   391,   393,   394,   395,   396,
     397,   399,   400,   401,   403,   405,   406,   407,   408,   410,
     411,   412,   413,   414,   416,   417,   418,   419,   421,   422,
     423,   424,   425,   428,   429,   431,   433,   434,   435,   436
  };

  // Print the state stack on the debug stream.
  void
  BlastParser::yystack_print_ ()
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << i->state;
    *yycdebug_ << '\n';
  }

  // Report on the debug stream that the rule \a yyrule is going to be reduced.
  void
  BlastParser::yy_reduce_print_ (int yyrule)
  {
    unsigned yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):\n";
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // YYDEBUG

  BlastParser::token_number_type
  BlastParser::yytranslate_ (int t)
  {
    // YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to
    // TOKEN-NUM as returned by yylex.
    static
    const token_number_type
    translate_table[] =
    {
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    63,     2,     2,     2,    62,    57,     2,
      69,    76,    60,    58,    45,    59,     2,    61,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    47,    75,
      36,    48,    38,    46,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    70,     2,    79,    56,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    77,    55,    78,    64,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    37,    39,    40,    41,    42,    43,    44,    49,    50,
      51,    52,    53,    54,    65,    66,    67,    68,    71,    72,
      73,    74,    80,    81,    82
    };
    const unsigned user_token_number_max_ = 314;
    const token_number_type undef_token_ = 2;

    if (static_cast<int> (t) <= yyeof_)
      return yyeof_;
    else if (static_cast<unsigned> (t) <= user_token_number_max_)
      return translate_table[t];
    else
      return undef_token_;
  }

#line 3 "D:/ProjectsVCS/MyProjects/SPIR-V-Blast/lang/blast.y" // lalr1.cc:1242
} // gen
#line 2426 "D:/ProjectsVCS/MyProjects/SPIR-V-Blast/src/gen/blast_parser.hpp" // lalr1.cc:1242
#line 438 "D:/ProjectsVCS/MyProjects/SPIR-V-Blast/lang/blast.y" // lalr1.cc:1243


// Realization of context
void lexcontext::addTempAttribute(const std::string &name, AttributeParamType type, std::any param)
{
	Attribute attr;
	attr.ident = { IdentifierType::Attribute, name };
	attr.type = type;
	switch (type)
	{
	case AttributeParamType::Identifier:
		attr.paramIdent = std::any_cast<Identifier>(param);
		attr.paramIdent.type = IdentifierType::Variable;
		break;
	case AttributeParamType::Literal:
		attr.paramLiteral = std::any_cast<Literal>(param);
		break;
	}
	tempAttributes.push_back(attr);
}

void lexcontext::operator++()
{
	scopes.emplace_back();
}

void lexcontext::operator--()
{
	scopes.pop_back();
}

// TODO: [OOKAMI] Use re2c and use context to determine whenether the token is ident or user defined type.
// TODO: [OOKAMI] Maybe I should use function name as like user defined types? user defined functions xD
