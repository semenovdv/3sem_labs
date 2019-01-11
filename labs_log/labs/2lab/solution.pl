

%
%Герман. 
%Дима единственный из нас, кто любит историю. 
%Олег и я увлекаемся одними и теми же предметами. 
%Мы все считаем биологию интереснейшей наукой. 
%Двое из нас любяти химию, и биологию. 

%Олег. 
%Нам всем очень нравится математика. 
%Герман завзятый историк. 
%В одном из увлечений мы расходимся с Димой. 
%Герман и Дима любят химию. 

%Дима. 
%Есть только один предмет, который любим мы все. 
%Математикой увлекаюсь я один. 
%Каждый из нас любит разное сочетание дисциплин.
%Олег ошибается, говоря, что Герман и я увлекаемся химией.

%Известно, что только два из утверждений каждого студента соответствуют действительности.
%Попробуйте сказать, какими науками увлекается каждый из них?

:-discontiguous student/1.

student('dima').
student('german').
student('oleg').

subject('biology').
subject('chemistry').
subject('history').
subject('math').

mypermute([], []).
mypermute([X|L], P):-
	mypermute(L, L1),
	myinsert(X, L1, P).

myremove(X, [X|T], T):- !.
myremove(X, [Y|T], [Y|TN]):-
	myremove(X, T, TN).

myinsert(X, L, BiggerL):-
	myremove(X, BiggerL, L).

unique([]):-!.
unique([Head|Tail]):-
    member(Head, Tail), !, fail;
   unique(Tail).

mymember(X, [X, _T]).
mymember(X, [_H|T]):-
	mymember(X, T).

myappend([], X, X).
myappend([X|M], K, [X|L]):-
	append(M, K, L).

equalset(sl,sl).
equalset([A|L],L1):-myremove(A,L1,L2), equalset(L,L2).
equalset([],[]).




%German
gstate1(Solve):-
	member(combination('dima', Dg1), Solve),
	member('chemistry', Dg1),
	member(combination('german', Gg1), Solve),
	not(member('chemistry', Gg1)),
	member(combination('oleg', Og1), Solve),
	not(member('chemistry', Og1)).

gstate2(Solve):-
	member(combination('oleg', O), Solve),
	member(combination('german', G), Solve),
	equalset(O, G).

gstate3(Solve):-
	member(combination('dima', Dg3), Solve),
	member('biology', Dg3),
	member(combination('oleg', Og3), Solve),
	member('biology', Og3),
	member(combination('german', Gg3), Solve),
	member('biology', Gg3).

gstate4(Solve):-
	member(combination(X, Xs), Solve),
	member(combination(Y, Ys), Solve),
	X\=Y,
	member('biology', Xs),
	member('biology', Ys),
	member('chemistry', Xs),
	member('chemistry', Ys).

gsolve1(Solve):-
	gstate1(Solve),
	gstate2(Solve),
	not(gstate3(Solve)),
	not(gstate4(Solve)).

gsolve1(Solve):-
	gstate1(Solve),
	not(gstate2(Solve)),
	gstate3(Solve),
	not(gstate4(Solve)).

gsolve1(Solve):-
	gstate1(Solve),
	not(gstate2(Solve)),
	not(gstate3(Solve)),
	gstate4(Solve).

gsolve1(Solve):-
	not(gstate1(Solve)),
	gstate2(Solve),
	gstate3(Solve),
	not(gstate4(Solve)).

gsolve1(Solve):-
	not(gstate1(Solve)),
	gstate2(Solve),
	not(gstate3(Solve)),
	gstate4(Solve).

gsolve1(Solve):-
	not(gstate1(Solve)),
	not(gstate2(Solve)),
	gstate3(Solve),
	gstate4(Solve).

ostate1(Solve):-
	member(combination('dima', Dg3), Solve),
	member('math', Dg3),
	member(combination('oleg', Og3), Solve),
	member('math', Og3),
	member(combination('german', Gg3), Solve),
	member('math', Gg3).

ostate2(Solve):-
	member(combination('german', Gg3), Solve),
	member('history', Gg3).

