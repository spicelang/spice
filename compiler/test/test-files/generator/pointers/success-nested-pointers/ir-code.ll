; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [7 x i8] c"1: %d\0A\00", align 1
@1 = private unnamed_addr constant [7 x i8] c"2: %d\0A\00", align 1
@2 = private unnamed_addr constant [7 x i8] c"3: %d\0A\00", align 1
@3 = private unnamed_addr constant [7 x i8] c"4: %d\0A\00", align 1
@intArray = dso_local constant [4 x i32] [i32 1, i32 2, i32 3, i32 4]
@4 = private unnamed_addr constant [7 x i8] c"1: %d\0A\00", align 1

declare i32 @printf(i8*, ...)

; Function Attrs: nounwind
define void @"testProc(int[]***)"(i32**** %0) #0 {
entry:
  %nums = alloca i32****, align 8
  %nums1 = alloca i32**, align 8
  %nums2 = alloca i32*, align 8
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  %5 = alloca i32, align 4
  %6 = alloca i32, align 4
  store i32**** %0, i32***** %nums, align 8
  %7 = load i32****, i32***** %nums, align 8
  %8 = load i32***, i32**** %7, align 8
  %9 = load i32**, i32*** %8, align 8
  store i32** %9, i32*** %8, align 8
  %10 = load i32**, i32*** %8, align 8
  store i32** %10, i32*** %nums1, align 8
  %11 = load i32**, i32*** %nums1, align 8
  %12 = load i32*, i32** %11, align 8
  store i32* %12, i32** %11, align 8
  %13 = load i32*, i32** %11, align 8
  store i32* %13, i32** %nums2, align 8
  store i32 10, i32* %1, align 4
  %14 = load i32, i32* %1, align 4
  %15 = load i32*, i32** %nums2, align 8
  store i32 2, i32* %2, align 4
  %16 = load i32, i32* %2, align 4
  %17 = getelementptr inbounds i32, i32* %15, i32 %16
  store i32 %14, i32* %17, align 4
  %18 = load i32*, i32** %nums2, align 8
  store i32 0, i32* %3, align 4
  %19 = load i32, i32* %3, align 4
  %20 = getelementptr inbounds i32, i32* %18, i32 %19
  %21 = load i32, i32* %20, align 4
  %22 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @0, i32 0, i32 0), i32 %21)
  %23 = load i32*, i32** %nums2, align 8
  store i32 1, i32* %4, align 4
  %24 = load i32, i32* %4, align 4
  %25 = getelementptr inbounds i32, i32* %23, i32 %24
  %26 = load i32, i32* %25, align 4
  %27 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @1, i32 0, i32 0), i32 %26)
  %28 = load i32*, i32** %nums2, align 8
  store i32 2, i32* %5, align 4
  %29 = load i32, i32* %5, align 4
  %30 = getelementptr inbounds i32, i32* %28, i32 %29
  %31 = load i32, i32* %30, align 4
  %32 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @2, i32 0, i32 0), i32 %31)
  %33 = load i32*, i32** %nums2, align 8
  store i32 3, i32* %6, align 4
  %34 = load i32, i32* %6, align 4
  %35 = getelementptr inbounds i32, i32* %33, i32 %34
  %36 = load i32, i32* %35, align 4
  %37 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @3, i32 0, i32 0), i32 %36)
  ret void
}

define i32 @main() {
entry:
  %result = alloca i32, align 4
  %intArray = alloca [4 x i32], align 4
  %0 = alloca i32, align 4
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  %5 = alloca [4 x i32]*, align 8
  %6 = alloca [4 x i32]**, align 8
  %7 = alloca [4 x i32]***, align 8
  %8 = alloca i32*, align 8
  %9 = alloca i32**, align 8
  %10 = alloca i32***, align 8
  %11 = alloca i1, align 1
  store i32 0, i32* %result, align 4
  store i32 1, i32* %0, align 4
  store i32 2, i32* %1, align 4
  store i32 3, i32* %2, align 4
  store i32 4, i32* %3, align 4
  %12 = load [4 x i32], [4 x i32]* @intArray, align 4
  store [4 x i32] %12, [4 x i32]* %intArray, align 4
  %13 = load [4 x i32], [4 x i32]* %intArray, align 4
  store i32 1, i32* %4, align 4
  %14 = load i32, i32* %4, align 4
  %15 = getelementptr inbounds [4 x i32], [4 x i32]* %intArray, i32 0, i32 %14
  %16 = load i32, i32* %15, align 4
  %17 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @4, i32 0, i32 0), i32 %16)
  %18 = load [4 x i32], [4 x i32]* %intArray, align 4
  store [4 x i32]* %intArray, [4 x i32]** %5, align 8
  store [4 x i32]** %5, [4 x i32]*** %6, align 8
  store [4 x i32]*** %6, [4 x i32]**** %7, align 8
  store [4 x i32]*** %6, [4 x i32]**** %7, align 8
  %19 = load [4 x i32]***, [4 x i32]**** %7, align 8
  %20 = load [4 x i32]**, [4 x i32]*** %19, align 8
  %21 = load [4 x i32]*, [4 x i32]** %20, align 8
  %22 = getelementptr inbounds [4 x i32], [4 x i32]* %21, i32 0, i32 0
  store i32* %22, i32** %8, align 8
  store i32** %8, i32*** %9, align 8
  store i32*** %9, i32**** %10, align 8
  call void @"testProc(int[]***)"(i32**** %10)
  store i1 true, i1* %11, align 1
  %23 = load i32, i32* %result, align 4
  ret i32 %23
}

attributes #0 = { nounwind }
