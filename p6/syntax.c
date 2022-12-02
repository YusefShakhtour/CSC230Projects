/**@file expand.c
 * @author (Partially) Yusef Shakhtour (yfshakht)
 * This file defines functions that will be used to evaluate syntax from a program
 */

#include "syntax.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//////////////////////////////////////////////////////////////////////
// Error-reporting functions

/** Report an error for a program with bad types, then exit. */
static int reportTypeMismatch()
{
  fprintf( stderr, "Type mismatch\n" );
  exit( EXIT_FAILURE );
}

/** Report an error for a program that goes out of bounds of a sequence, then exit **/
static int reportOutofBounds() {
  fprintf(stderr, "Index out of bounds\n");
  exit(EXIT_FAILURE);
}

/** Require a given value to be an IntType value.  Exit with an error
    message if not.
    @param v value to check, passed by address.
 */
static void requireIntType( Value const *v )
{
  if ( v->vtype != IntType )
    reportTypeMismatch();
}

/** Require a given value to be a SeqType value. Exit with an error
 * message if not
 * @param v value to check, passed by address
 */
static void requireSeqType( Value const *v )
{
  if ( v->vtype != SeqType )
    reportTypeMismatch();
}

//////////////////////////////////////////////////////////////////////
// LiteralInt

/** Representation for a LiteralInt expression, a subclass of Expr that
    evaluates to a constant value. */
typedef struct {
  Value (*eval)( Expr *expr, Environment *env );
  void (*destroy)( Expr *expr );

  /** Integer value this expression evaluates to. */
  int val;
} LiteralInt;

/** Implementation of eval for LiteralInt expressions. */
static Value evalLiteralInt( Expr *expr, Environment *env )
{
  // If this function gets called, expr must really be a LiteralInt.
  LiteralInt *this = (LiteralInt *)expr;

  // Return an int value containing a copy of the value we represent.
  return (Value){ IntType, .ival = this->val };
}

/** Implementation of destroy for LiteralInt expressions. */
static void destroyLiteralInt( Expr *expr )
{
  // This object is just one block of memory.  We can free it without
  // even having to type-cast its pointer.
  free( expr );
}

Expr *makeLiteralInt( int val )
{
  // Allocate space for the LiteralInt object
  LiteralInt *this = (LiteralInt *) malloc( sizeof( LiteralInt ) );

  // Remember the pointers to functions for evaluating and destroying ourself.
  this->eval = evalLiteralInt;
  this->destroy = destroyLiteralInt;

  // Remember the integer value we contain.
  this->val = val;

  // Return the result, as an instance of the Expr superclass.
  return (Expr *) this;
}

//////////////////////////////////////////////////////////////////////
// Sequence initializer

/** Representation for a Sequence Initializer expression, a subclass of Expr that
    evaluates to a sequence of ints. **/
typedef struct {
  Value (*eval)( Expr *expr, Environment *env );
  void (*destroy)( Expr *expr );

  int len;
  Expr **elist;
} SeqInit;

/** Implementation of eval for SeqInit expressions. */
static Value evalSeqInit( Expr *expr, Environment *env )
{
  // If this function gets called, expr must really be a SeqInit.
  SeqInit *this = (SeqInit *)expr;

  Sequence *newSeq = makeSequence();
  grabSequence(newSeq);                                //Where am I suppoed to release this sequence?

  for (int i = 0; i < this->len; i++) {
    Value v = this->elist[i]->eval( this->elist[i], env );
    if (newSeq->len < newSeq->cap) {
      newSeq->seq[i] = v.ival;
      newSeq->len++;
    }
    else {
      newSeq->cap *= 2;
      newSeq->seq = (int *)realloc(newSeq->seq, sizeof(int) * newSeq->cap);
      newSeq->seq[i] = v.ival;
      newSeq->len++;
    }
  }

  // Return an int value containing a copy of the value we represent.
  return (Value){ SeqType, .sval = newSeq };
}

static void destroySeqInit( Expr *expr )
{
  SeqInit *this = (SeqInit *)expr;
  for (int i = 0; i < this->len; i++) {
    free(this->elist[i]);
  }
  free(this->elist);
  free( this );
}

Expr *makeSeqInit(int len, Expr **elist)
{
  // Allocate space for the SeqInit object
  SeqInit *this = (SeqInit *) malloc( sizeof( SeqInit ) );

  // Remember the pointers to functions for evaluating and destroying ourself.
  this->eval = evalSeqInit;
  this->destroy = destroySeqInit;

  // Remember the sequence we contain.
  //this->seq = seq;
  this->len = len;
  this->elist = (Expr **)malloc(sizeof(Expr *) * len);
  this->elist = elist; 

  // Return the result, as an instance of the Expr superclass.
  return (Expr *) this;
}

