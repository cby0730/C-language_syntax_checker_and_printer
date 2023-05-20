# include <stdio.h>
# include <iostream>
# include <vector>
# include <string.h>
# include <string>
# include <sstream>
# include <cctype>
# include <iomanip>
# include <cmath>
# include <stack>

using namespace std ;

int gline = 1 ;

struct Command{
    
  string name ;
  string variableType ;
  vector<float> variable_array ;
  bool is_vari, is_func ;
  
  vector<string> func_variable ; // if it is a function, store it's variable ;
  vector<string> token ; // every token in this command
    
} ;

class Lexical_error : public exception{
    
public:
    
  Lexical_error( string error_str )
  {
        
    cout << "> Line " << gline << " : unrecognized token with first char : '" << error_str << "'" << endl;
        
  } // Lexical_error()
    
} ; // Lexical_error

class Syntax_error : public exception{
    
public:
    
  Syntax_error( string error_str )
  {
        
    cout << "> Line " << gline << " : unexpected token : '" << error_str << "'" << endl;
        
  } // Syntax_error()
    
} ; // Syntax_error

class Defined_error : public exception{
    
public:
    
  Defined_error( string error_str )
  {
        
    cout << "> Line " << gline << " : undefined identifier : '" << error_str << "'" << endl;
        
  } // Defined_error()
    
} ; // Defined_error

class Error : public exception {
  
public:
  
  Error( ) {
    
    cout << "> Line " << gline << " : Error" << endl;
    
  } // Error()
  
} ;

class Input{

private:
    
  void Skip_white_space()
  {
        
    while ( cin.peek() == '\n' || cin.peek() == '\t' || cin.peek() == ' ' )
      if ( cin.get() == '\n' ) gline ++ ;
        
  } // Skip_white_space()
    
  bool Is_number_correct( string& str )
  {
    
    int dot_num = 0 ;
    string error_str ;
        
    for ( int i = 0; i < str.length() ; i ++ )
    { // check wether number has more than one dot
            
      if ( str[i] == '.' ) dot_num ++ ;
            
      if ( dot_num == 2 && str[i] == '.'  )
      {
        do {
          error_str += str[i] ;
          i ++ ;
        } while ( isdigit( str[i] ) ) ;
                
      } // if
            
    } // for
        
    if ( dot_num > 1 )
    {
      str = error_str ;
      return false ;
            
    } // if
    else return true ;
        
  } // Is_number_correct()
    
public:
  
  string GetChar( ) {
    
    string ch ;
    
    Skip_white_space() ;
    ch += cin.get() ;
    
    if ( ch == "/" && cin.peek() == '/' ) {
      
      // cin.getline( trash, 200 ) ;
      // gline ++ ;
      while ( cin.peek() != '\n' ) cin.get() ;
      return GetChar() ;
      
    } // if
    else return ch ;
    
  } // GetChar()
  
  string PeekChar() {
    
    string ch ;
    
    ch += GetChar() ;
    cin.putback( ch[0] ) ;
    return ch ;
    
  } // PeekChar()
    
    // ===================================getToken()========================================
    
  void GetToken( string& tokenType, string& tokenValue ) {  // only get one token
                        // peek first, actually get the char if it is ok
        
    string input_token = "" ;
    bool is_error = false ;
    Skip_white_space() ; // skip white space at first
        
    if ( cin.peek() == EOF )
    {
            
      tokenType = "EOF" ;
      tokenValue = EOF ;
      return;
            
    } // if
    else if ( isalpha( cin.peek() ) ) { // this token is id-type
            
      input_token += cin.get() ;
            
      while ( isdigit( cin.peek() ) || isalpha( cin.peek() ) || cin.peek() == '_' )
        input_token += cin.get() ; // only number, alpha and _ can be behind the id-type
            
      if ( input_token == "int" || input_token == "void" || input_token == "float" ||
           input_token == "char" || input_token == "bool" || input_token == "string" ||
           input_token == "if" || input_token == "do" || input_token == "while" ||
           input_token == "else" || input_token == "return" || input_token == "cin" ||
           input_token == "cout" || input_token == "true" || input_token == "false" )
        tokenType = "keep_id" ;
      else tokenType = "id" ;
            
    } // else if
    else if ( isdigit( cin.peek() ) ) { // this token is number-type
            
      input_token += cin.get() ;
            
      while ( isdigit( cin.peek() ) || cin.peek() == '.' )
        input_token += cin.get() ; // only number and . can be behind the number-type
            
      if ( Is_number_correct( input_token ) ) tokenType = "constant" ; // the number is correct
      else throw Syntax_error( input_token ) ; // if happen 3.4.5
            
    } // else if
    else if ( ispunct( cin.peek() ) ) {
            // this token is special-type, someone need to check next char, if don't just return
            
      tokenType = "special" ;
      input_token += cin.get() ;
            
      if ( input_token == "<" ) { // <
                
        if ( cin.peek() == '<' ) input_token += cin.get() ; // <<
        else if ( cin.peek() == '=' ) input_token += cin.get() ; // <=
        else if ( cin.peek() == '>' ) input_token += cin.get() ; // <>
                
      } // else if
      else if ( input_token == ">" ) { // >
                
        if ( cin.peek() == '>' ) input_token += cin.get() ; // >>
        else if ( cin.peek() == '=' ) input_token += cin.get() ; // >=
                
      } // else if
      else if ( input_token == "=" ) { // =
        
        if ( cin.peek() == '=' ) input_token += cin.get() ; // ==
              
      } // else if
      else if ( input_token == "/" ) { // /
                
        if ( cin.peek() == '/' ) { // //
                    
          // cin.getline( trash, 200 ) ;
          // gline ++ ;
          while ( cin.peek() != '\n' ) cin.get() ;
          return GetToken( tokenType, tokenValue ) ;
                    
        } // if
        else if ( cin.peek() == '=' ) input_token += cin.get() ; // /=
        
      } // else if
      else if ( input_token == "+" ) { // +
             
        if ( cin.peek() == '+' ) {
          
          input_token += cin.get() ; // ++
          tokenType = "PP" ;
          
        } // if
        else if ( cin.peek() == '=' ) input_token += cin.get() ; // +=
      
      } // else if
      else if ( input_token == "-" ) { // -
             
        if ( cin.peek() == '-' ) {
          
          input_token += cin.get() ; // --
          tokenType = "MM" ;
          
        } // if
        else if ( cin.peek() == '=' ) input_token += cin.get() ; // -=
        
      } // else if
      else if ( input_token == "*" ) { // *
        
        if ( cin.peek() == '=' ) input_token += cin.get() ; // *=
                
      } // else if
      else if ( input_token == "%" ) {
        
        if ( cin.peek() == '=' ) input_token += cin.get() ; // %=
                
      } // else if
      
      else if ( input_token == "&" ) {
                
        if ( cin.peek() == '&' ) input_token += cin.get() ; // &&
                
      } // else if
      
      else if ( input_token == "|" ) { // |
                
        if ( cin.peek() == '|' ) input_token += cin.get() ; // ||
                
      } // else if
      else if ( input_token == "!" ) { // !
                
        if ( cin.peek() == '=' ) input_token += cin.get() ; // !=
                
      } // else if
      else if ( input_token == "\'" ) { // the first '
                
        // if ( cin.peek() == '\n' ) throw Lexical_error( "" ) ;
        // this still need to be confirmed !!!!!!!!!!!!!!!
                
        while ( cin.peek() != '\'' ) {
          
          if ( cin.peek() == '\n' ) throw Lexical_error( "\'" ) ;
          else if ( cin.peek() == '\\' ) {
            
            input_token += cin.get() ;
            if ( cin.peek() == '\'' ) input_token += cin.get() ;
            
          } // else if
          else
            input_token += cin.get() ; // get everything between two '
          
        } // while
                
        input_token += cin.get() ; // the correspond '
        
        tokenType = "constant" ;
                
      } // else if
      else if ( input_token == "\"" ) { // the first "
                
        // if ( cin.peek() == '\n' ) throw Lexical_error( input_token ) ;
        // this still need to be confirmed !!!!!!!!!!!!!!!
                
        while ( cin.peek() != '"' ) {
          
          if ( cin.peek() == '\n' ) throw Lexical_error( "\"" ) ;
          else if ( cin.peek() == '\\' ) {
            
            input_token += cin.get() ;
            if ( cin.peek() == '"' ) input_token += cin.get() ;
            
          } // else if
          else
            input_token += cin.get() ; // get everything between two '
          
        } // while
                
        input_token += cin.get() ; // the correspond '
        
        tokenType = "constant" ;
        
      } // else if
      else if ( input_token == "." ) { // .
                
        if ( isdigit( cin.peek() ) ) { // peek is digit behind .
                    
          while ( isdigit( cin.peek() ) || cin.peek() == '.' ) // get every number behind the .
            input_token += cin.get() ;
                    
          if ( Is_number_correct( input_token ) ) tokenType = "constant" ; // the number is correct
          else throw Syntax_error( input_token ) ; // if happened .3.4.5
          
        } // if
        else is_error = true ; // . can't be alone
        if ( is_error ) throw Lexical_error( input_token ) ;
                
      } // else if
      else if ( input_token == ";" || input_token == "(" || input_token == ")" ||
                input_token == "[" || input_token == "]" || input_token == "{" ||
                input_token == "}" || input_token == "^" || input_token == "," ||
                input_token == "?" || input_token == ":" ) {
        // this kinds of special char don't need to check next char
                
        tokenType = "special" ;
                
      } // else if
      else throw Lexical_error( input_token ) ; // other delimiters are wrong
            
    } // else if
    else is_error = true ; // if this char don't belong to above, then this is a wrong char
    if ( is_error ) { // 夏氏 Style
      
      input_token += cin.get() ;
      throw Lexical_error( input_token ) ;
    
    } // if
    
    tokenValue = input_token ;
        
  } // GetToken()
    
