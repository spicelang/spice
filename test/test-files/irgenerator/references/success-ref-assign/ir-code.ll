; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@anon.string.0 = private unnamed_addr constant [68 x i8] c"Assertion failed: Condition '&test == &testRef' evaluated to false.\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  %test = alloca i32, align 4
  %testRef = alloca i32, align 4
  store i32 0, ptr %result, align 4
  store i32 123, ptr %test, align 4
  %1 = load i32, ptr %test, align 4
  store i32 %1, ptr %testRef, align 4
  %2 = icmp eq ptr %test, %testRef
  br i1 %2, label %assert.exit.L4, label %assert.then.L4

assert.then.L4:                                   ; preds = %0
  %3 = call i32 (ptr, ...) @printf(ptr @anon.string.0)
  call void @exit(i32 1)
  unreachable

assert.exit.L4:                                   ; preds = %0
  %4 = load i32, ptr %result, align 4
  ret i32 %4
}

declare i32 @printf(ptr noundef, ...)

declare void @exit(i32)

attributes #0 = { noinline nounwind optnone uwtable }