//////////////////////////////////////////////////////////////////////
// SimpleExpr Struct

/** Representation for an expression with either one or two
    sub-expressionts.  With the right eval funciton, this struct should
    be able to help implement any expression with either one or two
    sub-expressiosn. */
typedef struct {
  Value (*eval)( Expr *expr, Environment *env );
  void (*destroy)( Expr *oper );

  /** The first sub-expression */
  Expr *expr1;
  
  /** The second sub-expression, or NULL if it's not needed. */
  Expr *expr2;
} SimpleExpr;

/** General-purpose function for freeing an expression represented by
    SimpleExpr.  It frees the two sub-expressions, then frees the strucct
    itself. */
static void destroySimpleExpr( Expr *expr )
{
  // If this function gets called, expr must really be a SimpleExpr.
  SimpleExpr *this = (SimpleExpr *)expr;

  // Free the first sub-expression.
  this->expr1->destroy( this->expr1 );

  // Free the second one, if it exists.
  if ( this->expr2 )
    this->expr2->destroy( this->expr2 );

  // Then the SimpleExpr struct itself.
  free( this );
}

/** Helper funciton to construct a SimpleExpr representation and fill
    in the fields.
    @param first sub-expression in the expression.
    @param second sub-expression in the expression, or null if it only
    has one sub-expression.
    @param eval function implementing the eval mehod for this expression.
    @return new expression, as a poiner to Expr.
*/
static Expr *buildSimpleExpr( Expr *expr1, Expr *expr2,
                              Value (*eval)( Expr *, Environment * ) )
{
  // Allocate space for a new SimpleExpr and fill in the pointer for
  // its destroy function.
  SimpleExpr *this = (SimpleExpr *) malloc( sizeof( SimpleExpr ) );
  this->destroy = destroySimpleExpr;

  // Fill in the two parameters and the eval funciton.
  this->eval = eval;
  this->expr1 = expr1;
  this->expr2 = expr2;

  return (Expr *) this;
}


//////////////////////////////////////////////////////////////////////
// Sequence Index Expression

/**Implementation of eval function for sequence index epxression**/
static Value evalSeqIdx(Expr *expr, Environment *env) {
  SimpleExpr *this = (SimpleExpr *)expr;

  Value v1 = this->expr1->eval(this->expr1, env);
  Value v2 = this->expr2->eval(this->expr2, env);
  requireSeqType(&v1);
  requireIntType(&v2);
   
  if (v2.ival > v1.sval->len - 1) {
    reportOutofBounds();
  }
  else if (v2.ival < 0) {
    reportOutofBounds();
  }

  return (Value) {IntType, .ival = v1.sval->seq[v2.ival]};

}

Expr *makeSeqIdx(Expr *expr1, Expr *expr2) {
  return buildSimpleExpr(expr1, expr2, evalSeqIdx);
}

//////////////////////////////////////////////////////////////////////
// Len Expression

/** Implementation of the eval function for len expression. */
static Value evalLen( Expr *expr, Environment *env )
{
  // If this function gets called, expr must really be a SimpleExpr.
  SimpleExpr *this = (SimpleExpr *)expr;
  // Evaluate operand
  Value v = this->expr1->eval( this->expr1, env );
  // Make sure the operand is a sequence.
  requireSeqType( &v );

//  releaseSequence(v.sval);
  // Return the len of the sequence.
  return (Value){ IntType, .ival = v.sval->len };
}  

Expr *makeLen( Expr *expr )
{
  // Use the convenience function to build a SimpleExpr for addition
  return buildSimpleExpr( expr, NULL, evalLen );
}

//////////////////////////////////////////////////////////////////////
// Integer addition