  void PeekToken( string& tokenType, string& tokenValue )
  { // peek the next token
        
    GetToken( tokenType, tokenValue ) ;
    for ( int i = ( int ) tokenValue.length() - 1 ; i >= 0 ; i -- ) // put every char back
      cin.putback( tokenValue[i] ) ;
        
  } // PeekToken()

} ; // end class Input

class Parser {
  
private:
  bool myfunc_declar, myis_cin, myis_cout ;
  bool myis_signed ;
  Input mdata ;
  vector<Command> myCommand_list ;
  
public:
  
  bool User_input( ) {
    // user_input : ( definition | statement ) { definition | statement }
    
    string tokenType, tokenValue ;
    Command command ;
    command.is_func = false ;
    command.is_vari = false ;
    myis_cin = false ;
    myis_cout = false ;
    myfunc_declar = false ;
    myis_signed = false ;
    
    mdata.PeekToken( tokenType, tokenValue ) ;
    if ( tokenValue == "Done" ) {
      
      mdata.GetToken( tokenType, tokenValue ) ; // get 'Done'
      
      mdata.GetToken( tokenType, tokenValue ) ; // get '('
      if ( tokenValue != "(" ) throw Syntax_error( tokenValue ) ;
      
      mdata.GetToken( tokenType, tokenValue ) ; // get ')'
      if ( tokenValue != ")" ) throw Syntax_error( tokenValue ) ;
      
      mdata.GetToken( tokenType, tokenValue ) ; // get ';'
      if ( tokenValue != ";" ) throw Syntax_error( tokenValue ) ;
      
      return false ;
      
    } // if
    else if ( tokenValue == "ListAllVariables" ) {
      
      mdata.GetToken( tokenType, tokenValue ) ; // get 'ListAllVariables'
      
      mdata.GetToken( tokenType, tokenValue ) ; // get '('
      if ( tokenValue != "(" ) throw Syntax_error( tokenValue ) ;
      
      mdata.GetToken( tokenType, tokenValue ) ; // get ')'
      if ( tokenValue != ")" ) throw Syntax_error( tokenValue ) ;
      
      mdata.GetToken( tokenType, tokenValue ) ; // get ';'
      if ( tokenValue != ";" ) throw Syntax_error( tokenValue ) ;
      
      ListAllVariables() ;
      cout << "Statement executed ..." << endl ;
      
    } // else if
    else if ( tokenValue == "ListAllFunctions" ) {
      
      mdata.GetToken( tokenType, tokenValue ) ; // get 'ListAllFunctions'
      
      mdata.GetToken( tokenType, tokenValue ) ; // get '('
      if ( tokenValue != "(" ) throw Syntax_error( tokenValue ) ;
      
      mdata.GetToken( tokenType, tokenValue ) ; // get ')'
      if ( tokenValue != ")" ) throw Syntax_error( tokenValue ) ;
      
      mdata.GetToken( tokenType, tokenValue ) ; // get ';'
      if ( tokenValue != ";" ) throw Syntax_error( tokenValue ) ;
      
      ListAllFunctions() ;
      cout << "Statement executed ..." << endl ;
      
    } // else if
    else if ( tokenValue == "ListVariable" ) {
      
      string keep_id ;
      
      mdata.GetToken( tokenType, tokenValue ) ; // get 'ListVariable'
      
      mdata.GetToken( tokenType, tokenValue ) ; // get '('
      if ( tokenValue != "(" ) throw Syntax_error( tokenValue ) ;
      
      mdata.GetToken( tokenType, tokenValue ) ; // get 'id'
      tokenValue = tokenValue.substr( 1, tokenValue.size() - 2 ) ;
      if ( tokenType != "constant" ) throw Syntax_error( tokenValue ) ;
      else if ( !Check_for_functions( tokenValue, "variable" ) ) throw Defined_error( tokenValue ) ;
      else keep_id = tokenValue ;
      
      mdata.GetToken( tokenType, tokenValue ) ; // get ')'
      if ( tokenValue != ")" ) throw Syntax_error( tokenValue ) ;
      
      mdata.GetToken( tokenType, tokenValue ) ; // get ';'
      if ( tokenValue != ";" ) throw Syntax_error( tokenValue ) ;
      
      ListVariable( keep_id ) ;
      cout << "Statement executed ..." << endl ;
      
    } // else if
    else if ( tokenValue == "ListFunction" ) {
      
      string keep_id ;
      
      mdata.GetToken( tokenType, tokenValue ) ; // get 'ListFunction'
      
      mdata.GetToken( tokenType, tokenValue ) ; // get '('
      if ( tokenValue != "(" ) throw Syntax_error( tokenValue ) ;
      
      mdata.GetToken( tokenType, tokenValue ) ; // get 'id'
      tokenValue = tokenValue.substr( 1, tokenValue.size() - 2 ) ;
      if ( tokenType != "constant" ) throw Syntax_error( tokenValue ) ;
      else if ( !Check_for_functions( tokenValue, "function" ) ) throw Defined_error( tokenValue ) ;
      else keep_id = tokenValue ;
      
      mdata.GetToken( tokenType, tokenValue ) ; // get ')'
      if ( tokenValue != ")" ) throw Syntax_error( tokenValue ) ;
      
      mdata.GetToken( tokenType, tokenValue ) ; // get ';'
      if ( tokenValue != ";" ) throw Syntax_error( tokenValue ) ;
      
      ListFunction( keep_id ) ;
      cout << "Statement executed ..." << endl ;
      
    } // else if
    else if ( tokenValue == "void" || tokenValue == "int" || tokenValue == "char" ||
              tokenValue == "float" || tokenValue == "string" || tokenValue == "bool" ) {
              // VOID Identifier function_definition_without_ID
      
      Definition( command ) ;
      cout << "> " ;
      Define_Name( command ) ;
      
    } // if

    else {
      
      Statement( command ) ;
      cout << "> Statement executed ..." << endl ;
      
    } // else
    
    command.token.clear() ;
    command.func_variable.clear() ;
    return true ;
    
  } // User_input()
  
  void Definition( Command& command ) {
    // definition : VOID Identifier function_definition_without_ID
    // | type_specifier Identifier function_definition_or_declarators
    
    string tokenType, tokenValue ;
    
    mdata.PeekToken( tokenType, tokenValue ) ;
    if ( tokenValue == "void" ) { // VOID Identifier function_definition_without_ID
      
      mdata.GetToken( tokenType, tokenValue ) ;
      command.token.push_back( tokenValue ) ; // push 'void' into vector
      
      mdata.GetToken( tokenType, tokenValue ) ;
      if ( tokenType == "id" ) { // Identifier function_definition_without_ID
        
        command.name = tokenValue ; // store the function's name
        command.is_func = true ;
        command.token.push_back( tokenValue ) ;
        Function_definition_without_ID( command ) ;
        
      } // if
      
      else throw Syntax_error( tokenValue ) ;
      
    } // if

    else { // type_specifier Identifier function_definition_or_declarators
      
      Type_specifier( command ) ;
      
      mdata.GetToken( tokenType, tokenValue ) ;
      if ( tokenType == "id" ) { // Identifier function_definition_or_declarators
        
        command.name = tokenValue ;
        command.token.push_back( tokenValue ) ;
        Function_definition_or_declarators( command ) ;
        
      } // if
      
      else throw Syntax_error( tokenValue ) ;
      
    } // else
    
  } // Definition()
  
  void Type_specifier( Command& command ) {
    // type_specifier : INT | CHAR | FLOAT | STRING | BOOL
    
    string tokenType, tokenValue ;
    
    mdata.GetToken( tokenType, tokenValue ) ;
    if ( tokenValue == "int" || tokenValue == "char" || tokenValue == "float" ||
         tokenValue == "string" || tokenValue == "bool" ) {
      command.token.push_back( tokenValue ) ;
      command.variableType = tokenValue ;
    } // if
    else throw Syntax_error( tokenValue ) ;
    
  } // Type_specifier()
  
  void Function_definition_or_declarators( Command& command ) {
    // function_definition_or_declarators : function_definition_without_ID
    // | rest_of_declarators
    
    string tokenType, tokenValue ;
    
    mdata.PeekToken( tokenType, tokenValue ) ;
    if ( tokenValue == "(" ) {
      Function_definition_without_ID( command ) ;
      command.is_func = true ;
    } // if
    else {
      Rest_of_declarators( command ) ;
      command.is_vari = true ;
    } // else
    
  } // Function_definition_or_declarators()
  
  void Rest_of_declarators( Command& command ) {
    // rest_of_declarators : [ '[' Constant ']' ]
    // { ',' Identifier [ '[' Constant ']' ] } ';'
    
    string tokenType, tokenValue ;
    
    mdata.PeekToken( tokenType, tokenValue ) ;
    if ( tokenValue == "[" ) { // if there is a '['
      
      mdata.GetToken( tokenType, tokenValue ) ;
      command.token.push_back( tokenValue ) ; // push '[' into vector
      
      mdata.GetToken( tokenType, tokenValue ) ;
      if ( tokenType == "constant" ) command.token.push_back( tokenValue ) ; // push constant into vector
      else throw Syntax_error( tokenValue ) ;
      
      mdata.GetToken( tokenType, tokenValue ) ;
      if ( tokenValue == "]" ) command.token.push_back( tokenValue ) ; // push ']' into vector
      else throw Syntax_error( tokenValue ) ;
      
    } // if
    
    do {
      
      mdata.PeekToken( tokenType, tokenValue ) ;
      if ( tokenValue == "," ) {
        
        mdata.GetToken( tokenType, tokenValue ) ;
        command.token.push_back( tokenValue ) ; // push ',' into vector
        
        mdata.GetToken( tokenType, tokenValue ) ;
        if ( tokenType == "id" ) {
          command.token.push_back( tokenValue ) ; // push Identifier into vector
          command.func_variable.push_back( tokenValue ) ;
        } // if
        else throw Syntax_error( tokenValue ) ;
        
        mdata.PeekToken( tokenType, tokenValue ) ;
        if ( tokenValue == "[" ) { // if there is a '['
          
          mdata.GetToken( tokenType, tokenValue ) ;
          command.token.push_back( tokenValue ) ; // push '[' into vector
          
          mdata.GetToken( tokenType, tokenValue ) ;
          if ( tokenType == "constant" ) command.token.push_back( tokenValue ) ; // push constant into vector
          else throw Syntax_error( tokenValue ) ;
          
          mdata.GetToken( tokenType, tokenValue ) ;
          if ( tokenValue == "]" ) command.token.push_back( tokenValue ) ; // push ']' into vector
          else throw Syntax_error( tokenValue ) ;
          
        } // if
        
      } // if
      else if ( tokenValue == ";" ) {
        
        mdata.GetToken( tokenType, tokenValue ) ;
        command.token.push_back( tokenValue ) ; // push ';' into vector
        return ;
        
      } // else if
      else throw Syntax_error( tokenValue ) ;
      
    } while ( true ) ;
    
  } // Rest_of_declarators()
  
