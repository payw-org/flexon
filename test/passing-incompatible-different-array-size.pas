mainprog passingIncompatibleDifferentArraySize;
int[3] globalA;
float[4] globalB;

  procedure printArray(a: int[3]; b: float[5]);

  begin
    print(a);
    print(b)
  end

begin
  printArray(globalA, globalB)
end
