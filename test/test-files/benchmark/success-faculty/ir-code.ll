; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [21 x i8] c"Faculty of %d is: %d\00", align 1

define internal i32 @_f__void__int__faculty__int(i32 %0) {
entry.l1:
  %input = alloca i32, align 4
  %result = alloca i32, align 4
  %1 = alloca i1, align 1
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  %5 = alloca i32, align 4
  %6 = alloca i32, align 4
  %7 = alloca i32, align 4
  store i32 %0, ptr %input, align 4
  %8 = load i32, ptr %input, align 4
  %9 = icmp slt i32 %8, 2
  store i1 %9, ptr %1, align 1
  %10 = load i1, ptr %1, align 1
  br i1 %10, label %if.then.l2, label %if.end.l2

if.then.l2:                                       ; preds = %entry.l1
  store i32 1, ptr %2, align 4
  %11 = load i32, ptr %2, align 4
  ret i32 %11

if.end.l2:                                        ; preds = %entry.l1
  %12 = load i32, ptr %input, align 4
  %13 = load i32, ptr %input, align 4
  %14 = sub i32 %13, 1
  store i32 %14, ptr %3, align 4
  %15 = load i32, ptr %3, align 4
  %16 = call i32 @_f__void__int__faculty__int(i32 %15)
  store i32 %16, ptr %4, align 4
  %17 = load i32, ptr %4, align 4
  %18 = load i32, ptr %input, align 4
  %19 = sub i32 %18, 1
  store i32 %19, ptr %5, align 4
  %20 = load i32, ptr %5, align 4
  %21 = call i32 @_f__void__int__faculty__int(i32 %20)
  store i32 %21, ptr %6, align 4
  %22 = mul i32 %12, %17
  store i32 %22, ptr %7, align 4
  %23 = load i32, ptr %7, align 4
  store i32 %23, ptr %result, align 4
  %24 = load i32, ptr %result, align 4
  ret i32 %24
}

define i32 @main() {
entry.l8:
  %result = alloca i32, align 4
  %input = alloca i32, align 4
  %0 = alloca i32, align 4
  %faculty = alloca i32, align 4
  store i32 0, ptr %result, align 4
  store i32 10, ptr %input, align 4
  %1 = load i32, ptr %input, align 4
  %2 = call i32 @_f__void__int__faculty__int(i32 %1)
  store i32 %2, ptr %0, align 4
  %3 = load i32, ptr %0, align 4
  store i32 %3, ptr %faculty, align 4
  %4 = load i32, ptr %input, align 4
  %5 = load i32, ptr %faculty, align 4
  %6 = call i32 (ptr, ...) @printf(ptr @0, i32 %4, i32 %5)
  %7 = load i32, ptr %result, align 4
  ret i32 %7
}

declare i32 @printf(ptr, ...)