#include "value.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

//////////////////////////////////////////////////////////////////////
// Sequence.

Sequence *makeSequence()
{
  Sequence *sequence = (Sequence *)malloc(sizeof(sequence));
  sequence->cap = 5;
  sequence->seq = (int *)malloc(sizeof(int) * sequence->cap);
  sequence->len = 0;
  sequence->ref = 0;
  return sequence;
}

void freeSequence( Sequence *seq )
{
  free(seq->seq);
  free(seq);
}

void grabSequence( Sequence *seq )
{
  seq->ref += 1;
}

void releaseSequence( Sequence *seq )
{
  seq->ref -= 1;

  if ( seq->ref <= 0 ) {
    assert( seq->ref == 0 );

    // Once the reference count hits zero, we can free the sequence memory.
    freeSequence( seq );
  }
}

//////////////////////////////////////////////////////////////////////
// Environment.

typedef struct {
  char name[ MAX_VAR_NAME + 1 ];
  Value val;
} VarRec;

// Hidden implementation of the environment.
struct EnvironmentStruct {
  VarRec *vlist;

  // Number of name/value pairs.
  int len;

  // Capacity of the name/value list.
  int capacity;
};

Environment *makeEnvironment()
{
  Environment *env = (Environment *) malloc( sizeof( Environment ) );
  env->capacity = 5;
  env->len = 0;
  env->vlist = (VarRec *) malloc( sizeof( VarRec ) * env->capacity );
  return env;
}

Value lookupVariable( Environment *env, char const *name )
{
  // Linear search for a variable name, not too efficient.
  for ( int i = 0; i < env->len; i++ )
    if ( strcmp( env->vlist[ i ].name, name ) == 0 )
      return env->vlist[ i ].val;

  // Return zero for uninitialized variables.
  return (Value){ IntType, .ival = 0 };
}

void setVariable( Environment *env, char const *name, Value value )
{
  int pos = 0;
  while ( pos < env->len && strcmp( env->vlist[ pos ].name, name ) != 0 )
    pos++;

  if ( pos >= env->len ) {
    if ( env->len >= env->capacity ) {
      env->capacity *= 2;
      env->vlist = (VarRec *) realloc( env->vlist, sizeof( VarRec ) * env->capacity );
    }
  }

  // Is this a new variable, or one that already existed?
  if ( pos == env->len ) {
    pos = env->len++;
    strcpy( env->vlist[ pos ].name, name );
  }
  
  env->vlist[ pos ].val = value;
}

void freeEnvironment( Environment *env )
{
  //Go through the vlist and check if it is a seq, if it is free it. vlist[0]->val.vtype == sequecnetype
//  for (int i = 0; i < env->len; i++) {
//    if (env->vlist[i].val.vtype == SeqType) {
//      free(env->vlist[i].val.sval->seq);
//    }
//  }
  free( env->vlist );
  free( env );
}