  void Function_definition_without_ID( Command& command ) {
    // function_definition_without_ID : '(' [ VOID | formal_parameter_list ] ')' compound_statement
    
    string tokenType, tokenValue ;
    
    mdata.GetToken( tokenType, tokenValue ) ;
    if ( tokenValue == "(" ) command.token.push_back( tokenValue ) ; // push '(' into vector
    else throw Syntax_error( tokenValue ) ;
    
    mdata.PeekToken( tokenType, tokenValue ) ;
    if ( tokenValue == "void" ) command.token.push_back( tokenValue ) ; // push VIOD into vector
    else if ( tokenValue == "int" || tokenValue == "char" || tokenValue == "float" ||
              tokenValue == "string" || tokenValue == "bool" )
      Formal_parameter_list( command ) ;
    
    mdata.GetToken( tokenType, tokenValue ) ;
    if ( tokenValue == ")" ) command.token.push_back( tokenValue ) ; // push ')' into vector
    else throw Syntax_error( tokenValue ) ;
    
    Compound_statement( command ) ;
    
  } // Function_definition_without_ID()
  
  void Formal_parameter_list( Command& command ) {
    // formal_parameter_list : type_specifier [ '&' ] Identifier [ '[' Constant ']' ]
    // { ',' type_specifier [ '&' ] Identifier [ '[' Constant ']' ] }
    
    string tokenType, tokenValue ;
    
    Type_specifier( command ) ;
    
    mdata.PeekToken( tokenType, tokenValue ) ;
    if ( tokenValue == "&" ) { // if there is a '&', push it into vector
      
      mdata.GetToken( tokenType, tokenValue ) ;
      command.token.push_back( tokenValue ) ;
    
    } // if
    
    mdata.GetToken( tokenType, tokenValue ) ;
    if ( tokenType == "id" ) {
      
      command.token.push_back( tokenValue ) ;
      command.func_variable.push_back( tokenValue ) ;
      
    } // if
    else throw Syntax_error( tokenValue ) ; // push Identifier into vector
    
    mdata.PeekToken( tokenType, tokenValue ) ;
    if ( tokenValue == "[" ) { // if there is a '[', push it into vector
      
      mdata.GetToken( tokenType, tokenValue ) ;
      command.token.push_back( tokenValue ) ; // push '[' into vector
      
      mdata.GetToken( tokenType, tokenValue ) ;
      if ( tokenType == "constant" ) command.token.push_back( tokenValue ) ; // push Constant into vector
      else throw Syntax_error( tokenValue ) ;
      
      mdata.GetToken( tokenType, tokenValue ) ;
      if ( tokenValue == "]" ) command.token.push_back( tokenValue ) ; // push ']' into vector
      else throw Syntax_error( tokenValue ) ;
      
    } // if
    
    do {
      
      mdata.PeekToken( tokenType, tokenValue ) ;
      if ( tokenValue == "," ) {
        
        mdata.GetToken( tokenType, tokenValue ) ;
        command.token.push_back( tokenValue ) ; // push ',' into vector
        
        Type_specifier( command ) ;
        
        mdata.PeekToken( tokenType, tokenValue ) ;
        if ( tokenValue == "&" ) { // if there is a '&', push it into vector
          
          mdata.GetToken( tokenType, tokenValue ) ;
          command.token.push_back( tokenValue ) ;
        
        } // if
        
        mdata.GetToken( tokenType, tokenValue ) ;
        if ( tokenType == "id" ) {
          
          command.token.push_back( tokenValue ) ;
          command.func_variable.push_back( tokenValue ) ;
          
        } // if
        else throw Syntax_error( tokenValue ) ; // push Identifier into vector
        
        mdata.PeekToken( tokenType, tokenValue ) ;
        if ( tokenValue == "[" ) { // if there is a '[', push it into vector
          
          mdata.GetToken( tokenType, tokenValue ) ;
          command.token.push_back( tokenValue ) ; // push '[' into vector
          
          mdata.GetToken( tokenType, tokenValue ) ;
          if ( tokenType == "constant" ) command.token.push_back( tokenValue ) ; // push Constant into vector
          else throw Syntax_error( tokenValue ) ;
          
          mdata.GetToken( tokenType, tokenValue ) ;
          if ( tokenValue == "]" ) command.token.push_back( tokenValue ) ; // push ']' into vector
          else throw Syntax_error( tokenValue ) ;
          
        } // if
        
      } // if
      else return ;
      
    } while ( true ) ;
    
  } // Formal_parameter_list()
  
  void Compound_statement( Command& command ) {
    // compound_statement : '{' { declaration | statement } '}'
    
    string tokenType, tokenValue ;
    
    myfunc_declar = true ;
    
    mdata.GetToken( tokenType, tokenValue ) ;
    if ( tokenValue != "{" ) throw Syntax_error( tokenValue ) ;
    else command.token.push_back( tokenValue ) ; // push the first '{' into vector
    
    do {
      
      mdata.PeekToken( tokenType, tokenValue ) ;
      if ( tokenValue == "}" ) {
        
        mdata.GetToken( tokenType, tokenValue ) ;
        command.token.push_back( tokenValue ) ; // push the first '}' into vector
        return ;
        
      } // if
      else if ( tokenValue == "int" || tokenValue == "char" || tokenValue == "float" ||
                tokenValue == "string" || tokenValue == "bool" )
        Declaration( command ) ;
      else Statement( command ) ;
      
    } while ( true ) ; // while
    
  } // Compound_statement()
  
  void Declaration( Command& command ) {
    // declaration : type_specifier Identifier rest_of_declarators
    
    string tokenType, tokenValue ;

    Type_specifier( command ) ;

    mdata.PeekToken( tokenType, tokenValue ) ;
    if ( tokenType == "id" ) {
      
      // Is_IDinList( idinlist_correct, idinlist_tokenValue, tokenValue ) ; // TODO
      
      mdata.GetToken( tokenType, tokenValue ) ;
      command.token.push_back( tokenValue ) ; // push 'Identifier' into vector
      command.func_variable.push_back( tokenValue ) ;
      
      Rest_of_declarators( command ) ;
      
    } // if
    else throw Syntax_error( tokenValue ) ;
    
  } // Declaration()
  
  void Statement( Command& command ) {
    // statement : ';'     // the null statement
    // | expression ';'  /* expression here should not be empty */
    // | RETURN [ expression ] ';'
    // | compound_statement
    // | IF '(' expression ')' statement [ ELSE statement ]
    // | WHILE '(' expression ')' statement
    // | DO statement WHILE '(' expression ')' ';'
    
    string tokenType, tokenValue ;

    mdata.PeekToken( tokenType, tokenValue ) ;
    if ( tokenValue == ";" ) { // ';'     // the null statement
      
      mdata.GetToken( tokenType, tokenValue ) ;
      command.token.push_back( tokenValue ) ; // push the first ';' into vector
      
    } // if
    else if ( tokenValue == "return" ) { // RETURN [ expression ] ';'
      
      mdata.GetToken( tokenType, tokenValue ) ;
      command.token.push_back( tokenValue ) ; // push the "return" into vector
      
      mdata.PeekToken( tokenType, tokenValue ) ;
      if ( tokenValue != ";" ) Exp( command ) ; // [ expression ] ';'
      
      mdata.GetToken( tokenType, tokenValue ) ;
      if ( tokenValue != ";" ) throw Syntax_error( tokenValue ) ;
      else command.token.push_back( tokenValue ) ; // push the ";" into vector
      
    } // else if

    else if ( tokenValue == "{" ) { // compound_statement
      
      Compound_statement( command ) ;
      
    } // else if

    else if ( tokenValue == "if" ) { // IF '(' expression ')' statement [ ELSE statement ]
      
      mdata.GetToken( tokenType, tokenValue ) ;
      command.token.push_back( tokenValue ) ; // push the "if" into vector
      
      mdata.GetToken( tokenType, tokenValue ) ; // '(' expression ')' statement [ ELSE statement ]
      if ( tokenValue != "(" ) throw Syntax_error( tokenValue ) ;
      else command.token.push_back( tokenValue ) ; // push the "(" into vector
      
      Exp( command ) ; // expression ')' statement [ ELSE statement ]
      
      mdata.GetToken( tokenType, tokenValue ) ;
      if ( tokenValue != ")" ) throw Syntax_error( tokenValue ) ;
      else command.token.push_back( tokenValue ) ; // push the ")" into vector
      
      Statement( command ) ; // statement [ ELSE statement ]
      
      mdata.PeekToken( tokenType, tokenValue ) ;
      if ( tokenValue == "else" ) { // [ ELSE statement ]
        
        mdata.GetToken( tokenType, tokenValue ) ;
        command.token.push_back( tokenValue ) ; // push the "else" into vector
        
        Statement( command ) ;
        
      } // if
      else return ; // IF '(' expression ')' statement
      
    } // else if

    else if ( tokenValue == "while" ) { // WHILE '(' expression ')' statement
      
      mdata.GetToken( tokenType, tokenValue ) ;
      command.token.push_back( tokenValue ) ; // push the "while" into vector
      
      mdata.GetToken( tokenType, tokenValue ) ;
      if ( tokenValue != "(" ) throw Syntax_error( tokenValue ) ;
      else command.token.push_back( tokenValue ) ; // push the "(" into vector
      
      Exp( command ) ; // expression ')' statement
      
      mdata.GetToken( tokenType, tokenValue ) ;
      if ( tokenValue != ")" ) throw Syntax_error( tokenValue ) ;
      else command.token.push_back( tokenValue ) ; // push the ")" into vector
        
      Statement( command ) ;
      
    } // else if

    else if ( tokenValue == "do" ) { // DO statement WHILE '(' expression ')' ';'
      
      mdata.GetToken( tokenType, tokenValue ) ; // DO
      command.token.push_back( tokenValue ) ; // push the "do" into vector
      
      Statement( command ) ; // statement WHILE '(' expression ')' ';'
      
      mdata.GetToken( tokenType, tokenValue ) ; // WHILE
      if ( tokenValue != "while" ) throw Syntax_error( tokenValue ) ;
      else command.token.push_back( tokenValue ) ; // push the "while" into vector
      
      mdata.GetToken( tokenType, tokenValue ) ; // '('
      if ( tokenValue != "(" ) throw Syntax_error( tokenValue ) ;
      else command.token.push_back( tokenValue ) ; // push the "(" into vector
      
      Exp( command ) ; // expression ')' ';'
      
      mdata.GetToken( tokenType, tokenValue ) ; // ')'
      if ( tokenValue != ")" ) throw Syntax_error( tokenValue ) ;
      else command.token.push_back( tokenValue ) ; // push the ")" into vector
      
      mdata.GetToken( tokenType, tokenValue ) ; // ;
      if ( tokenValue != ";" ) throw Syntax_error( tokenValue ) ;
      else command.token.push_back( tokenValue ) ; // push the ";" into vector
      
    } // else if

    else { // expression ';'
      
      Exp( command ) ;
      
      mdata.GetToken( tokenType, tokenValue ) ; // ;
      if ( tokenValue != ";" ) throw Syntax_error( tokenValue ) ;
      else command.token.push_back( tokenValue ) ; // push the ";" into vector
      
    } // else
    
  } // Statement()
  
