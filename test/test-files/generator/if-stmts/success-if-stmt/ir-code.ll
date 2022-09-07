; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [16 x i8] c"Condition true\0A\00", align 1
@1 = private unnamed_addr constant [17 x i8] c"Condition false\0A\00", align 1

define i32 @main() {
entry.l1:
  %result = alloca i32, align 4
  %0 = alloca i1, align 1
  %1 = alloca i32, align 4
  store i32 0, ptr %result, align 4
  store i1 true, ptr %0, align 1
  %2 = load i1, ptr %0, align 1
  br i1 %2, label %if.then.l3, label %if.end.l3

if.then.l3:                                       ; preds = %entry.l1
  %3 = call i32 (ptr, ...) @printf(ptr @0)
  store i32 0, ptr %1, align 4
  %4 = load i32, ptr %1, align 4
  ret i32 %4

if.end.l3:                                        ; preds = %entry.l1
  %5 = call i32 (ptr, ...) @printf(ptr @1)
  %6 = load i32, ptr %result, align 4
  ret i32 %6
}

declare i32 @printf(ptr, ...)
