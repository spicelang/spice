; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [6 x i8] c"Hello\00", align 1
@1 = private unnamed_addr constant [6 x i8] c"World\00", align 1
@anonymous.0 = constant [2 x i8*] [i8* getelementptr inbounds ([6 x i8], [6 x i8]* @0, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @1, i32 0, i32 0)]

define i32 @main() {
entry:
  %result = alloca i32, align 4
  %0 = alloca double, align 8
  %1 = alloca i1, align 1
  %2 = alloca i32, align 4
  %3 = alloca i1, align 1
  %4 = alloca i8*, align 8
  %5 = alloca i8*, align 8
  %6 = alloca i1, align 1
  store i32 0, i32* %result, align 4
  store double 1.123000e+00, double* %0, align 8
  %7 = load double, double* %0, align 8
  call void @_p_printFormat_double(double %7)
  store i1 true, i1* %1, align 1
  store i32 543, i32* %2, align 4
  %8 = load i32, i32* %2, align 4
  call void @_p_printFormat_int(i32 %8)
  store i1 true, i1* %3, align 1
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @0, i32 0, i32 0), i8** %4, align 8
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @1, i32 0, i32 0), i8** %5, align 8
  call void @"_p_printFormat_string[]"(i8** getelementptr inbounds ([2 x i8*], [2 x i8*]* @anonymous.0, i32 0, i32 0))
  store i1 true, i1* %6, align 1
  %9 = load i32, i32* %result, align 4
  ret i32 %9
}

declare void @_p_printFormat_double(double)

declare void @_p_printFormat_int(i32)

declare void @"_p_printFormat_string[]"(i8**)
