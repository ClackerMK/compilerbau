/*** BNFC-Generated Visitor Design Pattern TypeChecker. ***/
/* This implements the common visitor design pattern.
   Note that this method uses Visitor-traversal of lists, so
   List->accept() does NOT traverse the list. This allows different
   algorithms to use context information differently. */

#include <algorithm>
#include "TypeChecker.H"

std::vector<Types> TypeChecker::typecheck(Visitable *v) {
    v->accept(this);
    return _type;
}

bool isNumeric(Types t)
{
    return t == DOUBLE || t == INT;
}


void TypeChecker::visitProgram(Program* t) {} //abstract class
void TypeChecker::visitDef(Def* t) {} //abstract class
void TypeChecker::visitArg(Arg* t) {} //abstract class
void TypeChecker::visitStm(Stm* t) {} //abstract class
void TypeChecker::visitExp(Exp* t) {} //abstract class
void TypeChecker::visitType(Type* t) {} //abstract class

void TypeChecker::visitPDefs(PDefs *pdefs)
{
    /* Code For PDefs Goes Here */
    variableContext = new std::vector<VariableTable>;
    variableContext->push_back(VariableTable());

    pdefs->listdef_->accept(this);

}

void TypeChecker::visitDFun(DFun *dfun)
{
    /* Code For DFun Goes Here */
    variableContext->push_back(VariableTable());


    _currFunctionType = typecheck(dfun->type_)[0];

    dfun->listarg_->accept(this);
    dfun->liststm_->accept(this);

    variableContext->pop_back();
}

void TypeChecker::visitADecl(ADecl *adecl)
{
    /* Code For ADecl Goes Here */
    if (variableContext->back().count(adecl->id_) == 0)
    {
        variableContext->back()[adecl->id_] = typecheck(adecl->type_)[0];
    }
    else
    {
        // TBA Error Handling

    }
}

void TypeChecker::visitSExp(SExp *sexp)
{
    /* Code For SExp Goes Here */

    sexp->exp_->accept(this);

}

void TypeChecker::visitSDecls(SDecls *sdecls)
{
    /* Code For SDecls Goes Here */
    Types ty = typecheck(sdecls->type_)[0];
    for (auto name : *sdecls->listid_)
    {
        if (variableContext->back().count(name) == 0)
        {
            variableContext->back()[name] = ty;
        }
        else
        {
            // TBA Error Handling
            throw std::runtime_error("TYPE ERROR: Multiple Variable Declarations in the same Block");
        }
    }
}

void TypeChecker::visitSInit(SInit *sinit)
{
    /* Code For SInit Goes Here */
    auto ExprType = typecheck(sinit->exp_);
    auto varType = typecheck(sinit->type_)[0];
    if (ExprType.size() == 1 && varType == ExprType[0])
    {
        if (variableContext->back().count(sinit->id_) == 0)
        {
            variableContext->back()[sinit->id_] = ExprType[0];
        }
        else
        {
            // TBA Error Handling
            throw std::runtime_error("TYPE ERROR: Multiple Variable Declarations in the same Block");
        }
    } else {
        // TBA Error Handling

        throw std::runtime_error("TYPE ERROR: Wrong type @initialization: expected " + std::string(TypeStrings[varType]) +
                                         " but got " + std::string(TypeStrings[ExprType[0]]));
    }
}

void TypeChecker::visitSReturn(SReturn *sreturn)
{
    /* Code For SReturn Goes Here */
    auto ts = typecheck(sreturn->exp_);
    if (ts.size() == 1 && ts[0] == _currFunctionType)
    {

    }
    else
    {
        // TBA Error Handling
        throw std::runtime_error("TYPE ERROR: Wrong return type: expected " + std::string(TypeStrings[_currFunctionType]) +
                                 " but got " + std::string(TypeStrings[ts[0]]));
    }

}

void TypeChecker::visitSReturnVoid(SReturnVoid *sreturnvoid)
{
    /* Code For SReturnVoid Goes Here */
    if (_currFunctionType != VOID)
    {
        // TBA Error Handling
        throw std::runtime_error("TYPE ERROR: Wrong return type: expected " + std::string(TypeStrings[_currFunctionType]) +
                                 " but got void");
    }
}

void TypeChecker::visitSWhile(SWhile *swhile)
{
    /* Code For SWhile Goes Here */
    auto type = typecheck(swhile->exp_);
    if (type.size() == 1 && type[0] == BOOL)
    {

    } else {
        // TBA Error Handling
        throw std::runtime_error("TYPE ERROR: Wrong conditional type @while: expected bool but got " +
                                         std::string(TypeStrings[type[0]]));
    }

    variableContext->push_back(VariableTable());
    swhile->stm_->accept(this);
    variableContext->pop_back();
}

