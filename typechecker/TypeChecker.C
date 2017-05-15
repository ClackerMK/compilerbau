/*** BNFC-Generated Visitor Design Pattern TypeChecker. ***/
/* This implements the common visitor design pattern.
   Note that this method uses Visitor-traversal of lists, so
   List->accept() does NOT traverse the list. This allows different
   algorithms to use context information differently. */

#include "TypeChecker.H"
#include <typeinfo>


FunctionInfo::FunctionInfo(Id id, Type *type) {
    this->id = id;
    this->funcType = type;
}


//constructor
TypeChecker::Env::Env() {}

//destructor
TypeChecker::Env::~Env() {}

//constructor
TypeChecker::TypeChecker() {
    this->passNo = 0;
    this->env = new TypeChecker::Env();
}

//destructor
TypeChecker::~TypeChecker() {
    delete this->env;
}

void TypeChecker::typeCheck(Program* p) {
    p->accept(this);    //build a symbol table with all function and parameter types
                  //debug
    std::cout << "\n### Round 2 ###\n " << std::endl;
    this->passNo += 1;
    p->accept(this);    //type check and annotate the code by using this symbol table
}

std::string TypeChecker::getTypeName(Type *type) {
    if (typeid(*type) == typeid(Type_int)) return "int";
    if (typeid(*type) == typeid(Type_double)) return "double";
    if (typeid(*type) == typeid(Type_string)) return "string";
    if (typeid(*type) == typeid(Type_bool)) return "bool";
    if (typeid(*type) == typeid(Type_void)) return "void";
    return "unknown type";
}

void TypeChecker::visitProgram(Program* t) {} //abstract class
void TypeChecker::visitDef(Def* t) {} //abstract class
void TypeChecker::visitArg(Arg* t) {} //abstract class
void TypeChecker::visitStm(Stm* t) {} //abstract class
void TypeChecker::visitExp(Exp* t) {} //abstract class
void TypeChecker::visitType(Type* t) {} //abstract class

void TypeChecker::visitPDefs(PDefs *pdefs)
{

            //debug
    std::cout << "visitPDefs " << std::endl;
  /* Code For PDefs Goes Here */
    pdefs->listdef_->accept(this);

}

void TypeChecker::visitDFun(DFun *dfun){

            //debug
    std::cout << "visitDFun " << std::endl;
  /* Code For DFun Goes Here */
    if (this->passNo == 0) {


        //test whether function already in funcTable
        std::map<Id, FunctionInfo*>::iterator it = this->env->funcTable.find(dfun->id_);
        if (it != this->env->funcTable.end()) {
            std::cout << "TYPE ERROR\n\n" << "Function " << dfun->id_ << " has already been declared." << std::endl;
            exit(1);
        }
        //add function to funcTable
        FunctionInfo *funcId = new FunctionInfo(dfun->id_, dfun->type_);
        this->env->funcTable.insert(std::pair<Id, FunctionInfo*>(dfun->id_, funcId));


        this->env->visitingFunc = funcId;
        dfun->listarg_->accept(this);
        this->env->visitingFunc = NULL;
    }


    if (this->passNo == 1) {
        /*...
        dfun->type_->accept(this);
        visitId(dfun->id_);*/

        //remember function in which we descend
        this->env->visitingFunc = this->env->funcTable[dfun->id_];
        //save context before entering new block    !!!!unnecessary!!!! weil jeden function eigene var map hat
        std::map<Id, Type*> oldVars = this->env->funcTable[dfun->id_]->vars;

        //
        dfun->liststm_->accept(this);
        //

        //restore old context
        this->env->funcTable[dfun->id_]->vars = oldVars;
        //no longer in any function
        this->env->visitingFunc = NULL;
    }

}

