/*** BNFC-Generated Visitor Design Pattern ConstantFolder. ***/
/* This implements the common visitor design pattern.
   Note that this method uses Visitor-traversal of lists, so
   List->accept() does NOT traverse the list. This allows different
   algorithms to use context information differently. */

#include "ConstantFolder.H"

Visitable* ConstantFolder::fold(Visitable *v) {
    v->accept(this);
    return _tree;
}

Types ConstantFolder::getType(Visitable *v) {
    // Es tut mir leid
    EId* var;
    if (dynamic_cast<ETrue*>(v) != nullptr || dynamic_cast<EFalse*>(v) != nullptr)
    {
        return Types::BOOL;
    } else if (dynamic_cast<EInt*>(v) != nullptr)
    {
        return Types::INT;
    } else if (dynamic_cast<EDouble*>(v) != nullptr)
    {
        return Types::DOUBLE;
    } else if (dynamic_cast<EString*>(v) != nullptr)
    {
        return Types::STRING;
    } else if ((var = dynamic_cast<EId*>(v)) != nullptr)
    {
        return variables[var->id_].first;
    }
    else {
        throw new std::invalid_argument("Expected Constant or ID");
    }
}


ConstantFolder::value ConstantFolder::getValue(Visitable *v) {
    ConstantFolder::value val;
    EInt* _int;
    EDouble* _double;
    EString* _string;
    EId* _id;
    if (dynamic_cast<ETrue*>(v) != nullptr) {
        val.Bl = true;
    }else if (dynamic_cast<EFalse*>(v) != nullptr)
    {
        val.Bl = false;
    } else if ((_int = dynamic_cast<EInt*>(v)) != nullptr)
    {
        val.Int = _int->integer_;
    } else if ((_double = dynamic_cast<EDouble*>(v)) != nullptr)
    {
        val.Dbl = _double->double_;
    } else if ((_string = dynamic_cast<EString*>(v)) != nullptr)
    {
        val.Str = _string->string_;
    } else if ((_id = dynamic_cast<EId*>(v)))
    {
        val = variables[_id->id_].second;
    }
    else {
        throw new std::invalid_argument("Expected Constant");
    }
    return val;
}


void ConstantFolder::visitProgram(Program* t) {} //abstract class
void ConstantFolder::visitDef(Def* t) {} //abstract class
void ConstantFolder::visitArg(Arg* t) {} //abstract class
void ConstantFolder::visitStm(Stm* t) {} //abstract class
void ConstantFolder::visitExp(Exp* t) {} //abstract class
void ConstantFolder::visitType(Type* t) {} //abstract class

void ConstantFolder::visitPDefs(PDefs *pdefs)
{
    /* Code For PDefs Goes Here */
    //pdefs->listdef_->accept(this);
    auto pdefs_ = pdefs->clone();
    delete (pdefs_->listdef_);
    dynamic_cast<PDefs*>(pdefs_)->listdef_ = dynamic_cast<ListDef*>(fold(pdefs->listdef_));
    _tree = pdefs_;
}


void ConstantFolder::visitDFun(DFun *dfun)
{
    /* Code For DFun Goes Here */
    auto _dfun = dfun->clone();
    delete(_dfun->liststm_);
    removeVar();
    _dfun->liststm_ = dynamic_cast<ListStm*>(fold(dfun->liststm_));
    _tree = _dfun;
    removeVar();
}

void ConstantFolder::visitADecl(ADecl *adecl)
{
    /* Code For ADecl Goes Here */
/*
    adecl->type_->accept(this);
    visitId(adecl->id_);
*/
    // TODO Add Variable handling
    _tree = adecl->clone();
}

void ConstantFolder::visitSExp(SExp *sexp)
{

    /* Code For SExp Goes Here */
    auto _sexp = sexp->clone();

    _sexp->exp_ = dynamic_cast<Exp*>(fold(sexp->exp_));
    _tree = _sexp;
}

void ConstantFolder::visitSDecls(SDecls *sdecls)
{

    /* Code For SDecls Goes Here */
    _tree = sdecls->clone();
}

void ConstantFolder::visitSInit(SInit *sinit)
{
    /* Code For SInit Goes Here */
    // TODO Add Variable cloning
    auto _e1 = fold(sinit->exp_);

    if (isConstant(_e1))
    {
        setVar(sinit->id_, getType(_e1), getValue(_e1));
    }

    _tree = new SInit(sinit->type_->clone(), sinit->id_, dynamic_cast<Exp*>(_e1));
}