void TypeChecker::visitSBlock(SBlock *sblock)
{
    /* Code For SBlock Goes Here */

    variableContext->push_back(VariableTable());
    sblock->liststm_->accept(this);
    variableContext->pop_back();
}

void TypeChecker::visitSIfElse(SIfElse *sifelse)
{
    /* Code For SIfElse Goes Here */
    auto type = typecheck(sifelse->exp_);
    if (type.size() == 1 && type[0] == BOOL)
    {

    } else {
        throw std::runtime_error("TYPE ERROR: Wrong conditional type @while: expected bool but got " +
                                 std::string(TypeStrings[type[0]]));
    }

    variableContext->push_back(VariableTable());
    sifelse->stm_1->accept(this);
    variableContext->pop_back();
    variableContext->push_back(VariableTable());
    sifelse->stm_2->accept(this);
    variableContext->pop_back();
}

void TypeChecker::visitETrue(ETrue *etrue)
{
    /* Code For ETrue Goes Here */
    _type = {BOOL};
    isVar = false;
}

void TypeChecker::visitEFalse(EFalse *efalse)
{
    /* Code For EFalse Goes Here */
    _type = {BOOL};
    isVar = false;
}

void TypeChecker::visitEInt(EInt *eint)
{
    /* Code For EInt Goes Here */

    visitInteger(eint->integer_);
    _type = {INT};
    isVar = false;
}

void TypeChecker::visitEDouble(EDouble *edouble)
{
    /* Code For EDouble Goes Here */

    visitDouble(edouble->double_);
    _type = {DOUBLE};
    isVar = false;
}

void TypeChecker::visitEString(EString *estring)
{
    /* Code For EString Goes Here */

    visitString(estring->string_);
    _type = {STRING};
    isVar = false;
}

void TypeChecker::visitEId(EId *eid)
{
    /* Code For EId Goes Here */

    _type = {lookupVariable(eid->id_)};
    isVar = true;
}

void TypeChecker::visitEApp(EApp *eapp) {
    /* Code For EApp Goes Here */

    auto exp_types = typecheck(eapp->listexp_);
    if (functionTable.count(eapp->id_)) {
        auto overloads = functionTable[eapp->id_];
        if (overloads.count(exp_types) == 1) {
            _type = {overloads[exp_types]};
            isVar = false;
        } else {
            // TBA Error Handling
            std::string args = "(";
            for(int i = 0; i < exp_types.size(); i++)
            {
                args += TypeStrings[exp_types[i]];
                if (i < exp_types.size() - 1)
                {
                    args += ", ";
                }
            }
            args += ")";
            throw std::runtime_error("SYNTAX ERROR: Unexpected function call: " + eapp->id_ + " with the arguments " + args + "  doesn't exist");
        }
    } else
    {
        throw std::runtime_error("SYNTAX ERROR: Unexpected function call: " + eapp->id_ + " doesn't exist");
    }
}

void TypeChecker::visitEPIncr(EPIncr *epincr)
{
    /* Code For EPIncr Goes Here */

    auto type = typecheck(epincr->exp_)[0];
    if (isVar)
    {
        if (type == INT || type == DOUBLE)
        {
            _type = {type};
            isVar = false;
        } else
        {
            throw std::runtime_error("TYPE ERROR: Can't increment " + std::string(TypeStrings[type]));
        }
    } else
    {
        throw std::runtime_error("TYPE ERROR: Can't increment non-variable");
    }

}

void TypeChecker::visitEPDecr(EPDecr *epdecr)
{
    /* Code For EPDecr Goes Here */
    auto type = typecheck(epdecr->exp_)[0];
    if (isVar)
    {
        if (type == INT || type == DOUBLE)
        {
            _type = {type};
            isVar = false;
        } else
        {
            throw std::runtime_error("TYPE ERROR: Can't decrement " + std::string(TypeStrings[type]));
        }
    } else
    {
        throw std::runtime_error("TYPE ERROR: Can't increment non-variable");
    }
}

void TypeChecker::visitEIncr(EIncr *eincr)
{
    auto type = typecheck(eincr->exp_)[0];
    if (isVar)
    {
        if (type == INT || type == DOUBLE)
        {
            _type = {type};
            isVar = false;
        } else
        {
            throw std::runtime_error("TYPE ERROR: Can't increment " + std::string(TypeStrings[type]));
        }
    } else
    {
        throw std::runtime_error("TYPE ERROR: Can't increment non-variable");
    }
}

