; ModuleID = 'os-test.spice'
source_filename = "os-test.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@item = dso_local constant [5 x i32] [i32 1, i32 2, i32 3, i32 4, i32 5]
@0 = private unnamed_addr constant [13 x i8] c"Item %d: %d\0A\00", align 1

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %result = alloca i32, align 4
  %idx = alloca i32, align 4
  %item = alloca i32, align 4
  %0 = alloca i32, align 4
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  store i32 0, i32* %result, align 4
  store i32 0, i32* %idx, align 4
  store i32 1, i32* %0, align 4
  store i32 2, i32* %1, align 4
  store i32 3, i32* %2, align 4
  store i32 4, i32* %3, align 4
  store i32 5, i32* %4, align 4
  %5 = load [5 x i32], [5 x i32]* @item, align 4
  %6 = load i32, i32* %idx, align 4
  %7 = getelementptr inbounds [5 x i32], [5 x i32]* @item, i32 0, i32 %6
  %8 = load i32, i32* %7, align 4
  store i32 %8, i32* %item, align 4
  br label %foreach.loop

foreach.loop:                                     ; preds = %foreach.inc, %entry
  %9 = load i32, i32* %idx, align 4
  %10 = load i32, i32* %item, align 4
  %11 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([13 x i8], [13 x i8]* @0, i32 0, i32 0), i32 %9, i32 %10)
  %12 = load i32, i32* %idx, align 4
  %foreach_idx_cmp = icmp slt i32 %12, 4
  br i1 %foreach_idx_cmp, label %foreach.inc, label %foreach.end

foreach.inc:                                      ; preds = %foreach.loop
  %13 = load i32, i32* %idx, align 4
  %foreach_idx_inc = add i32 %13, 1
  store i32 %foreach_idx_inc, i32* %idx, align 4
  %14 = getelementptr inbounds [5 x i32], [5 x i32]* @item, i32 0, i32 %foreach_idx_inc
  %15 = load i32, i32* %14, align 4
  store i32 %15, i32* %item, align 4
  br label %foreach.loop

foreach.end:                                      ; preds = %foreach.loop
  %16 = load i32, i32* %result, align 4
  ret i32 %16
}
