; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [20 x i8] c"Fibonacci of %d: %d\00", align 1

define internal i32 @_f__void__fib__int(i32 %0) {
entry:
  %n = alloca i32, align 4
  %result = alloca i32, align 4
  %1 = alloca i1, align 1
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  %5 = alloca i32, align 4
  %6 = alloca i32, align 4
  %7 = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  %8 = load i32, i32* %n, align 4
  %9 = icmp sle i32 %8, 2
  store i1 %9, i1* %1, align 1
  %10 = load i1, i1* %1, align 1
  br i1 %10, label %if.then, label %if.end

if.then:                                          ; preds = %entry
  store i32 1, i32* %2, align 4
  %11 = load i32, i32* %2, align 4
  ret i32 %11

if.end:                                           ; preds = %entry
  %12 = load i32, i32* %n, align 4
  %13 = sub i32 %12, 1
  store i32 %13, i32* %3, align 4
  %14 = load i32, i32* %3, align 4
  %15 = call i32 @_f__void__fib__int(i32 %14)
  store i32 %15, i32* %4, align 4
  %16 = load i32, i32* %4, align 4
  %17 = load i32, i32* %n, align 4
  %18 = sub i32 %17, 2
  store i32 %18, i32* %5, align 4
  %19 = load i32, i32* %5, align 4
  %20 = call i32 @_f__void__fib__int(i32 %19)
  store i32 %20, i32* %6, align 4
  %21 = load i32, i32* %6, align 4
  %22 = add i32 %16, %21
  store i32 %22, i32* %7, align 4
  %23 = load i32, i32* %7, align 4
  ret i32 %23
}

define i32 @main() {
entry:
  %result = alloca i32, align 4
  %base = alloca i32, align 4
  %0 = alloca i32, align 4
  %1 = alloca i32, align 4
  store i32 0, i32* %result, align 4
  store i32 46, i32* %0, align 4
  %2 = load i32, i32* %0, align 4
  %3 = load i32, i32* %0, align 4
  %4 = call i32 @_f__void__fib__int(i32 %3)
  store i32 %4, i32* %1, align 4
  %5 = load i32, i32* %1, align 4
  %6 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([20 x i8], [20 x i8]* @0, i32 0, i32 0), i32 %2, i32 %5)
  %7 = load i32, i32* %result, align 4
  ret i32 %7
}

declare i32 @printf(i8*, ...)
