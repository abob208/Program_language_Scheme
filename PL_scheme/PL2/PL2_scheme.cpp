# include <string>
# include <string.h> 
# include <stdio.h>
# include <stdlib.h>
# include <iostream>
# include <vector>
using namespace std;

enum TokenType { LEFT_PAREN = 34512, RIGHT_PAREN = 87232, INT = 29742, 
                 STRING = 88888, DOT = 456465, FLOAT, NIL, T, QUOTE, SYMBOL 
} ;
enum ParserType { ATOM, ELSE } ;


struct Token {
  string name ;
  int line ;
  int g_lin ; 
  int column ;
  TokenType type ;
  ParserType ptype ;
};

struct Expr {
  Token token ;
  string value ;
  bool isNil ;
  bool isFirst ;
  bool havenode ;
  bool printParen ;
  Expr * left ;
  Expr * right ;
};

struct Var {
  string name ;
  Expr * value ;  
};

class DealToken {
  public :
  bool IsNum( char a ) ;
  bool Isfloat( string temp ) ;
  bool IsNumber( string temp ) ;
  char GetNextChar() ;
  char GetNextNonSpaceChar() ;
  Token Gettoken( Token token ) ;
  Token Peektoken() ;
};

class Parser {
  public :
  void Eval( Expr * head, Expr * ans ) ; 
  void ReadEXP() ;
  void PrintEXP( Expr * head ) ;
  void S_exp( Expr * root ) ;
  void SetNextToken() ;
  void Printstring( string temp ) ;
  bool IsExit() ;
  Expr * mexp ;
  private :
  vector<Var> mVarList ;
  vector<Token> mTokenList ;
  Token mnextReadToken ;
  bool misFirstPrint ;
  bool mnonSpace ;
  int mparenspace ;
  int mspace ; 
  bool misleft ;
  string mprint_string ;
};

int glo_line = 1 ;
int gline = 1 ;
int gcol = 1 ;  
int gscope = 0 ;
int gtempline = 0 ;
int gerror = 0 ;
bool gsameexp = false ;
int main() {
  int uTestNum = 0 ;
  char ch = '\0' ;
  scanf( "%d%c", &uTestNum, &ch ) ;
  char trash = '0' ;
  DealToken scanner ;
  Parser parser ;
  Token token ;
  Expr * head = NULL ;
  bool isExit = false ;
  Expr * ans = NULL ;
  cout << "Welcome to OurScheme!\n\n> " ;
  int i = 0 ;

  while ( !isExit && !parser.IsExit() ) {
    try {
      parser.ReadEXP() ;
      head = parser.mexp ;
      if ( !parser.IsExit() ) {
        parser.Eval( head, ans ) ;
        // if ( head->printParen )
        //  cout << "( " ;
        // parser.PrintEXP( head ) ;
        // if ( head->printParen )
        //  cout << ")\n" ;
      } // if
    } // try
    catch ( Token eof ) {
      cout << "ERROR (no more input) : END-OF-FILE encountered" ;
      isExit = true ;
    } // catch
    catch ( int col ) {
      cout << "ERROR (no closing quote) : END-OF-LINE encountered at Line " 
      << gline << " Column " << gcol + 1 << "\n" ;
      while ( trash != '\n' && trash != '\0' ) trash = scanner.GetNextChar() ;
      if ( trash == '\0' ) {
        isExit = true ;
      } // if
    } // catch
    catch ( string name ) {
      if ( gerror == 1 ) {
        cout << "ERROR (unexpected token) : atom or '(' expected when token at Line "
        << gline << " Column " << gcol << " is >>" << name << "<<\n" ;
      } // if

      if ( gerror == 2 ) {
        cout << "ERROR (unexpected token) : ')' expected when token at Line "
        << gline << " Column " << gcol << " is >>" << name << "<<\n" ;
      } // if

      while ( trash != '\n' && trash != '\0' ) trash = scanner.GetNextChar() ;
      if ( trash == '\0' ) {
        isExit = true ;
      } // if

      gerror = 0 ;
    } // catch

    if ( !isExit && !parser.IsExit() )
      cout << "\n> " ;

    if ( trash == '\0' ) {
      cout << "\n> ERROR (no more input) : END-OF-FILE encountered" ;
      isExit = true ;
    } // if

    gsameexp = false ;
    trash = '0' ;
    gline = 1 ;
    gcol = 0 ;
  } // while

  cout << "\nThanks for using OurScheme!" ;
} // main()

