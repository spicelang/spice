; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [21 x i8] c"Faculty of %d is: %d\00", align 1

define internal i32 @_f__void__faculty__int(i32 %0) {
entry.l1:
  %input = alloca i32, align 4
  store i32 %0, ptr %input, align 4
  %result = alloca i32, align 4
  %1 = load i32, ptr %input, align 4
  %2 = icmp slt i32 %1, 2
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
  %7 = load i32, ptr %input, align 4
  %8 = load i32, ptr %input, align 4
  %9 = sub i32 %8, 1
  %10 = alloca i32, align 4
  store i32 %9, ptr %10, align 4
  %11 = load i32, ptr %10, align 4
  %12 = call i32 @_f__void__faculty__int(i32 %11)
  %13 = alloca i32, align 4
  store i32 %12, ptr %13, align 4
  %14 = load i32, ptr %13, align 4
  %15 = mul i32 %7, %14
  %16 = alloca i32, align 4
  store i32 %15, ptr %16, align 4
  %17 = load i32, ptr %16, align 4
  store i32 %17, ptr %result, align 4
  %18 = load i32, ptr %result, align 4
  ret i32 %18
}

define i32 @main() {
entry.l8:
  %result = alloca i32, align 4
  store i32 0, ptr %result, align 4
  %input = alloca i32, align 4
  store i32 10, ptr %input, align 4
  %0 = load i32, ptr %input, align 4
  %1 = call i32 @_f__void__faculty__int(i32 %0)
  %2 = alloca i32, align 4
  store i32 %1, ptr %2, align 4
  %3 = load i32, ptr %input, align 4
  %4 = load i32, ptr %2, align 4
  %5 = call i32 (ptr, ...) @printf(ptr @0, i32 %3, i32 %4)
  %6 = load i32, ptr %result, align 4
  ret i32 %6
}

declare i32 @printf(ptr, ...)
