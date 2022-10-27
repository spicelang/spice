; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [10 x i8] c"If branch\00", align 1
@1 = private unnamed_addr constant [10 x i8] c"Else if 1\00", align 1
@2 = private unnamed_addr constant [10 x i8] c"Else if 2\00", align 1
@3 = private unnamed_addr constant [5 x i8] c"Else\00", align 1

define i32 @main() {
entry.l1:
  %result = alloca i32, align 4
  %0 = alloca i32, align 4
  %1 = alloca i1, align 1
  %2 = alloca double, align 8
  %3 = alloca i1, align 1
  %4 = alloca double, align 8
  %5 = alloca i1, align 1
  store i32 0, ptr %result, align 4
  store i32 1, ptr %0, align 4
  %6 = load i32, ptr %0, align 4
  %7 = icmp ne i32 %6, 1
  store i1 %7, ptr %1, align 1
  %8 = load i1, ptr %1, align 1
  br i1 %8, label %if.then.l2, label %if.else.l2

if.then.l2:                                       ; preds = %entry.l1
  %9 = call i32 (ptr, ...) @printf(ptr @0)
  br label %if.end.l2

if.else.l2:                                       ; preds = %entry.l1
  store double 2.000000e+00, ptr %2, align 8
  %10 = load double, ptr %2, align 8
  %11 = fcmp oeq double %10, 3.141500e+00
  store i1 %11, ptr %3, align 1
  %12 = load i1, ptr %3, align 1
  br i1 %12, label %if.then.l4, label %if.else.l4

if.then.l4:                                       ; preds = %if.else.l2
  %13 = call i32 (ptr, ...) @printf(ptr @1)
  br label %if.end.l4

if.else.l4:                                       ; preds = %if.else.l2
  store double 2.000000e+00, ptr %4, align 8
  %14 = load double, ptr %4, align 8
  %15 = fcmp oeq double %14, 2.718300e+00
  store i1 %15, ptr %5, align 1
  %16 = load i1, ptr %5, align 1
  br i1 %16, label %if.then.l6, label %if.else.l6

if.then.l6:                                       ; preds = %if.else.l4
  %17 = call i32 (ptr, ...) @printf(ptr @2)
  br label %if.end.l6

if.else.l6:                                       ; preds = %if.else.l4
  %18 = call i32 (ptr, ...) @printf(ptr @3)
  br label %if.end.l6

if.end.l6:                                        ; preds = %if.else.l6, %if.then.l6
  br label %if.end.l4

if.end.l4:                                        ; preds = %if.end.l6, %if.then.l4
  br label %if.end.l2

if.end.l2:                                        ; preds = %if.end.l4, %if.then.l2
  %19 = load i32, ptr %result, align 4
  ret i32 %19
}

declare i32 @printf(ptr, ...)
