; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [23 x i8] c"Item 0: %d, item 2: %d\00", align 1

define i32 @main() {
entry.l1:
  %result = alloca i32, align 4
  %value0 = alloca i32, align 4
  %0 = alloca [5 x i32], align 4
  %intArray = alloca [5 x i32], align 4
  store i32 0, ptr %result, align 4
  store i32 2, ptr %value0, align 4
  %1 = load i32, ptr %value0, align 4
  %2 = getelementptr inbounds [5 x i32], ptr %0, i32 0, i32 0
  store i32 %1, ptr %2, align 4
  %3 = getelementptr inbounds [5 x i32], ptr %0, i32 0, i32 1
  store i32 7, ptr %3, align 4
  %4 = getelementptr inbounds [5 x i32], ptr %0, i32 0, i32 2
  store i32 4, ptr %4, align 4
  %5 = getelementptr inbounds [5 x i32], ptr %0, i32 0, i32 3
  store i32 0, ptr %5, align 4
  %6 = getelementptr inbounds [5 x i32], ptr %0, i32 0, i32 4
  store i32 0, ptr %6, align 4
  %7 = load [5 x i32], ptr %0, align 4
  store [5 x i32] %7, ptr %intArray, align 4
  %8 = getelementptr inbounds [5 x i32], ptr %intArray, i32 0, i32 2
  %9 = load i32, ptr %8, align 4
  %10 = mul i32 %9, 11
  store i32 %10, ptr %8, align 4
  %11 = getelementptr inbounds [5 x i32], ptr %intArray, i32 0, i32 0
  store i32 3, ptr %11, align 4
  %12 = getelementptr inbounds [5 x i32], ptr %intArray, i32 0, i32 0
  %13 = load i32, ptr %12, align 4
  %14 = getelementptr inbounds [5 x i32], ptr %intArray, i32 0, i32 2
  %15 = load i32, ptr %14, align 4
  %16 = call i32 (ptr, ...) @printf(ptr @0, i32 %13, i32 %15)
  %17 = load i32, ptr %result, align 4
  ret i32 %17
}

declare i32 @printf(ptr, ...)
