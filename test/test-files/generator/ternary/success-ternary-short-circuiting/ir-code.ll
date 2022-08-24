; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@0 = private unnamed_addr constant [11 x i8] c"Result: %d\00", align 1

define internal i1 @_f__void__condition1() {
entry.l1:
  %result = alloca i1, align 1
  %0 = alloca i1, align 1
  store i1 false, ptr %0, align 1
  %1 = load i1, ptr %0, align 1
  ret i1 %1
}

define internal i1 @_f__void__condition2() {
entry.l5:
  %result = alloca i1, align 1
  %0 = alloca i1, align 1
  store i1 true, ptr %0, align 1
  %1 = load i1, ptr %0, align 1
  ret i1 %1
}

define i32 @main() {
entry.l9:
  %result = alloca i32, align 4
  store i32 0, ptr %result, align 4
  %0 = call i1 @_f__void__condition1()
  %1 = alloca i1, align 1
  store i1 %0, ptr %1, align 1
  %2 = load i1, ptr %1, align 1
  br i1 %2, label %land.1.l10, label %land.end.l10

land.1.l10:                                       ; preds = %entry.l9
  %3 = call i1 @_f__void__condition2()
  %4 = alloca i1, align 1
  store i1 %3, ptr %4, align 1
  %5 = load i1, ptr %4, align 1
  br label %land.end.l10

land.end.l10:                                     ; preds = %land.1.l10, %entry.l9
  %land_phi = phi i1 [ %2, %entry.l9 ], [ %5, %land.1.l10 ]
  %6 = alloca i1, align 1
  store i1 %land_phi, ptr %6, align 1
  %7 = alloca i32, align 4
  store i32 2, ptr %7, align 4
  %8 = alloca i32, align 4
  store i32 3, ptr %8, align 4
  %9 = load i1, ptr %6, align 1
  %10 = select i1 %9, ptr %7, ptr %8
  %11 = load i32, ptr %10, align 4
  %12 = call i32 (ptr, ...) @printf(ptr @0, i32 %11)
  %13 = load i32, ptr %result, align 4
  ret i32 %13
}

declare i32 @printf(ptr, ...)
