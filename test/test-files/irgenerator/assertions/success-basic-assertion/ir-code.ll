; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [55 x i8] c"Assertion failed: Condition '1!=1' evaluated to false.\00", align 1
@1 = private unnamed_addr constant [12 x i8] c"Unreachable\00", align 1

define i32 @main() {
entry.l1:
  %result = alloca i32, align 4
  %0 = alloca i32, align 4
  %1 = alloca i1, align 1
  store i32 0, ptr %result, align 4
  store i32 1, ptr %0, align 4
  %2 = load i32, ptr %0, align 4
  %3 = icmp ne i32 %2, 1
  store i1 %3, ptr %1, align 1
  %4 = load i1, ptr %1, align 1
  br i1 %4, label %assert.end.l2, label %assert.then.l2

assert.then.l2:                                   ; preds = %entry.l1
  %5 = call i32 (ptr, ...) @printf(ptr @0)
  call void @exit(i32 0)
  unreachable

assert.end.l2:                                    ; preds = %entry.l1
  %6 = call i32 (ptr, ...) @printf(ptr @1)
  %7 = load i32, ptr %result, align 4
  ret i32 %7
}

declare i32 @printf(ptr, ...)

declare void @exit(i32)