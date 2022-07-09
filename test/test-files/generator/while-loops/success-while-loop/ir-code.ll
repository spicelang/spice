; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [17 x i8] c"i is now at: %d\0A\00", align 1

define i32 @main() {
entry:
  %result = alloca i32, align 4
  %i = alloca i32, align 4
  %0 = alloca i32, align 4
  %1 = alloca i1, align 1
  store i32 0, i32* %result, align 4
  store i32 0, i32* %0, align 4
  br label %while.cond

while.cond:                                       ; preds = %while, %entry
  %2 = load i32, i32* %0, align 4
  %3 = icmp slt i32 %2, 10
  store i1 %3, i1* %1, align 1
  %4 = load i1, i1* %1, align 1
  br i1 %4, label %while, label %while.end

while:                                            ; preds = %while.cond
  %5 = load i32, i32* %0, align 4
  %6 = add i32 %5, 1
  store i32 %6, i32* %0, align 4
  %7 = load i32, i32* %0, align 4
  %8 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @0, i32 0, i32 0), i32 %7)
  br label %while.cond

while.end:                                        ; preds = %while.cond
  %9 = load i32, i32* %result, align 4
  ret i32 %9
}

declare i32 @printf(i8*, ...)
