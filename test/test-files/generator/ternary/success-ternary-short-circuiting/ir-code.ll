; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [11 x i8] c"Result: %d\00", align 1

define internal i1 @_f__void__condition1() {
entry.l1:
  %result = alloca i1, align 1
  %0 = alloca i1, align 1
  store i1 false, i1* %0, align 1
  %1 = load i1, i1* %0, align 1
  ret i1 %1
}

define internal i1 @_f__void__condition2() {
entry.l5:
  %result = alloca i1, align 1
  %0 = alloca i1, align 1
  store i1 true, i1* %0, align 1
  %1 = load i1, i1* %0, align 1
  ret i1 %1
}

define i32 @main() {
entry.l9:
  %result = alloca i32, align 4
  %0 = alloca i1, align 1
  %1 = alloca i1, align 1
  %2 = alloca i1, align 1
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  store i32 0, i32* %result, align 4
  %5 = call i1 @_f__void__condition1()
  store i1 %5, i1* %0, align 1
  %6 = load i1, i1* %0, align 1
  br i1 %6, label %land.1.l10, label %land.end.l10

land.1.l10:                                       ; preds = %entry.l9
  %7 = call i1 @_f__void__condition2()
  store i1 %7, i1* %1, align 1
  %8 = load i1, i1* %1, align 1
  br label %land.end.l10

land.end.l10:                                     ; preds = %land.1.l10, %entry.l9
  %land_phi = phi i1 [ %6, %entry.l9 ], [ %8, %land.1.l10 ]
  store i1 %land_phi, i1* %2, align 1
  store i32 2, i32* %3, align 4
  store i32 3, i32* %4, align 4
  %9 = load i1, i1* %2, align 1
  %10 = select i1 %9, i32* %3, i32* %4
  %11 = load i32, i32* %10, align 4
  %12 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @0, i32 0, i32 0), i32 %11)
  %13 = load i32, i32* %result, align 4
  ret i32 %13
}

declare i32 @printf(i8*, ...)
