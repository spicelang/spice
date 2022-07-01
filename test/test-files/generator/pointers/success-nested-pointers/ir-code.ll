; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [7 x i8] c"1: %d\0A\00", align 1
@1 = private unnamed_addr constant [7 x i8] c"2: %d\0A\00", align 1
@2 = private unnamed_addr constant [7 x i8] c"3: %d\0A\00", align 1
@3 = private unnamed_addr constant [7 x i8] c"4: %d\0A\00", align 1
@intArray = constant [4 x i32] [i32 1, i32 2, i32 3, i32 4]
@4 = private unnamed_addr constant [7 x i8] c"1: %d\0A\00", align 1

define internal void @_p__void__testProc__intarrayptrptrptr(i32**** %0) {
entry:
  %nums = alloca i32****, align 8
  %nums1 = alloca i32**, align 8
  %nums2 = alloca i32*, align 8
  store i32**** %0, i32***** %nums, align 8
  %1 = load i32****, i32***** %nums, align 8
  %2 = load i32***, i32**** %1, align 8
  %3 = load i32**, i32*** %2, align 8
  store i32** %3, i32*** %2, align 8
  %4 = load i32**, i32*** %2, align 8
  store i32** %4, i32*** %nums1, align 8
  %5 = load i32**, i32*** %nums1, align 8
  %6 = load i32*, i32** %5, align 8
  store i32* %6, i32** %5, align 8
  %7 = load i32*, i32** %5, align 8
  store i32* %7, i32** %nums2, align 8
  %8 = load i32*, i32** %nums2, align 8
  %9 = getelementptr inbounds i32, i32* %8, i32 2
  store i32 10, i32* %9, align 4
  %10 = load i32*, i32** %nums2, align 8
  %11 = getelementptr inbounds i32, i32* %10, i32 0
  %12 = load i32, i32* %11, align 4
  %13 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @0, i32 0, i32 0), i32 %12)
  %14 = load i32*, i32** %nums2, align 8
  %15 = getelementptr inbounds i32, i32* %14, i32 1
  %16 = load i32, i32* %15, align 4
  %17 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @1, i32 0, i32 0), i32 %16)
  %18 = load i32*, i32** %nums2, align 8
  %19 = getelementptr inbounds i32, i32* %18, i32 2
  %20 = load i32, i32* %19, align 4
  %21 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @2, i32 0, i32 0), i32 %20)
  %22 = load i32*, i32** %nums2, align 8
  %23 = getelementptr inbounds i32, i32* %22, i32 3
  %24 = load i32, i32* %23, align 4
  %25 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @3, i32 0, i32 0), i32 %24)
  ret void
}

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %result = alloca i32, align 4
  %intArray = alloca [4 x i32], align 4
  %0 = alloca i32, align 4
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  %4 = alloca [4 x i32]*, align 8
  %5 = alloca [4 x i32]**, align 8
  %6 = alloca [4 x i32]***, align 8
  %7 = alloca i32*, align 8
  %8 = alloca i32**, align 8
  %9 = alloca i32***, align 8
  %10 = alloca i1, align 1
  store i32 0, i32* %result, align 4
  store i32 1, i32* %0, align 4
  store i32 2, i32* %1, align 4
  store i32 3, i32* %2, align 4
  store i32 4, i32* %3, align 4
  %11 = load [4 x i32], [4 x i32]* @intArray, align 4
  store [4 x i32] %11, [4 x i32]* %intArray, align 4
  %12 = load [4 x i32], [4 x i32]* %intArray, align 4
  %13 = getelementptr inbounds [4 x i32], [4 x i32]* %intArray, i32 0, i32 1
  %14 = load i32, i32* %13, align 4
  %15 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @4, i32 0, i32 0), i32 %14)
  %16 = load [4 x i32], [4 x i32]* %intArray, align 4
  store [4 x i32]* %intArray, [4 x i32]** %4, align 8
  store [4 x i32]** %4, [4 x i32]*** %5, align 8
  store [4 x i32]*** %5, [4 x i32]**** %6, align 8
  store [4 x i32]*** %5, [4 x i32]**** %6, align 8
  %17 = load [4 x i32]***, [4 x i32]**** %6, align 8
  %18 = load [4 x i32]**, [4 x i32]*** %17, align 8
  %19 = load [4 x i32]*, [4 x i32]** %18, align 8
  %20 = getelementptr inbounds [4 x i32], [4 x i32]* %19, i32 0, i32 0
  store i32* %20, i32** %7, align 8
  store i32** %7, i32*** %8, align 8
  store i32*** %8, i32**** %9, align 8
  call void @_p__void__testProc__intarrayptrptrptr(i32**** %9)
  store i1 true, i1* %10, align 1
  %21 = load i32, i32* %result, align 4
  ret i32 %21
}