void TypeChecker::visitEDecr(EDecr *edecr)
{
    /* Code For EDecr Goes Here */
    auto type = typecheck(edecr->exp_)[0];
    if (isVar)
    {
        if (type == INT || type == DOUBLE)
        {
            _type = {type};
            isVar = false;
        } else
        {
            throw std::runtime_error("TYPE ERROR: Can't decrement " + std::string(TypeStrings[type]));
        }
    } else
    {
        throw std::runtime_error("TYPE ERROR: Can't increment non-variable");
    }

}

void TypeChecker::visitETimes(ETimes *etimes)
{
    /* Code For ETimes Goes Here */
    auto lhs = typecheck(etimes->exp_1)[0];
    auto rhs = typecheck(etimes->exp_2)[0];

    if (lhs == rhs)
    {
        if (lhs == DOUBLE || lhs == INT)
        {
            _type = {lhs};
            isVar = false;
        } else {
            throw std::runtime_error("TYPE ERROR: Can't apply multiplication to " + std::string(TypeStrings[lhs]));
        }
    } else {
        throw std::runtime_error("TYPE ERROR: @multiplication left and right hand side variables don't match (" +
                                    std::string(TypeStrings[lhs]) + " & " + std::string(TypeStrings[rhs]) + ")");
    }
}

void TypeChecker::visitEDiv(EDiv *ediv)
{
    /* Code For EDiv Goes Here */
    auto lhs = typecheck(ediv->exp_1)[0];
    auto rhs = typecheck(ediv->exp_2)[0];

    if (lhs == rhs)
    {
        if (lhs == DOUBLE || lhs == INT)
        {
            _type = {lhs};
            isVar = false;
        } else {
            throw std::runtime_error("TYPE ERROR: Can't apply division to " + std::string(TypeStrings[lhs]));
        }
    } else {
        throw std::runtime_error("TYPE ERROR: @division left and right hand side variables don't match (" +
                                 std::string(TypeStrings[lhs]) + " & " + std::string(TypeStrings[rhs]) + ")");
    }

}

void TypeChecker::visitEPlus(EPlus *eplus)
{
    /* Code For EPlus Goes Here */
    auto lhs = typecheck(eplus->exp_1)[0];
    auto rhs = typecheck(eplus->exp_2)[0];

    if (lhs == rhs)
    {
        if (lhs == DOUBLE || lhs == INT || lhs == STRING)
        {
            _type = {lhs};
            isVar = false;
        } else {
            throw std::runtime_error("TYPE ERROR: Can't apply addition to " + std::string(TypeStrings[lhs]));
        }
    } else {
        throw std::runtime_error("TYPE ERROR: @addition left and right hand side variables don't match (" + std::string(TypeStrings[lhs])  + std::string(TypeStrings[rhs]) + ")");
    }
}

void TypeChecker::visitEMinus(EMinus *eminus)
{
    /* Code For EMinus Goes Here */
    auto lhs = typecheck(eminus->exp_1)[0];
    auto rhs = typecheck(eminus->exp_2)[0];

    if (lhs == rhs)
    {
        if (lhs == DOUBLE || lhs == INT)
        {
            _type = {lhs};
            isVar = false;
        } else {
            throw std::runtime_error("TYPE ERROR: Can't apply subtraction to " + std::string(TypeStrings[lhs]));
        }
    } else {
        throw std::runtime_error("TYPE ERROR: @subtraction left and right hand side variables don't match (" +
                                 std::string(TypeStrings[lhs]) + " & " + std::string(TypeStrings[rhs]) + ")");
    }
}

void TypeChecker::visitELt(ELt *elt)
{
    /* Code For ELt Goes Here */
    auto lhs = typecheck(elt->exp_1)[0];
    auto rhs = typecheck(elt->exp_2)[0];

    if (lhs == rhs)
    {
        if (lhs == INT || lhs == DOUBLE || lhs == STRING || lhs == BOOL)
        {
            _type = {BOOL};
            isVar = false;
            return;
        }
        throw std::runtime_error("TYPE ERROR: Can't compare " + std::string(TypeStrings[lhs]));
    } else {
        throw std::runtime_error("TYPE ERROR: Can't compare " + std::string(TypeStrings[lhs])
                                 + " and " + std::string(TypeStrings[lhs])) ;
    }
}