void TypeChecker::visitADecl(ADecl *adecl)
{ std::cout << "passNo: " << this->passNo << " visitADecl" << "\n" << std::endl;
  /* Code For ADecl Goes Here */

    if (this->passNo == 0) {
        if (this->env->visitingFunc) {
            //assume currentType != NULL
            //test whether parameter ID has already been used
            std::map<Id, Type*>::iterator it = this->env->visitingFunc->params.find(adecl->id_);
            if (it != this->env->visitingFunc->params.end()) {
                std::cout << "TYPE ERROR\n\n" << "Parameter ID " << adecl->id_ << " has already been used." << std::endl;
                exit(1);
            }
            //add parameter to map
            this->env->visitingFunc->params.insert(std::pair<Id, Type*>(adecl->id_, adecl->type_));

            //debug
            std::cout << "Added param " << adecl->id_;
            std::cout << " to function " << this->env->visitingFunc->id << std::endl;

            this->env->currentType = NULL;
        }
    }


    /* muss nach visitSDecls, weil ADecls nur fuer params ist

    if (this->passNo == 1) {
        //test whether ID has already been used in parameter list
        std::map<Id, Type*>::iterator paramsIt = this->env->visitingFunc->params.find(adecl->id_);
        //debug
        std::cout << this->env->visitingFunc->params.find(adecl->id_)->first << "<<<<<<K\n\n\n\n\nLKDFJ<<<<<<<<<<<";
        if (paramsIt != this->env->visitingFunc->params.end()) {
            std::cout << "TYPE ERROR\n\n" << "Variable ID " << adecl->id_ << " has already been used in parameter list." << std::endl;
            exit(1);
        }

    } */
    adecl->type_->accept(this);
    visitId(adecl->id_);

}

void TypeChecker::visitSExp(SExp *sexp)
{
  /* Code For SExp Goes Here */
    this->env->declaring = true;//das brauchen wir glaube ich
  sexp->exp_->accept(this);

}

void TypeChecker::visitSDecls(SDecls *sdecls)
{              //debug
    std::cout << "visitSDecls " << std::endl;
  /* Code For SDecls Goes Here */
    if (this->passNo == 1) {
        this->env->currentType = sdecls->type_;
        this->env->declaring = true;

        //adds variable to variable map if not already declared or used in parameter list
        sdecls->listid_->accept(this);

        this->env->currentType = NULL;
        this->env->declaring = false;

    }

    //sdecls->type_->accept(this);        //don't think we need this anymore


}

void TypeChecker::visitSInit(SInit *sinit)
{
  /* Code For SInit Goes Here */
    if (this->passNo == 1) {
        this->env->declaring = true;

        //adds variable to variable map if not already declared or used in parameter list
        this->env->currentType = sinit->type_;      //sinit->type_->accept(this); does the same (I think)
        visitId(sinit->id_);

        this->env->declaring = false;

        sinit->exp_->accept(this);

        this->env->currentType = NULL;
    }

}

void TypeChecker::visitSReturn(SReturn *sreturn)
{
  /* Code For SReturn Goes Here */
    if (this->passNo == 1) {

    }


    sreturn->exp_->accept(this);

}

void TypeChecker::visitSReturnVoid(SReturnVoid *sreturnvoid)
{
  /* Code For SReturnVoid Goes Here */
              //debug
    std::cout << "visitSReturnVoid " << std::endl;
    if (this->passNo == 1) {
        if ( this->getTypeName(this->env->visitingFunc->funcType) !=  "void" ) {
            std::cout << "TYPE ERROR\n\n" << "Function " << this->env->visitingFunc->id << " returns type void instead of type "
                        << this->getTypeName(this->env->visitingFunc->funcType) << std::endl;
            exit(1);
        }
    }

}

void TypeChecker::visitSWhile(SWhile *swhile)
{
  /* Code For SWhile Goes Here */

  swhile->exp_->accept(this);
  swhile->stm_->accept(this);

}

void TypeChecker::visitSBlock(SBlock *sblock)
{
  /* Code For SBlock Goes Here */

  sblock->liststm_->accept(this);

}

void TypeChecker::visitSIfElse(SIfElse *sifelse)
{
  /* Code For SIfElse Goes Here */

  sifelse->exp_->accept(this);
  sifelse->stm_1->accept(this);
  sifelse->stm_2->accept(this);

}

