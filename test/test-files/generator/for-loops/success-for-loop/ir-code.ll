; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [9 x i8] c"Step %d\0A\00", align 1

define i32 @main() {
entry:
  %result = alloca i32, align 4
  %i = alloca i32, align 4
  %0 = alloca i32, align 4
  %1 = alloca i1, align 1
  store i32 0, i32* %result, align 4
  store i32 0, i32* %0, align 4
  br label %for.cond

for:                                              ; preds = %for.cond
  %2 = load i32, i32* %0, align 4
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @0, i32 0, i32 0), i32 %2)
  br label %for.inc

for.inc:                                          ; preds = %for
  %4 = load i32, i32* %0, align 4
  %5 = add i32 %4, 2
  store i32 %5, i32* %0, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %6 = load i32, i32* %0, align 4
  %7 = icmp slt i32 %6, 10
  store i1 %7, i1* %1, align 1
  %8 = load i1, i1* %1, align 1
  br i1 %8, label %for, label %for.end

for.end:                                          ; preds = %for.cond
  %9 = load i32, i32* %result, align 4
  ret i32 %9
}

declare i32 @printf(i8*, ...)