  void Exp( Command& command ) {
    // expression : basic_expression { ',' basic_expression }
    
    string tokenType, tokenValue ;

    Basic_expression( command ) ;
    
    do {
      
      mdata.PeekToken( tokenType, tokenValue ) ;
      
      if ( tokenValue == "," ) {
        
        mdata.GetToken( tokenType, tokenValue ) ;
        command.token.push_back( tokenValue ) ; // push the first ',' into vector
        Basic_expression( command ) ;
        
      } // if
      else return ;
      
    } while ( true ) ;
    
  } // Exp()
  
  void Basic_expression( Command& command ) {
    // basic_expression : Identifier rest_of_Identifier_started_basic_exp
    // | ( PP | MM ) Identifier rest_of_PPMM_Identifier_started_basic_exp
    // | sign { sign } signed_unary_exp romce_and_romloe
    // | ( Constant | '(' expression ')' ) romce_and_romloe
    
    string tokenType, tokenValue ;
    string idinlist_tokenValue ;
    bool is_correct = false ;
    
    mdata.PeekToken( tokenType, tokenValue ) ;
    if ( tokenType == "PP" || tokenType == "MM" ) {
      // ( PP | MM ) Identifier rest_of_PPMM_Identifier_started_basic_exp

      mdata.GetToken( tokenType, tokenValue ) ;
      command.token.push_back( tokenValue ) ; // push PP or MM into vector
      
      mdata.PeekToken( tokenType, tokenValue ) ;
      if ( tokenType != "id" ) throw Syntax_error( tokenValue ) ;
      else if ( !Is_name_in_list( tokenValue, command ) ) throw Defined_error( tokenValue ) ;
      else {
        mdata.GetToken( tokenType, tokenValue ) ;
        command.token.push_back( tokenValue ) ; // push Identifier into vector
      } // else
      
      Rest_of_PPMM_Identifier_started_basic_exp( command ) ;
      
    } // if

    else if ( tokenValue == "+" || tokenValue == "-" || tokenValue == "!" ) {
      // sign { sign } signed_unary_exp romce_and_romloe
      
      
      do {
        
        Sign( command ) ;
        mdata.PeekToken( tokenType, tokenValue ) ;
        
      } while ( tokenValue == "+" || tokenValue == "-" || tokenValue == "!" ) ; // while
      
      Signed_unary_exp( command ) ;
      myis_signed = false ;
        
      Romce_and_romloe( command ) ;
      
    } // else if

    else if ( tokenType == "id" ) {
      // Identifier rest_of_Identifier_started_basic_exp
      
      mdata.GetToken( tokenType, tokenValue ) ;
      if ( tokenType != "id" ) throw Syntax_error( tokenValue ) ;
      else if ( !Is_name_in_list( tokenValue, command ) )
        throw Defined_error( tokenValue ) ;
      else command.token.push_back( tokenValue ) ; // push the Identifier into vector

      Rest_of_Identifier_started_basic_exp( command ) ;
      
    } // else if
    else if ( tokenValue == "cout" || tokenValue == "cin" ) {
      
      mdata.GetToken( tokenType, tokenValue ) ;
      command.token.push_back( tokenValue ) ; // push the 'cin' or 'cout' into vector
      if ( tokenValue == "cin" ) {
        myis_cin = true ;
        myis_cout = false ;
      } // if
      else {
        myis_cout = true ;
        myis_cin = false ;
      } // else
      
      Romce_and_romloe( command ) ;
      
      myis_cin = false ;
      myis_cout = false ;
      
    } // else if
    else if ( tokenValue == "true" || tokenValue == "false" ) {
      
      mdata.GetToken( tokenType, tokenValue ) ;
      command.token.push_back( tokenValue ) ; // push the 'true' or 'false' into vector
      
      Romce_and_romloe( command ) ;
      
    } // else if
    else if ( tokenType == "constant" || tokenValue == "(" ) {
      // ( Constant | '(' expression ')' ) romce_and_romloe
  
      mdata.GetToken( tokenType, tokenValue ) ;
      command.token.push_back( tokenValue ) ; // push the Constant or '(' into vector
      
      if ( tokenValue == "(" ) { // '(' expression ')' romce_and_romloe
        
        Exp( command ) ;
        
        mdata.GetToken( tokenType, tokenValue ) ;
        if ( tokenValue == ")" ) command.token.push_back( tokenValue ) ; // push ')' into vector
        else throw Syntax_error( tokenValue ) ;
        
      } // else if
      
      Romce_and_romloe( command ) ;
      
    } // if

    else is_correct = true ;
    if ( is_correct ) throw Syntax_error( tokenValue ) ;
    
  } // Basic_expression()
  
  void Rest_of_Identifier_started_basic_exp( Command& command ) {
    // rest_of_Identifier_started_basic_exp : [ '[' expression ']' ]
    // ( assignment_operator basic_expression | [ PP | MM ] romce_and_romloe )
    // | '(' [ actual_parameter_list ] ')' romce_and_romloe
    
    string tokenType, tokenValue ;
    
    mdata.PeekToken( tokenType, tokenValue ) ;
    if ( tokenValue == "(" ) { // '(' [ actual_parameter_list ] ')' romce_and_romloe
      
      mdata.GetToken( tokenType, tokenValue ) ;
      command.token.push_back( tokenValue ) ; // push '(' into vector
      
      mdata.PeekToken( tokenType, tokenValue ) ;
      if ( tokenValue != ")" ) Actual_parameter_list( command ) ;
      // [ actual_parameter_list ] ')' romce_and_romloe
      
      mdata.GetToken( tokenType, tokenValue ) ; // ')' romce_and_romloe
      if ( tokenValue != ")" ) throw Syntax_error( tokenValue ) ;
      else command.token.push_back( tokenValue ) ; // push ')' into vector
      
      Romce_and_romloe( command ) ;
      
    } // if
    else { // [ '[' expression ']' ]
           // ( assignment_operator basic_expression | [ PP | MM ] romce_and_romloe )
      
      if ( tokenValue == "[" ) { // [ '[' expression ']' ]
        
        mdata.GetToken( tokenType, tokenValue ) ;
        command.token.push_back( tokenValue ) ; // push '[' into vector
        
        Exp( command ) ;
        
        mdata.GetToken( tokenType, tokenValue ) ;
        if ( tokenValue != "]" ) throw Syntax_error( tokenValue ) ;
        else command.token.push_back( tokenValue ) ; // push ']' into vector
        
        mdata.PeekToken( tokenType, tokenValue ) ;
        
      } // if
      
      if ( tokenType == "PP" || tokenType == "MM" ) { // [ PP | MM ] romce_and_romloe
        
        mdata.GetToken( tokenType, tokenValue ) ;
        command.token.push_back( tokenValue ) ; // push '++' or '--' into vector
        
        Romce_and_romloe( command ) ;
        
      } // if
      else { // assignment_operator basic_expression | romce_and_romloe
        
        mdata.PeekToken( tokenType, tokenValue ) ;
        if ( tokenValue == "=" || tokenValue == "+=" ||
             tokenValue == "-=" || tokenValue == "*=" || tokenValue == "/=" || tokenValue == "%=" ) {
          
          Assignment_operator( command ) ;
          
          Basic_expression( command ) ;
          
        } // if
        else {
         
          Romce_and_romloe( command ) ;
          
        } // else
        
      } // else
      
    } // else
    
  } // Rest_of_Identifier_started_basic_exp()
  
