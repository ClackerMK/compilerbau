/*** BNFC-Generated Visitor Design Pattern ConstantFolder. ***/
/* This implements the common visitor design pattern.
   Note that this method uses Visitor-traversal of lists, so
   List->accept() does NOT traverse the list. This allows different
   algorithms to use context information differently. */

#include "ConstantFolder.H"

bool ConstantFolder::isConstant(Visitable *v) {
    v->accept(this);
    return _constant;
}

Visitable* ConstantFolder::fold(Visitable *v) {
    v->accept(this);
    return _tree;
}

Types ConstantFolder::getType(Visitable *v) {
    v->accept(this);
    return _type;
}

void* ConstantFolder::getValue(Visitable *v) {
    v->accept(this);
    return _value;
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
    _tree = pdefs->clone();
    delete (dynamic_cast<PDefs*>(_tree)->listdef_);
    dynamic_cast<PDefs*>(_tree)->listdef_ = dynamic_cast<ListDef*>(fold(pdefs->listdef_));
}

void ConstantFolder::visitDFun(DFun *dfun)
{
    /* Code For DFun Goes Here */
    _tree = dfun->clone();
    delete(dynamic_cast<DFun*>(_tree)->liststm_);
    dynamic_cast<DFun*>(_tree)->liststm_ = dynamic_cast<ListStm*>(fold(dfun->liststm_));
}

void ConstantFolder::visitADecl(ADecl *adecl)
{
    /* Code For ADecl Goes Here */
/*
    adecl->type_->accept(this);
    visitId(adecl->id_);
*/
    // TODO Add Variable handling
    _tree = adecl;
}

void ConstantFolder::visitSExp(SExp *sexp)
{

    /* Code For SExp Goes Here */
    _tree = sexp->clone();
    delete(dynamic_cast<SExp*>(_tree)->exp_);
    dynamic_cast<SExp*>(_tree)->exp_ = dynamic_cast<Exp*>(fold(sexp->exp_));
}

void ConstantFolder::visitSDecls(SDecls *sdecls)
{

    // TODO Add Variable handling
    /* Code For SDecls Goes Here */
    _tree = std::unique_ptr(sdecls->clone());
}

void ConstantFolder::visitSInit(SInit *sinit)
{
    /* Code For SInit Goes Here */
    _tree = sinit->clone();

    delete(dynamic_cast<SInit*>(_tree)->exp_);
    dynamic_cast<SInit*>(_tree)->exp_ = dynamic_cast<Exp*>(fold(sinit->exp_));
}

void ConstantFolder::visitSReturn(SReturn *sreturn)
{
    /* Code For SReturn Goes Here */
    _tree = sreturn->clone();

    delete(dynamic_cast<SReturn*>(_tree)->exp_);
    dynamic_cast<SReturn*>(_tree)->exp_ = dynamic_cast<Exp*>(fold(sreturn->exp_));
}

void ConstantFolder::visitSReturnVoid(SReturnVoid *sreturnvoid)
{
    /* Code For SReturnVoid Goes Here */
    _tree = sreturnvoid;
}

void ConstantFolder::visitSWhile(SWhile *swhile)
{
    /* Code For SWhile Goes Here */

    _tree = new SWhile(dynamic_cast<Exp*>(fold(swhile->exp_)), dynamic_cast<Stm*>(fold(swhile->stm_)));
}

void ConstantFolder::visitSBlock(SBlock *sblock)
{
    /* Code For SBlock Goes Here */

    _tree = new SBlock(dynamic_cast<ListStm*>(fold(sblock->liststm_)));

}

void ConstantFolder::visitSIfElse(SIfElse *sifelse)
{
    /* Code For SIfElse Goes Here */
    if (isConstant(sifelse->exp_))
    {
        if(dynamic_cast<ETrue*>(fold(sifelse->exp_)))
        {
            _tree = fold(sifelse->stm_1);
        } else
        {
            _tree = fold(sifelse->stm_2);
        }
    } else
    {
        _tree = new SIfElse(dynamic_cast<Exp*>(fold(sifelse->exp_)),
                            dynamic_cast<Stm*>(fold(sifelse->stm_1)), dynamic_cast<Stm*>(fold(sifelse->stm_2)));
    }
}

void ConstantFolder::visitETrue(ETrue *etrue)
{
    /* Code For ETrue Goes Here */
    _type = BOOL;
    delete (_value);
    _value = new bool;
    *dynamic_cast<bool*>(_value) = true;
    _constant = true;
}

void ConstantFolder::visitEFalse(EFalse *efalse)
{
    /* Code For EFalse Goes Here */
    _type = BOOL;
    delete (_value);
    _value = new bool;
    *dynamic_cast<bool*>(_value) = false;
    _constant = true;
}

void ConstantFolder::visitEInt(EInt *eint)
{
    /* Code For EInt Goes Here */

    _type = INT;
    delete (_value);
    _value = new Integer;
    *dynamic_cast<Integer*>(_value) = eint->integer_;
    _constant = true;

}

void ConstantFolder::visitEDouble(EDouble *edouble)
{
    /* Code For EDouble Goes Here */

    _type = DOUBLE;
    delete (_value);
    _value = new Double;
    *dynamic_cast<Double*>(_value) = edouble->double_;
    _constant = true;
}