void ConstantFolder::visitSReturn(SReturn *sreturn)
{
    /* Code For SReturn Goes Here */
    auto _sreturn = sreturn->clone();

    delete _sreturn->exp_;
    _sreturn ->exp_ = dynamic_cast<Exp*>(fold(sreturn->exp_));
    _tree = _sreturn;
}

void ConstantFolder::visitSReturnVoid(SReturnVoid *sreturnvoid)
{
    /* Code For SReturnVoid Goes Here */
    _tree = sreturnvoid->clone();
}

void ConstantFolder::visitSWhile(SWhile *swhile)
{
    /* Code For SWhile Goes Here */

    removeVar();
    _tree = new SWhile(dynamic_cast<Exp*>(fold(swhile->exp_)), dynamic_cast<Stm*>(fold(swhile->stm_)));
    removeVar();
}

void ConstantFolder::visitSBlock(SBlock *sblock)
{
    /* Code For SBlock Goes Here */
    removeVar();
    _tree = new SBlock(dynamic_cast<ListStm*>(fold(sblock->liststm_)));
    removeVar();

}

void ConstantFolder::visitSIfElse(SIfElse *sifelse)
{
    /* Code For SIfElse Goes Here */
    auto exp = fold(sifelse->exp_);
    if (isConstant(exp))
    {
        if (getType(exp) != BOOL)
        {
            throw new std::invalid_argument("Wrong Type @ If-Else");
        }
        if(getValue(exp).Bl)
        {
            removeVar();
            _tree = fold(sifelse->stm_1);
            removeVar();
        } else
        {
            removeVar();
            _tree = fold(sifelse->stm_2);
            removeVar();
        }
    } else
    {
        removeVar();
        _tree = new SIfElse(dynamic_cast<Exp*>(exp),
                            dynamic_cast<Stm*>(fold(sifelse->stm_1)), dynamic_cast<Stm*>(fold(sifelse->stm_2)));
        removeVar();

    }
}

void ConstantFolder::visitETrue(ETrue *etrue)
{
    /* Code For ETrue Goes Here */
    _tree = new ETrue;
}

void ConstantFolder::visitEFalse(EFalse *efalse)
{
    /* Code For EFalse Goes Here */
    _tree = new EFalse;
}

void ConstantFolder::visitEInt(EInt *eint)
{
    /* Code For EInt Goes Here */
    _tree = new EInt(eint->integer_);
}

void ConstantFolder::visitEDouble(EDouble *edouble)
{
    /* Code For EDouble Goes Here */
    _tree = new EDouble(edouble->double_);
}

void ConstantFolder::visitEString(EString *estring)
{
    /* Code For EString Goes Here */
    _tree = new EString(estring->string_);
}

void ConstantFolder::visitEId(EId *eid)
{
    /* Code For EId Goes Here */
    auto v = lookup(eid->id_);

    if (v != nullptr)
    {
        switch (v->first)
        {
            case INT:
                _tree = new EInt(v->second.Int);
                break;
            case DOUBLE:
                _tree = new EDouble(v->second.Dbl);
                break;
            case STRING:
                _tree = new EString(v->second.Str);
                break;
            case BOOL:
                if (v->second.Bl)
                {
                    _tree = new ETrue;
                } else
                {
                    _tree = new EFalse;
                }
                break;
        }
    } else
    {
        _tree = eid->clone();
    }
    // TODO Support for variables

}

void ConstantFolder::visitEApp(EApp *eapp)
{
    /* Code For EApp Goes Here */
    visitId(eapp->id_);
    eapp->listexp_->accept(this);
    _tree = new EApp(eapp->id_, dynamic_cast<ListExp*>(fold(eapp->listexp_)));
    // TODO Support for function calls?
}

// ToDo ?? Keep Variables intact if possible?
void ConstantFolder::visitEPIncr(EPIncr *epincr)
{
    /* Code For EPIncr Goes Here */
    _tree = epincr->clone();
    removeVar();
}

void ConstantFolder::visitEPDecr(EPDecr *epdecr)
{
    /* Code For EPDecr Goes Here */
    _tree = epdecr->clone();
    removeVar();

}

void ConstantFolder::visitEIncr(EIncr *eincr)
{
    /* Code For EIncr Goes Here */
    _tree = eincr->clone();
    removeVar();

}

void ConstantFolder::visitEDecr(EDecr *edecr)
{
    /* Code For EDecr Goes Here */
    _tree = edecr->clone();
    removeVar();

}