void Parser::Eval( Expr * head, Expr * ans ) {
  DealToken scanner ;
  if ( head->left == NULL && head->right == NULL ) {
    int index = 0 ;
    for ( int i = 0 ; i < mVarList.size() ; i++ ) {
      if ( mVarList[i].name == head->value ) {
        index = i ;
        Eval( mVarList[index].value, ans ) ;
        return ;
      } // if
    } // for

    PrintEXP( head ) ;
  } // if
  else if ( head->left->value[head->left->value.size() - 1] == '?' ) {
    if ( head->left->value == "atom?" ) {
      if ( head->right->left->left == NULL && head->right->left->right == NULL ) {
        cout << "#t\n" ;
      } // if
      else {
        cout << "nil\n" ;
      } // else
    } // if
    else if ( head->left->value == "pair?" ) {
      for ( int i = 0 ; i < mVarList.size() ; i++ ) {
        if ( mVarList[i].name == head->right->left->value ) {
          head->right->left = mVarList[i].value ;
        } // if
      } // for

      if ( head->right->left->left != NULL && head->right->left->right != NULL ) {
        cout << "#t\n" ;
      } // if
      else {
        cout << "nil\n" ;
      } // else
    } // else if
    else if ( head->left->value == "list?" ) {

    } // else if
    else if ( head->left->value == "null?" ) {
      if ( head->right->left->isNil == true ) {
        cout << "#t\n" ;
      } // if
      else {
        cout << "nil\n" ;
      } // else
    } // else if
    else if ( head->left->value == "integer?" ) {
      if ( scanner.IsNumber( head->right->left->value ) ) {
        cout << "#t\n" ;
      } // if
      else {
        cout << "nil\n" ;
      } // else
    } // else if
    else if ( head->left->value == "real?" ) {
      if ( scanner.IsNumber( head->right->left->value ) 
           || scanner.Isfloat( head->right->left->value ) ) {
        cout << "#t\n" ;
      } // if
      else {
        cout << "nil\n" ;
      } // else
    } // else if
    else if ( head->left->value == "number?" ) {
      if ( scanner.IsNumber( head->right->left->value ) 
           || scanner.Isfloat( head->right->left->value ) ) {
        cout << "#t\n" ;
      } // if
      else {
        cout << "nil\n" ;
      } // else
    } // else if
    else if ( head->left->value == "string?" ) {
      if (  head->right->left->token.type == STRING ) {
        cout << "#t\n" ;
      } // if
      else {
        cout << "nil\n" ;
      } // else
    } // else if
    else if ( head->left->value == "boolean?" ) {
      for ( int i = 0 ; i < mVarList.size() ; i++ ) {
        if ( mVarList[i].name == head->right->left->value ) {
          head->right->left = mVarList[i].value ;
        } // if
      } // for

      if ( head->right->left->token.type == NIL || head->right->left->token.type == T 
           || head->right->left->isNil ) {
        cout << "#t\n" ;
      } // if
      else {
        cout << "nil\n" ;
      } // else
    } // else if
    else if ( head->left->value == "symbol?" ) {
      for ( int i = 0 ; i < mVarList.size() ; i++ ) {
        if ( mVarList[i].name == head->right->left->value ) {
          cout << "nil\n" ;
          return ;
        } // if
      } // for

      if (  head->right->left->token.type == SYMBOL ) {
        cout << "#t\n" ;
      } // if
      else if ( head->right->left->right != NULL 
                && head->right->left->right->left->token.type == SYMBOL ) {
        cout << "#t\n" ;
      } // else if
      else {
        cout << "nil\n" ;
      } // else
    } // else if

  } // else if
  else if ( head->left->value == "define" ) {
    for ( int i = 0 ; i < mVarList.size() ; i++ ) {
      if ( mVarList[i].name == head->right->left->value ) {
        if ( head->right->right->left->left != NULL ) {
          if ( head->right->right->left->left->value == "cons" ||
               head->right->right->left->left->value == "quote" ) {
            mVarList[i].value = head->right->right->left ;
          } // if
        } // if
        else {
          mVarList[i].value = head->right->right ;
        } // else

        cout << mVarList[i].name << " defined\n" ;
        return ;
      } // if
    } // for

    Var temp ;
    temp.name = head->right->left->value ;
    if ( head->right->right->left->left != NULL ) {
      if ( head->right->right->left->left->value == "cons" ||
           head->right->right->left->left->value == "quote" ) {
        temp.value = head->right->right->left ;
      } // if
    } // if
    else {
      temp.value = head->right->right ;
    } // else

    mVarList.push_back( temp ) ;
    cout << temp.name << " defined\n" ;
  } // else if
  else if ( head->left->value == "clean-environment" ) {
    mVarList.clear() ;
    cout << "environment cleaned\n" ;
  } // else if
  else if ( head->left->value == "cons" ) {
    /*
    Expr * temp = head ;
    temp = temp->right ;
    cout << "( " << temp->left->value << "\n" ;
    mspace = mspace + 2 ;
    mnonSpace = false ;
    misFirstPrint = false ;
    temp->right = temp->right->left ;
    // temp = temp->right ;
    while ( temp != NULL ) {
      if ( temp->left != NULL ) {
        if ( temp->left->left != NULL && temp->left->left->value == "quote" ) {
          temp->left->right->left->printParen = false ;
        } // if

        Eval( temp->left, ans ) ;
      } // if

      temp = temp->right ;
    } // while

    cout << ")\n" ;
    */


    if ( head->right->left->token.type == SYMBOL ) {
      for ( int i = 0 ; i < mVarList.size() ; i++ ) {
        if ( mVarList[i].name == head->right->left->value ) {
          if ( mVarList[i].value->right->left != NULL )
            head->right->left = mVarList[i].value->right->left ;
          else {
            head->right->left = mVarList[i].value->left ;
          } // else

          // head->right->left->printParen = true ;
          // head->right->left->left->printParen = false ;  // 真的不知道自己在寫三小 硬幹過去 
        } // if
      } // for
    } // if
    else if ( head->right->right->left->token.type == SYMBOL ) {
      for ( int i = 0 ; i < mVarList.size() ; i++ ) {
        if ( mVarList[i].name == head->right->right->left->value ) {
          if ( mVarList[i].value->right->left != NULL ) {
            head->right->right->left = mVarList[i].value ;
            // head->right->right->left->printParen = false ;
          } // if
          else {
            head->right->right->left = mVarList[i].value->left ;
          } // else

          // head->right->left->printParen = true ;
          // head->right->left->left->printParen = false ;  // 真的不知道自己在寫三小 硬幹過去 
        } // if
      } // for
    } // else if

    head->right->printParen = true ;

    if ( head->right->right->left->left != NULL 
         && head->right->right->left->left->value == "quote" ) {


      if ( head->right->left->left == NULL && head->right->left->right == NULL ) {
        cout << "( " ;
        PrintEXP( head->right->left ) ;
        mspace = mspace + 2 ;
        mnonSpace = false ;
        misFirstPrint = false ;
        head->right->right = head->right->right->left ; // 不確定可不可以這樣 

        bool temp = head->right->right->right->left->printParen ;
        head->right->right->right->left->printParen = false ;
        // cout << head->right->right->right->right->value ;
        PrintEXP( head->right->right->right ) ;
        cout << ")\n" ; // 有點像硬幹 不確定可不可 
        head->right->right->right->left->printParen = temp ;
      } // if
      else {
        cout << "( " ;
        mspace = mspace + 2 ;
        head->right->right = head->right->right->left ; // 不確定可不可以這樣 
        bool temp = head->right->right->right->left->printParen ;
        head->right->right->right->left->printParen = false ;
        PrintEXP( head->right->left ) ;
        PrintEXP( head->right->right->right ) ;
        cout << ")\n" ; // 有點像硬幹 不確定可不可   
        head->right->right->right->left->printParen = temp ;  
      } // else

    } // if
    else { 
      head->right->right = head->right->right->left ; // 不確定可不可以這樣 
      mexp->havenode = true ;
      PrintEXP( head->right ) ;
    } // else


  } // else if
  else if ( head->left->value == "list" ) {
    Expr * temp = head ;
    temp = temp->right ;
    cout << "( " ;
    mspace = mspace + 2 ;
    mnonSpace = true ;
    misFirstPrint = true ;
    Eval( temp->left, ans ) ;
    /*
    cout << "( " << temp->left->value << "\n" ;
    mspace = mspace + 2 ;
    mnonSpace = false ;
    misFirstPrint = false ;
    */

    temp = temp->right ;
    while ( temp != NULL ) {
      if ( temp->left != NULL ) {
        misleft = true ;
        Eval( temp->left, ans ) ;
      } // if

      temp = temp->right ;
    } // while

    cout << ")\n" ;

    /*
    if ( head->right->right->left->left != NULL && 
         head->right->right->left->left->value == "quote" ) {
      // cout << head->right->right->right->right->left->left->value ;
      
    } // if
    else {
      head->right->printParen = true ;
      PrintEXP( head->right ) ;
    } // else
    */
  } // else if
  else if ( head->left->value == "quote" ) {
    mexp->havenode = true ;
    PrintEXP( head->right ) ;
  } // else if
  else if ( head->left->value == "car" ) {
    mexp->havenode = true ;
    if ( head->right->left->token.type == SYMBOL ) {
      for ( int i = 0 ; i < mVarList.size() ; i++ ) {
        if ( mVarList[i].name == head->right->left->value ) {
          if ( mVarList[i].value->right->left != NULL ) {
            head->right->left = mVarList[i].value ;
            // head->right->right->left->printParen = false ;
          } // if
          else {
            head->right->left = mVarList[i].value->left ;
          } // else

          // head->right->left->printParen = true ;
          // head->right->left->left->printParen = false ;  // 真的不知道自己在寫三小 硬幹過去 
        } // if
      } // for
    } // if

    bool temp = head->right->left->right->left->left->printParen ;
    if ( head->right->left->right->left->left->left != NULL &&
         head->right->left->right->left->left->right != NULL ) {
      head->right->left->right->left->left->printParen = true ;
    } // if

    PrintEXP( head->right->left->right->left->left ) ;
    head->right->left->right->left->left->printParen = temp ;
  } // else if
  else if ( head->left->value == "cdr" ) {
    mexp->havenode = true ;
    if ( head->right->left->token.type == SYMBOL ) {
      for ( int i = 0 ; i < mVarList.size() ; i++ ) {
        if ( mVarList[i].name == head->right->left->value ) {
          if ( mVarList[i].value->right->left != NULL ) {
            head->right->left = mVarList[i].value ;
            // head->right->right->left->printParen = false ;
          } // if
          else {
            head->right->left = mVarList[i].value->left ;
          } // else

          // head->right->left->printParen = true ;
          // head->right->left->left->printParen = false ;  // 真的不知道自己在寫三小 硬幹過去 
        } // if
      } // for
    } // if

    bool temp = head->right->left->right->left->right->printParen ;
    if ( head->right->left->right->left->right->left != NULL &&
         head->right->left->right->left->right->right != NULL ) {
      head->right->left->right->left->right->printParen = true ;
    } // if

    PrintEXP( head->right->left->right->left->right ) ;
    temp = head->right->left->right->left->right->printParen ;
  } // else if
  else if ( head->left->value == "+" ) {
    Expr * temp = head ;
    float result = 0 ;
    temp = temp->right ;
    bool isfloat = false ;
    while ( temp != NULL && temp->left != NULL ) {
      if ( scanner.Isfloat( temp->left->value ) ) {
        result = result + atof( temp->left->value.c_str() ) ;
        isfloat = true ;
      } // else if
      else if ( scanner.IsNumber( temp->left->value ) ) {
        result = result + atoi( temp->left->value.c_str() ) ;
      } // else if

      temp = temp->right ;
    } // while


    if ( isfloat ) {
      printf( "%.3f\n", result ) ;
    } // if
    else if ( !isfloat ) {
      int result2 = result ;
      printf( "%d\n", result2 ) ;
    } // else if
  } // else if
  else if ( head->left->value == "-" ) {
    Expr * temp = head ;
    float result = 0 ;
    bool isfloat = false ;
    temp = temp->right ;
    if ( scanner.Isfloat( temp->left->value ) ) {
      result = atof( temp->left->value.c_str() ) ;
      isfloat = true ;
    } // else if
    else if ( scanner.IsNumber( temp->left->value ) ) {
      result = atoi( temp->left->value.c_str() ) ;
    } // else if

    temp = temp->right ;
    while ( temp != NULL && temp->left != NULL ) {
      if ( scanner.Isfloat( temp->left->value ) ) {
        result = result - atof( temp->left->value.c_str() ) ;
        isfloat = true ;
      } // else if
      else if ( scanner.IsNumber( temp->left->value ) ) {
        result = result - atoi( temp->left->value.c_str() ) ;
      } // else if

      temp = temp->right ;
    } // while


    if ( isfloat ) {
      printf( "%.3f\n", result ) ;
    } // if
    else if ( !isfloat ) {
      int result2 = result ;
      printf( "%d\n", result2 ) ;
    } // else if
  } // else if
  else if ( head->left->value == "*" ) {
    Expr * temp = head ;
    float result = 0 ;
    bool isfloat = false ;
    temp = temp->right ;
    if ( scanner.Isfloat( temp->left->value ) ) {
      result = atof( temp->left->value.c_str() ) ;
      isfloat = true ;
    } // else if
    else if ( scanner.IsNumber( temp->left->value ) ) {
      result = atoi( temp->left->value.c_str() ) ;
    } // else if

    temp = temp->right ;
    while ( temp != NULL && temp->left != NULL ) {
      if ( scanner.Isfloat( temp->left->value ) ) {
        result = result * atof( temp->left->value.c_str() ) ;
        isfloat = true ;
      } // else if
      else if ( scanner.IsNumber( temp->left->value ) ) {
        result = result * atoi( temp->left->value.c_str() ) ;
      } // else if

      temp = temp->right ;
    } // while


    if ( isfloat ) {
      printf( "%.3f\n", result ) ;
    } // if
    else if ( !isfloat ) {
      int result2 = result ;
      printf( "%d\n", result2 ) ;
    } // else if
  } // else if
  else if ( head->left->value == "/" ) {
    Expr * temp = head ;
    float result = 0 ;
    bool isfloat = false ;
    temp = temp->right ;
    if ( scanner.Isfloat( temp->left->value ) ) {
      result = atof( temp->left->value.c_str() ) ;
      isfloat = true ;
    } // else if
    else if ( scanner.IsNumber( temp->left->value ) ) {
      result = atoi( temp->left->value.c_str() ) ;
    } // else if

    temp = temp->right ;
    while ( temp != NULL && temp->left != NULL ) {
      if ( scanner.Isfloat( temp->left->value ) ) {
        result = result / atof( temp->left->value.c_str() ) ;
        isfloat = true ;
      } // else if
      else if ( scanner.IsNumber( temp->left->value ) ) {
        result = result / atoi( temp->left->value.c_str() ) ;
      } // else if

      temp = temp->right ;
    } // while


    if ( isfloat ) {
      printf( "%.3f\n", result ) ;
    } // if
    else if ( !isfloat ) {
      int result2 = result ;
      printf( "%d\n", result2 ) ;
    } // else if
  } // else if
  else {
    PrintEXP( head->left ) ;
  } // else
} // Parser::Eval()

