/*** BNFC-Generated Visitor Design Pattern CodeGenerator. ***/
/* This implements the common visitor design pattern.
   Note that this method uses Visitor-traversal of lists, so
   List->accept() does NOT traverse the list. This allows different
   algorithms to use context information differently. */

#include "CodeGenerator.H"



void CodeGenerator::visitProgram(Program* t) {} //abstract class
void CodeGenerator::visitDef(Def* t) {} //abstract class
void CodeGenerator::visitArg(Arg* t) {} //abstract class
void CodeGenerator::visitStm(Stm* t) {} //abstract class
void CodeGenerator::visitExp(Exp* t) {} //abstract class
void CodeGenerator::visitType(Type* t) {} //abstract class

void CodeGenerator::visitPDefs(PDefs *pdefs)
{
  /* Code For PDefs Goes Here */

  pdefs->listdef_->accept(this);

}

void CodeGenerator::visitDFun(DFun *dfun)
{
  /* Code For DFun Goes Here */
	llvm::Function *TheFunction = llvm::TheModule->getFunction(dfun->id_);
	//function has already been decalred
	if (TheFunction) {
		std::cout << "Function " dfun->id_ " has already been decalred" << std::endl;
		exit(1);
	}
	
	if (!TheFunction) {
		dfun->type_->accept(this);
		llvm::Type *funcType = _types[0];
		NamedValues.clear();
		dfun->listarg_->accept(this);
		if (_types.size() == 0) {
			llvm::FunctionType *FT = llvm::FunctionType::get(funcType, false);			//??false??
		}
		llvm::FunctionType *FT = llvm::FunctionType::get(funcType, _types, false);	//??false??
		_types.clear();
	}

  
  visitId(dfun->id_);
  
  dfun->liststm_->accept(this);

}

void CodeGenerator::visitADecl(ADecl *adecl)
{
  /* Code For ADecl Goes Here */

  adecl->type_->accept(this);
  visitId(adecl->id_);

}

void CodeGenerator::visitSExp(SExp *sexp)
{
  /* Code For SExp Goes Here */

  sexp->exp_->accept(this);

}

void CodeGenerator::visitSDecls(SDecls *sdecls)
{
  /* Code For SDecls Goes Here */

  sdecls->type_->accept(this);
  sdecls->listid_->accept(this);

}

void CodeGenerator::visitSInit(SInit *sinit)
{
  /* Code For SInit Goes Here */

  sinit->type_->accept(this);
  visitId(sinit->id_);
  sinit->exp_->accept(this);

}

void CodeGenerator::visitSReturn(SReturn *sreturn)
{
  /* Code For SReturn Goes Here */

  sreturn->exp_->accept(this);

}

void CodeGenerator::visitSReturnVoid(SReturnVoid *sreturnvoid)
{
  /* Code For SReturnVoid Goes Here */


}

void CodeGenerator::visitSWhile(SWhile *swhile)
{
  /* Code For SWhile Goes Here */

  swhile->exp_->accept(this);
  swhile->stm_->accept(this);

}

void CodeGenerator::visitSBlock(SBlock *sblock)
{
  /* Code For SBlock Goes Here */

  sblock->liststm_->accept(this);

}

void CodeGenerator::visitSIfElse(SIfElse *sifelse)
{
  /* Code For SIfElse Goes Here */

  sifelse->exp_->accept(this);
  sifelse->stm_1->accept(this);
  sifelse->stm_2->accept(this);

}

void CodeGenerator::visitETrue(ETrue *etrue)
{
  /* Code For ETrue Goes Here */


}

void CodeGenerator::visitEFalse(EFalse *efalse)
{
  /* Code For EFalse Goes Here */


}

void CodeGenerator::visitEInt(EInt *eint)
{
  /* Code For EInt Goes Here */

  visitInteger(eint->integer_);

}

void CodeGenerator::visitEDouble(EDouble *edouble)
{
  /* Code For EDouble Goes Here */

  visitDouble(edouble->double_);

}

void CodeGenerator::visitEString(EString *estring)
{
  /* Code For EString Goes Here */

  visitString(estring->string_);

}

void CodeGenerator::visitEId(EId *eid)
{
  /* Code For EId Goes Here */

  visitId(eid->id_);

}

void CodeGenerator::visitEApp(EApp *eapp)
{
  /* Code For EApp Goes Here */

  visitId(eapp->id_);
  eapp->listexp_->accept(this);

}

void CodeGenerator::visitEPIncr(EPIncr *epincr)
{
  /* Code For EPIncr Goes Here */

  epincr->exp_->accept(this);

}

void CodeGenerator::visitEPDecr(EPDecr *epdecr)
{
  /* Code For EPDecr Goes Here */

  epdecr->exp_->accept(this);

}

void CodeGenerator::visitEIncr(EIncr *eincr)
{
  /* Code For EIncr Goes Here */

  eincr->exp_->accept(this);

}

void CodeGenerator::visitEDecr(EDecr *edecr)
{
  /* Code For EDecr Goes Here */

  edecr->exp_->accept(this);

}

void CodeGenerator::visitETimes(ETimes *etimes)
{
  /* Code For ETimes Goes Here */

  etimes->exp_1->accept(this);
  etimes->exp_2->accept(this);

}

