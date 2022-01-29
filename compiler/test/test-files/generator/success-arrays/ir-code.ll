; ModuleID = 'source.spice'
source_filename = "source.spice"

@0 = private unnamed_addr constant [35 x i8] c"Item 0: %d, item 2: %d, item 4: %d\00", align 1

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %result = alloca i32, align 4
  %value0 = alloca i32, align 4
  %0 = alloca i32, align 4
  %intArray = alloca [5 x i32], align 4
  %1 = alloca [5 x i32], align 4
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  %5 = alloca i32, align 4
  %6 = alloca i32, align 4
  %7 = alloca i32, align 4
  %8 = alloca i32, align 4
  %9 = alloca i32, align 4
  %10 = alloca i32, align 4
  store i32 0, i32* %result, align 4
  store i32 2, i32* %0, align 4
  %11 = load i32, i32* %0, align 4
  store i32 %11, i32* %value0, align 4
  store i32 7, i32* %2, align 4
  store i32 4, i32* %3, align 4
  %12 = load i32, i32* %value0, align 4
  %13 = getelementptr inbounds [5 x i32], [5 x i32]* %1, i32 0, i32 0
  store i32 %12, i32* %13, align 4
  %14 = load i32, i32* %2, align 4
  %15 = getelementptr inbounds [5 x i32], [5 x i32]* %1, i32 0, i32 1
  store i32 %14, i32* %15, align 4
  %16 = load i32, i32* %3, align 4
  %17 = getelementptr inbounds [5 x i32], [5 x i32]* %1, i32 0, i32 2
  store i32 %16, i32* %17, align 4
  %18 = load [5 x i32], [5 x i32]* %1, align 4
  store [5 x i32] %18, [5 x i32]* %intArray, align 4
  store i32 11, i32* %4, align 4
  %19 = load i32, i32* %4, align 4
  %20 = load [5 x i32], [5 x i32]* %intArray, align 4
  store i32 2, i32* %5, align 4
  %21 = load i32, i32* %5, align 4
  %22 = getelementptr inbounds [5 x i32], [5 x i32]* %intArray, i32 0, i32 %21
  store i32 %19, i32* %22, align 4
  store i32 3, i32* %6, align 4
  %23 = load i32, i32* %6, align 4
  %24 = load [5 x i32], [5 x i32]* %intArray, align 4
  store i32 0, i32* %7, align 4
  %25 = load i32, i32* %7, align 4
  %26 = getelementptr inbounds [5 x i32], [5 x i32]* %intArray, i32 0, i32 %25
  store i32 %23, i32* %26, align 4
  %27 = load [5 x i32], [5 x i32]* %intArray, align 4
  store i32 0, i32* %8, align 4
  %28 = load i32, i32* %8, align 4
  %29 = getelementptr inbounds [5 x i32], [5 x i32]* %intArray, i32 0, i32 %28
  %30 = load i32, i32* %29, align 4
  %31 = load [5 x i32], [5 x i32]* %intArray, align 4
  store i32 2, i32* %9, align 4
  %32 = load i32, i32* %9, align 4
  %33 = getelementptr inbounds [5 x i32], [5 x i32]* %intArray, i32 0, i32 %32
  %34 = load i32, i32* %33, align 4
  %35 = load [5 x i32], [5 x i32]* %intArray, align 4
  store i32 4, i32* %10, align 4
  %36 = load i32, i32* %10, align 4
  %37 = getelementptr inbounds [5 x i32], [5 x i32]* %intArray, i32 0, i32 %36
  %38 = load i32, i32* %37, align 4
  %39 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([35 x i8], [35 x i8]* @0, i32 0, i32 0), i32 %30, i32 %34, i32 %38)
  %40 = load i32, i32* %result, align 4
  ret i32 %40
}
