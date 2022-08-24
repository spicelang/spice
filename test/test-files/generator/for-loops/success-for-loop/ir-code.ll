; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@0 = private unnamed_addr constant [9 x i8] c"Step %d\0A\00", align 1

define i32 @main() {
entry.l1:
  %result = alloca i32, align 4
  store i32 0, ptr %result, align 4
  %i = alloca i32, align 4
  store i32 0, ptr %i, align 4
  br label %for.cond.l2

for.l2:                                           ; preds = %for.cond.l2
  %0 = load i32, ptr %i, align 4
  %1 = call i32 (ptr, ...) @printf(ptr @0, i32 %0)
  br label %for.inc.l2

for.inc.l2:                                       ; preds = %for.l2
  %2 = load i32, ptr %i, align 4
  %3 = add i32 %2, 2
  store i32 %3, ptr %i, align 4
  br label %for.cond.l2

for.cond.l2:                                      ; preds = %for.inc.l2, %entry.l1
  %4 = load i32, ptr %i, align 4
  %5 = icmp slt i32 %4, 10
  %6 = alloca i1, align 1
  store i1 %5, ptr %6, align 1
  %7 = load i1, ptr %6, align 1
  br i1 %7, label %for.l2, label %for.end.l2

for.end.l2:                                       ; preds = %for.cond.l2
  %8 = load i32, ptr %result, align 4
  ret i32 %8
}

declare i32 @printf(ptr, ...)