void Parser::SetNextToken() {
  DealToken scanner ;
  mnextReadToken = scanner.Peektoken() ;
} // Parser::SetNextToken()

bool DealToken::IsNum( char a ) {
  if ( a >= '0' && a <= '9' )
    return true ;

  return false ;
} // DealToken::IsNum()

bool Parser::IsExit() {


  if ( mTokenList.size() < 3 )
    return false ;

  if ( mTokenList[0].name == "(" && mTokenList[1].name == "exit" ) {
    for ( int i = 2 ; i < mTokenList.size() ; i++ ) {
      // cout << mTokenList[i].name << "\n" ;
      if ( mTokenList[i].type != DOT && mTokenList[i].type != LEFT_PAREN 
           && mTokenList[i].type != RIGHT_PAREN && mTokenList[i].type != NIL ) {
        // cout << mTokenList[i].name << "--------------\n" ;
        return false ;
      } // if
    } // for


    if ( mTokenList[mTokenList.size() - 1].name != ")" )
      return false ;
 
    return true ;
  } // if 

  // 要再思考怎麼改比較好 
  return false ;
} // Parser::IsExit()

Token DealToken::Gettoken( Token token ) {
  return token ;
} // DealToken::Gettoken()

void Parser::ReadEXP() {
  DealToken scanner ;
  mTokenList.clear() ;
  mparenspace = 0 ;
  mspace = 0 ;
  mnonSpace = false ;
  misleft = true ;
  mexp = new Expr() ;
  // head = mexp ;
  mexp->isNil = true ;
  mexp->isFirst = true ;
  mexp->havenode = false ;
  mexp->printParen = false ;
  mnextReadToken.name = "\0" ;

  bool have_n = false ;
  char temp = '\0' ;
  Token eof ;
  temp = cin.peek() ;
  if ( temp == '\0' )
    throw eof ;

  int temp_col = gcol ;
  while ( temp == '\t' || temp == ' ' ) {
    temp = scanner.GetNextChar() ;
    if ( temp == '\0' )
      throw eof ;

    temp = cin.peek() ;
  } // while

  if ( temp == '\n' ) {
    temp = scanner.GetNextChar() ;
    have_n = true ;
    gline = 1 ;
    gcol = 0 ;
  } // if

  if ( temp == ';' ) {
    temp = cin.peek() ;
    while ( temp != '\n' && temp != '\0' ) {
      temp = scanner.GetNextChar() ;
      temp = cin.peek() ;
    } // while

    if ( temp == '\0' )
      throw eof ;
    
    if ( temp == '\n' ) {
      temp = scanner.GetNextChar() ;
      have_n = true ;
      gline = 1 ;
      gcol = 0 ;
    } // if
      
  } // if

  mnextReadToken = scanner.Peektoken() ;
  gsameexp = true ;
  S_exp( mexp ) ;

  if ( mexp->left != NULL || mexp->right != NULL ) {
    mexp->printParen = true ;
  } // if
} // Parser::ReadEXP()

