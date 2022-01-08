; ModuleID = 'source.spice'
source_filename = "source.spice"

@0 = private unnamed_addr constant [7 x i8] c"1: %d\0A\00", align 1
@1 = private unnamed_addr constant [7 x i8] c"2: %d\0A\00", align 1
@2 = private unnamed_addr constant [7 x i8] c"3: %d\0A\00", align 1
@3 = private unnamed_addr constant [7 x i8] c"4: %d\0A\00", align 1
@intArray = dso_local global [4 x i32] [i32 1, i32 2, i32 3, i32 4]
@4 = private unnamed_addr constant [7 x i8] c"1: %d\0A\00", align 1
@5 = private unnamed_addr constant [5 x i8] c"test\00", align 1
@6 = private unnamed_addr constant [10 x i8] c"Char: %c\0A\00", align 1

declare i32 @printf(i8*, ...)

; Function Attrs: nounwind
define void @"testProc(int[]**)"(i32*** %0) #0 {
entry:
  %nums = alloca i32***, align 8
  store i32*** %0, i32**** %nums, align 8
  %1 = load i32***, i32**** %nums, align 8
  %2 = load i32**, i32*** %1, align 8
  %3 = load i32*, i32** %2, align 8
  %nums1 = alloca i32*, align 8
  store i32* %3, i32** %nums1, align 8
  %4 = alloca i32, align 4
  store i32 10, i32* %4, align 4
  %5 = load i32, i32* %4, align 4
  %6 = alloca i32, align 4
  store i32 2, i32* %6, align 4
  %7 = load i32, i32* %6, align 4
  %8 = load i32*, i32** %nums1, align 8
  %9 = getelementptr inbounds i32, i32* %8, i32 %7
  store i32 %5, i32* %9, align 4
  %10 = alloca i32, align 4
  store i32 0, i32* %10, align 4
  %11 = load i32, i32* %10, align 4
  %12 = load i32*, i32** %nums1, align 8
  %13 = getelementptr inbounds i32, i32* %12, i32 %11
  %14 = load i32, i32* %13, align 4
  %15 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @0, i32 0, i32 0), i32 %14)
  %16 = alloca i32, align 4
  store i32 1, i32* %16, align 4
  %17 = load i32, i32* %16, align 4
  %18 = load i32*, i32** %nums1, align 8
  %19 = getelementptr inbounds i32, i32* %18, i32 %17
  %20 = load i32, i32* %19, align 4
  %21 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @1, i32 0, i32 0), i32 %20)
  %22 = alloca i32, align 4
  store i32 2, i32* %22, align 4
  %23 = load i32, i32* %22, align 4
  %24 = load i32*, i32** %nums1, align 8
  %25 = getelementptr inbounds i32, i32* %24, i32 %23
  %26 = load i32, i32* %25, align 4
  %27 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @2, i32 0, i32 0), i32 %26)
  %28 = alloca i32, align 4
  store i32 3, i32* %28, align 4
  %29 = load i32, i32* %28, align 4
  %30 = load i32*, i32** %nums1, align 8
  %31 = getelementptr inbounds i32, i32* %30, i32 %29
  %32 = load i32, i32* %31, align 4
  %33 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @3, i32 0, i32 0), i32 %32)
  ret void
}

define i32 @main() {
entry:
  %result = alloca i32, align 4
  store i32 0, i32* %result, align 4
  %intArray = alloca [4 x i32], align 4
  %0 = alloca i32, align 4
  store i32 1, i32* %0, align 4
  %1 = alloca i32, align 4
  store i32 2, i32* %1, align 4
  %2 = alloca i32, align 4
  store i32 3, i32* %2, align 4
  %3 = alloca i32, align 4
  store i32 4, i32* %3, align 4
  %4 = alloca i32, align 4
  store i32 1, i32* %4, align 4
  %5 = load i32, i32* %4, align 4
  %6 = getelementptr inbounds [4 x i32], [4 x i32]* @intArray, i32 0, i32 %5
  %7 = load i32, i32* %6, align 4
  %8 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @4, i32 0, i32 0), i32 %7)
  %9 = alloca [4 x i32]*, align 8
  store [4 x i32]* @intArray, [4 x i32]** %9, align 8
  %10 = alloca [4 x i32]**, align 8
  store [4 x i32]** %9, [4 x i32]*** %10, align 8
  %11 = load [4 x i32]**, [4 x i32]*** %10, align 8
  %12 = load [4 x i32]*, [4 x i32]** %11, align 8
  %13 = getelementptr inbounds [4 x i32], [4 x i32]* %12, i32 0, i32 0
  %14 = alloca i32*, align 8
  store i32* %13, i32** %14, align 8
  %15 = alloca i32**, align 8
  store i32** %14, i32*** %15, align 8
  call void @"testProc(int[]**)"(i32*** %15)
  %16 = alloca i1, align 1
  store i1 true, i1* %16, align 1
  %17 = alloca i8*, align 8
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @5, i32 0, i32 0), i8** %17, align 8
  %18 = load i8*, i8** %17, align 8
  %test = alloca i8*, align 8
  store i8* %18, i8** %test, align 8
  %19 = alloca i32, align 4
  store i32 2, i32* %19, align 4
  %20 = load i32, i32* %19, align 4
  %21 = load i8*, i8** %test, align 8
  %22 = getelementptr inbounds i8, i8* %21, i32 %20
  %23 = load i8, i8* %22, align 1
  %c1 = alloca i8, align 1
  store i8 %23, i8* %c1, align 1
  %24 = load i8, i8* %c1, align 1
  %25 = zext i8 %24 to i32
  %26 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @6, i32 0, i32 0), i32 %25)
  %27 = load i32, i32* %result, align 4
  ret i32 %27
}

attributes #0 = { nounwind }
