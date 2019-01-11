printlist([ ]):- !.
printlist([H | T]):- write(H), nl, printlist(T).


mymember(X, [X, _T]).
mymember(X, [_H|T]):-
	mymember(X, T).

mylen([], 0).
mylen([_X|T], N) :- mylen(T, N1), N is N1+1.

myappend([], X, X).
myappend([X|M], K, [X|L]):-
	append(M, K, L).

myremove(X, [X|T], T).
myremove(X, [Y|T], [Y|TN]):-
	myremove(X, T, TN).

myinsert(X, L, BiggerL):-
	myremove(X, BiggerL, L).

mypermute([], []).
mypermute([X|L], P):-
	mypermute(L, L1),
	myinsert(X, L1, P).

sub_start([], _L):-!.
sub_start([H|TSub], [H|T]):-
   sub_start(TSub, T).

sublist(Sub, L):-
   sub_start(Sub, L), !.

sublist(Sub, [_H|T]):-
   sublist(Sub, T).

deletelast([_|[]], []).
deletelast([H|T], [H|LL]):-
	deletelast(T, LL).

%%%%%%%%%%%%%%%%%%%

getfirstelements(_,N,[]) :- N=<0.
getfirstelements(_X,0,[]).
getfirstelements([X|XT],N,[X|YT]):- N>0,
	N1 is N-1, 
	getfirstelements(XT,N1,YT).

deletelastthree1([], []).
deletelastthree1(L, LL) :-
	mylen(L,D), N1 is D-3, N1>=0,
	getfirstelements(L, N1, LL), !.

deletelastthree2(X, R):-
	myappend(R, S2, X),
	mylen(S2, 3), !.

deletelastthree3(X, R):-
	deletelast(X, R1),
	deletelast(R1, R2),
	deletelast(R2, R), !.

%%%%%%%%%%%%%yporyadochennostpovozrastaniyu

checkincrease([_]):- !.
checkincrease([X,Y|T]):- X<Y,!,
	checkincrease([Y|T]), !.

checkincrease2([_]).
checkincrease2([A,B|T]):-







