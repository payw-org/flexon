mainprog passingIncompatibleNonArrayToArray;
int[3] globalA;
float globalB;

  procedure printArray(a: int[3]; b: float[4]);

  begin
    print(a);
    print(b)
  end

begin
  printArray(globalA, globalB)
end