  void Rest_of_PPMM_Identifier_started_basic_exp( Command& command ) {
    // rest_of_PPMM_Identifier_started_basic_exp : [ '[' expression ']' ] romce_and_romloe
    
    string tokenType, tokenValue ;
    
    mdata.PeekToken( tokenType, tokenValue ) ;
    if ( tokenValue == "[" ) {
      
      mdata.GetToken( tokenType, tokenValue ) ;
      command.token.push_back( tokenValue ) ; // push '[' into vector
      
      Exp( command ) ;
      
      mdata.GetToken( tokenType, tokenValue ) ;
      if ( tokenValue == "]" ) command.token.push_back( tokenValue ) ; // push ']' into vector
      else throw Syntax_error( tokenValue ) ;
      
    } // if
    
    Romce_and_romloe( command ) ;
    
  } // Rest_of_PPMM_Identifier_started_basic_exp()
  
  void Sign( Command& command ) { // TODO
    // sign : '+' | '-' | '!'
    
    string tokenType, tokenValue ;
    
    mdata.GetToken( tokenType, tokenValue ) ;
    if ( tokenValue == "+" || tokenValue == "-" || tokenValue == "!" )
      command.token.push_back( tokenValue ) ; // push into vector
    else throw Syntax_error( tokenValue ) ;
    
    myis_signed = true ;
    
  } // Sign()
  
  void Actual_parameter_list( Command& command ) {
    // actual_parameter_list : basic_expression { ',' basic_expression }
    
    string tokenType, tokenValue ;
    
    Basic_expression( command ) ;
    
    do {
      
      mdata.PeekToken( tokenType, tokenValue ) ;
      if ( tokenValue == "," ) {
        
        mdata.GetToken( tokenType, tokenValue ) ;
        command.token.push_back( tokenValue ) ; // push ',' into vector
        
        Basic_expression( command ) ;
        
      } // if
      else return ;
      
    } while ( true ) ; // while
    
  } // Actual_parameter_list()
  
  void Assignment_operator( Command& command ) { // TODO
    // assignment_operator : '=' | TE | DE | RE | PE | ME
    
    string tokenType, tokenValue ;
    
    mdata.GetToken( tokenType, tokenValue ) ;
    if ( tokenValue == "=" || tokenValue == "+=" ||
         tokenValue == "-=" || tokenValue == "*=" || tokenValue == "/=" || tokenValue == "%=" ) {
      
      command.token.push_back( tokenValue ) ;
      
    } // if
    else throw Syntax_error( tokenValue ) ;
    
  } // Assignment_operator()
  
  void Romce_and_romloe( Command& command ) {
    // romce_and_romloe : rest_of_maybe_logical_OR_exp [ '?' basic_expression ':' basic_expression ]
    
    string tokenType, tokenValue ;
    
    if ( !myis_cin && !myis_cout ) // this is for prevent happning cout = 5, cout + 5
      Rest_of_maybe_logical_OR_exp( command ) ;
    else
      Rest_of_maybe_shift_exp( command ) ;
    
    mdata.PeekToken( tokenType, tokenValue ) ;
    if ( tokenValue == "?" ) { // [ '?' basic_expression ':' basic_expression ]
    
      mdata.GetToken( tokenType, tokenValue ) ;
      command.token.push_back( tokenValue ) ; // push '?' into vector
      
      Basic_expression( command ) ;
      
      mdata.GetToken( tokenType, tokenValue ) ;
      if ( tokenValue != ":" ) throw Syntax_error( tokenValue ) ; // ':' basic_expression
      else command.token.push_back( tokenValue ) ; // push ':' into vector
      
      Basic_expression( command ) ;
      
    } // if
    else return ;
    
  } // Romce_and_romloe()
  
  void Rest_of_maybe_logical_OR_exp( Command& command ) {
    // rest_of_maybe_logical_OR_exp : rest_of_maybe_logical_AND_exp { OR maybe_logical_AND_exp }
    
    string tokenType, tokenValue ;
    
    Rest_of_maybe_logical_AND_exp( command ) ;
    
    do {
      
      mdata.PeekToken( tokenType, tokenValue ) ;
      if ( tokenValue == "||" ) {
        
        mdata.GetToken( tokenType, tokenValue ) ;
        command.token.push_back( tokenValue ) ; // push '||' into vector
        
        Maybe_logical_AND_exp( command ) ;
        
      } // if
      else return ;
      
    } while ( true ) ; // while
    
  } // Rest_of_maybe_logical_OR_exp()
  
  void Maybe_logical_AND_exp( Command& command ) {
    // maybe_logical_AND_exp : maybe_bit_OR_exp { AND maybe_bit_OR_exp }
    
    string tokenType, tokenValue ;
    
    Maybe_bit_OR_exp( command ) ;
    
    do {
      
      mdata.PeekToken( tokenType, tokenValue ) ;
      if ( tokenValue == "&&" ) {
        
        mdata.GetToken( tokenType, tokenValue ) ;
        command.token.push_back( tokenValue ) ; // push '&&' into vector
        
        Maybe_bit_OR_exp( command ) ;
        
      } // if
      else return ;
      
    } while ( true ) ; // while
    
  } // Maybe_logical_AND_exp()
  
  void Rest_of_maybe_logical_AND_exp( Command& command ) {
    // rest_of_maybe_logical_AND_exp : rest_of_maybe_bit_OR_exp { AND maybe_bit_OR_exp }
    
    string tokenType, tokenValue ;
    
    Rest_of_maybe_bit_OR_exp( command ) ;
    
    do {
      
      mdata.PeekToken( tokenType, tokenValue ) ;
      if ( tokenValue == "&&" ) {
        
        mdata.GetToken( tokenType, tokenValue ) ;
        command.token.push_back( tokenValue ) ; // push '&&' into vector
        
        Maybe_bit_OR_exp( command ) ;
        
      } // if
      else return ;
      
    } while ( true ) ; // while
    
  } // Rest_of_maybe_logical_AND_exp()
  
  void Maybe_bit_OR_exp( Command& command ) {
    // maybe_bit_OR_exp : maybe_bit_ex_OR_exp { '|' maybe_bit_ex_OR_exp }
    
    string tokenType, tokenValue ;
    
    Maybe_bit_ex_OR_exp( command ) ;
    
    do {
      
      mdata.PeekToken( tokenType, tokenValue ) ;
      if ( tokenValue == "|" ) {
        
        mdata.GetToken( tokenType, tokenValue ) ;
        command.token.push_back( tokenValue ) ;
        
        Maybe_bit_ex_OR_exp( command ) ;
        
      } // if
      else return ;
      
    } while ( true ) ; // while
    
  } // Maybe_bit_OR_exp()
  
  void Rest_of_maybe_bit_OR_exp( Command& command ) {
    // rest_of_maybe_bit_OR_exp : rest_of_maybe_bit_ex_OR_exp { '|' maybe_bit_ex_OR_exp }
    
    string tokenType, tokenValue ;
    
    Rest_of_maybe_bit_ex_OR_exp( command ) ;
    
    do {
      
      mdata.PeekToken( tokenType, tokenValue ) ;
      if ( tokenValue == "|" ) {
        
        mdata.GetToken( tokenType, tokenValue ) ;
        command.token.push_back( tokenValue ) ; // push '|' into vector
        
        Maybe_bit_ex_OR_exp( command ) ;
        
      } // if
      else return ;
      
    } while ( true ) ; // while
    
  } // Rest_of_maybe_bit_OR_exp()
  
  void Maybe_bit_ex_OR_exp( Command& command ) {
    // maybe_bit_ex_OR_exp : maybe_bit_AND_exp { '^' maybe_bit_AND_exp }
    
    string tokenType, tokenValue ;
    
    Maybe_bit_AND_exp( command ) ;
    
    do {
      
      mdata.PeekToken( tokenType, tokenValue ) ;
      if ( tokenValue == "^" ) {
        
        mdata.GetToken( tokenType, tokenValue ) ;
        command.token.push_back( tokenValue ) ; // push ^ into vector
        
        Maybe_bit_AND_exp( command ) ;
        
      } // if
      else return ;
      
    } while ( true ) ; // while
    
  } // Maybe_bit_ex_OR_exp()
  
  void Rest_of_maybe_bit_ex_OR_exp( Command& command ) {
    // rest_of_maybe_bit_ex_OR_exp : rest_of_maybe_bit_AND_exp { '^' maybe_bit_AND_exp }
    
    string tokenType, tokenValue ;
    
    Rest_of_maybe_bit_AND_exp( command ) ;
    
    do {
      
      mdata.PeekToken( tokenType, tokenValue ) ;
      if ( tokenValue == "^" ) {
        
        mdata.GetToken( tokenType, tokenValue ) ;
        command.token.push_back( tokenValue ) ; // push ^ into vector
        
        Maybe_bit_AND_exp( command ) ;
        
      } // if
      else return ;
      
    } while ( true ) ; // while
    
  } // Rest_of_maybe_bit_ex_OR_exp()
  
  void Maybe_bit_AND_exp( Command& command ) {
    // maybe_bit_AND_exp : maybe_equality_exp { '&' maybe_equality_exp }
    
    string tokenType, tokenValue ;
    
    Maybe_equality_exp( command ) ;
    
    do {
      
      mdata.PeekToken( tokenType, tokenValue ) ;
      if ( tokenValue == "&" ) {
        
        mdata.GetToken( tokenType, tokenValue ) ;
        command.token.push_back( tokenValue ) ; // push & into vector
        
        Maybe_equality_exp( command ) ;
        
      } // if
      else return ;
      
    } while ( true ) ; // while
    
  } // Maybe_bit_AND_exp()
  
  void Rest_of_maybe_bit_AND_exp( Command& command ) {
    // rest_of_maybe_bit_AND_exp : rest_of_maybe_equality_exp { '&' maybe_equality_exp }
    
    string tokenType, tokenValue ;
    
    Rest_of_maybe_equality_exp( command ) ;
    do {
      
      mdata.PeekToken( tokenType, tokenValue ) ;
      if ( tokenValue == "&" ) {
        
        mdata.GetToken( tokenType, tokenValue ) ;
        command.token.push_back( tokenValue ) ; // push & into vector
        
        Maybe_equality_exp( command ) ;
        
      } // if
      else return ;
      
    } while ( true ) ; // while
    
  } // Rest_of_maybe_bit_AND_exp()
  
