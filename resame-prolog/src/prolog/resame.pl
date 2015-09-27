% vim: set ft=prolog:

% Neste arquivo estão especificados os predicados que devem ser implementados.
% Você pode criar predicados auxiliares quando necessário.
%
% No arquivo resame_testes.pl estão os testes para alguns predicados.
%
% Para implementar cada predicado, primeiro você deve ler e entender a
% especificação e o teste.
%
% A especificação dos parâmetros dos predicados segue o formato descrito em
% http://www.swi-prolog.org/pldoc/doc_for?object=section%282,%274.1%27,swi%28%27/doc/Manual/preddesc.html%27%29%29
%
% Um Jogo same é representado por uma lista de colunas, sem os elementos nulos
% (zeros).
% Por exemplo, o jogo
% 2 | 3 0 0 0
% 1 | 2 2 2 0
% 0 | 2 3 3 1
% --+--------
%   | 0 1 2 3
% é representado como [[2, 2, 3], [3, 2], [3, 2], [1]].
% O tamanho deste jogo é 3x4 (linhas x colunas).
%
% Uma posição no jogo é representado por uma estrutura pos com dois argumentos
% (lin, col), onde lin é o número da linha e col é o número da coluna.  No
% exemplo anterior, a posição pos(0, 1) tem cor 3, e a posição pos(1, 2) tem
% cor 2.

% Você pode utilizar os predicados definidos no arquivo resame_utils.pl
:- consult(resame_utils).
% Funcao para pegar a cor de uma determinada posicao.
get_color(M, pos(X, Y), C) :-
   nth0(Y, M, Line),
   nth0(X, Line, C).

% Compara cores de duas posicoes.
comp_elem(M, P1, P2) :-
   get_color(M, P1, C1),
   get_color(M, P2, C2),
   C1 == C2, !.

%% main(+File) is det
%
%  Carrega um jogo same do arquivo File e imprime uma resolução na saída padrão
%  ou sem-solucao se o jogo não tem solução.

main(File) :-
   read_matrix_file(File, M),
   transpose(M, Same),
   solve(Same, Moves), !,
   length(Same, Len),
   play(Moves, Same, Len).

play([], _, _).
play([M|Moves], Same, Lsame) :-
   group(Same, M, Group),
   remove_group(Same, Group, NewSame),
   nth0(0, Same, A),
   length(A, Hsame),
   length(NewSame, Lnew),
   Zcol is Lsame - Lnew,
   add_column(Hsame, Zcol, NewSame, PrintSame),
   M = pos(X,Y),
   write(X), write(' '), write(Y),
   writeln(''), writeln(''),
   transpose(PrintSame, Tsame),
   write_matrix(Tsame),
   writeln(''),
   play(Moves, NewSame, Lsame), !.

add_column(_,0,Same,Same).
add_column(Height, Zero, Same, NewSame) :-
   Zero \= 0,
   mklist(Height, Col),
   append(Same, [Col], New),
   Newz is Zero -1,
   add_column(Height, Newz, New, NewSame), !.

%% solve(+Same, -Moves) is nondet
%
%  Verdadeiro se Moves é uma sequência de jogadas (lista de posições) que
%  quando realizadas ("clicadas") resolvem o jogo Same.
%  Este predicado não tem teste de unidade. Ele é testado pelo testador.

solve([], []) :- !.
solve(Same, [M|Moves]) :-
   group(Same, Group),
   remove_group(Same, Group, NewSame),
   [M|_] = Group,
   solve(NewSame, Moves).

%% group(+Same, ?Group) is nondet
%
%  Verdadeiro se Group é um grupo de Same. Group é uma lista de posições
%  (estrutura pos(lin,col)). Este predicado é não determinístico e deve ser
%  capaz de gerar todos os grupos de Same. Este predicado não deve gerar grupos
%  repetidos. Este predicado e group/3 para vão utilizar os mesmos precicados
%  auxiliares.

group(Same, Group) :-
   grupos_existentes(Same, [], List), !,
   member_any(Group, List).

member_any(Group, [H|Tail]) :-
   Tail \= [],
   same_length(Group, H),
   subset(H, Group).

member_any(Group, [H|Tail]) :-
   Tail = [], !,
   same_length(Group, H),
   subset(H, Group).

member_any(Group, [_|Tail]) :-
   member_any(Group, Tail).