void TypeChecker::visitETrue(ETrue *etrue)
{
                  //debug
    std::cout << "visitETrue " << std::endl;
  /* Code For ETrue Goes Here */
    if (this->passNo == 1) {
        if (this->getTypeName(this->env->currentType) != "bool") {
            std::cout << "TYPE ERROR\n\n" << "Variable expected type "
                        << this->getTypeName(this->env->currentType) << " but found type bool" << std::endl;
            exit(1);
        }
    }

}

void TypeChecker::visitEFalse(EFalse *efalse)
{

            //debug
      std::cout << "visitETrue " << std::endl;
  /* Code For EFalse Goes Here */
    if (this->passNo == 1) {
        if (this->getTypeName(this->env->currentType) != "bool") {
            std::cout << "TYPE ERROR\n\n" << "Variable expected type "
                        << this->getTypeName(this->env->currentType) << " but found type bool" << std::endl;
            exit(1);
        }
    }

}

void TypeChecker::visitEInt(EInt *eint)
{
  /* Code For EInt Goes Here */

  visitInteger(eint->integer_);

}

void TypeChecker::visitEDouble(EDouble *edouble)
{
  /* Code For EDouble Goes Here */

  visitDouble(edouble->double_);

}

void TypeChecker::visitEString(EString *estring)
{
  /* Code For EString Goes Here */

  visitString(estring->string_);

}

void TypeChecker::visitEId(EId *eid)
{
  /* Code For EId Goes Here */
    if (this->passNo == 1) {
        visitId(eid->id_);
    }


}

void TypeChecker::visitEApp(EApp *eapp)
{
  /* Code For EApp Goes Here */

  visitId(eapp->id_);
  eapp->listexp_->accept(this);

}

void TypeChecker::visitEPIncr(EPIncr *epincr)
{
  /* Code For EPIncr Goes Here */

  epincr->exp_->accept(this);

}

void TypeChecker::visitEPDecr(EPDecr *epdecr)
{
  /* Code For EPDecr Goes Here */

  epdecr->exp_->accept(this);

}

void TypeChecker::visitEIncr(EIncr *eincr)
{
  /* Code For EIncr Goes Here */

  eincr->exp_->accept(this);

}

void TypeChecker::visitEDecr(EDecr *edecr)
{
  /* Code For EDecr Goes Here */

  edecr->exp_->accept(this);

}

void TypeChecker::visitETimes(ETimes *etimes)
{
  /* Code For ETimes Goes Here */

  etimes->exp_1->accept(this);
  etimes->exp_2->accept(this);

}

void TypeChecker::visitEDiv(EDiv *ediv)
{
  /* Code For EDiv Goes Here */

  ediv->exp_1->accept(this);
  ediv->exp_2->accept(this);

}

void TypeChecker::visitEPlus(EPlus *eplus)
{
  /* Code For EPlus Goes Here */

  eplus->exp_1->accept(this);
  eplus->exp_2->accept(this);

}

void TypeChecker::visitEMinus(EMinus *eminus)
{
  /* Code For EMinus Goes Here */

  eminus->exp_1->accept(this);
  eminus->exp_2->accept(this);

}

void TypeChecker::visitELt(ELt *elt)
{
  /* Code For ELt Goes Here */

  elt->exp_1->accept(this);
  elt->exp_2->accept(this);

}

void TypeChecker::visitEGt(EGt *egt)
{
  /* Code For EGt Goes Here */

  egt->exp_1->accept(this);
  egt->exp_2->accept(this);

}

void TypeChecker::visitELtEq(ELtEq *elteq)
{
  /* Code For ELtEq Goes Here */

  elteq->exp_1->accept(this);
  elteq->exp_2->accept(this);

}

void TypeChecker::visitEGtEq(EGtEq *egteq)
{
  /* Code For EGtEq Goes Here */

  egteq->exp_1->accept(this);
  egteq->exp_2->accept(this);

}

void TypeChecker::visitEEq(EEq *eeq)
{
  /* Code For EEq Goes Here */

  eeq->exp_1->accept(this);
  eeq->exp_2->accept(this);

}

