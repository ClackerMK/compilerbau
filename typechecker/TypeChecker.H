#ifndef TYPECHECKER_HEADER
#define TYPECHECKER_HEADER
/* You might want to change the above name. */

#include "Absyn.H"
#include <cstdlib>
#include <iostream>
#include <map>

class FunctionInfo {
public:
    FunctionInfo(Id id);
    ~FunctionInfo();

    Id id;
    Type *funcType;
    //parameter map, map type to parameter ID of function
    std::map<Type*, Id> params;

};


enum TypeCode {CInt, CDouble, CString, CBool, CVoid};

class TypeChecker : public Visitor
{

private:
    class Env {
    public:
        //expands the environment by a function
        Env *expand(Env *env, Id *funcId, FunctionInfo *func);
        //expands the environment by a variable
        Env *expand(Env *env, Id *id, Type *type);
        void updateVar(Type *t, Id *id);
        static Type* lookUpVar(Id *id);
        //function table, map function ID to corresponding FunctionInfo object
        std::map<Id, FunctionInfo*> funcTable;

    };
    Env *env;

public:

    TypeChecker();
    ~TypeChecker();
    //currently/last visited function
    //FunctionInfo *curFunc;
    void typeCheck(Program* p);
    int passNo;

  void visitProgram(Program* p);
  void visitDef(Def* p);
  void visitArg(Arg* p);
  void visitStm(Stm* p);
  void visitExp(Exp* p);
  void visitType(Type* p);
  void visitPDefs(PDefs* p);
  void visitDFun(DFun* p);
  void visitADecl(ADecl* p);
  void visitSExp(SExp* p);
  void visitSDecls(SDecls* p);
  void visitSInit(SInit* p);
  void visitSReturn(SReturn* p);
  void visitSReturnVoid(SReturnVoid* p);
  void visitSWhile(SWhile* p);
  void visitSBlock(SBlock* p);
  void visitSIfElse(SIfElse* p);
  void visitETrue(ETrue* p);
  void visitEFalse(EFalse* p);
  void visitEInt(EInt* p);
  void visitEDouble(EDouble* p);
  void visitEString(EString* p);
  void visitEId(EId* p);
  void visitEApp(EApp* p);
  void visitEPIncr(EPIncr* p);
  void visitEPDecr(EPDecr* p);
  void visitEIncr(EIncr* p);
  void visitEDecr(EDecr* p);
  void visitETimes(ETimes* p);
  void visitEDiv(EDiv* p);
  void visitEPlus(EPlus* p);
  void visitEMinus(EMinus* p);
  void visitELt(ELt* p);
  void visitEGt(EGt* p);
  void visitELtEq(ELtEq* p);
  void visitEGtEq(EGtEq* p);
  void visitEEq(EEq* p);
  void visitENEq(ENEq* p);
  void visitEAnd(EAnd* p);
  void visitEOr(EOr* p);
  void visitEAss(EAss* p);
  void visitETyped(ETyped* p);
  void visitType_bool(Type_bool* p);
  void visitType_int(Type_int* p);
  void visitType_double(Type_double* p);
  void visitType_void(Type_void* p);
  void visitType_string(Type_string* p);
  void visitListDef(ListDef* p);
  void visitListArg(ListArg* p);
  void visitListStm(ListStm* p);
  void visitListExp(ListExp* p);
  void visitListId(ListId* p);

  void visitId(Id x);
  void visitInteger(Integer x);
  void visitChar(Char x);
  void visitDouble(Double x);
  void visitString(String x);
  void visitIdent(Ident x);

};


#endif