void ConstantFolder::visitEString(EString *estring)
{
    /* Code For EString Goes Here */


    _type = STRING;
    delete (_value);
    _value = new String;
    *dynamic_cast<String*>(_value) = estring->string_;
    _constant = true;

}

void ConstantFolder::visitEId(EId *eid)
{
    /* Code For EId Goes Here */
    _constant  = false;
    // TODO Support for variables
}

void ConstantFolder::visitEApp(EApp *eapp)
{
    /* Code For EApp Goes Here */

    visitId(eapp->id_);
    eapp->listexp_->accept(this);
    // TODO Support for function calls?
}

void ConstantFolder::visitEPIncr(EPIncr *epincr)
{
    /* Code For EPIncr Goes Here */

    epincr->exp_->accept(this);

}

void ConstantFolder::visitEPDecr(EPDecr *epdecr)
{
    /* Code For EPDecr Goes Here */

    epdecr->exp_->accept(this);

}

void ConstantFolder::visitEIncr(EIncr *eincr)
{
    /* Code For EIncr Goes Here */

    eincr->exp_->accept(this);

}

void ConstantFolder::visitEDecr(EDecr *edecr)
{
    /* Code For EDecr Goes Here */

    edecr->exp_->accept(this);

}

void ConstantFolder::visitETimes(ETimes *etimes)
{
    /* Code For ETimes Goes Here */

    etimes->exp_1->accept(this);
    etimes->exp_2->accept(this);

}

void ConstantFolder::visitEDiv(EDiv *ediv)
{
    /* Code For EDiv Goes Here */

    ediv->exp_1->accept(this);
    ediv->exp_2->accept(this);

}

void ConstantFolder::visitEPlus(EPlus *eplus)
{
    /* Code For EPlus Goes Here */

    eplus->exp_1->accept(this);
    eplus->exp_2->accept(this);

}

void ConstantFolder::visitEMinus(EMinus *eminus)
{
    /* Code For EMinus Goes Here */

    eminus->exp_1->accept(this);
    eminus->exp_2->accept(this);

}

void ConstantFolder::visitELt(ELt *elt)
{
    /* Code For ELt Goes Here */

    elt->exp_1->accept(this);
    elt->exp_2->accept(this);

}

void ConstantFolder::visitEGt(EGt *egt)
{
    /* Code For EGt Goes Here */

    egt->exp_1->accept(this);
    egt->exp_2->accept(this);

}

void ConstantFolder::visitELtEq(ELtEq *elteq)
{
    /* Code For ELtEq Goes Here */

    elteq->exp_1->accept(this);
    elteq->exp_2->accept(this);

}

void ConstantFolder::visitEGtEq(EGtEq *egteq)
{
    /* Code For EGtEq Goes Here */

    egteq->exp_1->accept(this);
    egteq->exp_2->accept(this);

}

void ConstantFolder::visitEEq(EEq *eeq)
{
    /* Code For EEq Goes Here */

    eeq->exp_1->accept(this);
    eeq->exp_2->accept(this);

}

void ConstantFolder::visitENEq(ENEq *eneq)
{
    /* Code For ENEq Goes Here */

    eneq->exp_1->accept(this);
    eneq->exp_2->accept(this);

}

void ConstantFolder::visitEAnd(EAnd *eand)
{
    /* Code For EAnd Goes Here */

    eand->exp_1->accept(this);
    eand->exp_2->accept(this);

}

void ConstantFolder::visitEOr(EOr *eor)
{
    /* Code For EOr Goes Here */

    eor->exp_1->accept(this);
    eor->exp_2->accept(this);

}

void ConstantFolder::visitEAss(EAss *eass)
{
    /* Code For EAss Goes Here */

    eass->exp_1->accept(this);
    eass->exp_2->accept(this);

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
    for (ListDef::iterator i = listdef->begin() ; i != listdef->end() ; ++i)
    {
        (*i)->accept(this);
    }
}

void ConstantFolder::visitListArg(ListArg* listarg)
{
    for (ListArg::iterator i = listarg->begin() ; i != listarg->end() ; ++i)
    {
        (*i)->accept(this);
    }
}

void ConstantFolder::visitListStm(ListStm* liststm)
{
    for (ListStm::iterator i = liststm->begin() ; i != liststm->end() ; ++i)
    {
        (*i)->accept(this);
    }
}

void ConstantFolder::visitListExp(ListExp* listexp)
{
    for (ListExp::iterator i = listexp->begin() ; i != listexp->end() ; ++i)
    {
        (*i)->accept(this);
    }
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
    _constant = false;
}

void ConstantFolder::visitInteger(Integer x)
{
    /* Code for Integer Goes Here */
    _constant = true;
}

void ConstantFolder::visitChar(Char x)
{
    /* Code for Char Goes Here */
    _constant = true;
}

void ConstantFolder::visitDouble(Double x)
{
    /* Code for Double Goes Here */
    _constant = true;
}

void ConstantFolder::visitString(String x)
{
    /* Code for String Goes Here */
    _constant = true;
}

void ConstantFolder::visitIdent(Ident x)
{
    /* Code for Ident Goes Here */
    _constant = false;
}