void TypeChecker::visitENEq(ENEq *eneq)
{
  /* Code For ENEq Goes Here */

  eneq->exp_1->accept(this);
  eneq->exp_2->accept(this);

}

void TypeChecker::visitEAnd(EAnd *eand)
{
  /* Code For EAnd Goes Here */
//curtype saven und danach restoren
  eand->exp_1->accept(this);
  //type von exp_1 saven
  eand->exp_2->accept(this);
  //type von exp_2 saven
  // type von exp_1 und exp_2 vergleichen
  //curtype mit anfangs wert ueberschreiben

}

void TypeChecker::visitEOr(EOr *eor)
{
  /* Code For EOr Goes Here */

  eor->exp_1->accept(this);
  eor->exp_2->accept(this);

}

void TypeChecker::visitEAss(EAss *eass)
{
  /* Code For EAss Goes Here */

  eass->exp_1->accept(this);
  eass->exp_2->accept(this);

}

void TypeChecker::visitETyped(ETyped *etyped)
{
  /* Code For ETyped Goes Here */

  etyped->exp_->accept(this);
  etyped->type_->accept(this);

}

void TypeChecker::visitType_bool(Type_bool *type_bool)
{

            //debug
    std::cout << "visitType_bool " << std::endl;
  /* Code For Type_bool Goes Here */
    if (this->passNo == 0) {
        if (this->env->visitingFunc) {
            this->env->currentType = type_bool;
        }
    }

}

void TypeChecker::visitType_int(Type_int *type_int)
{

            //debug
    std::cout << "visitType_int " << std::endl;
  /* Code For Type_int Goes Here */
    if (this->passNo == 0) {
        if (this->env->visitingFunc) {
            this->env->currentType = type_int;
        }
    }


}

void TypeChecker::visitType_double(Type_double *type_double)
{
            //debug
    std::cout << "visitType_double " << std::endl;
  /* Code For Type_double Goes Here */
    if (this->passNo == 0) {
        if (this->env->visitingFunc) {
            this->env->currentType = type_double;
        }
    }


}

void TypeChecker::visitType_void(Type_void *type_void)
{
                //debug
    std::cout << "visitType_void " << std::endl;
  /* Code For Type_void Goes Here */
    if (this->passNo == 0) {
        if (this->env->visitingFunc) {
            this->env->currentType = type_void;
        }
    }


}

void TypeChecker::visitType_string(Type_string *type_string)
{

        //debug
    std::cout << "visitType_string " << std::endl;

  /* Code For Type_string Goes Here */
    if (this->passNo == 0) {
        if (this->env->visitingFunc) {
            this->env->currentType = type_string;
        }
    }


}


void TypeChecker::visitListDef(ListDef* listdef)
{
  for (ListDef::iterator i = listdef->begin() ; i != listdef->end() ; ++i)
  {
                    //debug
    std::cout << "visitListDef for loop " << std::endl;
    (*i)->accept(this);
  }
}

void TypeChecker::visitListArg(ListArg* listarg)
{
  for (ListArg::iterator i = listarg->begin() ; i != listarg->end() ; ++i)
  {
              //debug
    std::cout << "visitListArg for loop " << std::endl;
    (*i)->accept(this);
  }
}

void TypeChecker::visitListStm(ListStm* liststm)
{
  for (ListStm::iterator i = liststm->begin() ; i != liststm->end() ; ++i)
  {
                    //debug
    std::cout << "visitListStm for loop " << std::endl;
    (*i)->accept(this);
  }
}

void TypeChecker::visitListExp(ListExp* listexp)
{
  for (ListExp::iterator i = listexp->begin() ; i != listexp->end() ; ++i)
  {
    (*i)->accept(this);
  }
}

void TypeChecker::visitListId(ListId* listid)
{
  for (ListId::iterator i = listid->begin() ; i != listid->end() ; ++i)
  {
    visitId(*i) ;
  }
}