void Parser::Printstring( string temp ) {
  for ( int i = 0 ; i < temp.size() ; i++ ) {
    if ( temp[i] == '\\' ) {
      if ( temp[i + 1] == 'n' ) {
        cout << "\n" ;
        i++ ;
      } // if
      else if ( temp[i + 1] == 't' ) {
        cout << "\t" ;
        i++ ;
      } // else if
      else if ( temp[i + 1] == '"' || temp[i + 1] == '\\' ) {
        i++ ;
        cout << temp[i] ;
      } // else if
      else {
        cout << temp[i] ;
      } // else
    } // if
    else {
      cout << temp[i] ;
    } // else
  } // for
} // Parser::Printstring()


void Parser::PrintEXP( Expr * head ) {
  DealToken scanner ; 
  if ( IsExit() ) {
    return ;
  } // if 


  if ( head->left == NULL && head -> right == NULL ) {
    if ( mexp->havenode && !misFirstPrint && !head->isNil && !mnonSpace ) {
      for ( int i = 0 ; i < mspace ; i++ ) {
        cout << " " ;
      } // for

      // cout << "  " ;
    } // if

    

    if ( head->isNil && misleft ) {
      if ( mexp->havenode && !mnonSpace ) {
        for ( int i = 0 ; i < mspace ; i++ ) {
          cout << " " ;

        } // for

        // if ( !misFirstPrint )
        //  cout << "  " ;
      } // if

      cout << "nil\n" ;
    } // if
    else if ( head->value == "#f" && misleft ) {
      if ( mexp->havenode && !mnonSpace ) {
        for ( int i = 0 ; i < mspace ; i++ ) {
          cout << " " ;

        } // for

        // if ( !misFirstPrint )
        //  cout << "  " ;
      } // if

      cout << "nil\n" ;

    } // else if
    else if ( head->value == "t" ) {
      cout << "#t\n" ;
    } // if
    else if ( scanner.Isfloat( head->value ) ) {
      printf( "%.3f\n", atof( head->value.c_str() ) ) ;
    } // else if
    else if ( scanner.IsNumber( head->value ) ) {
      printf( "%d\n", atoi( head->value.c_str() ) ) ;
    } // else if
    else {
      if ( mexp->havenode && !misFirstPrint && head->isNil ) {
        return ;
      } // if

      // cout << mexp.value << "\n" ;
      Printstring( head->value ) ;
      cout << "\n" ;
    } // else

    mnonSpace = false ;
    misFirstPrint = false ;
    return ;
  } // if


  if ( head->printParen ) {
    for ( int i = 0 ; i < mspace && !misFirstPrint && !mnonSpace ; i++ ) {
      cout << " " ;
    } // for

    cout << "( " ; // 要再修改空格印的數量 

    mspace = mspace + 2 ;
    mnonSpace = true ;
  } // if

  misleft = true ;
  PrintEXP( head->left ) ;
  misleft = false ;
  if ( head->left->left == NULL && head->left-> right == NULL 
       && head->right->left == NULL && head->right-> right == NULL 
       && !head->right->isNil ) {
    for ( int i = 0 ; i < mspace ; i++ ) {
      cout << " " ;
    } // for

    cout << ".\n" ;
  } // if
  else if ( head->left->printParen && head->right->left == NULL && head->right->right == NULL 
            && !head->right->isNil ) {
    for ( int i = 0 ; i < mspace ; i++ ) {
      cout << " " ;
    } // for

    cout << ".\n" ;
  } // else if

  PrintEXP( head->right ) ;

  if ( head->printParen ) {
    mspace = mspace - 2 ;
    for ( int i = 0 ; i < mspace ; i++ ) {
      cout << " " ;
    } // for

    cout << ")\n" ;  // 要再修改空格印的數量 
  } // if

} // Parser::PrintEXP()


