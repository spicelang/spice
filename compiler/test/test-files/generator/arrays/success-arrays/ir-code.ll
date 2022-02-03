; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [23 x i8] c"Item 0: %d, item 2: %d\00", align 1

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
  store i32 0, i32* %result, align 4
  store i32 2, i32* %0, align 4
  %10 = load i32, i32* %0, align 4
  store i32 %10, i32* %value0, align 4
  store i32 7, i32* %2, align 4
  store i32 4, i32* %3, align 4
  %11 = load i32, i32* %value0, align 4
  %12 = getelementptr inbounds [5 x i32], [5 x i32]* %1, i32 0, i32 0
  store i32 %11, i32* %12, align 4
  %13 = load i32, i32* %2, align 4
  %14 = getelementptr inbounds [5 x i32], [5 x i32]* %1, i32 0, i32 1
  store i32 %13, i32* %14, align 4
  %15 = load i32, i32* %3, align 4
  %16 = getelementptr inbounds [5 x i32], [5 x i32]* %1, i32 0, i32 2
  store i32 %15, i32* %16, align 4
  %17 = load [5 x i32], [5 x i32]* %1, align 4
  store [5 x i32] %17, [5 x i32]* %intArray, align 4
  store i32 11, i32* %4, align 4
  %18 = load i32, i32* %4, align 4
  %19 = load [5 x i32], [5 x i32]* %intArray, align 4
  store i32 2, i32* %5, align 4
  %20 = load i32, i32* %5, align 4
  %21 = getelementptr inbounds [5 x i32], [5 x i32]* %intArray, i32 0, i32 %20
  store i32 %18, i32* %21, align 4
  store i32 3, i32* %6, align 4
  %22 = load i32, i32* %6, align 4
  %23 = load [5 x i32], [5 x i32]* %intArray, align 4
  store i32 0, i32* %7, align 4
  %24 = load i32, i32* %7, align 4
  %25 = getelementptr inbounds [5 x i32], [5 x i32]* %intArray, i32 0, i32 %24
  store i32 %22, i32* %25, align 4
  %26 = load [5 x i32], [5 x i32]* %intArray, align 4
  store i32 0, i32* %8, align 4
  %27 = load i32, i32* %8, align 4
  %28 = getelementptr inbounds [5 x i32], [5 x i32]* %intArray, i32 0, i32 %27
  %29 = load i32, i32* %28, align 4
  %30 = load [5 x i32], [5 x i32]* %intArray, align 4
  store i32 2, i32* %9, align 4
  %31 = load i32, i32* %9, align 4
  %32 = getelementptr inbounds [5 x i32], [5 x i32]* %intArray, i32 0, i32 %31
  %33 = load i32, i32* %32, align 4
  %34 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([23 x i8], [23 x i8]* @0, i32 0, i32 0), i32 %29, i32 %33)
  %35 = load i32, i32* %result, align 4
  ret i32 %35
}