grupos_existentes(Same, Ac, List) :-
   is_valid(Same, P),
   not(subset([P], Ac)),
   group(Same, P, Group),
   sort(Group, Sorted),
   append(Sorted, Ac, NewAc),
   [Sorted|Tail] = List,
   grupos_existentes(Same, NewAc, Tail), !.

grupos_existentes(_,_,List) :-
   List = [].

is_valid(Same, pos(X,Y)) :-
   length(Same, L),
   Len is L-1,
   between(0, Len, Y),
   nth0(Y, Same, Line),
   length(Line, A),
   Alt is A-1,
   between(0, Alt, X),
   get_color(Same, pos(X,Y), Color),
   Color \= 0.

%% grupo(+Same, +P, -Group) is semidet
%
%  Verdadeiro se Group é um grupo de Same que contém a posição P.

group(Same, P, Group) :-
   neighbor(Same, P, [P], [], Group),
   length(Group, L), !,
   L > 1.

neighbor(Same, Origin, [P|Tail], Ac, Group) :-
   comp_elem(Same, Origin, P),
   append([P], Ac, N),
   member(P, Group), !,
   get_neighbors(P, Neigh),
   append_neigh(N, Neigh, Tail, NewCand),
   neighbor(Same, Origin, NewCand, N, Group), !.

neighbor(Same, Origin, [P|Tail], Ac, Group) :-
   not(comp_elem(Same, Origin, P)),
   neighbor(Same, Origin, Tail, Ac, Group), !.

neighbor(_, _, [], _, _).

append_neigh(Ac, [P|Tail], Candidates, N) :-
   not(member(P, Ac)),
   union([P], Candidates, New),
   append_neigh(Ac, Tail, New, N), !.

append_neigh(Ac, [P|Tail], Candidates, N) :-
   member(P, Ac),
   append_neigh(Ac, Tail, Candidates, N).

append_neigh(_, [], Cand, Cand).

get_neighbors(pos(X,Y), [pos(X1,Y), pos(X2,Y), pos(X,Y3), pos(X,Y4)]) :-
   X1 is X+1, X2 is X-1, Y3 is Y+1, Y4 is Y-1.

%% remove_group(+Same, +Group, -NewSame) is semidet
%
%  Verdadeiro se NewSame é obtido de Same removendo os elemento especificados
%  em Group. A remoção é feita de acordo com as regras do jogo same.
%  Dica:
%    - crie um predicado auxiliar remove_column_group, que remove os elementos
%    de uma coluna específica
remove_group(Same, Group, NewSame) :-
   sort(Group, Gsort),
   remove(Same, 0, Gsort, NewSame).

remove([Hs|Ts], Coln, Group, [Hn|Tn]) :-
   remove_column_group(Coln, 0, 0, Hs, Group, NewCol),
   NewCol \= [0|_],
   Hn = NewCol,
   NewColn is Coln +1,
   remove(Ts, NewColn, Group, Tn), !.

remove([Hs|Ts], Coln, Group, NewSame) :-
   remove_column_group(Coln, 0, 0, Hs, Group, NewCol),
   NewCol = [0|_],
   NewColn is Coln +1,
   remove(Ts, NewColn, Group, NewSame), !.

remove([],_,_,[]).

remove_column_group(Coln, Zeros, Lin, [_|Tc], [pos(X,Coln)|Tg], NewCol) :-
   Xr is X - Lin, Xr = 0,
   Newl is Lin + 1,
   Newz is Zeros + 1,
   remove_column_group(Coln, Newz, Newl, Tc, Tg, NewCol), !.

remove_column_group(Coln, Zeros, Lin, [Hc|Tc], [pos(X,Coln)|Tg], [Hn|Tn]) :-
   Xr is X - Lin, Xr \= 0, Hn = Hc,
   Newl is Lin +1,
   remove_column_group(Coln, Zeros, Newl, Tc, [pos(X,Coln)|Tg], Tn), !.

remove_column_group(Coln, Zeros, Lin, Col, [pos(_,Y)|Tg], NewCol) :-
   Y \= Coln, remove_column_group(Coln, Zeros, Lin, Col, Tg, NewCol), !.

remove_column_group(_, Zeros, _, Col, [], NewCol) :-
   mklist(Zeros, List),
   append(Col, List, NewCol), !.

mklist(Zeros, List) :-
   Zeros \= 0,
   Newz is Zeros - 1,
   [0|T] = List,
   mklist(Newz, T).

mklist(0,[]).
