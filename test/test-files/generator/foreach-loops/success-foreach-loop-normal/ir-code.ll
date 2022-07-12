; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@intArray = constant [7 x i32] [i32 1, i32 5, i32 4, i32 0, i32 12, i32 12345, i32 9]
@0 = private unnamed_addr constant [22 x i8] c"Item at index %d: %d\0A\00", align 1

define i32 @main() {
entry:
  %result = alloca i32, align 4
  %intArray = alloca [7 x i32], align 4
  %idx = alloca i32, align 4
  %item = alloca i32, align 4
  store i32 0, i32* %result, align 4
  store [7 x i32] [i32 1, i32 5, i32 4, i32 0, i32 12, i32 12345, i32 9], [7 x i32]* %intArray, align 4
  store i32 0, i32* %idx, align 4
  store i32 0, i32* %item, align 4
  %0 = load [7 x i32], [7 x i32]* %intArray, align 4
  %1 = load i32, i32* %idx, align 4
  %2 = getelementptr inbounds [7 x i32], [7 x i32]* %intArray, i32 0, i32 %1
  %3 = load i32, i32* %2, align 4
  store i32 %3, i32* %item, align 4
  br label %foreach.loop

foreach.loop:                                     ; preds = %foreach.cond, %entry
  %4 = load i32, i32* %idx, align 4
  %5 = load i32, i32* %item, align 4
  %6 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([22 x i8], [22 x i8]* @0, i32 0, i32 0), i32 %4, i32 %5)
  br label %foreach.inc

foreach.inc:                                      ; preds = %foreach.loop
  %idx1 = load i32, i32* %idx, align 4
  %idx.inc = add i32 %idx1, 1
  store i32 %idx.inc, i32* %idx, align 4
  %7 = getelementptr inbounds [7 x i32], [7 x i32]* %intArray, i32 0, i32 %idx.inc
  %8 = load i32, i32* %7, align 4
  store i32 %8, i32* %item, align 4
  br label %foreach.cond

foreach.cond:                                     ; preds = %foreach.inc
  %9 = load i32, i32* %idx, align 4
  %10 = icmp ult i32 %9, 7
  br i1 %10, label %foreach.loop, label %foreach.end

foreach.end:                                      ; preds = %foreach.cond
  %11 = load i32, i32* %result, align 4
  ret i32 %11
}

declare i32 @printf(i8*, ...)
