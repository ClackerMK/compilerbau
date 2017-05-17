/*** BNFC-Generated Visitor Design Pattern SymbolTableBuilder. ***/
/* This implements the common visitor design pattern.
   Note that this method uses Visitor-traversal of lists, so
   List->accept() does NOT traverse the list. This allows different
   algorithms to use context information differently. */

#include "SymbolTableBuilder.H"



void SymbolTableBuilder::visitProgram(Program* t) {} //abstract class
void SymbolTableBuilder::visitDef(Def* t) {} //abstract class
void SymbolTableBuilder::visitArg(Arg* t) {} //abstract class
void SymbolTableBuilder::visitStm(Stm* t) {} //abstract class
void SymbolTableBuilder::visitExp(Exp* t) {} //abstract class
void SymbolTableBuilder::visitType(Type* t) {} //abstract class

void SymbolTableBuilder::visitPDefs(PDefs *pdefs)
{
    /* Code For PDefs Goes Here */

    pdefs->listdef_->accept(this);

}

void SymbolTableBuilder::visitDFun(DFun *dfun)
{
    /* Code For DFun Goes Here */

    auto ret_type = getType(dfun->type_)[0];
    auto args     = getType(dfun->listarg_);

    if (table.count(dfun->id_) == 1 ) {
        if (table[dfun->id_].count(args) == 0)
        {
            table[dfun->id_][args] = ret_type;
        } else
        {
            // TBA Error Handling
            throw std::runtime_error("SYNTAX ERROR\n\n Multiple function declarations with the same name and arguments");
        }
    }
    else
    {
        table[dfun->id_] = FunctionSignature();
        table[dfun->id_][args] = ret_type;
    }
}

void SymbolTableBuilder::visitADecl(ADecl *adecl)
{
    /* Code For ADecl Goes Here */

    _type = getType(adecl->type_);
}

void SymbolTableBuilder::visitSExp(SExp *sexp)
{
    /* Code For SExp Goes Here */

    sexp->exp_->accept(this);

}

void SymbolTableBuilder::visitSDecls(SDecls *sdecls)
{
    /* Code For SDecls Goes Here */

    sdecls->type_->accept(this);
    sdecls->listid_->accept(this);

}

void SymbolTableBuilder::visitSInit(SInit *sinit)
{
    /* Code For SInit Goes Here */

    sinit->type_->accept(this);
    visitId(sinit->id_);
    sinit->exp_->accept(this);

}

void SymbolTableBuilder::visitSReturn(SReturn *sreturn)
{
    /* Code For SReturn Goes Here */

    sreturn->exp_->accept(this);

}

void SymbolTableBuilder::visitSReturnVoid(SReturnVoid *sreturnvoid)
{
    /* Code For SReturnVoid Goes Here */


}

void SymbolTableBuilder::visitSWhile(SWhile *swhile)
{
    /* Code For SWhile Goes Here */

    swhile->exp_->accept(this);
    swhile->stm_->accept(this);

}

void SymbolTableBuilder::visitSBlock(SBlock *sblock)
{
    /* Code For SBlock Goes Here */

    sblock->liststm_->accept(this);

}

void SymbolTableBuilder::visitSIfElse(SIfElse *sifelse)
{
    /* Code For SIfElse Goes Here */

    sifelse->exp_->accept(this);
    sifelse->stm_1->accept(this);
    sifelse->stm_2->accept(this);

}

void SymbolTableBuilder::visitETrue(ETrue *etrue)
{
    /* Code For ETrue Goes Here */


}

void SymbolTableBuilder::visitEFalse(EFalse *efalse)
{
    /* Code For EFalse Goes Here */


}

void SymbolTableBuilder::visitEInt(EInt *eint)
{
    /* Code For EInt Goes Here */

    visitInteger(eint->integer_);

}

void SymbolTableBuilder::visitEDouble(EDouble *edouble)
{
    /* Code For EDouble Goes Here */

    visitDouble(edouble->double_);

}

void SymbolTableBuilder::visitEString(EString *estring)
{
    /* Code For EString Goes Here */

    visitString(estring->string_);

}

void SymbolTableBuilder::visitEId(EId *eid)
{
    /* Code For EId Goes Here */

    visitId(eid->id_);

}

void SymbolTableBuilder::visitEApp(EApp *eapp)
{
    /* Code For EApp Goes Here */

    visitId(eapp->id_);
    eapp->listexp_->accept(this);

}

void SymbolTableBuilder::visitEPIncr(EPIncr *epincr)
{
    /* Code For EPIncr Goes Here */

    epincr->exp_->accept(this);

}

void SymbolTableBuilder::visitEPDecr(EPDecr *epdecr)
{
    /* Code For EPDecr Goes Here */

    epdecr->exp_->accept(this);

}

void SymbolTableBuilder::visitEIncr(EIncr *eincr)
{
    /* Code For EIncr Goes Here */

    eincr->exp_->accept(this);

}

void SymbolTableBuilder::visitEDecr(EDecr *edecr)
{
    /* Code For EDecr Goes Here */

    edecr->exp_->accept(this);

}

void SymbolTableBuilder::visitETimes(ETimes *etimes)
{
    /* Code For ETimes Goes Here */

    etimes->exp_1->accept(this);
    etimes->exp_2->accept(this);

}

