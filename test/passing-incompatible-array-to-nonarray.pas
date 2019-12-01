mainprog passingIncompatibleArrayToNonArray;
int[3] globalA;
float[2] globalB;

  procedure printArray(a: int[3]; b: float);

  begin
    print(a);
    print(b)
  end

begin
  printArray(globalA, globalB)
end