void ConstantFolder::visitETimes(ETimes *etimes)
{
    /* Code For ETimes Goes Here */

    auto _e1 = fold(etimes->exp_1);
    auto _e2 = fold(etimes->exp_2);

    if (isConstant(_e1) && isConstant(_e2))
    {
        switch (getType(_e1))
        {
            case Types::INT :
                _tree = new EInt(getValue(_e1).Int * getValue(_e2).Int);
                break;
            case Types::DOUBLE:
                _tree = new EDouble(getValue(_e1).Dbl * getValue(_e2).Dbl);
                break;
            default:
                throw new std::invalid_argument("Wrong Type @ ETimes");
        }
    } else
    {
        _tree = new ETimes(dynamic_cast<Exp*>(_e1), dynamic_cast<Exp*>(_e2));
    }
}

void ConstantFolder::visitEDiv(EDiv *ediv)
{
    /* Code For EDiv Goes Here */
    auto _e1 = fold(ediv->exp_1);
    auto _e2 = fold(ediv->exp_2);

    if (isConstant(_e1) && isConstant(_e2))
    {
        switch (getType(_e1))
        {
            case Types::INT :
                _tree = new EInt(getValue(_e1).Int / getValue(_e2).Int);
                break;
            case Types::DOUBLE:
                _tree = new EDouble(getValue(_e1).Dbl / getValue(_e2).Dbl);
                break;
            default:
                throw new std::invalid_argument("Wrong Type @ EDiv");
        }
    } else
    {
        _tree = new EDiv(dynamic_cast<Exp*>(_e1), dynamic_cast<Exp*>(_e2));
    }
}

void ConstantFolder::visitEPlus(EPlus *eplus)
{
    /* Code For EPlus Goes Here */
    auto _e1 = fold(eplus->exp_1);
    auto _e2 = fold(eplus->exp_2);

    if (isConstant(_e1) && isConstant(_e2))
    {
        switch (getType(_e1))
        {
            case Types::INT :
                _tree = new EInt(getValue(_e1).Int + getValue(_e2).Int);
                break;
            case Types::DOUBLE:
                _tree = new EDouble(getValue(_e1).Dbl + getValue(_e2).Dbl);
                break;
            case Types::STRING:
                _tree = new EString(getValue(_e1).Str + getValue(_e2).Str);
            default:
                throw new std::invalid_argument("Wrong Type @ EPlus");
        }
    } else
    {
        _tree = new EPlus(dynamic_cast<Exp*>(_e1), dynamic_cast<Exp*>(_e2));
    }
}

void ConstantFolder::visitEMinus(EMinus *eminus)
{
    /* Code For EMinus Goes Here */

    auto _e1 = fold(eminus->exp_1);
    auto _e2 = fold(eminus->exp_2);

    if (isConstant(_e1) && isConstant(_e2))
    {
        switch (getType(_e1))
        {
            case Types::INT :
                _tree = new EInt(getValue(_e1).Int - getValue(_e2).Int);
                break;
            case Types::DOUBLE:
                _tree = new EDouble(getValue(_e1).Dbl - getValue(_e2).Dbl);
                break;
            default:
                throw new std::invalid_argument("Wrong Type @ EMinus");
        }
    } else
    {
        _tree = new EMinus(dynamic_cast<Exp*>(_e1), dynamic_cast<Exp*>(_e2));
    }

}

void ConstantFolder::visitELt(ELt *elt)
{
    /* Code For ELt Goes Here */
    auto _e1 = fold(elt->exp_1);
    auto _e2 = fold(elt->exp_2);

    if (isConstant(_e1) && isConstant(_e2))
    {
        switch (getType(_e1))
        {
            case Types::INT :
                if (getValue(_e1).Int < getValue(_e2).Int)
                {
                    _tree = new ETrue;
                } else
                {
                    _tree = new EFalse;
                }
                break;
            case Types::DOUBLE:
                if (getValue(_e1).Dbl < getValue(_e2).Dbl)
                {
                    _tree = new ETrue;
                } else
                {
                    _tree = new EFalse;
                }
                break;
            case Types::BOOL:
                if (getValue(_e1).Bl < getValue(_e2).Bl)
                {
                    _tree = new ETrue;
                } else
                {
                    _tree = new EFalse;
                }
                break;
            case Types::STRING:
                if (getValue(_e1).Str < getValue(_e2).Str)
                {
                    _tree = new ETrue;
                } else
                {
                    _tree = new EFalse;
                }
                break;
            default:
                throw new std::invalid_argument("Wrong Type @ EMinus");
        }
    } else
    {
        _tree = new ELt(dynamic_cast<Exp*>(_e1), dynamic_cast<Exp*>(_e2));
    }
}

