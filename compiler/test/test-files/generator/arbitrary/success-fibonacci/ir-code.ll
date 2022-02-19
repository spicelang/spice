; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [20 x i8] c"Fibonacci of %d: %d\00", align 1

declare i32 @printf(i8*, ...)

; Function Attrs: nounwind
define internal i32 @"fib(int)"(i32 %0) #0 {
entry:
  %n = alloca i32, align 4
  %result = alloca i32, align 4
  %1 = alloca i32, align 4
  %2 = alloca i1, align 1
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  %5 = alloca i32, align 4
  %6 = alloca i32, align 4
  %7 = alloca i32, align 4
  %8 = alloca i32, align 4
  %9 = alloca i32, align 4
  %10 = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  store i32 2, i32* %1, align 4
  %11 = load i32, i32* %n, align 4
  %12 = load i32, i32* %1, align 4
  %13 = icmp sle i32 %11, %12
  store i1 %13, i1* %2, align 1
  %14 = load i1, i1* %2, align 1
  br i1 %14, label %if.then, label %if.end

if.then:                                          ; preds = %entry
  store i32 1, i32* %3, align 4
  %15 = load i32, i32* %3, align 4
  ret i32 %15

if.end:                                           ; preds = %entry
  %16 = load i32, i32* %n, align 4
  store i32 1, i32* %4, align 4
  %17 = load i32, i32* %4, align 4
  %18 = sub i32 %16, %17
  store i32 %18, i32* %5, align 4
  %19 = load i32, i32* %5, align 4
  %20 = call i32 @"fib(int)"(i32 %19)
  store i32 %20, i32* %6, align 4
  %21 = load i32, i32* %6, align 4
  %22 = load i32, i32* %n, align 4
  store i32 2, i32* %7, align 4
  %23 = load i32, i32* %7, align 4
  %24 = sub i32 %22, %23
  store i32 %24, i32* %8, align 4
  %25 = load i32, i32* %8, align 4
  %26 = call i32 @"fib(int)"(i32 %25)
  store i32 %26, i32* %9, align 4
  %27 = load i32, i32* %9, align 4
  %28 = add i32 %21, %27
  store i32 %28, i32* %10, align 4
  %29 = load i32, i32* %10, align 4
  ret i32 %29
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
  store i32 %2, i32* %base, align 4
  %3 = load i32, i32* %base, align 4
  %4 = load i32, i32* %base, align 4
  %5 = call i32 @"fib(int)"(i32 %4)
  store i32 %5, i32* %1, align 4
  %6 = load i32, i32* %1, align 4
  %7 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([20 x i8], [20 x i8]* @0, i32 0, i32 0), i32 %3, i32 %6)
  %8 = load i32, i32* %result, align 4
  ret i32 %8
}

attributes #0 = { nounwind }
