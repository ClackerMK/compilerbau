#ifndef ConstantFolder_HEADER
#define ConstantFolder_HEADER
/* You might want to change the above name. */

#include "Absyn.H"
#include <memory>
#include <stack>
#include <map>

class ConstantFolder : public Visitor
{
private:
    struct value {
        Double Dbl;
        Integer Int;
        String Str;
        bool Bl;
    };

    Visitable*  _tree;
    bool* _constant;
    value* _value;
    Types* _type;


    std::map<Id, std::pair<Types, value>> variables;

    std::unique_ptr<std::pair<Types, value>> lookup(Id id);
    void removeVar();
    void removeVar(Id id);
    void setVar(Id id, Types type, ConstantFolder::value val);

    virtual Types       getType(Visitable *v);

    virtual value       getValue(Visitable *v);



    virtual bool        isConstant(Visitable *v) {
        return dynamic_cast<ETrue*>(v) != nullptr || dynamic_cast<EFalse*>(v) != nullptr ||
                dynamic_cast<EDouble*>(v) != nullptr ||
                dynamic_cast<EInt*>(v) != nullptr ||
                (dynamic_cast<EId*>(v) != nullptr && lookup(dynamic_cast<EId*>(v)->id_) != nullptr) ;
    }

public:
    Visitable*  fold(Visitable *v);

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