void SymbolTableBuilder::visitEDiv(EDiv *ediv)
{
    /* Code For EDiv Goes Here */

    ediv->exp_1->accept(this);
    ediv->exp_2->accept(this);

}

void SymbolTableBuilder::visitEPlus(EPlus *eplus)
{
    /* Code For EPlus Goes Here */

    eplus->exp_1->accept(this);
    eplus->exp_2->accept(this);

}

void SymbolTableBuilder::visitEMinus(EMinus *eminus)
{
    /* Code For EMinus Goes Here */

    eminus->exp_1->accept(this);
    eminus->exp_2->accept(this);

}

void SymbolTableBuilder::visitELt(ELt *elt)
{
    /* Code For ELt Goes Here */

    elt->exp_1->accept(this);
    elt->exp_2->accept(this);

}

void SymbolTableBuilder::visitEGt(EGt *egt)
{
    /* Code For EGt Goes Here */

    egt->exp_1->accept(this);
    egt->exp_2->accept(this);

}

void SymbolTableBuilder::visitELtEq(ELtEq *elteq)
{
    /* Code For ELtEq Goes Here */

    elteq->exp_1->accept(this);
    elteq->exp_2->accept(this);

}

void SymbolTableBuilder::visitEGtEq(EGtEq *egteq)
{
    /* Code For EGtEq Goes Here */

    egteq->exp_1->accept(this);
    egteq->exp_2->accept(this);

}

void SymbolTableBuilder::visitEEq(EEq *eeq)
{
    /* Code For EEq Goes Here */

    eeq->exp_1->accept(this);
    eeq->exp_2->accept(this);

}

void SymbolTableBuilder::visitENEq(ENEq *eneq)
{
    /* Code For ENEq Goes Here */

    eneq->exp_1->accept(this);
    eneq->exp_2->accept(this);

}

void SymbolTableBuilder::visitEAnd(EAnd *eand)
{
    /* Code For EAnd Goes Here */

    eand->exp_1->accept(this);
    eand->exp_2->accept(this);

}

void SymbolTableBuilder::visitEOr(EOr *eor)
{
    /* Code For EOr Goes Here */

    eor->exp_1->accept(this);
    eor->exp_2->accept(this);

}

void SymbolTableBuilder::visitEAss(EAss *eass)
{
    /* Code For EAss Goes Here */

    eass->exp_1->accept(this);
    eass->exp_2->accept(this);

}

void SymbolTableBuilder::visitETyped(ETyped *etyped)
{
    /* Code For ETyped Goes Here */

    etyped->exp_->accept(this);
    etyped->type_->accept(this);

}

void SymbolTableBuilder::visitType_bool(Type_bool *type_bool)
{
    /* Code For Type_bool Goes Here */
    _type = {BOOL};
}

void SymbolTableBuilder::visitType_int(Type_int *type_int)
{
    /* Code For Type_int Goes Here */
    _type = {INT};


}

void SymbolTableBuilder::visitType_double(Type_double *type_double)
{
    /* Code For Type_double Goes Here */
    _type = {DOUBLE};


}

void SymbolTableBuilder::visitType_void(Type_void *type_void)
{
    /* Code For Type_void Goes Here */
    _type = {VOID};


}

void SymbolTableBuilder::visitType_string(Type_string *type_string)
{
    /* Code For Type_string Goes Here */
    _type = {STRING};

}


void SymbolTableBuilder::visitListDef(ListDef* listdef)
{
    for (ListDef::iterator i = listdef->begin() ; i != listdef->end() ; ++i)
    {
        (*i)->accept(this);
    }
}

void SymbolTableBuilder::visitListArg(ListArg* listarg)
{
    auto type = std::vector<Types>();
    for (ListArg::iterator i = listarg->begin() ; i != listarg->end() ; ++i)
    {
        type.push_back(getType(*i)[0]);
    }
    _type = type;
}

void SymbolTableBuilder::visitListStm(ListStm* liststm)
{
    for (ListStm::iterator i = liststm->begin() ; i != liststm->end() ; ++i)
    {
        (*i)->accept(this);
    }
}

void SymbolTableBuilder::visitListExp(ListExp* listexp)
{
    for (ListExp::iterator i = listexp->begin() ; i != listexp->end() ; ++i)
    {
        (*i)->accept(this);
    }
}

void SymbolTableBuilder::visitListId(ListId* listid)
{
    for (ListId::iterator i = listid->begin() ; i != listid->end() ; ++i)
    {
        visitId(*i) ;
    }
}


void SymbolTableBuilder::visitId(Id x)
{
    /* Code for Id Goes Here */
}

void SymbolTableBuilder::visitInteger(Integer x)
{
    /* Code for Integer Goes Here */
    _type = {INT};
}

void SymbolTableBuilder::visitChar(Char x)
{
    /* Code for Char Goes Here */
}

void SymbolTableBuilder::visitDouble(Double x)
{
    /* Code for Double Goes Here */
    _type = {DOUBLE};
}

void SymbolTableBuilder::visitString(String x)
{
    /* Code for String Goes Here */
    _type = {STRING};
}

void SymbolTableBuilder::visitIdent(Ident x)
{
    /* Code for Ident Goes Here */
}



