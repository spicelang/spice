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
  %intArray = alloca [5 x i32], align 4
  %2 = alloca i32, align 4
  store i32 7, i32* %2, align 4
  %3 = alloca i32, align 4
  store i32 4, i32* %3, align 4
  %4 = load i32, i32* %value0, align 4
  %5 = getelementptr inbounds [5 x i32], [5 x i32]* %intArray, i32 0, i32 0
  store i32 %4, i32* %5, align 4
  %6 = load i32, i32* %2, align 4
  %7 = getelementptr inbounds [5 x i32], [5 x i32]* %intArray, i32 0, i32 1
  store i32 %6, i32* %7, align 4
  %8 = load i32, i32* %3, align 4
  %9 = getelementptr inbounds [5 x i32], [5 x i32]* %intArray, i32 0, i32 2
  store i32 %8, i32* %9, align 4
  %10 = alloca i32, align 4
  store i32 11, i32* %10, align 4
  %11 = load i32, i32* %10, align 4
  %12 = alloca i32, align 4
  store i32 2, i32* %12, align 4
  %13 = load i32, i32* %12, align 4
  %14 = getelementptr inbounds [5 x i32], [5 x i32]* %intArray, i32 0, i32 %13
  store i32 %11, i32* %14, align 4
  %15 = alloca i32, align 4
  store i32 3, i32* %15, align 4
  %16 = load i32, i32* %15, align 4
  %17 = alloca i32, align 4
  store i32 0, i32* %17, align 4
  %18 = load i32, i32* %17, align 4
  %19 = getelementptr inbounds [5 x i32], [5 x i32]* %intArray, i32 0, i32 %18
  store i32 %16, i32* %19, align 4
  %20 = alloca i32, align 4
  store i32 0, i32* %20, align 4
  %21 = load i32, i32* %20, align 4
  %22 = getelementptr inbounds [5 x i32], [5 x i32]* %intArray, i32 0, i32 %21
  %23 = load i32, i32* %22, align 4
  %24 = alloca i32, align 4
  store i32 2, i32* %24, align 4
  %25 = load i32, i32* %24, align 4
  %26 = getelementptr inbounds [5 x i32], [5 x i32]* %intArray, i32 0, i32 %25
  %27 = load i32, i32* %26, align 4
  %28 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([35 x i8], [35 x i8]* @0, i32 0, i32 0), i32 %23, i32 %27)
  %29 = load i32, i32* %result, align 4
  ret i32 %29
}
