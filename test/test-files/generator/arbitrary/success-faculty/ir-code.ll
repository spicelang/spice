; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [21 x i8] c"Faculty of %d is: %d\00", align 1

define internal i32 @_f__void__faculty__int(i32 %0) {
entry.l1:
  %input = alloca i32, align 4
  %result = alloca i32, align 4
  %1 = alloca i1, align 1
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  %5 = alloca i32, align 4
  store i32 %0, i32* %input, align 4
  %6 = load i32, i32* %input, align 4
  %7 = icmp slt i32 %6, 2
  store i1 %7, i1* %1, align 1
  %8 = load i1, i1* %1, align 1
  br i1 %8, label %if.then.l2, label %if.end.l2

if.then.l2:                                       ; preds = %entry.l1
  store i32 1, i32* %2, align 4
  %9 = load i32, i32* %2, align 4
  ret i32 %9

if.end.l2:                                        ; preds = %entry.l1
  %10 = load i32, i32* %input, align 4
  %11 = load i32, i32* %input, align 4
  %12 = sub i32 %11, 1
  store i32 %12, i32* %3, align 4
  %13 = load i32, i32* %3, align 4
  %14 = call i32 @_f__void__faculty__int(i32 %13)
  store i32 %14, i32* %4, align 4
  %15 = load i32, i32* %4, align 4
  %16 = mul i32 %10, %15
  store i32 %16, i32* %5, align 4
  %17 = load i32, i32* %5, align 4
  store i32 %17, i32* %result, align 4
  %18 = load i32, i32* %result, align 4
  ret i32 %18
}

define i32 @main() {
entry.l8:
  %result = alloca i32, align 4
  %input = alloca i32, align 4
  %0 = alloca i32, align 4
  store i32 0, i32* %result, align 4
  store i32 10, i32* %input, align 4
  %1 = load i32, i32* %input, align 4
  %2 = call i32 @_f__void__faculty__int(i32 %1)
  store i32 %2, i32* %0, align 4
  %3 = load i32, i32* %input, align 4
  %4 = load i32, i32* %0, align 4
  %5 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([21 x i8], [21 x i8]* @0, i32 0, i32 0), i32 %3, i32 %4)
  %6 = load i32, i32* %result, align 4
  ret i32 %6
}

declare i32 @printf(i8*, ...)