void TypeChecker::visitId(Id x)
{
  /* Code for Id Goes Here */
    //debug
    std::cout << "visitId " << x << std::endl;

    if (this->passNo == 1) {
        if (this->env->declaring) {

            //test whether ID has already been used in parameter list
            std::map<Id, Type*>::iterator paramsIt = this->env->visitingFunc->params.find(x);
            if (paramsIt != this->env->visitingFunc->params.end()) {
                std::cout << "TYPE ERROR\n\n" << "Variable ID " << x << " has already been used in parameter list." << std::endl;
                exit(1);
            }

            //test whether ID has already been declared, i.e. is in vars map
            std::map<Id, Type*>::iterator varsIt = this->env->visitingFunc->vars.find(x);
            if (varsIt != this->env->visitingFunc->vars.end()) {
                std::cout << "TYPE ERROR\n\n" << "Variable ID " << x << " has already been declared." << std::endl;
                exit(1);
            }

            //add variable to vars map
            this->env->visitingFunc->vars.insert(std::pair<Id, Type*>(x, this->env->currentType));
            //debug
            std::cout << "Added variable " << x << " of type " << this->getTypeName(this->env->currentType);
            std::cout << " to vars map in function " << this->env->visitingFunc->id << std::endl;
        }


        //test whether ID is in parameter list or in variable list (gets executed when a variable is used)
        std::map<Id, Type*>::iterator paramsIt = this->env->visitingFunc->params.find(x);
        std::map<Id, Type*>::iterator varsIt = this->env->visitingFunc->vars.find(x);
        if ( (paramsIt == this->env->visitingFunc->params.end()) && varsIt == this->env->visitingFunc->vars.end() )  {
            std::cout << "TYPE ERROR\n\n" << "Variable ID " << x << " has not been declared yet." << std::endl;
            exit(1);
        }


    }

    /*if (this->passNo == 0) {
        if (this->env->visitingFunc) {
            //assume currentType != NULL
            //test whether parameter ID has already been used
            std::map<Id, Type*>::iterator it = this->env->visitingFunc->params.find(x);
            if (it != this->env->visitingFunc->params.end()) {
                std::cout << "TYPE ERROR\n\n" << "Parameter ID " << x << " has already been used." << std::endl;
                exit(1);
            }
            //add parameter to map
            this->env->visitingFunc->params.insert(std::pair<Id, Type*>(x, this->env->currentType));

            //debug
            std::cout << "@TypeChecker::visitId(Id x)  Added param " << x;
            std::cout << " to function " << this->env->visitingFunc->id << std::endl;

            this->env->currentType = NULL;
        }
    }*/

}

void TypeChecker::visitInteger(Integer x)
{
  /* Code for Integer Goes Here */
  //debug
  std::cout << "visitInteger " << x << std::endl;

    if (this->passNo == 1) {
        if (this->getTypeName(this->env->currentType) != "int") {
            std::cout << "TYPE ERROR\n\n" << "Variable expected type "
                        << this->getTypeName(this->env->currentType) << " but found type integer" << std::endl;
            exit(1);
        }
    }
}

void TypeChecker::visitChar(Char x)
{
  /* Code for Char Goes Here */
    //debug
  std::cout << "visitChar " << x << std::endl;
}

void TypeChecker::visitDouble(Double x)
{
  /* Code for Double Goes Here */
    //debug
  std::cout << "visitDouble " << x << std::endl;
    if (this->passNo == 1) {
        if (this->getTypeName(this->env->currentType) != "double") {
            std::cout << "TYPE ERROR\n\n" << "Variable expected type "
                        << this->getTypeName(this->env->currentType) << " but found type double" << std::endl;
            exit(1);
        }
    }
}

void TypeChecker::visitString(String x)
{
  /* Code for String Goes Here */
    //debug
  std::cout << "visitString " << x << std::endl;
    if (this->passNo == 1) {
        if (this->getTypeName(this->env->currentType) != "string") {
            std::cout << "TYPE ERROR\n\n" << "Variable expected type "
                        << this->getTypeName(this->env->currentType) << " but found type string" << std::endl;
            exit(1);
        }
    }
}

void TypeChecker::visitIdent(Ident x)
{
  /* Code for Ident Goes Here */
    //debug
  std::cout << "visitIdent " << x << std::endl;
}



