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
  %2 = alloca [5 x i32], align 4
  %3 = getelementptr inbounds i32, i32* %value0, i32 0
  %4 = load i32, i32* %3, align 4
  %5 = getelementptr inbounds [5 x i32], [5 x i32]* %2, i32 0, i32 0
  store i32 %4, i32* %5, align 4
  %6 = alloca i32, align 4
  store i32 7, i32* %6, align 4
  %7 = load i32, i32* %6, align 4
  %8 = getelementptr inbounds [5 x i32], [5 x i32]* %2, i32 0, i32 1
  store i32 %7, i32* %8, align 4
  %9 = alloca i32, align 4
  store i32 4, i32* %9, align 4
  %10 = load i32, i32* %9, align 4
  %11 = getelementptr inbounds [5 x i32], [5 x i32]* %2, i32 0, i32 2
  store i32 %10, i32* %11, align 4
  %12 = alloca i32, align 4
  store i32 11, i32* %12, align 4
  %13 = load i32, i32* %12, align 4
  %14 = alloca i32, align 4
  store i32 2, i32* %14, align 4
  %15 = load i32, i32* %14, align 4
  %16 = getelementptr inbounds [5 x i32], [5 x i32]* %2, i32 0, i32 %15
  store i32 %13, i32* %16, align 4
  %17 = alloca i32, align 4
  store i32 3, i32* %17, align 4
  %18 = load i32, i32* %17, align 4
  %19 = alloca i32, align 4
  store i32 0, i32* %19, align 4
  %20 = load i32, i32* %19, align 4
  %21 = getelementptr inbounds [5 x i32], [5 x i32]* %2, i32 0, i32 %20
  store i32 %18, i32* %21, align 4
  %22 = alloca i32, align 4
  store i32 0, i32* %22, align 4
  %23 = load i32, i32* %22, align 4
  %24 = getelementptr inbounds [5 x i32], [5 x i32]* %2, i32 0, i32 %23
  %25 = load i32, i32* %24, align 4
  %26 = alloca i32, align 4
  store i32 2, i32* %26, align 4
  %27 = load i32, i32* %26, align 4
  %28 = getelementptr inbounds [5 x i32], [5 x i32]* %2, i32 0, i32 %27
  %29 = load i32, i32* %28, align 4
  %30 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([35 x i8], [35 x i8]* @0, i32 0, i32 0), i32 %25, i32 %29)
  %31 = load i32, i32* %result, align 4
  ret i32 %31
}
