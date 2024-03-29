{-# OPTIONS_GHC -fno-warn-incomplete-patterns #-}
module PrintCPP where

-- pretty-printer generated by the BNF converter

import AbsCPP
import Data.Char


-- the top-level printing method
printTree :: Print a => a -> String
printTree = render . prt 0

type Doc = [ShowS] -> [ShowS]

doc :: ShowS -> Doc
doc = (:)

render :: Doc -> String
render d = rend 0 (map ($ "") $ d []) "" where
  rend i ss = case ss of
    "["      :ts -> showChar '[' . rend i ts
    "("      :ts -> showChar '(' . rend i ts
    "{"      :ts -> showChar '{' . new (i+1) . rend (i+1) ts
    "}" : ";":ts -> new (i-1) . space "}" . showChar ';' . new (i-1) . rend (i-1) ts
    "}"      :ts -> new (i-1) . showChar '}' . new (i-1) . rend (i-1) ts
    ";"      :ts -> showChar ';' . new i . rend i ts
    t  : "," :ts -> showString t . space "," . rend i ts
    t  : ")" :ts -> showString t . showChar ')' . rend i ts
    t  : "]" :ts -> showString t . showChar ']' . rend i ts
    t        :ts -> space t . rend i ts
    _            -> id
  new i   = showChar '\n' . replicateS (2*i) (showChar ' ') . dropWhile isSpace
  space t = showString t . (\s -> if null s then "" else (' ':s))

parenth :: Doc -> Doc
parenth ss = doc (showChar '(') . ss . doc (showChar ')')

concatS :: [ShowS] -> ShowS
concatS = foldr (.) id

concatD :: [Doc] -> Doc
concatD = foldr (.) id

replicateS :: Int -> ShowS -> ShowS
replicateS n f = concatS (replicate n f)

-- the printer class does the job
class Print a where
  prt :: Int -> a -> Doc
  prtList :: Int -> [a] -> Doc
  prtList i = concatD . map (prt i)

instance Print a => Print [a] where
  prt = prtList

instance Print Char where
  prt _ s = doc (showChar '\'' . mkEsc '\'' s . showChar '\'')
  prtList _ s = doc (showChar '"' . concatS (map (mkEsc '"') s) . showChar '"')

mkEsc :: Char -> Char -> ShowS
mkEsc q s = case s of
  _ | s == q -> showChar '\\' . showChar s
  '\\'-> showString "\\\\"
  '\n' -> showString "\\n"
  '\t' -> showString "\\t"
  _ -> showChar s

prPrec :: Int -> Int -> Doc -> Doc
prPrec i j = if j<i then parenth else id


instance Print Integer where
  prt _ x = doc (shows x)


instance Print Double where
  prt _ x = doc (shows x)



instance Print UIdent where
  prt _ (UIdent i) = doc (showString ( i))
  prtList _ [x] = (concatD [prt 0 x])
  prtList _ (x:xs) = (concatD [prt 0 x, doc (showString ","), prt 0 xs])


instance Print Program where
  prt i e = case e of
    PDefs defs -> prPrec i 0 (concatD [prt 0 defs])

instance Print Def where
  prt i e = case e of
    DFun fun -> prPrec i 0 (concatD [prt 0 fun])
    DIFun infun -> prPrec i 0 (concatD [prt 0 infun])
    DTypeDef typedef -> prPrec i 0 (concatD [prt 0 typedef])
    DStructDef structdef -> prPrec i 0 (concatD [prt 0 structdef])
    DUsing qconst -> prPrec i 0 (concatD [doc (showString "using"), prt 0 qconst, doc (showString ";")])
    DDecVar decvar -> prPrec i 0 (concatD [prt 0 decvar, doc (showString ";")])
  prtList _ [] = (concatD [])
  prtList _ (x:xs) = (concatD [prt 0 x, prt 0 xs])
instance Print InFun where
  prt i e = case e of
    InlineFunction fun -> prPrec i 0 (concatD [doc (showString "inline"), prt 0 fun])

instance Print Fun where
  prt i e = case e of
    FuctionHead type_ uident args funbod -> prPrec i 0 (concatD [prt 0 type_, prt 0 uident, doc (showString "("), prt 0 args, doc (showString ")"), prt 0 funbod])

instance Print FunBod where
  prt i e = case e of
    FunctionBody -> prPrec i 0 (concatD [doc (showString ";")])
    FunctionBodyStm stms -> prPrec i 0 (concatD [doc (showString "{"), prt 0 stms, doc (showString "}")])

instance Print TypeDef where
  prt i e = case e of
    TypeDef type_ uident -> prPrec i 0 (concatD [doc (showString "typedef"), prt 0 type_, prt 0 uident, doc (showString ";")])

instance Print StructDef where
  prt i e = case e of
    StructDef uident decvars -> prPrec i 0 (concatD [doc (showString "struct"), prt 0 uident, doc (showString "{"), prt 0 decvars, doc (showString "}"), doc (showString ";")])

instance Print Arg where
  prt i e = case e of
    ArgIdLess type_ -> prPrec i 0 (concatD [prt 0 type_])
    ArgDecVar type_ uident -> prPrec i 0 (concatD [prt 0 type_, prt 0 uident])
    ArgDecVarDefault type_ uident exp -> prPrec i 0 (concatD [prt 0 type_, prt 0 uident, doc (showString "="), prt 0 exp])
  prtList _ [] = (concatD [])
  prtList _ [x] = (concatD [prt 0 x])
  prtList _ (x:xs) = (concatD [prt 0 x, doc (showString ","), prt 0 xs])
instance Print DecVar where
  prt i e = case e of
    DecVar type_ uidents -> prPrec i 0 (concatD [prt 0 type_, prt 0 uidents])
    DecVarDefault type_ uident exp -> prPrec i 0 (concatD [prt 0 type_, prt 0 uident, doc (showString "="), prt 0 exp])
  prtList _ [] = (concatD [])
  prtList _ (x:xs) = (concatD [prt 0 x, doc (showString ";"), prt 0 xs])
instance Print BType where
  prt i e = case e of
    TQConst qconst -> prPrec i 0 (concatD [prt 0 qconst])
    TInt -> prPrec i 0 (concatD [doc (showString "int")])
    TDouble -> prPrec i 0 (concatD [doc (showString "double")])
    TBool -> prPrec i 0 (concatD [doc (showString "bool")])
    TVoid -> prPrec i 0 (concatD [doc (showString "void")])

instance Print Type where
  prt i e = case e of
    TType btype -> prPrec i 0 (concatD [prt 0 btype])
    TConst btype -> prPrec i 0 (concatD [doc (showString "const"), prt 0 btype])
    TConstRef btype -> prPrec i 0 (concatD [doc (showString "const"), prt 0 btype, doc (showString "&")])
    TRef btype -> prPrec i 0 (concatD [prt 0 btype, doc (showString "&")])
  prtList _ [x] = (concatD [prt 0 x])
  prtList _ (x:xs) = (concatD [prt 0 x, doc (showString ","), prt 0 xs])
instance Print Value where
  prt i e = case e of
    VInt n -> prPrec i 0 (concatD [prt 0 n])
    VDouble d -> prPrec i 0 (concatD [prt 0 d])
    VChar c -> prPrec i 0 (concatD [prt 0 c])
    ValConcatString valstrings -> prPrec i 0 (concatD [prt 0 valstrings])

instance Print ValString where
  prt i e = case e of
    ValSingleString str -> prPrec i 0 (concatD [prt 0 str])
  prtList _ [x] = (concatD [prt 0 x])
  prtList _ (x:xs) = (concatD [prt 0 x, prt 0 xs])
instance Print QConst where
  prt i e = case e of
    QConstRec qconst1 qconst2 -> prPrec i 0 (concatD [prt 0 qconst1, doc (showString "::"), prt 0 qconst2])
    QConstBase uident -> prPrec i 0 (concatD [prt 0 uident])
    QConstTemplateBase tinstanciation -> prPrec i 0 (concatD [prt 0 tinstanciation])

instance Print TInstanciation where
  prt i e = case e of
    Template uident types -> prPrec i 0 (concatD [prt 0 uident, doc (showString "<"), prt 0 types, doc (showString ">")])

instance Print Stm where
  prt i e = case e of
    StmDec decvar -> prPrec i 0 (concatD [prt 0 decvar, doc (showString ";")])
    StmExp exp -> prPrec i 0 (concatD [prt 0 exp, doc (showString ";")])
    StmRet exp -> prPrec i 0 (concatD [doc (showString "return"), prt 0 exp, doc (showString ";")])
    StmWhile exp stm -> prPrec i 0 (concatD [doc (showString "while"), doc (showString "("), prt 0 exp, doc (showString ")"), prt 0 stm])
    StmDo stm exp -> prPrec i 0 (concatD [doc (showString "do"), prt 0 stm, doc (showString "while"), doc (showString "("), prt 0 exp, doc (showString ")"), doc (showString ";")])
    StmFor decvar exp1 exp2 stm -> prPrec i 0 (concatD [doc (showString "for"), doc (showString "("), prt 0 decvar, doc (showString ";"), prt 0 exp1, doc (showString ";"), prt 0 exp2, doc (showString ")"), prt 0 stm])
    StmBlock stms -> prPrec i 0 (concatD [doc (showString "{"), prt 0 stms, doc (showString "}")])
    StmIf exp stm -> prPrec i 0 (concatD [doc (showString "if"), doc (showString "("), prt 0 exp, doc (showString ")"), prt 0 stm])
    StmIfElse exp stm1 stm2 -> prPrec i 0 (concatD [doc (showString "if"), doc (showString "("), prt 0 exp, doc (showString ")"), prt 0 stm1, doc (showString "else"), prt 0 stm2])
    StmTypeDef typedef -> prPrec i 0 (concatD [prt 0 typedef])
    StmStructDef structdef -> prPrec i 0 (concatD [prt 0 structdef])
  prtList _ [] = (concatD [])
  prtList _ (x:xs) = (concatD [prt 0 x, prt 0 xs])
instance Print Exp where
  prt i e = case e of
    ExValue value -> prPrec i 16 (concatD [prt 0 value])
    ExIndex exp1 exp2 -> prPrec i 15 (concatD [prt 15 exp1, doc (showString "["), prt 0 exp2, doc (showString "]")])
    ExCall exp exps -> prPrec i 15 (concatD [prt 15 exp, doc (showString "("), prt 0 exps, doc (showString ")")])
    ExQConst qconst -> prPrec i 15 (concatD [prt 0 qconst])
    ExProjPoint exp1 exp2 -> prPrec i 14 (concatD [prt 14 exp1, doc (showString "."), prt 15 exp2])
    ExProjArrow exp1 exp2 -> prPrec i 14 (concatD [prt 14 exp1, doc (showString "->"), prt 15 exp2])
    ExPostInc exp -> prPrec i 14 (concatD [prt 15 exp, doc (showString "++")])
    ExPostDec exp -> prPrec i 14 (concatD [prt 15 exp, doc (showString "--")])
    ExDeref exp -> prPrec i 14 (concatD [doc (showString "*"), prt 15 exp])
    ExPreInc exp -> prPrec i 13 (concatD [doc (showString "++"), prt 14 exp])
    ExPreDec exp -> prPrec i 13 (concatD [doc (showString "--"), prt 14 exp])
    ExNot exp -> prPrec i 13 (concatD [doc (showString "!"), prt 14 exp])
    ExMult exp1 exp2 -> prPrec i 12 (concatD [prt 12 exp1, doc (showString "*"), prt 13 exp2])
    ExDiv exp1 exp2 -> prPrec i 12 (concatD [prt 12 exp1, doc (showString "/"), prt 13 exp2])
    ExMod exp1 exp2 -> prPrec i 12 (concatD [prt 12 exp1, doc (showString "%"), prt 13 exp2])
    ExAdd exp1 exp2 -> prPrec i 11 (concatD [prt 11 exp1, doc (showString "+"), prt 12 exp2])
    ExSub exp1 exp2 -> prPrec i 11 (concatD [prt 11 exp1, doc (showString "-"), prt 12 exp2])
    ExLeftShft exp1 exp2 -> prPrec i 10 (concatD [prt 10 exp1, doc (showString "<<"), prt 11 exp2])
    ExRightSft exp1 exp2 -> prPrec i 10 (concatD [prt 10 exp1, doc (showString ">>"), prt 11 exp2])
    ExLess exp1 exp2 -> prPrec i 9 (concatD [prt 9 exp1, doc (showString "<"), prt 10 exp2])
    ExGreater exp1 exp2 -> prPrec i 9 (concatD [prt 9 exp1, doc (showString ">"), prt 10 exp2])
    ExLessEq exp1 exp2 -> prPrec i 9 (concatD [prt 9 exp1, doc (showString "<="), prt 10 exp2])
    ExGreaterEq exp1 exp2 -> prPrec i 9 (concatD [prt 9 exp1, doc (showString ">="), prt 10 exp2])
    ExEqual exp1 exp2 -> prPrec i 8 (concatD [prt 8 exp1, doc (showString "=="), prt 9 exp2])
    ExInequal exp1 exp2 -> prPrec i 8 (concatD [prt 8 exp1, doc (showString "!="), prt 9 exp2])
    ExConjunc exp1 exp2 -> prPrec i 4 (concatD [prt 4 exp1, doc (showString "&&"), prt 5 exp2])
    ExDisjunc exp1 exp2 -> prPrec i 3 (concatD [prt 3 exp1, doc (showString "||"), prt 4 exp2])
    ExAssign exp1 exp2 -> prPrec i 2 (concatD [prt 2 exp1, doc (showString "="), prt 3 exp2])
    ExAddAssign exp1 exp2 -> prPrec i 2 (concatD [prt 2 exp1, doc (showString "+="), prt 3 exp2])
    ExSubAssign exp1 exp2 -> prPrec i 2 (concatD [prt 2 exp1, doc (showString "-="), prt 3 exp2])
    ExCond exp1 exp2 exp3 -> prPrec i 2 (concatD [prt 2 exp1, doc (showString "?"), prt 2 exp2, doc (showString ":"), prt 2 exp3])
    ExThrow exp -> prPrec i 1 (concatD [doc (showString "throw"), prt 0 exp])
  prtList _ [] = (concatD [])
  prtList _ [x] = (concatD [prt 0 x])
  prtList _ (x:xs) = (concatD [prt 0 x, doc (showString ","), prt 0 xs])

