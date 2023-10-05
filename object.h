#ifndef clox_object_h
#define clox_object_h

#include "common.h"
#include "value.h"

#define OBJ_TYPE(value)    (AS_OBJ(value)->type)

#define IS_STRING(value)   isObjType(value, OBJ_STRING)
#define IS_LIST(value)     isObjType(value, OBJ_LIST)

#define AS_STRING(value)   ((ObjString*)AS_OBJ(value))
#define AS_LIST(value)     ((ObjList*)AS_OBJ(value))
#define AS_CSTRING(value)  (((ObjString*)AS_OBJ(value))->chars)
#define AS_VLIST(value)    (((ObjList*)AS_OBJ(value))->values)
typedef enum {
  OBJ_STRING,
  OBJ_LIST,
} ObjType;

struct Obj {
  ObjType type;
  struct Obj* next;
};

struct ObjString {
  Obj obj;
  int length;
  char* chars;
  uint32_t hash;
};

typedef struct {
  Obj obj;
  int length;
  int capacity;
  Value* values;
} ObjList;

ObjString* takeString(char* chars, int length);
ObjString* copyString(const char* chars, int length);
void printObject(Value value);
void appendList(ObjList* list, Value value);
ObjString* copyString(const char* chars, int length);
ObjList* newList();
static inline bool isObjType(Value value, ObjType type) {
  return IS_OBJ(value) && (AS_OBJ(value)->type == type);
}

#endif
