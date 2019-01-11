append([], X, X).
append([X|M], K, [X|L]):-
	append(M, K, L).

member(X, [X, _T]).
member(X, [_H|T]):-
	member(X, T).

num(1).
num(N):-num(M),N is M+1.

:- use_module(library(statistics)).

move(A,B):-
    append(Head,['b','_'|T],A),
    append(Head,['_','b'|T],B).

move(A,B):-
    append(Head,['_','w'|T],A),
    append(Head,['w','_'|T],B).

move(A,B):-
    append(Head,['_','b','w'|T],A),
    append(Head,['w','b','_'|T],B).

move(A,B):-
    append(Head,['b','w','_'|T],A),
    append(Head,['_','w','b'|T],B).

prolong([X|T],[Y,X|T]):-
    move(X,Y),
    not(member(Y,[X|T])).

printlist([ ]):- !.
printlist([H | T]):- write(H), nl, printlist(T).

depth_search(Start,Finish):-
    statistics(walltime, _),
    pathd([Start],Finish,Way),
    statistics(walltime, [_ | [Exectime]]),
    printlist(Way),
    write(Exectime).

pathd([Finish|T],Finish,[Finish|T]).
pathd(TempWay,Finish,Way):-
    prolong(TempWay,NewWay),
    pathd(NewWay,Finish,Way).

width_search(Start,Finish):-
    statistics(walltime, _),
    pathb([[Start]],Finish,Way), 
    statistics(walltime, [_ | [Exectime]]),
    printlist(Way),
    write(Exectime).

pathb([[Finish|T]|_],Finish,[Finish|T]).
pathb([TempWay|OtherWays],Finish,Way):-
    findall(W,prolong(TempWay,W),Ways),
    append(OtherWays,Ways,NewWays), 
    pathb(NewWays,Finish,Way).



iter_search(Start,Finish):-
    num(Level), 
    statistics(walltime, _),
    (Level>100,
    !; 
    is([Start],Finish,Way,Level), 
    printlist(Way)),
    statistics(walltime, [_ | [Exectime]]),
    write(Exectime).

is([Finish|Tail],Finish,[Finish|Tail],0).
is(TempWay,Finish,Way,N):-
    N>0,
    prolong(TempWay,NewWay),
    N1 is N-1,
    is(NewWay,Finish,Way,N1).

solve1:-
    depth_search(['b','b','b','b','_','w','w','w'],
['w','w','w','_','b','b','b','b']).

solve2:-
    width_search(['b','b','b','b','_','w','w','w'],
['w','w','w','_','b','b','b','b']).

solve3:-
    iter_search(['b','b','b','b','_','w','w','w'],
['w','w','w','_','b','b','b','b']).
