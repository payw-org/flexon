mainprog procedureInExpression;
int[3] a;
int b;

  procedure printArray(x: int[3]);

  begin
    print(x)
  end

begin
  b = a[1] + printArray(a)
end