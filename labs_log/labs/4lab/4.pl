
gen_sush(L):- L = ['book', 'table', 'pen'].
gen_glag(L):- L = [].
gen_size(L):- L = ['big','little'].
gen_color(L):- L = ['red'].
gen_loc(L):- L = ['on', 'under'].



a_sush([X], X):- gen_sush(L), member(X, L).
a_glag([X], glag(X)):- gen_glag(L), member(X, L).
a_color([X], color(X)):- gen_color(L), member(X, L).
a_size([X], size(X)):- gen_size(L), member(X, L).
a_loc([X], loc(X)):- gen_loc(L), member(X, L).



a_group_sush([X], object(X)):-
    a_sush([X], X).

a_group_sush(S, object(Y, X)):-
    append(S1, S2, S),
    a_size(S1, X),
    a_sush(S2, Y).

a_group_sush(S, object(Y, X)):-
    append(S1, S2, S),
    a_color(S1, X),
    a_sush(S2, Y).

a_group_glag([X], Y):-
    a_size([X], Y).

a_group_glag([X], Y):-
    a_color([X], Y).

a_location('under', S,  under(Tout)):-
    a_group_sush(S, Tout).

a_location('on', S,  on(Tout)):-
    a_group_sush(S, Tout).

del([],_,[]).
del([El|T],El,T1):-!,del(T,El,T1).
del([X|T],El,[X|T1]):-del(T,El,T1).



a_sentence(L, R):-
    append(S1, ['is'|S2], L),
    %write(S1), nl, write(S2), nl, nl,
    a_group_sush(S1, X),
    a_group_glag(S2, Y),
    R = sentence(X, Y), !.

a_sentence(L, R):-
    append(S1, ['is', Loc | S2], L),
    a_group_sush(S1, X),
    a_location(Loc, S2, Y),
    R = sentence(location(X, Y)), !.

go(Ll, R):-
    del(Ll, 'the', L),
    a_sentence(L, R).

%удалить все the
    