void TypeChecker::visitEGt(EGt *egt)
{
    /* Code For EGt Goes Here */
    auto lhs = typecheck(egt->exp_1)[0];
    auto rhs = typecheck(egt->exp_2)[0];

    if (lhs == rhs)
    {
        if (lhs == INT || lhs == DOUBLE || lhs == STRING || lhs == BOOL)
        {
            _type = {BOOL};
            isVar = false;
            return;
        }
        throw std::runtime_error("TYPE ERROR: Can't compare " + std::string(TypeStrings[lhs]));
    } else {
        throw std::runtime_error("TYPE ERROR: Can't compare " + std::string(TypeStrings[lhs])
                                 + " and " + std::string(TypeStrings[lhs])) ;
    }
}

void TypeChecker::visitELtEq(ELtEq *elteq)
{

    auto lhs = typecheck(elteq->exp_1)[0];
    auto rhs = typecheck(elteq->exp_2)[0];

    if (lhs == rhs)
    {
        if (lhs == INT || lhs == DOUBLE || lhs == STRING || lhs == BOOL)
        {
            _type = {BOOL};
            isVar = false;
            return;
        }
        throw std::runtime_error("TYPE ERROR: Can't compare " + std::string(TypeStrings[lhs]));
    } else {
        throw std::runtime_error("TYPE ERROR: Can't compare " + std::string(TypeStrings[lhs])
                                 + " and " + std::string(TypeStrings[lhs])) ;
    }
}

void TypeChecker::visitEGtEq(EGtEq *egteq)
{
    /* Code For EGtEq Goes Here */

    auto lhs = typecheck(egteq->exp_1)[0];
    auto rhs = typecheck(egteq->exp_2)[0];

    if (lhs == rhs)
    {
        if (lhs == INT || lhs == DOUBLE || lhs == STRING || lhs == BOOL)
        {
            _type = {BOOL};
            isVar = false;
            return;
        }
        throw std::runtime_error("TYPE ERROR: Can't compare " + std::string(TypeStrings[lhs]));
    } else {
        throw std::runtime_error("TYPE ERROR: Can't compare " + std::string(TypeStrings[lhs])
                                 + " and " + std::string(TypeStrings[lhs])) ;
    }
}

void TypeChecker::visitEEq(EEq *eeq)
{
    /* Code For EEq Goes Here */

    auto lhs = typecheck(eeq->exp_1)[0];
    auto rhs = typecheck(eeq->exp_2)[0];

    if (lhs == BOOL)
    {
        if (rhs == BOOL)
        {
            _type = {BOOL};
            isVar = false;
            return;
        }
        throw std::runtime_error("TYPE ERROR: Right hand side of greater than or equal comparision not bool, instead " + std::string(TypeStrings[rhs]));
    } else {
        if (rhs != BOOL)
        {
            throw std::runtime_error("TYPE ERROR: Both sides of greater than or equal comparision not bool, instead " +
                                     std::string(TypeStrings[lhs]) + ", " + std::string(TypeStrings[rhs]));
        }
        throw std::runtime_error("TYPE ERROR: Left hand side of greater than or equal comparision not bool, instead " + std::string(TypeStrings[lhs]));
    }

}

void TypeChecker::visitENEq(ENEq *eneq)
{
    /* Code For ENEq Goes Here */

    auto lhs = typecheck(eneq->exp_1)[0];
    auto rhs = typecheck(eneq->exp_2)[0];

    if (lhs == BOOL)
    {
        if (rhs == BOOL)
        {
            _type = {BOOL};
            isVar = false;
            return;
        }
        throw std::runtime_error("TYPE ERROR: Right hand side of greater than or equal comparision not bool, instead " + std::string(TypeStrings[rhs]));
    } else {
        if (rhs != BOOL)
        {
            throw std::runtime_error("TYPE ERROR: Both sides of greater than or equal comparision not bool, instead " +
                                     std::string(TypeStrings[lhs]) + ", " + std::string(TypeStrings[rhs]));
        }
        throw std::runtime_error("TYPE ERROR: Left hand side of greater than or equal comparision not bool, instead " + std::string(TypeStrings[lhs]));
    }

}

void TypeChecker::visitEAnd(EAnd *eand)
{
    /* Code For EAnd Goes Here */

    auto lhs = typecheck(eand->exp_1)[0];
    auto rhs = typecheck(eand->exp_2)[0];

    if (lhs == BOOL)
    {
        if (rhs == BOOL)
        {
            _type = {BOOL};
            isVar = false;
            return;
        }
        throw std::runtime_error("TYPE ERROR: Right hand side of conjunction not bool, instead " + std::string(TypeStrings[rhs]));
    } else {
        if (rhs != BOOL)
        {
            throw std::runtime_error("TYPE ERROR: Both sides of conjunction not bool, instead " +
                                     std::string(TypeStrings[lhs]) + ", " + std::string(TypeStrings[rhs]));
        }
        throw std::runtime_error("TYPE ERROR: Left hand side of conjunction not bool, instead " + std::string(TypeStrings[lhs]));
    }

}

