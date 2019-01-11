test --> append([1, 2, 3]).

fact(0,1).
fact(N, F):-
    N1 is N - 1,
    fact(N1, F1),
    F is F1 * N.

f(X):-
    X = 4.

p([X, _|T], [X| R]):- p(T, R).
p([],[]).
p([X],[X]).

st(_X, 0, 1).
st(X, 1, X).
st(X, N, R):-
    N1 is N - 1,
    st(X, N1, Rr),
    R is Rr*X, !.

move(X, Y, MHead):-
    append([], [A, B | T], X),
    N is A + B,
    append([], [N | T], Y),
    MHead = '+'.

move(X, Y, MHead):-
    append([], [A, B | T], X),
    N is A - B,
    append([], [N | T], Y),
    MHead = '-'.



printlist([]).
printlist([H|T]):-
    write(H), write(','), printlist(T).

prolong([X|T], [Y, X|T], MHead):-
    move(X, Y, MHead),
    not(member(Y, [X|T])).

dfs(Start, Finish):-
    pathd([Start], Finish, Way, Moves),
    %printlist(Way),
    printlist(Moves).

pathd([Finish|T], Finish, [Finish|T], []).
pathd(TempWay, Finish, Way, [MHead | MTail]):-
    prolong(TempWay, NewWay, MHead),
    pathd(NewWay, Finish, Way, MTail).

:- dynamic((=)/2).
add(X, nil) = t(X, nil, nil).
add(X, t(Y, L, R)) = t(Y, L1, R1) :-
    X>Y -> L1 = L, R1 = add(X, R);
    X<Y -> L1 = add(X, L), R1 = R;
    L1 = L, R1 = R.