/** Implementation of the eval function for integer addition. */
static Value evalAdd( Expr *expr, Environment *env )
{
  // If this function gets called, expr must really be a SimpleExpr.
  SimpleExpr *this = (SimpleExpr *)expr;

  // Evaluate our left and right operands. 
  Value v1 = this->expr1->eval( this->expr1, env );
  Value v2 = this->expr2->eval( this->expr2, env );

  // Make sure the operands are both integers.
  if (v1.vtype == IntType && v2.vtype == IntType) {
    requireIntType( &v1 );
    requireIntType( &v2 );
    // Return the sum of the two expression values.
    return (Value){ IntType, .ival = v1.ival + v2.ival };
  }

  //Extra Credit
  else if (v1.vtype == SeqType && v2.vtype == SeqType) {
    int length = v1.sval->len + v2.sval->len;
    Sequence *newSeq = makeSequence();
    for (int i = 0; i < v1.sval->len; i++) {
      if (newSeq->len < newSeq->cap) {
        newSeq->seq[i] = v1.sval->seq[i];
        newSeq->len++;
      }
      else {
        newSeq->cap *= 2;
        newSeq->seq = (int *)realloc(newSeq->seq, sizeof(int) * newSeq->cap);
        newSeq->seq[i] = v1.sval->seq[i];
        newSeq->len++;
      }
    }
    int idx = 0;
    for (int i = v1.sval->len; i < length; i++) {
      if (newSeq->len < newSeq->cap) {
        newSeq->seq[i] = v2.sval->seq[idx];
        newSeq->len++;
        idx++;
      }
      else {
        newSeq->cap *= 2;
        newSeq->seq = (int *)realloc(newSeq->seq, sizeof(int) * newSeq->cap);
        newSeq->seq[i] = v2.sval->seq[idx];
        newSeq->len++;
        idx++;
      }
    }
    return (Value){ SeqType, .sval = newSeq };
  }
  else if (v1.vtype == IntType && v2.vtype == SeqType) {   //Opposite of push, add to beginning.
    Sequence *newSeq = makeSequence();
    for (int i = 0; i < v2.sval->len; i++) {
      if (newSeq->len < newSeq->cap) {
        newSeq->seq[i] = v2.sval->seq[i];
        newSeq->len++;
      }
      else {
        newSeq->cap *= 2;
        newSeq->seq = (int *)realloc(newSeq->seq, sizeof(int) * newSeq->cap);
        newSeq->seq[i] = v2.sval->seq[i];
        newSeq->len++;
      }
    }
    if (newSeq->len < newSeq->cap) {   //Can't just add to 0, have to move things over
      for (int i = newSeq->len + 1; i >= 1; i--) {
        newSeq->seq[i] = newSeq->seq[i - 1];
      }
      newSeq->seq[0] = v1.ival;
      newSeq->len++;
    }
    else {
      newSeq->cap *= 2;
      newSeq->seq = (int *)realloc(newSeq->seq, sizeof(int) * newSeq->cap);
      for (int i = newSeq->len + 1; i >= 1; i--) {
        newSeq->seq[i] = newSeq->seq[i - 1];
      }
      newSeq->seq[0] = v1.ival;
      newSeq->len++;
    }
    return (Value){ SeqType, .sval = newSeq };
  }
  else if (v1.vtype == SeqType && v2.vtype == IntType) {    //If first param if sequence and next is int. Basically push
    Sequence *newSeq = makeSequence();
    for (int i = 0; i < v1.sval->len; i++) {
      if (newSeq->len < newSeq->cap) {
        newSeq->seq[i] = v1.sval->seq[i];
        newSeq->len++;
      }
      else {
        newSeq->cap *= 2;
        newSeq->seq = (int *)realloc(newSeq->seq, sizeof(int) * newSeq->cap);
        newSeq->seq[i] = v1.sval->seq[i];
        newSeq->len++;
      }
    }
    if (newSeq->len < newSeq->cap) {
      newSeq->seq[newSeq->len] = v2.ival;
      newSeq->len++;
    }
    else {
      newSeq->cap *= 2;
      newSeq->seq = (int *)realloc(newSeq->seq, sizeof(int) * newSeq->cap);
      newSeq->seq[newSeq->len] = v2.ival;
      newSeq->len++;
    }
    return (Value){ SeqType, .sval = newSeq };
  }
  //Never evaluated
  return (Value){IntType, .ival = 0};
}

Expr *makeAdd( Expr *left, Expr *right )
{
  // Use the convenience function to build a SimpleExpr for addition
  return buildSimpleExpr( left, right, evalAdd );
}

//////////////////////////////////////////////////////////////////////
// Integer subtracton

/** Implementation of the eval function for integer subtraction. */
static Value evalSub( Expr *expr, Environment *env )
{
  // If this function gets called, expr must really be a SimpleExpr.
  SimpleExpr *this = (SimpleExpr *)expr;

  // Evaluate our left and right operands. 
  Value v1 = this->expr1->eval( this->expr1, env );
  Value v2 = this->expr2->eval( this->expr2, env );

  // Make sure the operands are both integers.
  requireIntType( &v1 );
  requireIntType( &v2 );

  // Return the difference of the two expression values.
  return (Value){ IntType, .ival = v1.ival - v2.ival };
}

