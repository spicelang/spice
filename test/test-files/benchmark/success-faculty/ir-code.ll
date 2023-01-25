; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@printf.str.0 = private unnamed_addr constant [21 x i8] c"Faculty of %d is: %d\00", align 1

define private i32 @_f__void__int__faculty__int(i32 %0) {
  %result = alloca i32, align 4
  %input = alloca i32, align 4
  store i32 %0, ptr %input, align 4
  %2 = load i32, ptr %input, align 4
  %3 = icmp slt i32 %2, 2
  br i1 %3, label %if.then.L2, label %if.exit.L2

if.then.L2:                                       ; preds = %1
  ret i32 1

if.exit.L2:                                       ; preds = %1
  %4 = load i32, ptr %input, align 4
  %5 = sub i32 %4, 1
  %6 = call i32 @_f__void__int__faculty__int(i32 %5)
  %7 = load i32, ptr %input, align 4
  %8 = mul i32 %7, %6
  store i32 %8, ptr %result, align 4
  %9 = load i32, ptr %result, align 4
  ret i32 %9
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  %input = alloca i32, align 4
  %faculty = alloca i32, align 4
  store i32 0, ptr %result, align 4
  store i32 10, ptr %input, align 4
  %1 = load i32, ptr %input, align 4
  %2 = call i32 @_f__void__int__faculty__int(i32 %1)
  store i32 %2, ptr %faculty, align 4
  %3 = load i32, ptr %input, align 4
  %4 = load i32, ptr %faculty, align 4
  %5 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 %3, i32 %4)
  %6 = load i32, ptr %result, align 4
  ret i32 %6
}

declare i32 @printf(ptr noundef, ...)

attributes #0 = { noinline nounwind optnone uwtable }
