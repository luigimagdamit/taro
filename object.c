#include <stdio.h>
#include <string.h>

#include "memory.h"
#include "object.h"
#include "table.h"
#include "value.h"
#include "vm.h"

#define ALLOCATE_OBJ(type, objectType) \
  (type*)allocateObject(sizeof(type), objectType)

static Obj* allocateObject(size_t size, ObjType type) {
  Obj* object = (Obj*)reallocate(NULL, 0, size);
  object->type = type;

  object->next = vm.objects;
  vm.objects = object;
  return object;
}
static ObjString* allocateString(char* chars, int length, uint32_t hash) {
  ObjString* string = ALLOCATE_OBJ(ObjString, OBJ_STRING);
  string->length = length;
  string->chars = chars;
  string->hash = hash;
  tableSet(&vm.strings, string, NIL_VAL);
  return string;
}
static uint32_t hashString(const char* key, int length) {
  uint32_t hash = 2166136261u;
  for(int i = 0; i < length; i++) {
    hash ^= (uint8_t)key[i];
    hash*= 16777619;
  }
  return hash;
}
ObjString* takeString(char* chars, int length) {
  uint32_t hash = hashString(chars, length);
  ObjString* interned = tableFindString(&vm.strings, chars, length, hash);

  if (interned != NULL) {
    FREE_ARRAY(char, chars, length + 1);
    return interned;
  }
  return allocateString(chars, length, hash);
}
ObjString* copyString(const char* chars, int length) {
  uint32_t hash = hashString(chars, length);
  ObjString* interned = tableFindString(&vm.strings, chars, length, hash);

  if (interned != NULL) return interned; 
  char* heapChars = ALLOCATE(char, length + 1);
  memcpy(heapChars, chars, length);
  heapChars[length] = '\0';
  return allocateString(heapChars, length, hash);
}

ObjEnum* newEnum() {
  ObjEnum* _enum = ALLOCATE_OBJ(ObjEnum, OBJ_ENUM);
  _enum->length = 0;
  initTable(&_enum->fields);
  return _enum;
}
ObjList* newList() {
  ObjList* list = ALLOCATE_OBJ(ObjList, OBJ_LIST);
  list->values = NULL;
  list->length = 0;
  list->capacity = 0;
  return list;
}

void appendList(ObjList* list, Value value) {
  if (list->length + 1 > list->capacity) {
    int oldCap = list->capacity;
    list->capacity = GROW_CAPACITY(oldCap);
    list->values = GROW_ARRAY(Value, list->values, oldCap, list->capacity);
  }
  list->values[list->length] = value;
  list->length++;
  return;
}

void printListVal(Value* value) {
  switch(value->type) {
    case VAL_NUMBER:
      printf("%g", AS_NUMBER(*value));
      break;
    case VAL_BOOL: {
      if(AS_BOOL(*value) == true) {
        printf("true");
      } else {
        printf("false");
      }
      break;
    }
    case VAL_NIL:
      printf("nil");
    case VAL_OBJ: {
      switch(AS_OBJ(*value)->type) {
        case OBJ_STRING:
          printf("\"%s\"", AS_CSTRING(*value));
          break;
        case OBJ_LIST:
          printObject(*value);
          break;
      }
    }
  }
}
void printObject(Value value) {
  switch(OBJ_TYPE(value)) {
    case OBJ_STRING:
      printf("%s", AS_CSTRING(value));
      break;
    case OBJ_LIST: {
      Value* values = AS_VLIST(value);
      printf("[");
      for(int i = 0; i < AS_LIST(value)->length; i++) {
        printListVal(&values[i]);
        printf(", ");
      }
      printf("]");
      break;
    }
    case OBJ_ENUM: {
      printf("ENUM: %d", AS_ENUM(value)->length);
      break;
    }
  }
}
