; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%_s__double__Vector__doubleptr_int = type { double*, i32 }

@0 = private unnamed_addr constant [24 x i8] c"Capacity of vector: %d\0A\00", align 1

define i32 @main() {
entry:
  %result = alloca i32, align 4
  %dbl = alloca double, align 8
  %0 = alloca double, align 8
  %doubleVec = alloca %_s__double__Vector__doubleptr_int, align 8
  %1 = alloca %_s__double__Vector__doubleptr_int, align 8
  %2 = alloca double*, align 8
  store i32 0, i32* %result, align 4
  store double 3.467000e+00, double* %0, align 8
  store double* %0, double** %2, align 8
  %3 = load double*, double** %2, align 8
  %4 = getelementptr inbounds %_s__double__Vector__doubleptr_int, %_s__double__Vector__doubleptr_int* %1, i32 0, i32 0
  store double* %3, double** %4, align 8
  %5 = getelementptr inbounds %_s__double__Vector__doubleptr_int, %_s__double__Vector__doubleptr_int* %1, i32 0, i32 1
  store i32 1, i32* %5, align 4
  %6 = load %_s__double__Vector__doubleptr_int, %_s__double__Vector__doubleptr_int* %1, align 8
  %7 = getelementptr inbounds %_s__double__Vector__doubleptr_int, %_s__double__Vector__doubleptr_int* %1, i32 0, i32 1
  %8 = load i32, i32* %7, align 4
  %9 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([24 x i8], [24 x i8]* @0, i32 0, i32 0), i32 %8)
  %10 = load i32, i32* %result, align 4
  ret i32 %10
}

declare i32 @printf(i8*, ...)
