; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [9 x i8] c"Step %d\0A\00", align 1

define i32 @main() {
entry:
  %result = alloca i32, align 4
  %i = alloca i32, align 4
  %0 = alloca i1, align 1
  store i32 0, i32* %result, align 4
  store i32 0, i32* %i, align 4
  br label %for.cond

for:                                              ; preds = %for.cond
  %1 = load i32, i32* %i, align 4
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @0, i32 0, i32 0), i32 %1)
  br label %for.inc

for.inc:                                          ; preds = %for
  %3 = load i32, i32* %i, align 4
  %4 = add i32 %3, 2
  store i32 %4, i32* %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %5 = load i32, i32* %i, align 4
  %6 = icmp slt i32 %5, 10
  store i1 %6, i1* %0, align 1
  %7 = load i1, i1* %0, align 1
  br i1 %7, label %for, label %for.end

for.end:                                          ; preds = %for.cond
  %8 = load i32, i32* %result, align 4
  ret i32 %8
}

declare i32 @printf(i8*, ...)
