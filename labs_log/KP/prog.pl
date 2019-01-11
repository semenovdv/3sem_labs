father('Владислав', 'Денис').
mother('Эльвира', 'Денис').
father('Владислав', 'Глеб').
mother('Эльвира', 'Глеб').
father('Ринат', 'Эльвира').
mother('Ольга', 'Эльвира').
father('Владислав', 'Глеб').
mother('Эльвира', 'Глеб').
father('Ринат', 'Руслан').
mother('Ольга', 'Руслан').
father('Федор', 'Ольга').
mother('Христинья', 'Ольга').
father('Валерьян', 'Владислав').
mother('Тослима', 'Владислав').
mother('Маргарита', 'Валерьян').
father('Гумер', 'Тослима').
mother('Росима', 'Тослима').
father('Павел', 'Маргарита').
mother('Мария', 'Маргарита').
father('Никита', 'Павел').
mother('Анна', 'Павел').
father('Андриан', 'Никита').
father('Дмитрий', 'Мария').
mother('Наталья', 'Мария').
father('Лев', 'Анна').
father('Камал', 'Федор').
father('Султан','Камал').

father('A', 'B').
father('A', 'C').
father('B', 'D').
father('B','E').
father('C', 'F').
father('C', 'G').
father('D', 'H').
father('D', 'I').
father('E', 'J').
father('E', 'K').
father('F', 'L').
father('F', 'M').
father('G', 'N').
father('G', 'O').

parent(X, Y):- mother(X, Y).
parent(X, Y):- father(X, Y).

bro1(X, Y):-
	parent(Z, X),
	parent(Z, Y),
	X \= Y.

bro2(X, Y):-
	parent(FX, X),
	parent(FY, Y),
	bro1(FX, FY).

bro3(X, Y):-
	parent(FX, X),
	parent(FY, Y),
	bro2(FX, FY).

move(X, Y, 'child'):-
	father(Y, X).

move(X, Y, 'child'):-
	mother(Y, X).

move(X, Y, 'father'):-
	father(X, Y).
move(X, Y, 'mother'):-
	mother(X, Y).


member(X, [X, _T]).
member(X, [_H|T]):-
	member(X, T).
	
prolong([X|T],[Y,X|T], Rel):-
    move(X, Y, Rel),
    not(member(Y,[X|T])).

printlist([ ]):- !.
printlist([H | T]):- print(H), nl, printlist(T).


num(1).
num(N):-num(M),N is M+1.

iter_search(Start,Finish, Rel):-
    num(Level), 
    statistics(walltime, _),
    (Level>10,
    !; 
    is([Start],Finish,Way,Level, Rel), 
    printlist(Way)),
    statistics(walltime, [_ | [Exectime]]),
    write(Exectime).

is([Finish|Tail],Finish,[Finish|Tail],0, []).
is(TempWay,Finish,Way,N, [R|TT]):-
    N>0,
    prolong(TempWay,NewWay, R),
    N1 is N-1,
    is(NewWay,Finish,Way,N1, TT), !.




%dont look there


width_search(Start, Finish, Rel):-
    statistics(walltime, _),
    pathb([[Start]],Finish,_, Rel), !, 
    statistics(walltime, [_ | [Exectime]]),
    printlist(Rel),
    write(Exectime).


%str(A,B).
getmy([str(Ways, R)], [Ways], R).
getmy([str(Ways, R) | T], [Ways], R):-
	getmy(T, NewWays, NewR).

pathb([[Finish|T]|_],Finish,[Finish|T],[]).
pathb([TempWay|OtherWays],Finish,Way, [R | TT]):-
    findall(str(W, Rel), prolong(TempWay,W, Rel), Ws),
	getmy(Ws, Ways, R),
	append(OtherWays, Ways, NewWays),
	pathb(NewWays, Finish, Way, TT).






