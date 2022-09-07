; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@anonymous.0 = global [5 x i32] [i32 1, i32 2, i32 3, i32 4, i32 5]
@0 = private unnamed_addr constant [13 x i8] c"Item %d: %d\0A\00", align 1

define i32 @main() {
entry.l1:
  %result = alloca i32, align 4
  %idx = alloca i32, align 4
  %item = alloca i32, align 4
  store i32 0, ptr %result, align 4
  store i32 0, ptr %idx, align 4
  store i32 0, ptr %item, align 4
  %0 = load [5 x i32], ptr @anonymous.0, align 4
  %1 = load i32, ptr %idx, align 4
  %2 = getelementptr inbounds [5 x i32], ptr @anonymous.0, i32 0, i32 %1
  %3 = load i32, ptr %2, align 4
  store i32 %3, ptr %item, align 4
  br label %foreach.loop.l2

foreach.loop.l2:                                  ; preds = %foreach.cond.l2, %entry.l1
  %4 = load i32, ptr %idx, align 4
  %5 = load i32, ptr %item, align 4
  %6 = call i32 (ptr, ...) @printf(ptr @0, i32 %4, i32 %5)
  br label %foreach.inc.l2

foreach.inc.l2:                                   ; preds = %foreach.loop.l2
  %idx1 = load i32, ptr %idx, align 4
  %idx.inc = add i32 %idx1, 1
  store i32 %idx.inc, ptr %idx, align 4
  %7 = getelementptr inbounds [5 x i32], ptr @anonymous.0, i32 0, i32 %idx.inc
  %8 = load i32, ptr %7, align 4
  store i32 %8, ptr %item, align 4
  br label %foreach.cond.l2

foreach.cond.l2:                                  ; preds = %foreach.inc.l2
  %9 = load i32, ptr %idx, align 4
  %10 = icmp ult i32 %9, 5
  br i1 %10, label %foreach.loop.l2, label %foreach.end.l2

foreach.end.l2:                                   ; preds = %foreach.cond.l2
  %11 = load i32, ptr %result, align 4
  ret i32 %11
}

declare i32 @printf(ptr, ...)
