#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "common.h"
#include "compiler.h"
#include "debug.h"
#include "object.h"
#include "memory.h"
#include "vm.h"

VM vm;

static void resetStack() {
  vm.stackTop = vm.stack;
}
static void runtimeError(const char* format, ...) {
  va_list args;
  va_start(args, format);
  vfprintf(stderr, format, args);
  va_end(args);
  fputs("\n", stderr);

  size_t instruction = vm.ip - vm.chunk->code - 1;
  int line = vm.chunk->lines[instruction];
  fprintf(stderr, "[line %d] in script\n", line);
  resetStack();
}
void initVM(){
  resetStack();
  vm.objects = NULL;

  initTable(&vm.globals);
  initTable(&vm.strings);
}

void freeVM() {
  freeTable(&vm.globals);
  freeTable(&vm.strings);
  freeObjects();
}

void push(Value value) {
  *vm.stackTop = value;
  vm.stackTop++; 
}
Value pop() {
  vm.stackTop--;
  return *vm.stackTop;
}
static Value peek(int distance) {
  return vm.stackTop[-1 - distance];
}
static bool isFalsey(Value value) {
  return IS_NIL(value) || (IS_BOOL(value) && !AS_BOOL(value));
}
static void concatenate() {
  ObjString* b = AS_STRING(pop());
  ObjString* a = AS_STRING(pop());

  int length = a->length + b->length;
  char* chars = ALLOCATE(char, length + 1);
  memcpy(chars, a->chars, a->length);
  memcpy(chars + a->length, b->chars, b->length);
  chars[length] = '\0';

  ObjString* result = takeString(chars, length);
  push(OBJ_VAL(result));
}
static InterpretResult run() {
#define READ_BYTE() (*vm.ip++)
#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])
#define READ_SHORT() \
    (vm.ip += 2, (uint16_t)((vm.ip[-2] << 8) | vm.ip[-1]))
#define READ_STRING() AS_STRING(READ_CONSTANT())
#define BINARY_OP(valueType, op) \
  do { \
    if (!IS_NUMBER(peek(0)) || !IS_NUMBER(peek(1))) { \
      runtimeError("Operands must be numbers."); \
      return INTERPRET_RUNTIME_ERROR; \
    } \
    double b = AS_NUMBER(pop()); \
    double a = AS_NUMBER(pop()); \
    push(valueType(a op b)); \
  } while (false)
  for(;;) {
#ifdef DEBUG_TRACE_EXECUTION
    printf("      ");
    for(Value* slot = vm.stack; slot < vm.stackTop; slot++) {
      printf("[  ");
      printValue(*slot);
      printf("  ]");
    }
    printf("\n");
    disassembleInstruction(vm.chunk, (int)(vm.ip - vm.chunk->code));
#endif
    uint8_t instruction;
    switch(instruction = READ_BYTE()) {
      case OP_GREATER: BINARY_OP(BOOL_VAL, >); break;
      case OP_LESS:    BINARY_OP(BOOL_VAL, <); break;
      case OP_ADD: {
        if (IS_STRING(peek(0)) && IS_STRING(peek(1))) {
          concatenate();
        } else if (IS_NUMBER(peek(0)) && IS_NUMBER(peek(1))) {
          double b = AS_NUMBER(pop());
          double a = AS_NUMBER(pop());
          push(NUMBER_VAL(a+b));
        } else {
          runtimeError(
            "Operands must be two numbers or strings"
          );
          return INTERPRET_RUNTIME_ERROR;
        }
        break;
      }
      case OP_SUBTRACT: 
        BINARY_OP(NUMBER_VAL, -);
        break;
      case OP_MULTIPLY:
        BINARY_OP(NUMBER_VAL, *);
        break;
      case OP_DIVIDE:
        BINARY_OP(NUMBER_VAL, /);
        break;
      case OP_MODULO: {
        int b = (int)AS_NUMBER(pop());
        int a = (int)AS_NUMBER(pop());

        int res = a % b;

        printf("OP_MODULO RES | %d", res);
        
        push(NUMBER_VAL((double)res));

        break;
      }
      case OP_NOT:
        push(BOOL_VAL(isFalsey(pop())));
        break;
      case OP_NEGATE:
        if (!IS_NUMBER(peek(0))) {
          runtimeError("Operand must be a number");
          return INTERPRET_RUNTIME_ERROR;
        }
        push(NUMBER_VAL(-AS_NUMBER(pop())));
        break;
      case OP_CONSTANT: {
        Value constant = READ_CONSTANT();
        // gets the next byte which is an index pointing to something in the constant table
        push(constant);
        printValue(constant);
        printf("\n");
        break;
      }
      case OP_NIL: push(NIL_VAL); break;
      case OP_TRUE: push(BOOL_VAL(true)); break;
      case OP_FALSE: push(BOOL_VAL(false)); break;
      case OP_POP: pop(); break;
      case OP_GET_LOCAL: {
        uint8_t slot = READ_BYTE();
        push(vm.stack[slot]);
        break;
      }
      case OP_SET_LOCAL: {
        uint8_t slot = READ_BYTE();
        vm.stack[slot] = peek(0);
        break;
      }
      case OP_GET_GLOBAL: {
        ObjString* name = READ_STRING();
        Value value;
        if(!tableGet(&vm.globals, name, &value)) {
          runtimeError("Undefined variable '%s'.", name->chars);
          return INTERPRET_RUNTIME_ERROR;
        }
        push(value);
        break;
      }
      case OP_DEFINE_GLOBAL: {
        ObjString* name = READ_STRING();
        tableSet(&vm.globals, name, peek(0));
        pop();
        break;
      }
      case OP_SET_GLOBAL: {
        ObjString* name = READ_STRING();
        if (tableSet(&vm.globals, name, peek(0))) {
          tableDelete(&vm.globals, name);
          runtimeError("Undefined variable '%s'", name->chars);
          return INTERPRET_RUNTIME_ERROR;
        }
        break;
      }
      case OP_EQUAL: {
        Value b = pop();
        Value a = pop();
        push(BOOL_VAL(valuesEqual(a, b)));
        break;
      }
      case OP_LIST: {
        // THIS MIGHT BREAK ONCE GC IS ADDED!!!
        if(!IS_NUMBER(peek(0))) {
          runtimeError("Incorrect token: Unable to get the size of the requested list");
          return INTERPRET_RUNTIME_ERROR;
        }
        double listCount = AS_NUMBER(pop());
        printf("ARRAY COUNT: %f", listCount);
        ObjList* list = newList();
        
        push(OBJ_VAL(list));
        for(int i = listCount; i > 0; i--) {
          printf("\nLISTCOUNT: %d", i);
          Value current = peek(i);
          printf("\n%f\n", AS_NUMBER(current));
          appendList(list, current);

        }
        pop();
        while(listCount-- > 0) {
          pop();
        }
        push(OBJ_VAL(list));
        break;
      }
      case OP_DICT: {
        double dictCount = AS_NUMBER(pop());
        ObjDict* dict = newDict();

        push(OBJ_VAL(dict));
        for(int i = dictCount; i > 0; i--) {
          printf("\nLISTCOUNT: %d", i);
          Value val = peek(i);
          Value key = peek(i+1);
          printf("KEY: %s, VALUE: %s\n", AS_CSTRING(key), AS_CSTRING(val));
          ObjDictEntry* entry = newDictEntry();
          entry->keyvalue[0] = key;
          entry->keyvalue[1] = val;
          tableSet(&dict->entries, AS_STRING(key), OBJ_VAL(entry));

          dict->length++;

        }
        pop();
        while(dictCount-- > 0) {
          pop();
          pop();
        }
        push(OBJ_VAL(dict));
        break;
      }
      case OP_LIST_SUB: {
        if(!IS_NUMBER(peek(0))) {
          runtimeError("List index value must evaluate to number type: list[:number:]");
          return INTERPRET_RUNTIME_ERROR;
        }
        double index = AS_NUMBER(pop());
        ObjList* list = AS_LIST(pop());

        push(list->values[(int)index]);
        break;
      }
      case OP_LIST_STORE: {
        if(!IS_NUMBER(peek(1))) {
          runtimeError("List index value must evaluate to number type: list[:number:]");
          return INTERPRET_RUNTIME_ERROR;
        }
        Value res = pop();
        int index = (int)AS_NUMBER(pop());
        ObjList* list = AS_LIST(pop());

        printf("RES: %f", AS_NUMBER(res));

        list->values[index] = res;
        break;
      }
      case OP_PUSH: {
        Value res = pop();
        ObjList* list = AS_LIST(pop());

        appendList(list, res);
        break;
      }
      case OP_ENUM: {
        ObjEnum* _enum = newEnum();
        double enumCount = AS_NUMBER(pop());

        push(OBJ_VAL(_enum));
        for(int i = enumCount; i > 0; i--) {
          printf("\nLISTCOUNT: %d", i);
          Value current = peek(i);
          printf("\n%f\n", AS_NUMBER(current));
          tableSet(&_enum->fields, AS_STRING(current), NUMBER_VAL(_enum->length));
          _enum->length++;

        }
        pop();
        while(enumCount-- > 0) {
          pop();
        }
        push(OBJ_VAL(_enum));
        break;
      }
      case OP_GET_ENUM: {
        printf("getenum in vm");
        
      }
      case OP_PRINT: {
        printValue(pop());
        printf("\n");
        break;
      }
      case OP_JUMP: {
        uint16_t offset = READ_SHORT();
        vm.ip += offset;
        break;
      }
      case OP_JUMP_IF_FALSE: {
        uint16_t offset = READ_SHORT();
        if (isFalsey(peek(0))) vm.ip += offset;
        break;
      }
      case OP_LOOP: {
        uint16_t offset = READ_SHORT();
        vm.ip -= offset;
        break;
      }
      case OP_RETURN: {
        return INTERPRET_OK;
      }
    }
  }
#undef READ_BYTE
#undef READ_SHORT
#undef READ_STRING
#undef BINARY_OP
#undef READ_CONSTANT
}
InterpretResult interpret(const char* source) {
  Chunk chunk;
  initChunk(&chunk);

  if(!compile(source, &chunk)) {
    freeChunk(&chunk);
    return INTERPRET_COMPILE_ERROR;
  }

  vm.chunk = &chunk;
  vm.ip = vm.chunk->code;
  
  InterpretResult result = run();

  freeChunk(&chunk);
  return result;
}
