; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@printf.str.0 = private unnamed_addr constant [11 x i8] c"Result: %d\00", align 1

define private i32 @_f__void__int__fibo__int(i32 %0) {
  %result = alloca i32, align 4
  %n = alloca i32, align 4
  store i32 %0, ptr %n, align 4
  %2 = load i32, ptr %n, align 4
  %3 = icmp sle i32 %2, 1
  br i1 %3, label %if.then.L2, label %if.exit.L2

if.then.L2:                                       ; preds = %1
  %4 = load i32, ptr %n, align 4
  ret i32 %4

if.exit.L2:                                       ; preds = %1
  %5 = load i32, ptr %n, align 4
  %6 = sub i32 %5, 1
  %7 = call i32 @_f__void__int__fibo__int(i32 %6)
  %8 = load i32, ptr %n, align 4
  %9 = sub i32 %8, 2
  %10 = call i32 @_f__void__int__fibo__int(i32 %9)
  %11 = add i32 %7, %10
  ret i32 %11
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  store i32 0, ptr %result, align 4
  %1 = call i32 @_f__void__int__fibo__int(i32 30)
  %2 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 %1)
  %3 = load i32, ptr %result, align 4
  ret i32 %3
}

declare i32 @printf(ptr noundef, ...)

attributes #0 = { noinline nounwind optnone uwtable }