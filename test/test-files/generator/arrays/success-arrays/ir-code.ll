; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [23 x i8] c"Item 0: %d, item 2: %d\00", align 1

define i32 @main() {
entry:
  %result = alloca i32, align 4
  %value0 = alloca i32, align 4
  %0 = alloca i32, align 4
  %intArray = alloca [5 x i32], align 4
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  %3 = alloca [5 x i32], align 4
  %4 = alloca i32, align 4
  %5 = alloca i32, align 4
  %6 = alloca i32, align 4
  %7 = alloca i32, align 4
  %8 = alloca i32, align 4
  %9 = alloca i32, align 4
  store i32 0, i32* %result, align 4
  store i32 2, i32* %0, align 4
  %10 = load i32, i32* %0, align 4
  store i32 %10, i32* %value0, align 4
  store i32 7, i32* %1, align 4
  store i32 4, i32* %2, align 4
  %11 = load i32, i32* %value0, align 4
  %12 = getelementptr inbounds [5 x i32], [5 x i32]* %3, i32 0, i32 0
  store i32 %11, i32* %12, align 4
  %13 = load i32, i32* %1, align 4
  %14 = getelementptr inbounds [5 x i32], [5 x i32]* %3, i32 0, i32 1
  store i32 %13, i32* %14, align 4
  %15 = load i32, i32* %2, align 4
  %16 = getelementptr inbounds [5 x i32], [5 x i32]* %3, i32 0, i32 2
  store i32 %15, i32* %16, align 4
  %17 = load [5 x i32], [5 x i32]* %3, align 4
  store [5 x i32] %17, [5 x i32]* %intArray, align 4
  store i32 11, i32* %4, align 4
  %18 = load i32, i32* %4, align 4
  %19 = load [5 x i32], [5 x i32]* %intArray, align 4
  store i32 2, i32* %5, align 4
  %20 = load i32, i32* %5, align 4
  %21 = getelementptr inbounds [5 x i32], [5 x i32]* %intArray, i32 0, i32 %20
  %22 = load i32, i32* %21, align 4
  %23 = mul i32 %22, %18
  store i32 %23, i32* %21, align 4
  store i32 3, i32* %6, align 4
  %24 = load i32, i32* %6, align 4
  %25 = load [5 x i32], [5 x i32]* %intArray, align 4
  store i32 0, i32* %7, align 4
  %26 = load i32, i32* %7, align 4
  %27 = getelementptr inbounds [5 x i32], [5 x i32]* %intArray, i32 0, i32 %26
  store i32 %24, i32* %27, align 4
  %28 = load [5 x i32], [5 x i32]* %intArray, align 4
  store i32 0, i32* %8, align 4
  %29 = load i32, i32* %8, align 4
  %30 = getelementptr inbounds [5 x i32], [5 x i32]* %intArray, i32 0, i32 %29
  %31 = load i32, i32* %30, align 4
  %32 = load [5 x i32], [5 x i32]* %intArray, align 4
  store i32 2, i32* %9, align 4
  %33 = load i32, i32* %9, align 4
  %34 = getelementptr inbounds [5 x i32], [5 x i32]* %intArray, i32 0, i32 %33
  %35 = load i32, i32* %34, align 4
  %36 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([23 x i8], [23 x i8]* @0, i32 0, i32 0), i32 %31, i32 %35)
  %37 = load i32, i32* %result, align 4
  ret i32 %37
}

declare i32 @printf(i8*, ...)
