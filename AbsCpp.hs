

module AbsCpp where

-- Haskell module generated by the BNF converter




newtype Ident = Ident String deriving (Eq, Ord, Show, Read)
data Program = PDefs [Def]
  deriving (Eq, Ord, Show, Read)

data Def
    = DFun Inline Type Ident [Arg] [Stm]
    | DecFun Inline Type Ident [Arg]
    | DTypeDef TypeDef
    | DStructDef StructDef
    | DUsing QConst
  deriving (Eq, Ord, Show, Read)

data Inline = IsInline | NoInline
  deriving (Eq, Ord, Show, Read)

data Arg
    = ArgIdLess Type | ArgId Type Ident | ArgIdDefault Type Ident Exp
  deriving (Eq, Ord, Show, Read)

data Value
    = ValInt Integer
    | ValDouble Double
    | ValChar Char
    | ValString [ValString]
  deriving (Eq, Ord, Show, Read)

data ValString = ValSingleString String
  deriving (Eq, Ord, Show, Read)

data TypeDef = TypeDef Type Ident
  deriving (Eq, Ord, Show, Read)

data StructDef = StructDef Ident [Decl]
  deriving (Eq, Ord, Show, Read)

data Stm
    = StmDec Decl
    | StmExp Exp
    | StmRet Exp
    | StmWhile Exp Stm
    | StmDo Stm Exp
    | StmFor Decl Exp Exp Stm
    | StmBlock [Stm]
    | StmIf Exp Stm
    | StmIfElse Exp Stm Stm
    | StmTypeDef TypeDef
    | StmStructDef StructDef
  deriving (Eq, Ord, Show, Read)

data Exp
    = ExVal Value
    | ExName Ident
    | ExIndex Exp Exp
    | ExCall Exp [Exp]
    | ExQConst QConst
    | ExProjPoint Exp Exp
    | ExProjArrow Exp Exp
    | ExPostInc Exp
    | ExPostDec Exp
    | ExDeref Exp
    | ExPreInc Exp
    | ExPreDec Exp
    | ExNot Exp
    | ExMult Exp Exp
    | ExDiv Exp Exp
    | ExMod Exp Exp
    | ExAdd Exp Exp
    | ExSub Exp Exp
    | ExLeftShft Exp Exp
    | ExRightSft Exp Exp
    | ExLess Exp Exp
    | ExGreater Exp Exp
    | ExLessEq Exp Exp
    | ExGreaterEq Exp Exp
    | ExEqual Exp Exp
    | ExInequal Exp Exp
    | ExConjunc Exp Exp
    | ExDisjunc Exp Exp
    | ExAssign Exp Exp
    | ExAddAssign Exp Exp
    | ExSubAssign Exp Exp
    | ExCond Exp Exp Exp
    | ExThrow Exp
  deriving (Eq, Ord, Show, Read)

data Type
    = TTemlate Type [Type]
    | TReference Type
    | TConst Type
    | TQConst QConst
  deriving (Eq, Ord, Show, Read)

data QConst = QConstBase Ident | QConst QConst QConst
  deriving (Eq, Ord, Show, Read)

data Decl = DecVar Type [DeclName] | DecVarConst Type [DeclName]
  deriving (Eq, Ord, Show, Read)

data DeclName = DeclName Ident | DeclNameInitial Ident Exp
  deriving (Eq, Ord, Show, Read)
