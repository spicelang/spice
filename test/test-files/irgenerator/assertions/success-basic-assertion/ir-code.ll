; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@anon.string.0 = private unnamed_addr constant [57 x i8] c"Assertion failed: Condition '1 != 1' evaluated to false.\00", align 1
@printf.str.0 = private unnamed_addr constant [12 x i8] c"Unreachable\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  store i32 0, ptr %result, align 4
  br i1 false, label %assert.exit.L2, label %assert.then.L2

assert.then.L2:                                   ; preds = %0
  %1 = call i32 (ptr, ...) @printf(ptr @anon.string.0)
  call void @exit(i32 1)
  unreachable

assert.exit.L2:                                   ; preds = %0
  %2 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0)
  %3 = load i32, ptr %result, align 4
  ret i32 %3
}

declare i32 @printf(ptr noundef, ...)

declare void @exit(i32)

attributes #0 = { noinline nounwind optnone uwtable }