void TypeChecker::visitEOr(EOr *eor)
{
    /* Code For EOr Goes Here */

    auto lhs = typecheck(eor->exp_1)[0];
    auto rhs = typecheck(eor->exp_2)[0];

    if (lhs == BOOL)
    {
        if (rhs == BOOL)
        {
            _type = {BOOL};
            isVar = false;
            return;
        }
        throw std::runtime_error("TYPE ERROR: Right hand side of disjunction not bool, instead " + std::string(TypeStrings[rhs]));
    } else {
        if (rhs != BOOL)
        {
            throw std::runtime_error("TYPE ERROR: Both sides of disjunction not bool, instead " +
                                     std::string(TypeStrings[lhs]) + ", " + std::string(TypeStrings[rhs]));
        }
        throw std::runtime_error("TYPE ERROR: Left hand side of disjunction not bool, instead " + std::string(TypeStrings[lhs]));
    }

}

void TypeChecker::visitEAss(EAss *eass)
{
    /* Code For EAss Goes Here */


    auto lhs = typecheck(eass->exp_1);
    if (isVar)
    {
        auto rhs = typecheck(eass->exp_2);
        if (lhs == rhs)
        {
            _type = {lhs};
            isVar = false;
        }
    }
}

void TypeChecker::visitETyped(ETyped *etyped)
{
    /* Code For ETyped Goes Here */

    etyped->exp_->accept(this);
    etyped->type_->accept(this);
}

void TypeChecker::visitType_bool(Type_bool *type_bool)
{
    /* Code For Type_bool Goes Here */
    _type = {BOOL};
    isVar = false;
}

void TypeChecker::visitType_int(Type_int *type_int)
{
    /* Code For Type_int Goes Here */
    _type = {INT};
    isVar = false;
}

void TypeChecker::visitType_double(Type_double *type_double)
{
    /* Code For Type_double Goes Here */
    _type = {DOUBLE};
    isVar = false;
}

void TypeChecker::visitType_void(Type_void *type_void)
{
    /* Code For Type_void Goes Here */
    _type = {VOID};
    isVar = false;
}

void TypeChecker::visitType_string(Type_string *type_string)
{
    /* Code For Type_string Goes Here */
    _type = {STRING};
    isVar = false;
}


void TypeChecker::visitListDef(ListDef* listdef)
{
    auto type = std::vector<Types>();
    for (auto i = listdef->begin() ; i != listdef->end() ; ++i)
    {
        type.push_back(typecheck(*i)[0]);
    }
    _type = type;
}

void TypeChecker::visitListArg(ListArg* listarg)
{
    auto type = std::vector<Types>();
    for (auto i = listarg->begin() ; i != listarg->end() ; ++i)
    {
        type.push_back(typecheck(*i)[0]);
    }
    _type = type;
}

void TypeChecker::visitListStm(ListStm* liststm)
{
    for (auto i = liststm->begin() ; i != liststm->end() ; ++i)
    {
        (*i)->accept(this);
    }
}

void TypeChecker::visitListExp(ListExp* listexp)
{
    auto type = std::vector<Types>();
    for (auto i = listexp->begin() ; i != listexp->end() ; ++i)
    {
        type.push_back(typecheck(*i)[0]);
    }
    _type = type;
}

void TypeChecker::visitListId(ListId* listid)
{
    auto type = std::vector<Types>();
    for (auto i = listid->begin() ; i != listid->end() ; ++i)
    {
        type.push_back(lookupVariable(*i));
    }
    _type = type;
}


void TypeChecker::visitId(Id x)
{
    /* Code for Id Goes Here */
    _type = {lookupVariable(x)};
}

void TypeChecker::visitInteger(Integer x)
{
    /* Code for Integer Goes Here */
    _type = {INT};
}

void TypeChecker::visitChar(Char x)
{
    /* Code for Char Goes Here */

}

void TypeChecker::visitDouble(Double x)
{
    /* Code for Double Goes Here */
    _type = {DOUBLE};
}

void TypeChecker::visitString(String x)
{
    /* Code for String Goes Here */
    _type = {STRING};
}

void TypeChecker::visitIdent(Ident x)
{
    /* Code for Ident Goes Here */
}