  void Maybe_equality_exp( Command& command ) {
    // maybe_equality_exp : maybe_relational_exp { ( EQ | NEQ ) maybe_relational_exp}
    
    string tokenType, tokenValue ;
    
    Maybe_relational_exp( command ) ;
    do {
      
      mdata.PeekToken( tokenType, tokenValue ) ;
      if ( tokenValue == "==" || tokenValue == "!=" ) {
        
        mdata.GetToken( tokenType, tokenValue ) ;
        command.token.push_back( tokenValue ) ; // push "==" or "!=" into vector
        
        Maybe_relational_exp( command ) ;
        
      } // if
      else return ;
      
    } while ( true ) ; // while
    
  } // Maybe_equality_exp()
  
  void Rest_of_maybe_equality_exp( Command& command ) {
    // rest_of_maybe_equality_exp : rest_of_maybe_relational_exp
    // { ( EQ | NEQ ) maybe_relational_exp }
    
    string tokenType, tokenValue ;
    
    Rest_of_maybe_relational_exp( command ) ;
    
    do {
      
      mdata.PeekToken( tokenType, tokenValue ) ;
      if ( tokenValue == "==" || tokenValue == "!=" ) {
        
        mdata.GetToken( tokenType, tokenValue ) ;
        command.token.push_back( tokenValue ) ; // push "==" or "!=" into vector
        
        Maybe_relational_exp( command ) ;
        
      } // if
      else return ;
      
    } while ( true ) ; // while
  } // Rest_of_maybe_equality_exp()
  
  void Maybe_relational_exp( Command& command ) {
    // maybe_relational_exp : maybe_shift_exp { ( '<' | '>' | LE | GE ) maybe_shift_exp }
    
    string tokenType, tokenValue ;
    
    Maybe_shift_exp( command ) ;
    
    do {
      
      mdata.PeekToken( tokenType, tokenValue ) ;
      if ( tokenValue == "<=" || tokenValue == ">=" || tokenValue == "<" || tokenValue == ">" ) {
        
        mdata.GetToken( tokenType, tokenValue ) ;
        command.token.push_back( tokenValue ) ; // push "<=", ">=", "<" or ">" into vector
        
        Maybe_shift_exp( command ) ;
        
      } // if
      else return ;
      
    } while ( true ) ; // while
    
  } // Maybe_relational_exp()
  
  void Rest_of_maybe_relational_exp( Command& command ) {
    // rest_of_maybe_relational_exp : rest_of_maybe_shift_exp
    // { ( '<' | '>' | LE | GE ) maybe_shift_exp }
    
    string tokenType, tokenValue ;
    
    Rest_of_maybe_shift_exp( command ) ;
    
    do {
      
      mdata.PeekToken( tokenType, tokenValue ) ;
      if ( tokenValue == "<=" || tokenValue == ">=" || tokenValue == "<" || tokenValue == ">" ) {
        
        mdata.GetToken( tokenType, tokenValue ) ;
        command.token.push_back( tokenValue ) ; // push "<=", ">=", "<" or ">" into vector
        
        Maybe_shift_exp( command ) ;
        
      } // if
      else return ;
      
    } while ( true ) ; // while
    
  } // Rest_of_maybe_relational_exp()
  
  void Maybe_shift_exp( Command& command ) {
    // maybe_shift_exp : maybe_additive_exp { ( LS | RS ) maybe_additive_exp }
    
    string tokenType, tokenValue ;
    
    Maybe_additive_exp( command ) ;
    
    do {
      
      mdata.PeekToken( tokenType, tokenValue ) ;
      if ( tokenValue == ">>" || tokenValue == "<<" ) {
        
        mdata.GetToken( tokenType, tokenValue ) ;
        
        command.token.push_back( tokenValue ) ; // push "<<" or ">>" into vector
        
        Maybe_additive_exp( command ) ;
        
      } // if
      else return ;
      
    } while ( true ) ; // while
    
  } // Maybe_shift_exp()
  
  void Rest_of_maybe_shift_exp( Command& command ) {
    // rest_of_maybe_shift_exp : rest_of_maybe_additive_exp { ( LS | RS ) maybe_additive_exp }
    
    string tokenType, tokenValue ;
    
    if ( !myis_cin && !myis_cout )
      Rest_of_maybe_additive_exp( command ) ;
    
    do {
      
      mdata.PeekToken( tokenType, tokenValue ) ;
      if ( tokenValue == ">>" || tokenValue == "<<" ) {
        
        mdata.GetToken( tokenType, tokenValue ) ;
        
        if ( myis_cin && ( tokenValue != ">>" && tokenValue != ";" ) ) throw Syntax_error( tokenValue ) ;
        else if ( myis_cout && ( tokenValue != "<<" && tokenValue != ";" ) )
          throw Syntax_error( tokenValue ) ;
        
        command.token.push_back( tokenValue ) ; // push "<<" or ">>" into vector
        
        Maybe_additive_exp( command ) ;
        
      } // if
      else return ;
      
    } while ( true ) ; // while
    
  } // Rest_of_maybe_shift_exp()
  
  void Maybe_additive_exp( Command& command ) {
    // maybe_additive_exp : maybe_mult_exp { ( '+' | '-' ) maybe_mult_exp }
    
    string tokenType, tokenValue ;
    
    Maybe_mult_exp( command ) ;
    
    do {
      
      mdata.PeekToken( tokenType, tokenValue ) ;
      if ( tokenValue == "+" || tokenValue == "-" ) {
        
        mdata.GetToken( tokenType, tokenValue ) ;
        command.token.push_back( tokenValue ) ; // push "+" or "-" into vector
        
        Maybe_mult_exp( command ) ;
        
      } // if
      else return ;
      
    } while ( true ) ; // while
    
  } // Maybe_additive_exp()
  
  void Rest_of_maybe_additive_exp( Command& command ) {
    // rest_of_maybe_additive_exp : rest_of_maybe_mult_exp { ( '+' | '-' ) maybe_mult_exp }
    
    string tokenType, tokenValue ;
    
    Rest_of_maybe_mult_exp( command ) ;
    do {
      
      mdata.PeekToken( tokenType, tokenValue ) ;
      if ( tokenValue == "+" || tokenValue == "-" ) {
        
        mdata.GetToken( tokenType, tokenValue ) ;
        command.token.push_back( tokenValue ) ; // push "+" or "-" into vector
        
        Maybe_mult_exp( command ) ;
        
      } // if
      else return ;
      
    } while ( true ) ; // while
    
  } // Rest_of_maybe_additive_exp()
  
  void Maybe_mult_exp( Command& command ) {
    // maybe_mult_exp : unary_exp rest_of_maybe_mult_exp
    
    Unary_exp( command ) ;
      
    Rest_of_maybe_mult_exp( command ) ;
    
  } // Maybe_mult_exp()
  
  void Rest_of_maybe_mult_exp( Command& command ) {
    // rest_of_maybe_mult_exp : { ( '*' | '/' | '%' ) unary_exp }  /* could be empty ! */
    
    string tokenValue, tokenType ;
    
    do {
      
      mdata.PeekToken( tokenType, tokenValue ) ;
    
      if ( tokenValue == "*" || tokenValue == "/" || tokenValue == "%" ) {
        
        mdata.GetToken( tokenType, tokenValue ) ;
        command.token.push_back( tokenValue ) ; // push '*', '/' or '%' into vector
        
        Unary_exp( command ) ;
        
      } // if
      else return ;
      
    } while ( true ) ; // do while
    
  } // Rest_of_maybe_mult_exp()
  
  void Unary_exp( Command& command ) {
    // unary_exp : sign { sign } signed_unary_exp | unsigned_unary_exp
    // | ( PP | MM ) Identifier [ '[' expression ']' ]
    
    string tokenType, tokenValue ;
    
    mdata.PeekToken( tokenType, tokenValue ) ;
    if ( tokenType == "PP" || tokenType == "MM" ) { // ( PP | MM ) Identifier [ '[' expression ']' ]
      
      mdata.GetToken( tokenType, tokenValue ) ;
      command.token.push_back( tokenValue ) ; // push PP or MM into vector
      
      mdata.PeekToken( tokenType, tokenValue ) ; // Identifier [ '[' expression ']' ]
      if ( tokenType != "id" ) throw Syntax_error( tokenValue ) ;
      else if ( !Is_name_in_list( tokenValue, command ) ) throw Defined_error( tokenValue ) ;
      else {
        
        mdata.GetToken( tokenType, tokenValue ) ;
        command.token.push_back( tokenValue ) ; // push 'Identifier' into vector
        
      } // else
      
      mdata.PeekToken( tokenType, tokenValue ) ;
      if ( tokenValue == "[" ) { // [ '[' expression ']' ]
        
        mdata.GetToken( tokenType, tokenValue ) ;
        command.token.push_back( tokenValue ) ; // push '[' into vector

        Exp( command ) ;
        
        mdata.GetToken( tokenType, tokenValue ) ;
        if ( tokenValue != "]" ) throw Syntax_error( tokenValue ) ;
        else command.token.push_back( tokenValue ) ; // push ']' into vector
        
      } // if
      
    } // if

    else if ( tokenValue == "+" || tokenValue == "-" || tokenValue == "!" ) {
      // sign { sign } signed_unary_exp
      
      do { // TODO
        
        Sign( command ) ;
        mdata.PeekToken( tokenType, tokenValue ) ;
        
      } while ( tokenValue == "+" || tokenValue == "-" || tokenValue == "!" ) ;
      
      Signed_unary_exp( command ) ;
      myis_signed = false ;
      
    } // else if

    else { // unsigned_unary_exp
      
      Unsigned_unary_exp( command ) ;
      
    } // else
    
  } // Unary_exp()
  