void CodeGenerator::visitEDiv(EDiv *ediv)
{
  /* Code For EDiv Goes Here */

  ediv->exp_1->accept(this);
  ediv->exp_2->accept(this);

}

void CodeGenerator::visitEPlus(EPlus *eplus)
{
  /* Code For EPlus Goes Here */

  eplus->exp_1->accept(this);
  eplus->exp_2->accept(this);

}

void CodeGenerator::visitEMinus(EMinus *eminus)
{
  /* Code For EMinus Goes Here */

  eminus->exp_1->accept(this);
  eminus->exp_2->accept(this);

}

void CodeGenerator::visitELt(ELt *elt)
{
  /* Code For ELt Goes Here */

  elt->exp_1->accept(this);
  elt->exp_2->accept(this);

}

void CodeGenerator::visitEGt(EGt *egt)
{
  /* Code For EGt Goes Here */

  egt->exp_1->accept(this);
  egt->exp_2->accept(this);

}

void CodeGenerator::visitELtEq(ELtEq *elteq)
{
  /* Code For ELtEq Goes Here */

  elteq->exp_1->accept(this);
  elteq->exp_2->accept(this);

}

void CodeGenerator::visitEGtEq(EGtEq *egteq)
{
  /* Code For EGtEq Goes Here */

  egteq->exp_1->accept(this);
  egteq->exp_2->accept(this);

}

void CodeGenerator::visitEEq(EEq *eeq)
{
  /* Code For EEq Goes Here */

  eeq->exp_1->accept(this);
  eeq->exp_2->accept(this);

}

void CodeGenerator::visitENEq(ENEq *eneq)
{
  /* Code For ENEq Goes Here */

  eneq->exp_1->accept(this);
  eneq->exp_2->accept(this);

}

void CodeGenerator::visitEAnd(EAnd *eand)
{
  /* Code For EAnd Goes Here */

  eand->exp_1->accept(this);
  eand->exp_2->accept(this);

}

void CodeGenerator::visitEOr(EOr *eor)
{
  /* Code For EOr Goes Here */

  eor->exp_1->accept(this);
  eor->exp_2->accept(this);

}

void CodeGenerator::visitEAss(EAss *eass)
{
  /* Code For EAss Goes Here */

  eass->exp_1->accept(this);
  eass->exp_2->accept(this);

}

void CodeGenerator::visitETyped(ETyped *etyped)
{
  /* Code For ETyped Goes Here */

  etyped->exp_->accept(this);
  etyped->type_->accept(this);

}

void CodeGenerator::visitType_bool(Type_bool *type_bool)
{
  /* Code For Type_bool Goes Here */
	//???
	//curType = llvm::Type::getInt1Ty(TheContext);

}

void CodeGenerator::visitType_int(Type_int *type_int)
{
  /* Code For Type_int Goes Here */
	_types = {llvm::Type::getInt32Ty(TheContext)};

}

void CodeGenerator::visitType_double(Type_double *type_double)
{
  /* Code For Type_double Goes Here */
	_types = {llvm::Type::getDoubleTy(TheContext)};

}

void CodeGenerator::visitType_void(Type_void *type_void)
{
  /* Code For Type_void Goes Here */
	_types = {llvm::Type::getVoidTy(TheContext)};

}

void CodeGenerator::visitType_string(Type_string *type_string)
{
  /* Code For Type_string Goes Here */


}


void CodeGenerator::visitListDef(ListDef* listdef)
{
  for (ListDef::iterator i = listdef->begin() ; i != listdef->end() ; ++i)
  {
    (*i)->accept(this);
  }
}

void CodeGenerator::visitListArg(ListArg* listarg)
{
	auto types = std::vector<llvm::Type*>();
	 for (ListArg::iterator i = listarg->begin() ; i != listarg->end() ; ++i)
	 {
		(*i)->accept(this);
		types.push_back(_types[0]);
	  }
	  _types = types;
}

void CodeGenerator::visitListStm(ListStm* liststm)
{
  for (ListStm::iterator i = liststm->begin() ; i != liststm->end() ; ++i)
  {
    (*i)->accept(this);
  }
}

void CodeGenerator::visitListExp(ListExp* listexp)
{
  for (ListExp::iterator i = listexp->begin() ; i != listexp->end() ; ++i)
  {
    (*i)->accept(this);
  }
}

void CodeGenerator::visitListId(ListId* listid)
{
  for (ListId::iterator i = listid->begin() ; i != listid->end() ; ++i)
  {
    visitId(*i) ;
  }
}


void CodeGenerator::visitId(Id x)
{
  /* Code for Id Goes Here */
}

void CodeGenerator::visitInteger(Integer x)
{
  /* Code for Integer Goes Here */
}

void CodeGenerator::visitChar(Char x)
{
  /* Code for Char Goes Here */
}

void CodeGenerator::visitDouble(Double x)
{
  /* Code for Double Goes Here */
}

void CodeGenerator::visitString(String x)
{
  /* Code for String Goes Here */
}

void CodeGenerator::visitIdent(Ident x)
{
  /* Code for Ident Goes Here */
}