Expr *makeSub( Expr *left, Expr *right )
{
  // Use the convenience function to build a SimpleExpr for subtraction.
  return buildSimpleExpr( left, right, evalSub );
}

//////////////////////////////////////////////////////////////////////
// Integer multiplication

/** Implementation of the eval function for integer multiplication. */
static Value evalMul( Expr *expr, Environment *env )
{
  // If this function gets called, expr must really be a SimpleExpr.
  SimpleExpr *this = (SimpleExpr *)expr;

  // Evaluate our left and right operands. 
  Value v1 = this->expr1->eval( this->expr1, env );
  Value v2 = this->expr2->eval( this->expr2, env );

  // Make sure the operands are both integers.
 // requireIntType( &v1 );
 // requireIntType( &v2 );
  if (v1.vtype == IntType && v2.vtype == IntType) {
    // Return the product of the two expression.
    return (Value){ IntType, .ival = v1.ival * v2.ival };
  }

  else if (v1.vtype == IntType && v2.vtype == SeqType) {
    Sequence *newSeq = makeSequence();
    for (int i = 0; i < v2.sval->len; i++) {
      if (newSeq->len < newSeq->cap) {
        newSeq->seq[i] = v2.sval->seq[i];
        newSeq->len++;
      }
      else {
        newSeq->cap *= 2;
        newSeq->seq = (int *)realloc(newSeq->seq, sizeof(int) * newSeq->cap);
        newSeq->seq[i] = v2.sval->seq[i];
        newSeq->len++;
      }
    }
    for (int j = newSeq->len; j < v2.sval->len * v1.ival; j += v2.sval->len)  {
      for (int k = 0; k < v2.sval->len; k++) {
        if (newSeq->len < newSeq->cap) {
          newSeq->seq[j + k] = v2.sval->seq[k];
          newSeq->len++;
        }
        else {
          newSeq->cap *= 2;
          newSeq->seq = (int *)realloc(newSeq->seq, sizeof(int) * newSeq->cap);
          newSeq->seq[j + k] = v2.sval->seq[k];
          newSeq->len++;
        }
      }
    }
    return (Value){ SeqType, .sval = newSeq };
  }

  else if (v1.vtype == SeqType && v2.vtype == IntType) {
    Sequence *newSeq = makeSequence();
    for (int i = 0; i < v1.sval->len; i++) {
      if (newSeq->len < newSeq->cap) {
        newSeq->seq[i] = v1.sval->seq[i];
        newSeq->len++;
      }
      else {
        newSeq->cap *= 2;
        newSeq->seq = (int *)realloc(newSeq->seq, sizeof(int) * newSeq->cap);
        newSeq->seq[i] = v1.sval->seq[i];
        newSeq->len++;
      }
    }

    for (int j = newSeq->len; j < v1.sval->len * v2.ival; j += v1.sval->len)  {
      for (int k = 0; k < v1.sval->len; k++) {
        if (newSeq->len < newSeq->cap) {
          newSeq->seq[j + k] = v1.sval->seq[k];
          newSeq->len++;
        }
        else {
          newSeq->cap *= 2;
          newSeq->seq = (int *)realloc(newSeq->seq, sizeof(int) * newSeq->cap);
          newSeq->seq[j + k] = v1.sval->seq[k];
          newSeq->len++;
        }
      }
    }
    return (Value){ SeqType, .sval = newSeq };
  } 
  
  else if (v1.vtype == SeqType && v2.vtype == SeqType) {
    reportTypeMismatch();
  }

  //Not evaluated
  return (Value){IntType, .ival = 0};  
}

Expr *makeMul( Expr *left, Expr *right )
{
  // Use the convenience function to build a SimpleExpr for multiplication.
  return buildSimpleExpr( left, right, evalMul );
}

//////////////////////////////////////////////////////////////////////
// Integer division

/** Implementation of the eval function for integer division. */
static Value evalDiv( Expr *expr, Environment *env )
{
  // If this function gets called, expr must really be a SimpleExpr.
  SimpleExpr *this = (SimpleExpr *)expr;

  // Evaluate our left and right operands. 
  Value v1 = this->expr1->eval( this->expr1, env );
  Value v2 = this->expr2->eval( this->expr2, env );

  // Make sure the operands are both integers.
  requireIntType( &v1 );
  requireIntType( &v2 );

  // Catch it if we try to divide by zero.
  if ( v2.ival == 0 ) {
    fprintf( stderr, "Divide by zero\n" );
    exit( EXIT_FAILURE );
  }

  // Return the quotient of the two expression.
  return (Value){ IntType, .ival = v1.ival / v2.ival };
}

