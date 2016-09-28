type
  TMatrix = array[0..99, 0..99] of integer;
var
  a, b: TMatrix;
  i, j, n, count: integer;

function MatrixDiv(a, b: TMatrix): TMatrix;
var
  tmp: TMatrix;
  i, j, k: integer;
begin

  for i := 0 to n-1 do
    for j := 0 to n-1 do
      tmp[i,j] := 0;

  for i := 0 to n-1 do
    for j := 0 to n-1 do
      for k := 0 to n-1 do
        tmp[i,j] := tmp[i,j] + a[i,k]*b[k,j];

  for i := 0 to n-1 do
    for j := 0 to n-1 do
      b[i,j] := tmp[i,j];
  result := b;
end;

function Rec(a, b: TMatrix; l: integer): TMatrix;
begin
  if (l <> 1) then
  begin
    b := MatrixDiv(a,b);
    result := Rec(a, b, l-1);
  end
  else
    result := b;
end;

begin
  try
    write('Enter N [1..100]: ');
    read(n);
    while (n < 1) or (n > 100) do
    begin
      writeln('N must be in [1..100] range. Try: ');
      read(n);
    end;
    writeln;
    writeln('Input start;');

    for i := 0 to n-1 do
      for j := 0 to n-1 do
        read(a[i,j]);
    writeln('Input end;');
    writeln;
    writeln('Matrix: (LOOP)');
    b := a;
    for count := 1 to 4 do
      b := MatrixDiv(a,b);
    for i := 0 to n-1 do
    begin
      for j := 0 to n-1 do
        write(b[i,j], ' ');
      writeln;
    end;
    writeln;
    writeln('Matrix: (RECURSION)');
    b := Rec(a, a, 5);

    for i := 0 to n-1 do
    begin
      for j := 0 to n-1 do
        write(b[i,j], ' ');
      writeln;
    end;

  except
    writeln('Error');
  end;
  read(i);
end.

