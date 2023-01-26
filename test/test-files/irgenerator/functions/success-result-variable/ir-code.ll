; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@printf.str.0 = private unnamed_addr constant [15 x i8] c"The age is: %d\00", align 1

define private i32 @_f__void__int__getAge() {
  %result = alloca i32, align 4
  %i = alloca i1, align 1
  store i1 false, ptr %i, align 1
  br i1 true, label %if.then.L3, label %if.else.L3

if.then.L3:                                       ; preds = %0
  store i32 20, ptr %result, align 4
  %1 = load i32, ptr %result, align 4
  ret i32 %1

if.else.L3:                                       ; preds = %0
  store i1 false, ptr %i, align 1
  br i1 false, label %if.then.L6, label %if.exit.L6

if.then.L6:                                       ; preds = %if.else.L3
  store i32 19, ptr %result, align 4
  br label %if.exit.L6

if.exit.L6:                                       ; preds = %if.then.L6, %if.else.L3
  br label %if.exit.L3

if.exit.L3:                                       ; preds = %if.exit.L6
  %2 = load i32, ptr %result, align 4
  ret i32 %2
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  %age = alloca i32, align 4
  store i32 0, ptr %result, align 4
  %1 = call i32 @_f__void__int__getAge()
  store i32 %1, ptr %age, align 4
  %2 = load i32, ptr %age, align 4
  %3 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 %2)
  %4 = load i32, ptr %result, align 4
  ret i32 %4
}

declare i32 @printf(ptr noundef, ...)

attributes #0 = { noinline nounwind optnone uwtable }
