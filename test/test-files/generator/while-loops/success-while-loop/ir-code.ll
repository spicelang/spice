; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [17 x i8] c"i is now at: %d\0A\00", align 1

define i32 @main() {
entry.l1:
  %result = alloca i32, align 4
  store i32 0, ptr %result, align 4
  %i = alloca i32, align 4
  store i32 0, ptr %i, align 4
  br label %while.cond.l3

while.cond.l3:                                    ; preds = %while.l3, %entry.l1
  %0 = load i32, ptr %i, align 4
  %1 = icmp slt i32 %0, 10
  %2 = alloca i1, align 1
  store i1 %1, ptr %2, align 1
  %3 = load i1, ptr %2, align 1
  br i1 %3, label %while.l3, label %while.end.l3

while.l3:                                         ; preds = %while.cond.l3
  %4 = load i32, ptr %i, align 4
  %5 = add i32 %4, 1
  store i32 %5, ptr %i, align 4
  %6 = load i32, ptr %i, align 4
  %7 = call i32 (ptr, ...) @printf(ptr @0, i32 %6)
  br label %while.cond.l3

while.end.l3:                                     ; preds = %while.cond.l3
  %8 = load i32, ptr %result, align 4
  ret i32 %8
}

declare i32 @printf(ptr, ...)