Expr *makeDiv( Expr *left, Expr *right )
{
  // Use the convenience function to build a SimpleExpr for division.
  return buildSimpleExpr( left, right, evalDiv );
}

//////////////////////////////////////////////////////////////////////
// Logical and

static Value evalAnd( Expr *expr, Environment *env )
{
  // If this function gets called, expr must really be a SimpleExpr.
  SimpleExpr *this = (SimpleExpr *)expr;

  // Evaluate the left operand; return immediately if it's false.
  Value v1 = this->expr1->eval( this->expr1, env );
  requireIntType( &v1 );
  if ( v1.ival == 0 )
    return v1;
  
  // Evaluate the right operand.
  Value v2 = this->expr2->eval( this->expr2, env );
  requireIntType( &v2 );

  // Return true if the right-hand operand is true.
  return v2;
}

Expr *makeAnd( Expr *left, Expr *right )
{
  // Use the convenience function to build a SimpleExpr for the logical and.
  return buildSimpleExpr( left, right, evalAnd );
}

//////////////////////////////////////////////////////////////////////
// Logical or

/**Implementation for logival or eval function**/
static Value evalOr( Expr *expr, Environment *env )
{
  // If this function gets called, expr must really be a SimpleExpr.
  SimpleExpr *this = (SimpleExpr *)expr;

  // Evaluate the left operand; return immediately if it's true.
  Value v1 = this->expr1->eval( this->expr1, env );
  requireIntType( &v1 );
  if ( v1.ival )
    return v1;
  
  // Evaluate the right operand
  Value v2 = this->expr2->eval( this->expr2, env );
  requireIntType( &v2 );

  // Return true if the right-hand operand is true.
  return v2;
}

Expr *makeOr( Expr *left, Expr *right )
{
  // Use the convenience function to build a SimpleExpr for the logical or
  return buildSimpleExpr( left, right, evalOr );
}

//////////////////////////////////////////////////////////////////////
// Less-than comparison

/** Implementation of eval for the less than operator. */
static Value evalLess( Expr *expr, Environment *env )
{
  // If this function gets called, expr must really be a SimpleExpr.
  SimpleExpr *this = (SimpleExpr *)expr;

  // Evaluate our left and right operands. 
  Value v1 = this->expr1->eval( this->expr1, env );
  Value v2 = this->expr2->eval( this->expr2, env );

  // Make sure the operands are both the same type.
  if ( v1.vtype != v2.vtype )
    reportTypeMismatch();

  if ( v1.vtype == IntType ) {
    // Is v1 less than v2
    return (Value){ IntType, .ival = v1.ival < v2.ival ? true : false };
  } 
  else {
      if (v1.sval->len != v2.sval->len) {
        int leng;
        if (v1.sval->len < v2.sval->len) {
           leng = v1.sval->len;
        }
        else {
           leng = v2.sval->len;
        }
        for (int i = 0; i < leng; i++) {
          if (v1.sval->seq[i] < v2.sval->seq[i]) {
            return (Value){IntType, .ival = 1};
          }
        }
        if (v1.sval->len == leng) {
          return (Value){IntType, .ival = 1};
        }
        else {
          return (Value){IntType, .ival = 0};
        }
      }
      else {
        for (int i = 0; i < v1.sval->len; i++) {
          if (v1.sval->seq[i] < v2.sval->seq[i]) {
            return (Value){IntType, .ival = 1};
          }
          else if (v1.sval->seq[i] > v2.sval->seq[i]) {
            return (Value){IntType, .ival = 0};
          }
        }
          return (Value){IntType, .ival = 0};  
     //   return (Value){IntType, .ival = 1};
      }
  //  return (Value){ IntType, .ival = v1.sval < v2.sval ? true : false };
    // Replace with code to compare sequences.
//    fprintf( stderr, "Sequence comparison not implemented\n" );
//    exit( 0 );
  }
}

Expr *makeLess( Expr *left, Expr *right )
{
  // Use the convenience function to build a SimpleExpr for the less-than
  // comparison.
  return buildSimpleExpr( left, right, evalLess );
}

//////////////////////////////////////////////////////////////////////
// Equality comparison