void Parser::S_exp( Expr * root ) {
  misFirstPrint = true ;
  DealToken scanner ;
  Token token ;
  if ( mnextReadToken.ptype == ATOM ) {
    mTokenList.push_back( mnextReadToken ) ; 


    if ( mnextReadToken.type == NIL ) {
      // mexp->isNil = true ;
      root->isNil = true ;
    } // if
    else {
      // mexp->isNil = false ;
      // mexp->value = mnextReadToken.name ;
      root->isNil = false ; 
      root->value = mnextReadToken.name ;
    } // else

    root->token = mnextReadToken ;
    root->isFirst = false ;
    root->left = NULL ;
    root->right = NULL ;
    // mexp->isFirst = false ;
    // mnextReadToken = scanner.Peektoken() ;
  } // if
  else if ( mnextReadToken.type == LEFT_PAREN ) {
    // mexp->left = new Expr() ;
    // mexp->right = new Expr() ;
    mTokenList.push_back( mnextReadToken ) ;
    mnextReadToken = scanner.Peektoken() ;
    if ( mnextReadToken.type == RIGHT_PAREN ) {
      root->isNil = true ;
      mTokenList.push_back( mnextReadToken ) ;
      // mnextReadToken = scanner.Peektoken() ;
    } // if
    else {
      root->left = new Expr() ;
      root->right = new Expr() ;
      root->right->isNil = true ;
      // root->printParen = true ;
      mexp->havenode = true ;

      if ( mnextReadToken.type == LEFT_PAREN ) {
        root->left->printParen = true ;
      } // if


      S_exp( root->left ) ;
      mnextReadToken = scanner.Peektoken() ;
      while ( mnextReadToken.ptype == ATOM || 
              mnextReadToken.type == LEFT_PAREN || mnextReadToken.type == QUOTE ) {

        if ( mnextReadToken.ptype == ATOM ) {
          root = root -> right ;
          root->left = new Expr() ;
          root->right = new Expr() ;
          root->right->isNil = true ;
          S_exp( root->left ) ;
        } // if
        else if ( mnextReadToken.type == LEFT_PAREN ) {

          root = root -> right ;
          root->left = new Expr() ;
          root->right = new Expr() ;
          root->left->printParen = true ;
          root->right->isNil = true ;
          S_exp( root->left ) ;
        } // else if
        else if ( mnextReadToken.type == QUOTE ) {
          /*
          root = root -> right ;
          root->left = new Expr() ;
          root->right = new Expr() ;
          root->printParen = true ;
          root->left->value = "quote" ;
          root->right->isNil = true ;
          S_exp( root->right ) ;
          */
          root = root -> right ;
          root->left = new Expr() ;
          root->right = new Expr() ;
          root->right->isNil = true ;
          root->left->printParen = true ;
          S_exp( root->left ) ;
          root->left->right->printParen = false ;
        } // else if

        mnextReadToken = scanner.Peektoken() ;
        // S_exp( root->right ) ;
      } // while

      if ( mnextReadToken.type == DOT ) {
        mTokenList.push_back( mnextReadToken ) ; 
        mnextReadToken = scanner.Peektoken() ;
        S_exp( root->right ) ;
        mnextReadToken = scanner.Peektoken() ;
      } // if

      if ( mnextReadToken.type == RIGHT_PAREN ) {
        // cout << "~~~" << mnextReadToken.name ;
        mTokenList.push_back( mnextReadToken ) ;
        // mnextReadToken = scanner.Peektoken() ;
      } // if
      else {
        gerror = 2 ;
        throw mnextReadToken.name ;
      } // else
    } // else
  } // else if
  else if ( mnextReadToken.type == QUOTE ) {
    /*
    mexp->havenode = true ;
    root->left = new Expr() ;
    root->right = new Expr() ;
    root->left->value = "quote" ;
    root->right->printParen = true ;
    mTokenList.push_back( mnextReadToken ) ;
    mnextReadToken = scanner.Peektoken() ;
    S_exp( root->right ) ; 
    */
    // -----------------------------------------
    mexp->havenode = true ;
    root->printParen = true ;
    root->left = new Expr() ;
    root->right = new Expr() ;
    // root->right->printParen = true ;
    root->left->value = "quote" ;
    root = root->right ;
    root->left = new Expr() ;
    root->right = new Expr() ;
    root->right->isNil = true ; 
    // root->right->printParen = true ;
    mTokenList.push_back( mnextReadToken ) ;
    mnextReadToken = scanner.Peektoken() ;
    if ( mnextReadToken.name == "(" ) {
      root->left->printParen = true ;
    } // if

    S_exp( root->left ) ; 
  } // else if
  else {
    gerror = 1 ;
    throw mnextReadToken.name ;
  } // else
} // Parser::S_exp()


