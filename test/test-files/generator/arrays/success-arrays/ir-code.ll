; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [23 x i8] c"Item 0: %d, item 2: %d\00", align 1

define i32 @main() {
entry:
  %result = alloca i32, align 4
  %value0 = alloca i32, align 4
  %intArray = alloca [5 x i32], align 4
  store i32 0, i32* %result, align 4
  store i32 2, i32* %value0, align 4
  %0 = load i32, i32* %value0, align 4
  %1 = getelementptr inbounds [5 x i32], [5 x i32]* %intArray, i32 0, i32 0
  store i32 %0, i32* %1, align 4
  %2 = getelementptr inbounds [5 x i32], [5 x i32]* %intArray, i32 0, i32 1
  store i32 7, i32* %2, align 4
  %3 = getelementptr inbounds [5 x i32], [5 x i32]* %intArray, i32 0, i32 2
  store i32 4, i32* %3, align 4
  %4 = getelementptr inbounds [5 x i32], [5 x i32]* %intArray, i32 0, i32 3
  store i32 0, i32* %4, align 4
  %5 = getelementptr inbounds [5 x i32], [5 x i32]* %intArray, i32 0, i32 4
  store i32 0, i32* %5, align 4
  %6 = load [5 x i32], [5 x i32]* %intArray, align 4
  %7 = getelementptr inbounds [5 x i32], [5 x i32]* %intArray, i32 0, i32 2
  %8 = load i32, i32* %7, align 4
  %9 = mul i32 %8, 11
  store i32 %9, i32* %7, align 4
  %10 = load [5 x i32], [5 x i32]* %intArray, align 4
  %11 = getelementptr inbounds [5 x i32], [5 x i32]* %intArray, i32 0, i32 0
  store i32 3, i32* %11, align 4
  %12 = load [5 x i32], [5 x i32]* %intArray, align 4
  %13 = getelementptr inbounds [5 x i32], [5 x i32]* %intArray, i32 0, i32 0
  %14 = load i32, i32* %13, align 4
  %15 = load [5 x i32], [5 x i32]* %intArray, align 4
  %16 = getelementptr inbounds [5 x i32], [5 x i32]* %intArray, i32 0, i32 2
  %17 = load i32, i32* %16, align 4
  %18 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([23 x i8], [23 x i8]* @0, i32 0, i32 0), i32 %14, i32 %17)
  %19 = load i32, i32* %result, align 4
  ret i32 %19
}

declare i32 @printf(i8*, ...)
