#!/usr/bin/env wolframscript
(* ::Package:: *)

root = $ScriptCommandLine[[2]];
div = ToExpression[$ScriptCommandLine[[3]]];
fs = ToExpression[$ScriptCommandLine[[4]]];
mod = ToExpression[$ScriptCommandLine[[5]]];

tails[{}]:={};
tails[l_]:=Append[tails[Rest[l]],l];
rel[l_]:=l\[UndirectedEdge]Rest[l];
relLabeled[l_]:=Labeled[l\[UndirectedEdge]Rest[l],First[l]];
tree[d_,ord_]:=Block[{},
mons=CoefficientRules[d,ord]/.{(l_->_):>l};
v=DeleteDuplicates[Flatten[tails/@mons,1]];
e=relLabeled/@v;
Return[TreeGraph[e]]
];
SortSplitBy[f_]:=SplitBy[SortBy[#,f],f]&;
RevRange0[l_]:=Range[l,0,-1];
Children[t_][n_]:=Map[First]@Cases[EdgeList[t],_\[UndirectedEdge]n];
GhostChildren[t_][n_]:=Map[Prepend[n,#]&]@*RevRange0@*Max@*Map[First]@Children[t][n]
Content[t_,d_,ord_][n_]:=If[MemberQ[VertexList[t],n],If[Children[t][n]=={},ToString[n/.CoefficientRules[d,ord]],StringJoin@Map[TreeWalk[t,d,ord]][GhostChildren[t][n]]],""];
TreeWalk[_,0,ord_][n_]:="["<>StringJoin["["&/@ord]<>"0"<>StringJoin["]"&/@ord]<>"]";
TreeWalk[t_,d_,ord_][n_]:=
"["<>Content[t,d,ord][n]<>"]";
sub=Exponent[#,x]&/@fs;
n=Total[sub];
CreateVars[{},_]:={};
CreateVars[sub_,k_]:=Prepend[CreateVars[Rest[sub],k+First[sub]],Table[a[i],{i,k,k+First[sub]-1}]];
CreateVars[sub_]:=CreateVars[sub,1];
vars=CreateVars[sub];
GenPol[l_,x_]:=Total@Table[l[[i]]x^(i-1),{i,1,Length[l]}];
Zip[x_,y_]:=Transpose[{x,y}];
fs=PolynomialMod[#,mod]&/@fs;
res=Resultant[#[[1]],y-GenPol[#[[2]],x],x]&/@Zip[fs,vars];
Res=Times@@res;
ResList=Reverse@Most@CoefficientList[Res,y];
ResList=ResList/div//Expand;
modList=mod/#&/@div;
ResListMod=PolynomialMod[#[[1]],#[[2]]]&/@Zip[ResList,modList];
CreateTree[f_,vars_]:=TreeWalk[tree[f,vars],f,vars][{}];
trees=CreateTree[#,Flatten[vars]]&/@ResListMod;

PolynomialContent[f_]:=If[SameQ[f,0],Infinity,GCD@@(CoefficientRules[f]/.{(_->c_):>c})];
VariableContent[f_,v_]:=PolynomialContent[f-(f/.{v->0})];
Varmod[fs_,v_,y_]:=If[#==-\[Infinity],1,#]&@Max[mod/GCD[mod,Min[VariableContent[#,v]],mod]&/@Select[fs,Not@SameQ[#-(#/.{v->0}),0]&]];
varmod=Varmod[ResListMod,#,y]&/@Flatten[vars];

For[i=1, i<=Length[trees], i++, Export[root<>ToString[i]<>".txt", trees[[i]]]];
Export[root<>"varmod.txt", "VarModuli="<>ToString[varmod]];






