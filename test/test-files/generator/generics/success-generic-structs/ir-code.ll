; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%_s__double__Vector__doubleptr_int = type { ptr, i32 }

@0 = private unnamed_addr constant [24 x i8] c"Capacity of vector: %d\0A\00", align 1

define i32 @main() {
entry.l8:
  %result = alloca i32, align 4
  %dbl = alloca double, align 8
  %0 = alloca %_s__double__Vector__doubleptr_int, align 8
  %1 = alloca ptr, align 8
  store i32 0, ptr %result, align 4
  store double 3.467000e+00, ptr %dbl, align 8
  store ptr %dbl, ptr %1, align 8
  %2 = load ptr, ptr %1, align 8
  %3 = getelementptr inbounds %_s__double__Vector__doubleptr_int, ptr %0, i32 0, i32 0
  store ptr %2, ptr %3, align 8
  %4 = getelementptr inbounds %_s__double__Vector__doubleptr_int, ptr %0, i32 0, i32 1
  store i32 1, ptr %4, align 4
  %5 = load %_s__double__Vector__doubleptr_int, ptr %0, align 8
  %6 = getelementptr inbounds %_s__double__Vector__doubleptr_int, ptr %0, i32 0, i32 1
  %7 = load i32, ptr %6, align 4
  %8 = call i32 (ptr, ...) @printf(ptr @0, i32 %7)
  %9 = load i32, ptr %result, align 4
  ret i32 %9
}

declare i32 @printf(ptr, ...)