/** Eval function for an equality test. */
static Value evalEquals( Expr *expr, Environment *env )
{
  // If this function gets called, expr must really be a SimpleExpr.
  SimpleExpr *this = (SimpleExpr *)expr;

  // Evaluate our left and right operands. 
  Value v1 = this->expr1->eval( this->expr1, env );
  Value v2 = this->expr2->eval( this->expr2, env );

  // Make sure the same type.
  if ( v1.vtype == IntType && v2.vtype == IntType ) {
    return (Value){ IntType, .ival = ( v1.ival == v2.ival ) };
  } else {
    // Replace with code to permit sequence-sequence comparison.
    // A sequence can also be compared to an int, but they should
    // never be considered equal.
    if (v1.vtype == IntType) {
    return (Value){IntType, .ival = 0};
    }
    else if (v2.vtype == IntType) {
      return (Value){IntType, .ival = 0};
    }
    else {
      if (v1.sval->len != v2.sval->len) {
      return (Value){IntType, .ival = 0};
      }
      else {
        for (int i = 0; i < v1.sval->len; i++) {
          if (v1.sval->seq[i] != v2.sval->seq[i]) {
            return (Value){IntType, .ival = 0};
          }
        }
        return (Value){IntType, .ival = 1};
      }
    }
  }

  // Never reached.
  return (Value){ IntType, .ival = 0 };
}

Expr *makeEquals( Expr *left, Expr *right )
{
  // Use the convenience function to build a SimpleExpr for the equals test.
  return buildSimpleExpr( left, right, evalEquals );
}

//////////////////////////////////////////////////////////////////////
// Variable in an expression

/** Representation for an expression representing an occurrence of a
    variable, subclass of Expr. */
typedef struct {
  Value (*eval)( Expr *expr, Environment *env );
  void (*destroy)( Expr *expr );

  /** Name of the variable. */
  char name[ MAX_VAR_NAME + 1 ];
} VariableExpr;

static Value evalVariable( Expr *expr, Environment *env )
{
  // If this function gets called, expr must really be a VariableExpr
  VariableExpr *this = (VariableExpr *) expr;

  // Get the value of this variable.
  Value val = lookupVariable( env, this->name );

  return val;
}

/** Implementation of destroy for Variable. */
static void destroyVariable( Expr *expr )
{
  free( expr );
}

Expr *makeVariable( char const *name )
{
  // Allocate space for the Variable statement, and fill in its function
  // pointers and a copy of the variable name.
  VariableExpr *this = (VariableExpr *) malloc( sizeof( VariableExpr ) );
  this->eval = evalVariable;
  this->destroy = destroyVariable;
  strcpy( this->name, name );

  return (Expr *) this;
}

//////////////////////////////////////////////////////////////////////
// SimpleStmt Struct

/** Generic representation for a statement that contains one or two
    expressions.  With different execute methods, this same struct
    can be used to represent print and push statements. */
typedef struct {
  void (*execute)( Stmt *stmt, Environment *env );
  void (*destroy)( Stmt *stmt );

  /** First (or only) expression used by this statement. */
  Expr *expr1;
  /** Second expression used by this statement, or null */
  Expr *expr2;
} SimpleStmt;

/** Generic destroy function for SimpleStmt, with either one
    or two sub-expressions. */
static void destroySimpleStmt( Stmt *stmt )
{
  // If this function gets called, stmt must really be a SimpleStmt.
  SimpleStmt *this = (SimpleStmt *)stmt;

  // Free our subexpression then the SimpleStmt object itself.
  this->expr1->destroy( this->expr1 );
  if ( this->expr2 )
    this->expr2->destroy( this->expr2 );
  free( this );
}

//////////////////////////////////////////////////////////////////////
// Push statement

/**Implementation of execut for a push statement**/
static void executePush( Stmt *stmt, Environment *env ) {

  // If this function gets called, stmt must really be a SimpleStmt.
  SimpleStmt *this = (SimpleStmt *)stmt;
  Value v1 = this->expr1->eval( this->expr1, env );
  Value v2 = this->expr2->eval( this->expr2, env );

  requireIntType( &v2 );
  requireSeqType( &v1 );

  if (v1.sval->len < v1.sval->cap) {
    v1.sval->seq[v1.sval->len] = v2.ival;
    v1.sval->len++; 
  }
  else {
    v1.sval->cap *= 2;
    v1.sval->seq = (int *)realloc(v1.sval->seq, sizeof(int) * v1.sval->cap);
    v1.sval->seq[v1.sval->len] = v2.ival;
    v1.sval->len++;
  }

}

