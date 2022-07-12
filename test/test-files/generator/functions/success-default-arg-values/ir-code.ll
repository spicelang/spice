; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [5 x i8] c"Test\00", align 1
@1 = private unnamed_addr constant [12 x i8] c"Result: %s\0A\00", align 1

define internal i8* @_f__void__getTestString__int_double_bool(i32 %0, double %1, i1 %2) {
entry:
  %arg0 = alloca i32, align 4
  %arg1 = alloca double, align 8
  %arg2 = alloca i1, align 1
  %arg3 = alloca double, align 8
  %result = alloca i8*, align 8
  %arg31 = alloca i8*, align 8
  store i32 %0, i32* %arg0, align 4
  store double %1, double* %arg1, align 8
  store i1 %2, i1* %arg2, align 1
  store double 1.340000e+00, double* %arg3, align 8
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @0, i32 0, i32 0), i8** %arg31, align 8
  %3 = load i8*, i8** %arg31, align 8
  ret i8* %3
}

define i32 @main() {
entry:
  %result = alloca i32, align 4
  %arg3 = alloca i32, align 4
  %arg31 = alloca double, align 8
  %arg32 = alloca i1, align 1
  %0 = alloca i8*, align 8
  store i32 0, i32* %result, align 4
  store i32 1, i32* %arg3, align 4
  %1 = load i32, i32* %arg3, align 4
  store double 3.400000e+00, double* %arg31, align 8
  %2 = load double, double* %arg31, align 8
  store i1 true, i1* %arg32, align 1
  %3 = load i1, i1* %arg32, align 1
  %4 = call i8* @_f__void__getTestString__int_double_bool(i32 %1, double %2, i1 %3)
  store i8* %4, i8** %0, align 8
  %5 = load i8*, i8** %0, align 8
  %6 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([12 x i8], [12 x i8]* @1, i32 0, i32 0), i8* %5)
  %7 = load i32, i32* %result, align 4
  ret i32 %7
}

declare i32 @printf(i8*, ...)
