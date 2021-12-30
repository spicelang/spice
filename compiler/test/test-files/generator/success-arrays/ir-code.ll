; ModuleID = 'source.spice'
source_filename = "source.spice"

@0 = private unnamed_addr constant [35 x i8] c"Array item 0: %d, array item 2: %d\00", align 1

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %result = alloca i32, align 4
  store i32 0, i32* %result, align 4
  %0 = alloca i32, align 4
  store i32 2, i32* %0, align 4
  %1 = load i32, i32* %0, align 4
  %value0 = alloca i32, align 4
  store i32 %1, i32* %value0, align 4
  %intArray = alloca [5 x i32], i32 5, align 4
  %2 = getelementptr inbounds i32, i32* %value0, i32 0
  %3 = load i32, i32* %2, align 4
  %4 = getelementptr inbounds [5 x i32], [5 x i32]* %intArray, i32 0, i32 0
  store i32 %3, i32* %4, align 4
  %5 = alloca i32, align 4
  store i32 7, i32* %5, align 4
  %6 = load i32, i32* %5, align 4
  %7 = getelementptr inbounds [5 x i32], [5 x i32]* %intArray, i32 0, i32 1
  store i32 %6, i32* %7, align 4
  %8 = alloca i32, align 4
  store i32 4, i32* %8, align 4
  %9 = load i32, i32* %8, align 4
  %10 = getelementptr inbounds [5 x i32], [5 x i32]* %intArray, i32 0, i32 2
  store i32 %9, i32* %10, align 4
  %11 = alloca i32, align 4
  store i32 11, i32* %11, align 4
  %12 = load i32, i32* %11, align 4
  %13 = alloca i32, align 4
  store i32 2, i32* %13, align 4
  %14 = load i32, i32* %13, align 4
  %15 = getelementptr inbounds [5 x i32], [5 x i32]* %intArray, i32 0, i32 %14
  store i32 %12, i32* %15, align 4
  %16 = alloca i32, align 4
  store i32 3, i32* %16, align 4
  %17 = load i32, i32* %16, align 4
  %18 = alloca i32, align 4
  store i32 0, i32* %18, align 4
  %19 = load i32, i32* %18, align 4
  %20 = getelementptr inbounds [5 x i32], [5 x i32]* %intArray, i32 0, i32 %19
  store i32 %17, i32* %20, align 4
  %21 = alloca i32, align 4
  store i32 0, i32* %21, align 4
  %22 = load i32, i32* %21, align 4
  %23 = getelementptr inbounds [5 x i32], [5 x i32]* %intArray, i32 0, i32 %22
  %24 = load i32, i32* %23, align 4
  %25 = alloca i32, align 4
  store i32 2, i32* %25, align 4
  %26 = load i32, i32* %25, align 4
  %27 = getelementptr inbounds [5 x i32], [5 x i32]* %intArray, i32 0, i32 %26
  %28 = load i32, i32* %27, align 4
  %29 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([35 x i8], [35 x i8]* @0, i32 0, i32 0), i32 %24, i32 %28)
  %30 = load i32, i32* %result, align 4
  ret i32 %30
}