Stmt *makePush( Expr *expr1, Expr *expr2 ) {
  // Allocate space for the SimpleStmt object
  SimpleStmt *this = (SimpleStmt *) malloc( sizeof( SimpleStmt ) );

  // Remember the pointers to execute and destroy this statement.
  this->execute = executePush;
  this->destroy = destroySimpleStmt;

  // Remember the expression for the thing we're supposed to push.
  this->expr1 = expr1;
  this->expr2 = expr2;

  // Return the SimpleStmt object, as an instance of the Stmt interface.
  return (Stmt *) this;
}


//////////////////////////////////////////////////////////////////////
// Print Statement

/** Implementation of execute for a print statement */
static void executePrint( Stmt *stmt, Environment *env )
{
  // If this function gets called, stmt must really be a SimpleStmt.
  SimpleStmt *this = (SimpleStmt *)stmt;

  // Evaluate our argument.
  Value v = this->expr1->eval( this->expr1, env );

  // Print the value of our expression appropriately, based on its type.
  if ( v.vtype == IntType ) {
    printf( "%d", v.ival );
  } else {
    // Replace with code to permit print a sequence as a string of
    // ASCII character codes.
    for (int i = 0; i < v.sval->len; i++) {
      
      printf("%c", (v.sval->seq[i]));
    }
  }
}

Stmt *makePrint( Expr *expr )
{
  // Allocate space for the SimpleStmt object
  SimpleStmt *this = (SimpleStmt *) malloc( sizeof( SimpleStmt ) );

  // Remember the pointers to execute and destroy this statement.
  this->execute = executePrint;
  this->destroy = destroySimpleStmt;

  // Remember the expression for the thing we're supposed to print.
  this->expr1 = expr;
  this->expr2 = NULL;

  // Return the SimpleStmt object, as an instance of the Stmt interface.
  return (Stmt *) this;
}

//////////////////////////////////////////////////////////////////////
// Compound Statement

/** Representation for a compound statement, derived from Stmt. */
typedef struct {
  void (*execute)( Stmt *stmt, Environment *env );
  void (*destroy)( Stmt *stmt );

  /** Number of statements in the compound. */
  int len;
  
  /** List of statements in the compound. */
  Stmt **stmtList;
} CompoundStmt;

/** Implementation of execute for CompountStmt */
static void executeCompound( Stmt *stmt, Environment *env )
{
  // If this function gets called, stmt must really be a CompoundStmt.
  CompoundStmt *this = (CompoundStmt *)stmt;

  // Execute the sequence of statements in this compound
  for ( int i = 0; i < this->len; i++ )
    this->stmtList[ i ]->execute( this->stmtList[ i ], env );
}

/** Implementation of destroy for CompountStmt */
static void destroyCompound( Stmt *stmt )
{
  // If this function gets called, stmt must really be a CompoundStmt.
  CompoundStmt *this = (CompoundStmt *)stmt;

  // Free the list of statements inside this compond.
  for ( int i = 0; i < this->len; i++ )
    this->stmtList[ i ]->destroy( this->stmtList[ i ] );

  // Then, free the array of pointers and the compund statement itself.
  free( this->stmtList );
  free( this );
}

Stmt *makeCompound( int len, Stmt **stmtList )
{
  // Allocate space for the CompoundStmt object
  CompoundStmt *this = (CompoundStmt *) malloc( sizeof( CompoundStmt ) );

  // Remember the pointers to execute and destroy this statement.
  this->execute = executeCompound;
  this->destroy = destroyCompound;

  // Remember the list of statements in the compound.
  this->len = len;
  this->stmtList = stmtList;

  // Return the result, as an instance of the Stmt interface.
  return (Stmt *) this;
}

///////////////////////////////////////////////////////////////////////
// ConditioanlStatement (for while/if)

/** Representation for either a while or if statement, subclass of Stmt. */
typedef struct {
  void (*execute)( Stmt *stmt, Environment *env );
  void (*destroy)( Stmt *stmt );

  // Condition to be checked before running the body.
  Expr *cond;

  // Body to execute if / while cond is true.
  Stmt *body;
} ConditionalStmt;

/** Implementation of destroy for either while of if statements. */
static void destroyConditional( Stmt *stmt )
{
  // If this function gets called, stmt must really be a ConditionalStmt.
  ConditionalStmt *this = (ConditionalStmt *)stmt;

  // Destroy the condition expression and the statement in the body.
  this->cond->destroy( this->cond );
  this->body->destroy( this->body );

  // Then, free the ConditionalStmt struct.
  free( this );
}

///////////////////////////////////////////////////////////////////////
// if statement

