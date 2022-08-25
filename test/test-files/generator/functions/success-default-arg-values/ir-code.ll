; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [5 x i8] c"Test\00", align 1
@1 = private unnamed_addr constant [12 x i8] c"Result: %s\0A\00", align 1

define internal ptr @_f__void__getTestString__int_double_bool(i32 %0, double %1, i1 %2) {
entry.l1:
  %arg0 = alloca i32, align 4
  store i32 %0, ptr %arg0, align 4
  %arg1 = alloca double, align 8
  store double %1, ptr %arg1, align 8
  %arg2 = alloca i1, align 1
  store i1 %2, ptr %arg2, align 1
  %arg3 = alloca double, align 8
  store double 1.340000e+00, ptr %arg3, align 8
  %result = alloca ptr, align 8
  %arg31 = alloca ptr, align 8
  store ptr @0, ptr %arg31, align 8
  %3 = load ptr, ptr %arg31, align 8
  ret ptr %3
}

define i32 @main() {
entry.l5:
  %result = alloca i32, align 4
  store i32 0, ptr %result, align 4
  %0 = alloca i32, align 4
  store i32 1, ptr %0, align 4
  %1 = load i32, ptr %0, align 4
  %2 = alloca double, align 8
  store double 3.400000e+00, ptr %2, align 8
  %3 = load double, ptr %2, align 8
  %4 = alloca i1, align 1
  store i1 true, ptr %4, align 1
  %5 = load i1, ptr %4, align 1
  %6 = call ptr @_f__void__getTestString__int_double_bool(i32 %1, double %3, i1 %5)
  %7 = alloca ptr, align 8
  store ptr %6, ptr %7, align 8
  %8 = load ptr, ptr %7, align 8
  %9 = call i32 (ptr, ...) @printf(ptr @1, ptr %8)
  %10 = load i32, ptr %result, align 4
  ret i32 %10
}

declare i32 @printf(ptr, ...)