ostate3(Solve):-
	member(combination('dima', D), Solve),
	member(combination('oleg', O), Solve),
	member(D1, D),
	member(D1, O),
	member(D2, D),
	D2 \= D1,
	member(D2, O),
	member(D3, D),
	D3 \= D1,
	D3 \= D2,
	not(member(D3, O)).
%???????????????

ostate4(Solve):-
	member(combination('dima', Dg1), Solve),
	member('chemistry', Dg1),
	member(combination('german', Gg1), Solve),
	member('chemistry', Gg1).

osolve1(Solve):-
	ostate1(Solve),
	not(ostate2(Solve)),
	ostate3(Solve),
	not(ostate4(Solve)).

osolve1(Solve):-
	ostate1(Solve),
	not(ostate2(Solve)),
	not(ostate3(Solve)),
	ostate4(Solve).

osolve1(Solve):-
	not(ostate1(Solve)),
	ostate2(Solve),
	ostate3(Solve),
	not(ostate4(Solve)).

osolve1(Solve):-
	not(ostate1(Solve)),
	ostate2(Solve),
	not(ostate3(Solve)),
	ostate4(Solve).

osolve1(Solve):-
	not(ostate1(Solve)),
	not(ostate2(Solve)),
	ostate3(Solve),
	ostate4(Solve).

osolve1(Solve):-
	ostate1(Solve),
	ostate2(Solve),
	not(ostate3(Solve)),
	not(ostate4(Solve)).

help_proc(A, A1, A2, A3):-
	member(A, A1), member(A, A2), member(A, A3).


dstate1(Solve):-
	member(combination('dima', Dg1), Solve),
	member(combination('oleg', Og1), Solve),
	member(combination('german', Gg1), Solve),
	subject(A), subject(B), subject(C), subject(D),
	unique([A, B, C, D]),
	member(A, Dg1), member(A, Og1), member(A, Gg1),

	not(help_proc(B, Dg1, Og1, Gg1)),
	not(help_proc(C, Dg1, Og1, Gg1)),
	not(help_proc(D, Dg1, Og1, Gg1)).

dstate2(Solve):-
	member(combination('oleg', Og1), Solve),
	member('math', Og1),
	member(combination('german', Gg1), Solve),
	not(member('math', Gg1)),
	member(combination('dima', Dg1), Solve),
	not(member('math', Dg1)).

%каждый из нас любит разное сочетание дисциплин

dstate3(Solve):-
	member(combination('oleg', Dg1), Solve),
	member(combination('german', Gg1), Solve),
	member(combination('dima', Og1), Solve),
	not(equalset(Dg1, Gg1)),
	not(equalset(Dg1, Og1)),
	not(equalset(Gg1, Og1)).


dstate4(Solve):-
	not(ostate4(Solve)).

dsolve1(Solve):-
	dstate1(Solve),
	dstate2(Solve),
	not(dstate3(Solve)),
	not(dstate4(Solve)).

dsolve1(Solve):-
	dstate1(Solve),
	not(dstate2(Solve)),
	dstate3(Solve),
	not(dstate4(Solve)).

dsolve1(Solve):-
	dstate1(Solve),
	not(dstate2(Solve)),
	not(dstate3(Solve)),
	dstate4(Solve).

dsolve1(Solve):-
	not(dstate1(Solve)),
	dstate2(Solve),
	dstate3(Solve),
	not(dstate4(Solve)).

dsolve1(Solve):-
	not(dstate1(Solve)),
	dstate2(Solve),
	not(dstate3(Solve)),
	dstate4(Solve).

dsolve1(Solve):-
	not(dstate1(Solve)),
	dstate3(Solve),
	dstate4(Solve),
	not(dstate2(Solve)).
	


solve(Solve):-
	Solve = [combination('dima', [A1p, A2p, A3p]), combination('oleg', [B1p, B2p, B3p]),
	combination('german', [C1p, C2p, C3p])],

	%student(A), student(B), student(C), unique([A,B,C]),
	subject(A1p), subject(A2p), subject(A3p), unique([A1p, A2p, A3p]),
	subject(B1p), subject(B2p), subject(B3p), unique([B1p, B2p, B3p]),
	subject(C1p), subject(C2p), subject(C3p), unique([C1p, C2p, C3p]),

	gsolve1(Solve),
	osolve1(Solve),
	dsolve1(Solve),!.


	

	


	

	
	