; ModuleID = 'source.spice'
source_filename = "source.spice"

@0 = private unnamed_addr constant [7 x i8] c"1: %d\0A\00", align 1
@1 = private unnamed_addr constant [7 x i8] c"2: %d\0A\00", align 1
@2 = private unnamed_addr constant [7 x i8] c"3: %d\0A\00", align 1
@3 = private unnamed_addr constant [7 x i8] c"4: %d\0A\00", align 1
@intArray = dso_local constant [4 x i32] [i32 1, i32 2, i32 3, i32 4]
@4 = private unnamed_addr constant [7 x i8] c"1: %d\0A\00", align 1
@5 = private unnamed_addr constant [5 x i8] c"test\00", align 1
@6 = private unnamed_addr constant [10 x i8] c"Char: %c\0A\00", align 1

declare i32 @printf(i8*, ...)

; Function Attrs: nounwind
define void @"testProc(int[]**)"(i32*** %0) #0 {
entry:
  %nums = alloca i32***, align 8
  %nums1 = alloca i32*, align 8
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  %5 = alloca i32, align 4
  %6 = alloca i32, align 4
  store i32*** %0, i32**** %nums, align 8
  %7 = load i32***, i32**** %nums, align 8
  %8 = load i32**, i32*** %7, align 8
  %9 = load i32*, i32** %8, align 8
  store i32* %9, i32** %8, align 8
  %10 = load i32*, i32** %8, align 8
  store i32* %10, i32** %nums1, align 8
  store i32 10, i32* %1, align 4
  %11 = load i32, i32* %1, align 4
  %12 = load i32*, i32** %nums1, align 8
  store i32 2, i32* %2, align 4
  %13 = load i32, i32* %2, align 4
  %14 = getelementptr inbounds i32, i32* %12, i32 %13
  store i32 %11, i32* %14, align 4
  %15 = load i32*, i32** %nums1, align 8
  store i32 0, i32* %3, align 4
  %16 = load i32, i32* %3, align 4
  %17 = getelementptr inbounds i32, i32* %15, i32 %16
  %18 = load i32, i32* %17, align 4
  %19 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @0, i32 0, i32 0), i32 %18)
  %20 = load i32*, i32** %nums1, align 8
  store i32 1, i32* %4, align 4
  %21 = load i32, i32* %4, align 4
  %22 = getelementptr inbounds i32, i32* %20, i32 %21
  %23 = load i32, i32* %22, align 4
  %24 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @1, i32 0, i32 0), i32 %23)
  %25 = load i32*, i32** %nums1, align 8
  store i32 2, i32* %5, align 4
  %26 = load i32, i32* %5, align 4
  %27 = getelementptr inbounds i32, i32* %25, i32 %26
  %28 = load i32, i32* %27, align 4
  %29 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @2, i32 0, i32 0), i32 %28)
  %30 = load i32*, i32** %nums1, align 8
  store i32 3, i32* %6, align 4
  %31 = load i32, i32* %6, align 4
  %32 = getelementptr inbounds i32, i32* %30, i32 %31
  %33 = load i32, i32* %32, align 4
  %34 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @3, i32 0, i32 0), i32 %33)
  ret void
}

define i32 @main() {
entry:
  %result = alloca i32, align 4
  %intArray = alloca [4 x i32], align 4
  %0 = alloca [4 x i32], align 4
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  %5 = alloca i32, align 4
  %6 = alloca [4 x i32]*, align 8
  %7 = alloca [4 x i32]**, align 8
  %8 = alloca i32*, align 8
  %9 = alloca i32**, align 8
  %10 = alloca i1, align 1
  %test = alloca i8*, align 8
  %11 = alloca i8*, align 8
  %c1 = alloca i8, align 1
  %12 = alloca i32, align 4
  store i32 0, i32* %result, align 4
  store i32 1, i32* %1, align 4
  store i32 2, i32* %2, align 4
  store i32 3, i32* %3, align 4
  store i32 4, i32* %4, align 4
  %13 = load [4 x i32], [4 x i32]* @intArray, align 4
  store [4 x i32] %13, [4 x i32]* %intArray, align 4
  %14 = load [4 x i32], [4 x i32]* %intArray, align 4
  store i32 1, i32* %5, align 4
  %15 = load i32, i32* %5, align 4
  %16 = getelementptr inbounds [4 x i32], [4 x i32]* %intArray, i32 0, i32 %15
  %17 = load i32, i32* %16, align 4
  %18 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @4, i32 0, i32 0), i32 %17)
  %19 = load [4 x i32], [4 x i32]* %intArray, align 4
  store [4 x i32]* %intArray, [4 x i32]** %6, align 8
  store [4 x i32]** %6, [4 x i32]*** %7, align 8
  store [4 x i32]** %6, [4 x i32]*** %7, align 8
  %20 = load [4 x i32]**, [4 x i32]*** %7, align 8
  %21 = load [4 x i32]*, [4 x i32]** %20, align 8
  %22 = getelementptr inbounds [4 x i32], [4 x i32]* %21, i32 0, i32 0
  store i32* %22, i32** %8, align 8
  store i32** %8, i32*** %9, align 8
  call void @"testProc(int[]**)"(i32*** %9)
  store i1 true, i1* %10, align 1
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @5, i32 0, i32 0), i8** %11, align 8
  %23 = load i8*, i8** %11, align 8
  store i8* %23, i8** %test, align 8
  %24 = load i8*, i8** %test, align 8
  store i32 2, i32* %12, align 4
  %25 = load i32, i32* %12, align 4
  %26 = getelementptr inbounds i8, i8* %24, i32 %25
  %27 = load i8, i8* %26, align 1
  store i8 %27, i8* %c1, align 1
  %28 = load i8, i8* %c1, align 1
  %29 = zext i8 %28 to i32
  %30 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @6, i32 0, i32 0), i32 %29)
  %31 = load i32, i32* %result, align 4
  ret i32 %31
}

attributes #0 = { nounwind }
