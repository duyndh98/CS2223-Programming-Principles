dfs(Dst, Dst, _, [Dst]) :-
  print(found),nl,
  print(Dst),nl,
  !.
dfs(Cur, Dst, Visited, [Cur|Path]) :-
  find_pos_2d(Cur, 0, ZeroRowId, ZeroColId),
  transit(Cur, ZeroRowId, ZeroColId, Next),
  Cur \= Next,
  \+member(Next, Visited),
  nl, print(Cur), nl,
  dfs(Next, Dst, [Cur|Visited], Path),
  length(Path, Len0),
  length(Visited, Len),
  write('Step '), print(Len), write(' '), print(Cur), write(' '),
  write('Len '), print(Len0), write(' '), nl.

transit(Cur, RowId, ColId, Next) :- pull_down(Cur, RowId, ColId, Next).
transit(Cur, RowId, ColId, Next) :- pull_up(Cur, RowId, ColId, Next).
transit(Cur, RowId, ColId, Next) :- pull_right(Cur, RowId, ColId, Next).
transit(Cur, RowId, ColId, Next) :- pull_left(Cur, RowId, ColId, Next).

pull_up(Src, RowId, ColId, Dst) :- RowId > 0,
  write('up '),
  PullRowId is RowId - 1,
  get_val_2d(Src, PullRowId, ColId, Val),
  set_val_2d(Src, PullRowId, ColId, 0, Tmp),
  set_val_2d(Tmp, RowId, ColId, Val, Dst),
  !.

pull_down(Src, RowId, ColId, Dst) :- length(Src, RowCount), RowId < RowCount - 1,
  write('down '),
  PullRowId is RowId + 1,
  get_val_2d(Src, PullRowId, ColId, Val),
  set_val_2d(Src, PullRowId, ColId, 0, Tmp),
  set_val_2d(Tmp, RowId, ColId, Val, Dst),
  !.

pull_left(Src, RowId, ColId, Dst) :- ColId > 0,
  write('left '),
  PullColId is ColId - 1,
  get_val_2d(Src, RowId, PullColId, Val),
  set_val_2d(Src, RowId, PullColId, 0, Tmp),
  set_val_2d(Tmp, RowId, ColId, Val, Dst),
  !.

pull_right(Src, RowId, ColId, Dst) :- get_val(Src, 0, FirstRow), length(FirstRow, ColCount), ColId < ColCount - 1,
  write('right '),
  PullColId is ColId + 1,
  get_val_2d(Src, RowId, PullColId, Val),
  set_val_2d(Src, RowId, PullColId, 0, Tmp),
  set_val_2d(Tmp, RowId, ColId, Val, Dst),
  !.

find_pos([Val|_], Val, 0) :-
  !.
find_pos([_|Tail], Val, Index) :-
  find_pos(Tail, Val, FoundIndex),
  Index is FoundIndex + 1.

find_pos_2d([Row|_], Val, 0, ColId) :- member(Val, Row),
  find_pos(Row, Val, ColId),
  !.
find_pos_2d([_|NextRows], Val, RowId, ColId) :-
  find_pos_2d(NextRows, Val, FoundRowId, ColId),
  RowId is FoundRowId + 1.

get_val([Val|_], 0, Val) :-
  !.
get_val([_|Tail], Index, Val) :- Index > 0,
  FindIndex is Index - 1,
  get_val(Tail, FindIndex, Val).

get_val_2d([Row|_], 0, ColId, Val) :-
  get_val(Row, ColId, Val),
  !.
get_val_2d([_|NextRows], RowId, ColId, Val) :- RowId > 0,
  FindRowId is RowId - 1,
  get_val_2d(NextRows, FindRowId, ColId, Val).

set_val([_|Tail], 0, Val, [Val|Tail]) :-
  !.
set_val([Head|SrcTail], Index, Val, [Head|DstTail]) :- Index > 0,
  FindIndex is Index - 1,
  set_val(SrcTail, FindIndex, Val, DstTail).

set_val_2d([SrcRow|NextRows], 0, ColId, Val, [DstRow|NextRows]) :-
  set_val(SrcRow, ColId, Val, DstRow),
  !.
set_val_2d([PrevRows|SrcNextRows], RowId, ColId, Val, [PrevRows|DstNextRows]) :- RowId > 0,
  FindRowId is RowId - 1,
  set_val_2d(SrcNextRows, FindRowId, ColId, Val, DstNextRows).