void ConstantFolder::visitEGt(EGt *egt)
{
    /* Code For EGt Goes Here */
    auto _e1 = fold(egt->exp_1);
    auto _e2 = fold(egt->exp_2);

    if (isConstant(_e1) && isConstant(_e2))
    {
        switch (getType(_e1))
        {
            case Types::INT :
                if (getValue(_e1).Int > getValue(_e2).Int)
                {
                    _tree = new ETrue;
                } else
                {
                    _tree = new EFalse;
                }
                break;
            case Types::DOUBLE:
                if (getValue(_e1).Dbl > getValue(_e2).Dbl)
                {
                    _tree = new ETrue;
                } else
                {
                    _tree = new EFalse;
                }
                break;
            case Types::BOOL:
                if (getValue(_e1).Bl > getValue(_e2).Bl)
                {
                    _tree = new ETrue;
                } else
                {
                    _tree = new EFalse;
                }
                break;
            case Types::STRING:
                if (getValue(_e1).Str > getValue(_e2).Str)
                {
                    _tree = new ETrue;
                } else
                {
                    _tree = new EFalse;
                }
                break;
            default:
                throw new std::invalid_argument("Wrong Type @ EMinus");
        }
    } else
    {
        _tree = new ELt(dynamic_cast<Exp*>(_e1), dynamic_cast<Exp*>(_e2));
    }
}

void ConstantFolder::visitELtEq(ELtEq *elteq)
{
    /* Code For ELtEq Goes Here */
    auto _e1 = fold(elteq->exp_1);
    auto _e2 = fold(elteq->exp_2);

    if (isConstant(_e1) && isConstant(_e2))
    {
        switch (getType(_e1))
        {
            case Types::INT :
                if (getValue(_e1).Int <= getValue(_e2).Int)
                {
                    _tree = new ETrue;
                } else
                {
                    _tree = new EFalse;
                }
                break;
            case Types::DOUBLE:
                if (getValue(_e1).Dbl <= getValue(_e2).Dbl)
                {
                    _tree = new ETrue;
                } else
                {
                    _tree = new EFalse;
                }
                break;
            case Types::BOOL:
                if (getValue(_e1).Bl <= getValue(_e2).Bl)
                {
                    _tree = new ETrue;
                } else
                {
                    _tree = new EFalse;
                }
                break;
            case Types::STRING:
                if (getValue(_e1).Str <= getValue(_e2).Str)
                {
                    _tree = new ETrue;
                } else
                {
                    _tree = new EFalse;
                }
                break;
            default:
                throw new std::invalid_argument("Wrong Type @ EMinus");
        }
    } else
    {
        _tree = new ELt(dynamic_cast<Exp*>(_e1), dynamic_cast<Exp*>(_e2));
    }
}

void ConstantFolder::visitEGtEq(EGtEq *egteq)
{
    /* Code For EGtEq Goes Here */
    auto _e1 = fold(egteq->exp_1);
    auto _e2 = fold(egteq->exp_2);

    if (isConstant(_e1) && isConstant(_e2))
    {
        switch (getType(_e1))
        {
            case Types::INT :
                if (getValue(_e1).Int >= getValue(_e2).Int)
                {
                    _tree = new ETrue;
                } else
                {
                    _tree = new EFalse;
                }
                break;
            case Types::DOUBLE:
                if (getValue(_e1).Dbl >= getValue(_e2).Dbl)
                {
                    _tree = new ETrue;
                } else
                {
                    _tree = new EFalse;
                }
                break;
            case Types::BOOL:
                if (getValue(_e1).Bl >= getValue(_e2).Bl)
                {
                    _tree = new ETrue;
                } else
                {
                    _tree = new EFalse;
                }
                break;
            case Types::STRING:
                if (getValue(_e1).Str >= getValue(_e2).Str)
                {
                    _tree = new ETrue;
                } else
                {
                    _tree = new EFalse;
                }
                break;
            default:
                throw new std::invalid_argument("Wrong Type @ EMinus");
        }
    } else
    {
        _tree = new ELt(dynamic_cast<Exp*>(_e1), dynamic_cast<Exp*>(_e2));
    }
}