/** Implementation of th execute function for an if statement. */
static void executeIf( Stmt *stmt, Environment *env )
{
  // If this function gets called, stmt must really be a ConditionalStmt.
  ConditionalStmt *this = (ConditionalStmt *)stmt;

  // Evaluate our operand and see if it's true.
  Value result = this->cond->eval( this->cond, env );
  requireIntType( &result );

  // Execute the body if the condition evaluated to true.
  if ( result.ival )
    this->body->execute( this->body, env );
}

Stmt *makeIf( Expr *cond, Stmt *body )
{
  // Allocate an instance of ConditionalStmt
  ConditionalStmt *this =
    (ConditionalStmt *) malloc( sizeof( ConditionalStmt ) );

  // Functions to execute and destroy an if statement.
  this->execute = executeIf;
  this->destroy = destroyConditional;

  // Fill in the condition and the body of the if.
  this->cond = cond;
  this->body = body;

  // Return the result, as an instance of the Stmt interface.
  return (Stmt *) this;
}

///////////////////////////////////////////////////////////////////////
// while statement

/** Implementation of th execute function for a while statement. */
static void executeWhile( Stmt *stmt, Environment *env )
{
  // If this function gets called, stmt must really be a ConditionalStmt.
  ConditionalStmt *this = (ConditionalStmt *)stmt;

  // Evaluate our condition and see if it's true.
  Value result = this->cond->eval( this->cond, env );
  requireIntType( &result );
  
  // Execute the body while the condition evaluates to true.
  while ( result.ival ) {
    this->body->execute( this->body, env );
    
    // Get the value of the condition for the next iteration.
    result = this->cond->eval( this->cond, env );
    requireIntType( &result );
  }
}

Stmt *makeWhile( Expr *cond, Stmt *body )
{
  // Allocate an instance of ConditionalStmt
  ConditionalStmt *this =
    (ConditionalStmt *) malloc( sizeof( ConditionalStmt ) );

  // Functions to execute and destroy a while statement.
  this->execute = executeWhile;
  this->destroy = destroyConditional;

  // Fill in the condition and the body of the while.
  this->cond = cond;
  this->body = body;

  // Return the result, as an instance of the Stmt interface.
  return (Stmt *) this;
}

///////////////////////////////////////////////////////////////////////
// assignment statement

/** Representation of an assignment statement, a subclass of
    Stmt. This representation should be suitable for assigning to a
    variable or an element of a sequence.  */
typedef struct {
  void (*execute)( Stmt *stmt, Environment *env );
  void (*destroy)( Stmt *stmt );

  /** Name of the variable we're assigning to. */
  char name[ MAX_VAR_NAME + 1 ];
  
  /** If we're assigning to an element of a sequence, this is the index
      expression. Otherwise, it's zero. */
  Expr *iexpr;

  /** Expression for the right-hand side of the assignment (the source). */
  Expr *expr;
} AssignmentStmt;

/** Implementation of destroy for assignment Statements. */
static void destroyAssignment( Stmt *stmt )
{
  AssignmentStmt *this = (AssignmentStmt *)stmt;

  // Destroy the source expression and the index (if there is one)
  this->expr->destroy( this->expr );
  if ( this->iexpr )
    this->iexpr->destroy( this->iexpr );
  free( this );
}

/** Implementation of execute for assignment Statements. */
static void executeAssignment( Stmt *stmt, Environment *env )
{
  // If we get to this function, stmt must be an AssignmentStmt.
  AssignmentStmt *this = (AssignmentStmt *) stmt;

  // Evaluate the right-hand side of the equals.
  Value result = this->expr->eval( this->expr, env );
  
  if ( this->iexpr ) {
    
    // Replace with code to permit assigning to a sequence element.
    Sequence *seq = lookupVariable(env, this->name).sval;
    grabSequence(seq);					//This make sense for grabbing?
    seq->seq[this->iexpr->eval(this->iexpr,env).ival] = result.ival;
  } else {
    // It's a variable, change its value
    setVariable( env, this->name, result );
  }
}

Stmt *makeAssignment( char const *name, Expr *iexpr, Expr *expr )
{
  // Allocate the AssignmentStmt representations.
  AssignmentStmt *this =
    (AssignmentStmt *) malloc( sizeof( AssignmentStmt ) );

  // Fill in functions to execute or destory this statement.
  this->execute = executeAssignment;
  this->destroy = destroyAssignment;

  // Get a copy of the destination variable name, the source
  // expression and the sequence index (if it's non-null).
  strcpy( this->name, name );
  this->iexpr = iexpr;
  this->expr = expr;

  // Return this object, as an instance of Stmt.
  return (Stmt *) this;
}
