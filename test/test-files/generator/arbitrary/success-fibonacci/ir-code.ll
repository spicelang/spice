; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [20 x i8] c"Fibonacci of %d: %d\00", align 1

define internal i32 @_f__void__fib__int(i32 %0) {
entry.l1:
  %n = alloca i32, align 4
  store i32 %0, ptr %n, align 4
  %result = alloca i32, align 4
  %1 = load i32, ptr %n, align 4
  %2 = icmp sle i32 %1, 2
  %3 = alloca i1, align 1
  store i1 %2, ptr %3, align 1
  %4 = load i1, ptr %3, align 1
  br i1 %4, label %if.then.l2, label %if.end.l2

if.then.l2:                                       ; preds = %entry.l1
  %5 = alloca i32, align 4
  store i32 1, ptr %5, align 4
  %6 = load i32, ptr %5, align 4
  ret i32 %6

if.end.l2:                                        ; preds = %entry.l1
  %7 = load i32, ptr %n, align 4
  %8 = sub i32 %7, 1
  %9 = alloca i32, align 4
  store i32 %8, ptr %9, align 4
  %10 = load i32, ptr %9, align 4
  %11 = call i32 @_f__void__fib__int(i32 %10)
  %12 = alloca i32, align 4
  store i32 %11, ptr %12, align 4
  %13 = load i32, ptr %12, align 4
  %14 = load i32, ptr %n, align 4
  %15 = sub i32 %14, 2
  %16 = alloca i32, align 4
  store i32 %15, ptr %16, align 4
  %17 = load i32, ptr %16, align 4
  %18 = call i32 @_f__void__fib__int(i32 %17)
  %19 = alloca i32, align 4
  store i32 %18, ptr %19, align 4
  %20 = load i32, ptr %19, align 4
  %21 = add i32 %13, %20
  %22 = alloca i32, align 4
  store i32 %21, ptr %22, align 4
  %23 = load i32, ptr %22, align 4
  ret i32 %23
}

define i32 @main() {
entry.l6:
  %result = alloca i32, align 4
  store i32 0, ptr %result, align 4
  %base = alloca i32, align 4
  store i32 46, ptr %base, align 4
  %0 = load i32, ptr %base, align 4
  %1 = load i32, ptr %base, align 4
  %2 = call i32 @_f__void__fib__int(i32 %1)
  %3 = alloca i32, align 4
  store i32 %2, ptr %3, align 4
  %4 = load i32, ptr %3, align 4
  %5 = call i32 (ptr, ...) @printf(ptr @0, i32 %0, i32 %4)
  %6 = load i32, ptr %result, align 4
  ret i32 %6
}

declare i32 @printf(ptr, ...)