void ConstantFolder::visitEEq(EEq *eeq)
{
    /* Code For EEq Goes Here */
    auto _e1 = fold(eeq->exp_1);
    auto _e2 = fold(eeq->exp_2);

    if (isConstant(_e1) && isConstant(_e2))
    {
        switch (getType(_e1))
        {
            case Types::INT :
                if (getValue(_e1).Int == getValue(_e2).Int)
                {
                    _tree = new ETrue;
                } else
                {
                    _tree = new EFalse;
                }
                break;
            case Types::DOUBLE:
                if (getValue(_e1).Dbl == getValue(_e2).Dbl)
                {
                    _tree = new ETrue;
                } else
                {
                    _tree = new EFalse;
                }
                break;
            case Types::BOOL:
                if (getValue(_e1).Bl == getValue(_e2).Bl)
                {
                    _tree = new ETrue;
                } else
                {
                    _tree = new EFalse;
                }
                break;
            case Types::STRING:
                if (getValue(_e1).Str == getValue(_e2).Str)
                {
                    _tree = new ETrue;
                } else
                {
                    _tree = new EFalse;
                }
                break;
            default:
                throw new std::invalid_argument("Wrong Type @ EEq");
        }
    } else
    {
        _tree = new ELt(dynamic_cast<Exp*>(_e1), dynamic_cast<Exp*>(_e2));
    }
}

void ConstantFolder::visitENEq(ENEq *eneq)
{
    /* Code For ENEq Goes Here */
    auto _e1 = fold(eneq->exp_1);
    auto _e2 = fold(eneq->exp_2);

    if (isConstant(_e1) && isConstant(_e2))
    {
        switch (getType(_e1))
        {
            case Types::INT :
                if (getValue(_e1).Int != getValue(_e2).Int)
                {
                    _tree = new ETrue;
                } else
                {
                    _tree = new EFalse;
                }
                break;
            case Types::DOUBLE:
                if (getValue(_e1).Dbl != getValue(_e2).Dbl)
                {
                    _tree = new ETrue;
                } else
                {
                    _tree = new EFalse;
                }
                break;
            case Types::BOOL:
                if (getValue(_e1).Bl != getValue(_e2).Bl)
                {
                    _tree = new ETrue;
                } else
                {
                    _tree = new EFalse;
                }
                break;
            case Types::STRING:
                if (getValue(_e1).Str != getValue(_e2).Str)
                {
                    _tree = new ETrue;
                } else
                {
                    _tree = new EFalse;
                }
                break;
            default:
                throw new std::invalid_argument("Wrong Type @ ENeq");
        }
    } else
    {
        _tree = new ELt(dynamic_cast<Exp*>(_e1), dynamic_cast<Exp*>(_e2));
    }
}

void ConstantFolder::visitEAnd(EAnd *eand)
{
    /* Code For EAnd Goes Here */
    auto _e1 = fold(eand->exp_1);
    auto _e2 = fold(eand->exp_2);

    if (isConstant(_e1) && isConstant(_e2))
    {
        switch (getType(_e1))
        {
            case Types::BOOL :
                if (getValue(_e1).Bl && getValue(_e2).Bl)
                {
                    _tree = new ETrue;
                } else
                {
                    _tree = new EFalse;
                }
                break;
            default:
                throw new std::invalid_argument("Wrong Type @ EAnd");
        }
    } else if ((isConstant(_e1) && getType(_e1) && !getValue(_e1).Bl) || isConstant(_e2) && getType(_e2) && !getValue(_e2).Bl )
    {
        _tree = new EFalse;
    } else
    {
        _tree = new ELt(dynamic_cast<Exp*>(_e1), dynamic_cast<Exp*>(_e2));
    }
}

void ConstantFolder::visitEOr(EOr *eor)
{
    /* Code For EOr Goes Here */
    auto _e1 = fold(eor->exp_1);
    auto _e2 = fold(eor->exp_2);

    if (isConstant(_e1) && isConstant(_e2))
    {
        switch (getType(_e1))
        {
            case Types::BOOL :
                if (getValue(_e1).Bl || getValue(_e2).Bl)
                {
                    _tree = new ETrue;
                } else
                {
                    _tree = new EFalse;
                }
                break;
            default:
                throw new std::invalid_argument("Wrong Type @ EOr");
        }
    } else if ((isConstant(_e1) && getType(_e1) && getValue(_e1).Bl) || isConstant(_e2) && getType(_e2) && getValue(_e2).Bl )
    {
        _tree = new ETrue;
    } else
    {
        _tree = new ELt(dynamic_cast<Exp*>(_e1), dynamic_cast<Exp*>(_e2));
    }

}