  void Signed_unary_exp( Command& command ) {
    // signed_unary_exp : Identifier [ '(' [ actual_parameter_list ] ')' | '[' expression ']' ]
    // | Constant
    // | '(' expression ')'

    string tokenType, tokenValue ;
    string idinlist_tokenValue ;
    bool is_correct = false ;
    
    mdata.GetToken( tokenType, tokenValue ) ;
    if ( tokenType == "constant" ) { // Constant
      
      if ( !myis_signed ) command.token.push_back( tokenValue ) ; // push constant into vector
      else command.token.back() = command.token.back() + tokenValue ;
      
    } // if
    else if ( tokenValue == "true" || tokenValue == "false" ) {
      
      command.token.push_back( tokenValue ) ; // push constant into vector
      
    } // else if

    else if ( tokenType == "id" ) { // Identifier [ '(' [ actual_parameter_list ] ')' | '[' expression ']' ]
      
      if ( !Is_name_in_list( tokenValue, command ) ) throw Defined_error( tokenValue ) ;
      command.token.push_back( tokenValue ) ; // push Identifier into vector
      
      mdata.PeekToken( tokenType, tokenValue ) ;
      if ( tokenValue == "(" ) { // '(' [ actual_parameter_list ] ')'
        
        mdata.GetToken( tokenType, tokenValue ) ;
        command.token.push_back( tokenValue ) ; // push '(' into vector
        
        mdata.PeekToken( tokenType, tokenValue ) ;
        if ( tokenValue != ")" ) Actual_parameter_list( command ) ; // [ actual_parameter_list ]
        
        mdata.GetToken( tokenType, tokenValue ) ;
        if ( tokenValue == ")" ) command.token.push_back( tokenValue ) ; // push ')' into vector
        else throw Syntax_error( tokenValue ) ;
        
      } // if
      else if ( tokenValue == "[" ) { // '[' expression ']'
        
        mdata.GetToken( tokenType, tokenValue ) ;
        command.token.push_back( tokenValue ) ; // push '[' into vector
        
        Exp( command ) ;
        
        mdata.GetToken( tokenType, tokenValue ) ;
        if ( tokenValue == "]" ) command.token.push_back( tokenValue ) ; // push ']' into vector
        else throw Syntax_error( tokenValue ) ;
        
      } // else if
      
    } // else if

    else if ( tokenValue == "(" ) { // '(' expression ')'
      
      command.token.push_back( tokenValue ) ; // push "(" into vector
      
      Exp( command ) ;
      
      mdata.GetToken( tokenType, tokenValue ) ;
      if ( tokenValue == ")" ) command.token.push_back( tokenValue ) ; // push ')' into vector
      else throw Syntax_error( tokenValue ) ;
      
    } // else if
    
    else is_correct = true ;
    if ( is_correct ) throw Syntax_error( tokenValue ) ;
      // if this token don't belong to above, then it is Unexpected token
    
  } // Signed_unary_exp()
  
  void Unsigned_unary_exp( Command& command ) {
    // unsigned_unary_exp
    // : Identifier [ '(' [ actual_parameter_list ] ')' | [ '[' expression ']' ] [ ( PP | MM ) ] ]
    // | Constant
    // | '(' expression ')'
    
    string tokenType, tokenValue ;
    bool is_correct = false ;
    
    mdata.GetToken( tokenType, tokenValue ) ;
    if ( tokenType == "constant" ) {
      
      command.token.push_back( tokenValue ) ; // push constant into vector
      
    } // if
    else if ( tokenValue == "true" || tokenValue == "false" ) {
      
      command.token.push_back( tokenValue ) ; // push constant into vector
      
    } // else if
    else if ( tokenType == "id" ) {
      // Identifier [ '(' [ actual_parameter_list ] ')' | [ '[' expression ']' ] [ ( PP | MM ) ] ]
      
      if ( !Is_name_in_list( tokenValue, command ) ) throw Defined_error( tokenValue ) ;
      command.token.push_back( tokenValue ) ; // push Identifier into vector
      
      mdata.PeekToken( tokenType, tokenValue ) ;
      if ( tokenValue == "(" ) { // '(' [ actual_parameter_list ] ')'
        
        mdata.GetToken( tokenType, tokenValue ) ;
        command.token.push_back( tokenValue ) ; // push '(' into vector
        
        mdata.PeekToken( tokenType, tokenValue ) ;
        if ( tokenValue != ")" ) Actual_parameter_list( command ) ; // [ actual_parameter_list ]
        
        mdata.GetToken( tokenType, tokenValue ) ;
        if ( tokenValue == ")" ) command.token.push_back( tokenValue ) ; // push ')' into vector
        else throw Syntax_error( tokenValue ) ;
        
        return ;
        
      } // if
      else if ( tokenValue == "[" ) { // '[' expression ']' [ ( PP | MM ) ]
        
        mdata.GetToken( tokenType, tokenValue ) ;
        command.token.push_back( tokenValue ) ; // push '[' into vector
        
        Exp( command ) ;
        
        mdata.GetToken( tokenType, tokenValue ) ;
        if ( tokenValue == "]" ) command.token.push_back( tokenValue ) ; // push ']' into vector
        else throw Syntax_error( tokenValue ) ;
        
      } // else if
      
      mdata.PeekToken( tokenType, tokenValue ) ;
      if ( tokenType == "PP" || tokenType == "MM" ) { // [ ( PP | MM ) ]
        
        mdata.GetToken( tokenType, tokenValue ) ;
        command.token.push_back( tokenValue ) ;
        
      } // if
      else return ;
      
    } // else if
    else if ( tokenValue == "(" ) { // '(' expression ')'
      
      command.token.push_back( tokenValue ) ; // push '(' into vector
      
      Exp( command ) ;
      
      mdata.GetToken( tokenType, tokenValue ) ;
      if ( tokenValue == ")" ) command.token.push_back( tokenValue ) ; // push ')' into vector
      else throw Syntax_error( tokenValue ) ;
      
    } // else if
    
    else is_correct = true ;
    if ( is_correct ) throw Syntax_error( tokenValue ) ;
      // if this token don't belong to above, then it is Unexpected token
    
  } // Unsigned_unary_exp()
  
  void Define_Name( Command &command ) {
    
    vector<Command>::iterator temp ;
    bool flag = false ;
    
    for ( temp = myCommand_list.begin() ; temp != myCommand_list.end() ; ++ temp ) {
      
      if ( temp->name == command.name && !flag ) {
        
        myCommand_list.erase( temp ) ;
        
        myCommand_list.push_back( command ) ; // add new one into vector
        if ( command.is_func ) cout << "New definition of " << command.name << "() entered ..." << endl ;
        else cout << "New definition of " << command.name << " entered ..." << endl ;
        flag = true ;
        
      } // if
      
    } // for
    
    if ( !flag ) {
      myCommand_list.push_back( command ) ;
      if ( command.is_func ) cout << "Definition of " << command.name << "() entered ..." << endl ;
      else cout << "Definition of " << command.name << " entered ..." << endl ;
      // there is no the same one
      // just add into vector
    } // if
    
    if ( !myfunc_declar && !command.func_variable.empty() ) {
      
      Command rest_of_id ;
      
      rest_of_id.is_func = false ;
      rest_of_id.is_vari = true ;
      rest_of_id.name = command.func_variable.front() ;
      rest_of_id.variableType = command.variableType ;
      command.func_variable.erase( command.func_variable.begin() ) ;
      rest_of_id.func_variable.assign( command.func_variable.begin(), command.func_variable.end() ) ;
      rest_of_id.token.assign( command.token.begin(), command.token.end() ) ;
      
      Define_Name( rest_of_id ) ;
      
    } // if
    
  } // Define_Name()
  
  void Sort_name( vector<string>& name_list ) {
    
    for ( int i = 0 ; i < ( int ) name_list.size() ; i ++ ) {
      
      for ( int j = i + 1 ; j < ( int ) name_list.size() ; j ++ ) {
        
        if ( name_list.at( i ) > name_list.at( j ) ) {
          
          string temp ;
          temp = name_list.at( i ) ;
          name_list.at( i ) = name_list.at( j ) ;
          name_list.at( j ) = temp ;
          
        } // if
        
      } // for
      
    } // for
    
  } // Sort_name()
  
  bool Check_for_functions( string name, string func_or_vari ) {
    
    for ( int i = 0, len = ( int ) myCommand_list.size() ; i < len ; i ++ ) {
      
      if ( myCommand_list.at( i ).name == name ) {
        
        if ( func_or_vari == "variable" && myCommand_list.at( i ).is_vari ) {
          
          return true ;
          
        } // if
        else if ( func_or_vari == "function" && myCommand_list.at( i ).is_func ) {
          
          return true ;
          
        } // else if
        
      } // if
      
    } // for
    
    return false ;
    
  } // end Check_for_functions()
  
  bool Is_name_in_list( string name, Command command ) {
    
    for ( int i = 0, len = ( int ) myCommand_list.size() ; i < len ; i ++ ) {
      
      if ( myCommand_list.at( i ).name == name )
      {
        
        int keep_line = gline ;
        string tokenType, tokenValue ;
        
        if ( myCommand_list.at( i ).is_func ) {
          
          mdata.PeekToken( tokenType, tokenValue ) ;
          if ( tokenValue != "(" ) {
            
            if ( gline != keep_line )
              for ( int cc = 0 ; cc < gline - keep_line ; cc ++ )
                cin.putback( '\n' ) ;
              gline = keep_line ;
             
            throw Defined_error( name ) ;
          } // if
          
          return true ;
          
        } // if
        else {
          
          mdata.PeekToken( tokenType, tokenValue ) ;
          if ( tokenValue == "(" ) {
            
            if ( gline != keep_line )
              for ( int cc = 0 ; cc < gline - keep_line ; cc ++ )
                cin.putback( '\n' ) ;
              gline = keep_line ;
            
            throw Defined_error( name ) ;
          } // if
          
          return true ;
          
        } // else
        
      } // if
      
    } // for
    
    if ( !myfunc_declar ) return false ;
    
    for ( int j = 0 ; j < ( int ) command.func_variable.size() ; j ++ ) {
      
      if ( command.func_variable.at( j ) == name ) {
        
        return true ;
        
      } // if
      
    } // for
    
    return false ;
    
  } // Is_name_in_list()
  
