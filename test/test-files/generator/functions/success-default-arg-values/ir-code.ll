; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [5 x i8] c"Test\00", align 1
@1 = private unnamed_addr constant [12 x i8] c"Result: %s\0A\00", align 1

define internal i8* @_f_getTestString_int_double_bool(i32 %0, double %1, i1 %2) {
entry:
  %arg0 = alloca i32, align 4
  %arg1 = alloca double, align 8
  %arg2 = alloca i1, align 1
  %arg3 = alloca double, align 8
  %3 = alloca double, align 8
  %result = alloca i8*, align 8
  %4 = alloca i8*, align 8
  store i32 %0, i32* %arg0, align 4
  store double %1, double* %arg1, align 8
  store i1 %2, i1* %arg2, align 1
  store double 1.340000e+00, double* %3, align 8
  %5 = load double, double* %3, align 8
  store double %5, double* %arg3, align 8
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @0, i32 0, i32 0), i8** %4, align 8
  %6 = load i8*, i8** %4, align 8
  ret i8* %6
}

define i32 @main() {
entry:
  %result = alloca i32, align 4
  %0 = alloca i32, align 4
  %1 = alloca double, align 8
  %2 = alloca i1, align 1
  %3 = alloca i8*, align 8
  store i32 0, i32* %result, align 4
  store i32 1, i32* %0, align 4
  %4 = load i32, i32* %0, align 4
  store double 3.400000e+00, double* %1, align 8
  %5 = load double, double* %1, align 8
  store i1 true, i1* %2, align 1
  %6 = load i1, i1* %2, align 1
  %7 = call i8* @_f_getTestString_int_double_bool(i32 %4, double %5, i1 %6)
  store i8* %7, i8** %3, align 8
  %8 = load i8*, i8** %3, align 8
  %9 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([12 x i8], [12 x i8]* @1, i32 0, i32 0), i8* %8)
  %10 = load i32, i32* %result, align 4
  ret i32 %10
}

declare i32 @printf(i8*, ...)
