; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@printf.str.0 = private unnamed_addr constant [13 x i8] c"Test func 1\0A\00", align 1
@printf.str.1 = private unnamed_addr constant [12 x i8] c"Result: %d\0A\00", align 1

define private i32 @_f__void__int__testFunc() {
  %result = alloca i32, align 4
  %1 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0)
  ret i32 1
}

declare i32 @printf(ptr noundef, ...)

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  %res = alloca i32, align 4
  store i32 0, ptr %result, align 4
  %1 = call i32 @_f__void__int__testFunc()
  store i32 %1, ptr %res, align 4
  %2 = load i32, ptr %res, align 4
  %3 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.1, i32 %2)
  %4 = load i32, ptr %result, align 4
  ret i32 %4
}

attributes #0 = { noinline nounwind optnone uwtable }
