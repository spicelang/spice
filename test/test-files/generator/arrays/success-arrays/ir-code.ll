; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@0 = private unnamed_addr constant [23 x i8] c"Item 0: %d, item 2: %d\00", align 1

define i32 @main() {
entry.l1:
  %result = alloca i32, align 4
  store i32 0, ptr %result, align 4
  %value0 = alloca i32, align 4
  store i32 2, ptr %value0, align 4
  %0 = load i32, ptr %value0, align 4
  %1 = alloca [5 x i32], align 4
  %2 = getelementptr inbounds [5 x i32], ptr %1, i32 0, i32 0
  store i32 %0, ptr %2, align 4
  %3 = getelementptr inbounds [5 x i32], ptr %1, i32 0, i32 1
  store i32 7, ptr %3, align 4
  %4 = getelementptr inbounds [5 x i32], ptr %1, i32 0, i32 2
  store i32 4, ptr %4, align 4
  %5 = getelementptr inbounds [5 x i32], ptr %1, i32 0, i32 3
  store i32 0, ptr %5, align 4
  %6 = getelementptr inbounds [5 x i32], ptr %1, i32 0, i32 4
  store i32 0, ptr %6, align 4
  %7 = getelementptr inbounds [5 x i32], ptr %1, i32 0, i32 2
  %8 = load i32, ptr %7, align 4
  %9 = mul i32 %8, 11
  store i32 %9, ptr %7, align 4
  %10 = getelementptr inbounds [5 x i32], ptr %1, i32 0, i32 0
  store i32 3, ptr %10, align 4
  %11 = getelementptr inbounds [5 x i32], ptr %1, i32 0, i32 0
  %12 = load i32, ptr %11, align 4
  %13 = getelementptr inbounds [5 x i32], ptr %1, i32 0, i32 2
  %14 = load i32, ptr %13, align 4
  %15 = call i32 (ptr, ...) @printf(ptr @0, i32 %12, i32 %14)
  %16 = load i32, ptr %result, align 4
  ret i32 %16
}

declare i32 @printf(ptr, ...)
