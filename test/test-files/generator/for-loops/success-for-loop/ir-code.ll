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

for.cond:                                         ; preds = %for.post, %entry
  %1 = load i32, i32* %i, align 4
  %2 = icmp slt i32 %1, 10
  store i1 %2, i1* %0, align 1
  %3 = load i1, i1* %0, align 1
  br i1 %3, label %for, label %for.end

for:                                              ; preds = %for.cond
  %4 = load i32, i32* %i, align 4
  %5 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @0, i32 0, i32 0), i32 %4)
  br label %for.post

for.post:                                         ; preds = %for
  %6 = load i32, i32* %i, align 4
  %7 = add i32 %6, 2
  store i32 %7, i32* %i, align 4
  br label %for.cond

for.end:                                          ; preds = %for.cond
  %8 = load i32, i32* %result, align 4
  ret i32 %8
}

declare i32 @printf(i8*, ...)