  void ListAllFunctions() {
    vector<Command>::iterator temp ;
    vector<string> name_list ;

    for ( temp = myCommand_list.begin() ; temp != myCommand_list.end() ; ++ temp ) {
      
      if ( temp->is_func ) name_list.push_back( temp->name ) ;
      
    } // for
    
    cout << "> " ;
    
    Sort_name( name_list ) ;
    for ( int i = 0 ; i < ( int ) name_list.size() ; i ++ ) {
      
      cout << name_list.at( i ) << "()" << endl ;
      
    } // for
    
  } // ListAllFunctions()

  void ListAllVariables() {
    vector<Command>::iterator temp ;
    vector<string> name_list ;

    for ( temp = myCommand_list.begin() ; temp != myCommand_list.end() ; ++ temp ) {
      
      if ( temp->is_vari ) name_list.push_back( temp->name ) ;
      
    } // for
    
    cout << "> " ;
    
    Sort_name( name_list ) ;
    for ( int i = 0 ; i < ( int ) name_list.size() ; i ++ ) {
      
      cout << name_list.at( i ) << endl ;
      
    } // for
    
  } // ListAllVariables()
  
  
  void ListFunction( string target ) {
    
    cout << "> " ;

    int quote_num = 0, big_quote_num = 0 ; // 括號數量，只有等於 0 時才會換行
    int space = 0 ; // 紀錄開頭空格數目
    bool need_count_quote = false ;
    bool finish_for = false, is_head = false ;
    bool need_new_line = false ;
    bool do_while = false ;

    int t1 = ( int ) myCommand_list.size() ;
    for ( int i = 0 ; i < t1 && finish_for != true ; i ++ ) { // 找到function位置，紀錄為 i

      if ( myCommand_list.at( i ).name == target ) {
        finish_for = true ;

        int t2 = ( int ) myCommand_list.at( i ).token.size() ; // 計算總token數目，為 t2
        for ( int j = 0 ; j < t2 ; j ++ ) { // 開始輸出function內容

          if ( myCommand_list.at( i ).token[j] == "}" ) { // 遇到 '}' 開頭空格-2
            space -= 2 ;
          } // if

          if ( is_head || need_new_line )  { // 輸出每行開頭之tab
            for ( int g = space ; g > 0 ; g -- ) {
              cout << " " ;
            } // for

            if ( need_new_line ) {
              need_new_line = false ;
              space -= 2 ;
            } // if

            is_head = false ;
          } // if


          if ( myCommand_list.at( i ).token[j] == "do" ) {
            do_while = true ;
          } // if // 出現 do while ，特殊處理
          else if ( myCommand_list.at( i ).token[j] == "if" ||
                    ( myCommand_list.at( i ).token[j] == "while" && !do_while ) ) {
            need_count_quote = true ;
          } // else if // 需要判斷是否為沒有大括號的 if || while

          if ( need_count_quote ) { // 只要碰到 if || while 後，開始計算括號數量
            if ( myCommand_list.at( i ).token[j] == "(" ) quote_num += 1 ;
            else if ( myCommand_list.at( i ).token[j] == ")" ) quote_num -= 1 ;
          } // if

          if ( do_while ) { // 只要碰到 do while 後，開始計算(大)括號數量
            if ( myCommand_list.at( i ).token[j] == "{" ) big_quote_num += 1 ;
            else if ( myCommand_list.at( i ).token[j] == "}" ) big_quote_num -= 1 ;
          } // if
          // ----------------------------------------------
          cout << myCommand_list.at( i ).token[j] ; // 輸出token
          // ----------------------------------------------
          // 控制換行
          if ( myCommand_list.at( i ).token[j] == ";" ||
               myCommand_list.at( i ).token[j] == "{" ) { // 遇到 ";" 和 "{" 必換行
            if ( myCommand_list.at( i ).token[j] == "{" ) space += 2 ;
            cout << endl ; // 輸出token
            is_head = true ;
          } // if
          else if ( myCommand_list.at( i ).token[j] == "}" && !do_while ) { // "}" 部分
            cout << endl ; // 輸出token
            is_head = true ;
          } // else if
          else if ( myCommand_list.at( i ).token[j] == "else" &&
                    myCommand_list.at( i ).token[j+1] != "{" ) { // 遇到 else 做判斷
            cout << endl ;
            need_new_line = true ;
            space += 2 ;
          } // else if
          else if ( need_count_quote && quote_num == 0 &&
                    ( myCommand_list.at( i ).token[j] != "if" &&
                      myCommand_list.at( i ).token[j] != "while" ) ) { // 遇到 if 或 while 做判斷
            if ( myCommand_list.at( i ).token[j+1] != "{" &&
                 myCommand_list.at( i ).token[j+1] != ";" ) {
              cout << endl ;
              need_new_line = true ;
              space += 2 ;
            } // if
            else {
              cout << " " ;
            } // else
            
            need_count_quote = false ;
          } // else if
          else if ( do_while && big_quote_num == 0 &&
                    myCommand_list.at( i ).token[j] == "}" ) { // 遇到 do 做判斷
            cout << " " ;
            
            do_while = false ;
          } // else if

          // 控制空白
          else {
            if ( myCommand_list.at( i ).token[j+1] == "++" || myCommand_list.at( i ).token[j+1] == "--" ||
                 ( myCommand_list.at( i ).token[j] == "++" && myCommand_list.at( i ).token[j+1] != ";" ) ||
                 ( myCommand_list.at( i ).token[j] == "--" && myCommand_list.at( i ).token[j+1] != ";" ) ||
                 myCommand_list.at( i ).token[j+1] == "[" || myCommand_list.at( i ).token[j+1] == "," ||
                 myCommand_list.at( i ).token[j] == "!" || myCommand_list.at( i ).token[j] == "&" ) {
              ; // 不給空白
            } // if
            else if ( ( myCommand_list.at( i ).token[j] != "if" &&
                        myCommand_list.at( i ).token[j] != "while" &&
                        myCommand_list.at( i ).token[j] != "?" &&
                        myCommand_list.at( i ).token[j] != ":" &&
                        myCommand_list.at( i ).token[j] != "+" &&
                        myCommand_list.at( i ).token[j] != "-" &&
                        myCommand_list.at( i ).token[j] != "*" &&
                        myCommand_list.at( i ).token[j] != "/" &&
                        myCommand_list.at( i ).token[j] != "%" &&
                        myCommand_list.at( i ).token[j] != ">" &&
                        myCommand_list.at( i ).token[j] != "<" &&
                        myCommand_list.at( i ).token[j] != "&&" &&
                        myCommand_list.at( i ).token[j] != "||" &&
                        myCommand_list.at( i ).token[j] != "+=" &&
                        myCommand_list.at( i ).token[j] != "-=" &&
                        myCommand_list.at( i ).token[j] != "/=" &&
                        myCommand_list.at( i ).token[j] != "*=" &&
                        myCommand_list.at( i ).token[j] != "%=" &&
                        myCommand_list.at( i ).token[j] != "(" ) &&
                      myCommand_list.at( i ).token[j+1] == "(" ) {
              ; // 不給空白
            } // else if
            else {
              cout << " " ; // 輸出token
            } // else
          } // else

        } // for // 輸出 token 結束
      } // if
    } // for
    
  } // ListFunction()
  
  void ListVariable( string target ) {
    
    vector<Command>::iterator temp ;
    
    cout << "> " ;
    
    temp = myCommand_list.begin() ;

    while ( temp->name != target ) {
      temp++ ;
    } // while

    cout << temp->variableType << " " << temp->name ; // type name
    
    vector<string> temp2 ;
    temp2.assign( temp->token.begin(), temp->token.end() ) ;
    
    for ( int i = 0 ; i < ( int ) temp2.size() ; i ++ ) {
      
      if ( temp2.at( i ) == temp->name && temp2.at( i + 1 ) == "[" ) {
        
        cout << temp2.at( i + 1 ) << " " << temp2.at( i + 2 ) << " " << temp2.at( i + 3 ) ;
        cout << " ;\n" ;
        return ;
        
      } // if
 
    } // for
    
    cout << " ;" << endl ;
    
  } // ListVariable()
  
} ; // Parser()

int main() {
  
  Parser t ;
  
  // char trash[200] ;
  bool keep_going = true ;
  string white_space ;
  
  // if ( cin.peek() == '2' ) cout << "y\n" ;
  while ( cin.peek() != '\n' ) cin.get() ;
  cin.get() ;
  
  cout << "Our-C running ..." << endl ;
  
  do {
      
    try {
      
      gline = 1 ;
      keep_going = t.User_input() ;
        
    }  // try
    catch ( Lexical_error& e ) {
      while ( cin.peek() != '\n' ) cin.get() ;
    } // catch
    catch ( Syntax_error& e ) {
      while ( cin.peek() != '\n' ) cin.get() ;
    }  // catch
    catch( Defined_error& e ) {
      while ( cin.peek() != '\n' ) cin.get() ;
    } // catch
    catch( Error& e ) {
      while ( cin.peek() != '\n' ) cin.get() ;
    } // catch
    
    if ( keep_going ) {
    
      while ( cin.peek() == ' ' || cin.peek() == '\t' ) cin.get() ;
      
      char last_char ;
      last_char = cin.get() ;
      
      if ( last_char == '\n' ) ;
      else if ( last_char == '/' && cin.peek() == '/' ) {
        while ( cin.peek() != '\n' ) cin.get() ;
        cin.get() ;
      } // else if
      else cin.putback( last_char ) ;
        
    } // if
      
  } while ( keep_going ) ;
  
  cout << "> Our-C exited ..." << endl ;
  
} // main()