char DealToken::GetNextChar() {
  char nextchar = '\0' ;
  Token eof ;
  if ( scanf( "%c", &nextchar ) == false ) {
    eof.name = "END-OF-FILE" ;
    return '\0' ;
  } // if
  else if ( nextchar == '\n' ) {
    gcol = 0 ;
    gline++ ;
    glo_line++ ;
  } // if
  else {
    gcol++;
  } // else
  
  return nextchar ;
} // DealToken::GetNextChar()

char DealToken::GetNextNonSpaceChar() {
  char nextchar = '\0' ;
  nextchar = GetNextChar() ;

  if ( nextchar == '\0' )
    return '\0' ;

  while ( nextchar == '\n' || nextchar == ' ' || nextchar == '\t' ) {
    nextchar = GetNextChar() ;
  } // while()
  
  return nextchar ;
} // DealToken::GetNextNonSpaceChar()

bool DealToken::Isfloat( string temp ) {
  int dot = 0 ;
  bool operat = false ;
  bool havenum = false ;
  for ( int i = 0 ; i < temp.size() ; i++ ) {
    if ( !IsNum( temp[i] ) && temp[i] != '.' && temp[i] != '-' 
         && temp[i] != '+' ) {
      return false ;
    } // if

    if ( temp[i] == '.' )
      dot++ ;

    if ( temp[i] == '+' || temp[i] == '-' ) {
      if ( operat )
        return false ;

      operat = true ;
    } // if

    if ( IsNum( temp[i] ) ) {
      havenum = true ;
    } // if
  } // for

  if ( dot > 1 )
    return false ;
  if ( !havenum )
    return false ;
  if ( dot == 0 ) 
    return false ;
  return true ;
} // DealToken::Isfloat()

