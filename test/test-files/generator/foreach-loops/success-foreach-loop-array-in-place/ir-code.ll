; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@anonymous.0 = constant [5 x i32] [i32 1, i32 2, i32 3, i32 4, i32 5]
@item = constant [5 x i32] [i32 1, i32 2, i32 3, i32 4, i32 5]
@0 = private unnamed_addr constant [13 x i8] c"Item %d: %d\0A\00", align 1

define i32 @main() {
entry.l1:
  %result = alloca i32, align 4
  %0 = alloca [5 x i32], align 4
  %idx = alloca i32, align 4
  %item = alloca i32, align 4
  %item1 = alloca [5 x i32], align 4
  store i32 0, i32* %result, align 4
  store [5 x i32] [i32 1, i32 2, i32 3, i32 4, i32 5], [5 x i32]* %0, align 4
  store i32 0, i32* %idx, align 4
  store i32 0, i32* %item, align 4
  store [5 x i32] [i32 1, i32 2, i32 3, i32 4, i32 5], [5 x i32]* %item1, align 4
  %1 = load [5 x i32], [5 x i32]* %item1, align 4
  %2 = load i32, i32* %idx, align 4
  %3 = getelementptr inbounds [5 x i32], [5 x i32]* %item1, i32 0, i32 %2
  %4 = load i32, i32* %3, align 4
  store i32 %4, i32* %item, align 4
  br label %foreach.loop.l2

foreach.loop.l2:                                  ; preds = %foreach.cond.l2, %entry.l1
  %5 = load i32, i32* %idx, align 4
  %6 = load i32, i32* %item, align 4
  %7 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([13 x i8], [13 x i8]* @0, i32 0, i32 0), i32 %5, i32 %6)
  br label %foreach.inc.l2

foreach.inc.l2:                                   ; preds = %foreach.loop.l2
  %idx2 = load i32, i32* %idx, align 4
  %idx.inc = add i32 %idx2, 1
  store i32 %idx.inc, i32* %idx, align 4
  %8 = getelementptr inbounds [5 x i32], [5 x i32]* %item1, i32 0, i32 %idx.inc
  %9 = load i32, i32* %8, align 4
  store i32 %9, i32* %item, align 4
  br label %foreach.cond.l2

foreach.cond.l2:                                  ; preds = %foreach.inc.l2
  %10 = load i32, i32* %idx, align 4
  %11 = icmp ult i32 %10, 5
  br i1 %11, label %foreach.loop.l2, label %foreach.end.l2

foreach.end.l2:                                   ; preds = %foreach.cond.l2
  %12 = load i32, i32* %result, align 4
  ret i32 %12
}

declare i32 @printf(i8*, ...)