void ConstantFolder::visitEAss(EAss *eass)
{
    /* Code For EAss Goes Here */
    auto _e1 = eass->exp_1;
    auto _e2 = fold(eass->exp_2);

    if (auto id = dynamic_cast<EId*>(_e1))
    {
        if (isConstant(_e2))
        {
            auto t = getType(_e2);
            auto v = getValue(_e2);
            setVar(id->id_, t, v);
        } else
        {
            removeVar(id->id_);
        }
    }
    _tree = new EAss(dynamic_cast<Exp*>(_e1), dynamic_cast<Exp*>(_e2));
}

void ConstantFolder::visitETyped(ETyped *etyped)
{
    /* Code For ETyped Goes Here */

    etyped->exp_->accept(this);
    etyped->type_->accept(this);

}

void ConstantFolder::visitType_bool(Type_bool *type_bool)
{
    /* Code For Type_bool Goes Here */

}

void ConstantFolder::visitType_int(Type_int *type_int)
{
    /* Code For Type_int Goes Here */


}

void ConstantFolder::visitType_double(Type_double *type_double)
{
    /* Code For Type_double Goes Here */


}

void ConstantFolder::visitType_void(Type_void *type_void)
{
    /* Code For Type_void Goes Here */


}

void ConstantFolder::visitType_string(Type_string *type_string)
{
    /* Code For Type_string Goes Here */


}


void ConstantFolder::visitListDef(ListDef* listdef)
{
    auto _listdef = new ListDef();
    for (ListDef::iterator i = listdef->begin() ; i != listdef->end() ; ++i)
    {
        auto dummy = (*i)->clone();
        _listdef->push_back(dynamic_cast<Def*>(fold(dummy)));
    }
    _tree = _listdef;
}

void ConstantFolder::visitListArg(ListArg* listarg)
{
    auto _listarg = new ListArg();
    for (ListArg::iterator i = listarg->begin() ; i != listarg->end() ; ++i)
    {
        auto dummy = (*i)->clone();
        _listarg->push_back(dynamic_cast<Arg*>(fold(dummy)));
    }
    _tree = _listarg;
}

void ConstantFolder::visitListStm(ListStm* liststm)
{
    auto _liststm = new ListStm();
    for (ListStm::iterator i = liststm->begin() ; i != liststm->end() ; ++i) {
        auto dummy = (*i)->clone();
        _liststm->push_back(dynamic_cast<Stm*>(fold(dummy)));
    }
    _tree = _liststm;
}

void ConstantFolder::visitListExp(ListExp* listexp)
{
    auto _listexp = new ListExp();
    for (ListExp::iterator i = listexp->begin() ; i != listexp->end() ; ++i)
    {
        auto dummy = (*i)->clone();
        _listexp->push_back(dynamic_cast<Exp*>(fold(dummy)));
    }
    _tree = _listexp;
}

void ConstantFolder::visitListId(ListId* listid)
{
    for (ListId::iterator i = listid->begin() ; i != listid->end() ; ++i)
    {
        visitId(*i) ;
    }
}


void ConstantFolder::visitId(Id x)
{
    /* Code for Id Goes Here */
    }

void ConstantFolder::visitInteger(Integer x)
{
    /* Code for Integer Goes Here */
    }

void ConstantFolder::visitChar(Char x)
{
    /* Code for Char Goes Here */
    }

void ConstantFolder::visitDouble(Double x)
{
    /* Code for Double Goes Here */
    }

void ConstantFolder::visitString(String x)
{
    /* Code for String Goes Here */
    }

void ConstantFolder::visitIdent(Ident x)
{
    /* Code for Ident Goes Here */
    }

std::unique_ptr<std::pair<Types, ConstantFolder::value>> ConstantFolder::lookup(Id id) {
    if (variables.count(id) == 1)
    {
        auto d = new std::pair<Types, ConstantFolder::value>();
        *d = variables[id];
        return std::unique_ptr<std::pair<Types, ConstantFolder::value>>(d);
    }
    return nullptr;
}

void ConstantFolder::removeVar() {
    variables.clear();
}

void ConstantFolder::removeVar(Id id) {
    if (variables.count(id) == 1)
    {
        variables.erase(id);
    }
}

void ConstantFolder::setVar(Id id, Types type, ConstantFolder::value val) {
    variables[id] = std::pair<Types, ConstantFolder::value>(type, val);
}