bool DealToken::IsNumber( string temp ) {
  bool operat = false ;
  bool havenum = false ; 
  for ( int i = 0 ; i < temp.size() ; i++ ) {
    if ( !IsNum( temp[i] ) && temp[i] != '-' 
         && temp[i] != '+' ) {
      return false ;
    } // if

    if ( temp[i] == '+' || temp[i] == '-' ) {
      if ( operat )
        return false ;

      operat = true ;
    } // if
    
    if ( IsNum( temp[i] ) )
      havenum = true ; 
  } // for

  if ( !havenum )
    return false ;
  return true ;
} // DealToken::IsNumber()

Token DealToken::Peektoken() {
  int to_throw = 0 ;
  Token eof ;
  Token token ;
  // bool have_n = false ; 
  char temp = cin.peek() ;
  while ( temp == '\n' || temp == ' ' || temp == '\t' ) {
    temp = GetNextChar() ;
    temp = cin.peek() ;
    /*
    if ( temp == '\n' ) {
      have_n = true ;
    } // if
    */
  } // while()



  char next_char = GetNextNonSpaceChar() ;
  // gline-- ;
  if ( next_char == '\0' ) {
    eof.name = "END-OF-FILE" ;
    throw eof ;
  } // if()

  char trash = '0' ;
  while ( next_char == ';' ) {
    while ( trash != '\n' && trash != '\0' ) trash = GetNextChar() ;
    if ( trash == '\0' )
      throw eof ;
    next_char = GetNextNonSpaceChar() ;
    if ( next_char == '\0' ) // 乾這是隱藏 卡超久的操 
      throw eof ;
    // cout << "~~~~~~~~~~~" << next_char << "\n" ;
    trash = '0' ;
  } // while

  /*
  if ( have_n )
    gline-- ;
  */

  token.g_lin = glo_line ;
  token.line = gline ;
  token.column = gcol ;

  // next_char = cin.peek() ;
  if ( next_char == '(' || next_char == ')' || next_char == '\'' || next_char == '"' ) {
    token.name = token.name + next_char ;
  } // if
  else {
    token.name = token.name + next_char ;
    next_char = cin.peek() ;
    while ( next_char != '(' && next_char != ')' && next_char != '\'' && next_char != '"'
            && next_char != ';' && next_char != '\n' && next_char != '\t' && next_char != ' ' ) {
      next_char = GetNextChar() ;
      token.name = token.name + next_char ;
      next_char = cin.peek() ;    
    } // while
  } // else

  if ( token.name == "\"" ) {
    next_char = cin.peek() ;
    if ( next_char == '\n' ) {
      throw to_throw ;
    } // if

    next_char = GetNextChar() ;
    while ( next_char != '"' ) {
      if ( next_char == '\\' ) {
        token.name = token.name + next_char ;
        next_char = GetNextChar() ;
      } // if
 
      token.name = token.name + next_char ;
      to_throw = gcol ;
      next_char = cin.peek() ;
      if ( next_char == '\n' ) {
        throw to_throw ;
      } // if

      next_char = GetNextChar() ;
    } // while

    token.name = token.name + next_char ;
  } // if 

  if ( token.name == "(" ) {
    token.type = LEFT_PAREN ;
    token.ptype = ELSE ;
  } // if
  else if ( token.name == ")" ) {
    token.type = RIGHT_PAREN ;
    token.ptype = ELSE ;
  } // else if
  else if ( token.name == "t" || token.name == "#t" ) {
    token.type = T ;
    token.ptype = ATOM ;
  } // else if
  else if ( token.name == "#f" || token.name == "nil" ) {
    token.type = NIL ;
    token.ptype = ATOM ;
  } // else if
  else if ( token.name == "." ) {
    token.type = DOT ;
    token.ptype = ELSE ;
  } // else if 
  else if ( Isfloat( token.name ) ) {
    token.type = FLOAT ;
    token.ptype = ATOM ;
  } // else if
  else if ( IsNumber( token.name ) ) {
    token.type = INT ;
    token.ptype = ATOM ;
  } // else if
  else if ( token.name == "'" ) {
    token.type = QUOTE ;
    token.ptype = ELSE ;
  } // else if
  else if ( token.name[0] == '"' ) {
    token.type = STRING ;
    token.ptype = ATOM ;
  } // else if
  else {
    token.type = SYMBOL ;
    token.ptype = ATOM ;
  } // else

  temp = cin.peek() ;
  /*
  while ( temp == '\n' || temp == ' ' || temp == '\t' ) {
    temp = GetNextChar() ;
    temp = cin.peek() ;
  } // while()
  */

  return token ;

} // DealToken::Peektoken()
