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
  %anonymous.0 = alloca [2 x i8*], align 8
  %4 = alloca i1, align 1
  store i32 0, i32* %result, align 4
  store double 1.123000e+00, double* %0, align 8
  %5 = load double, double* %0, align 8
  call void @_p__void__printFormat__double(double %5)
  store i1 true, i1* %1, align 1
  store i32 543, i32* %2, align 4
  %6 = load i32, i32* %2, align 4
  call void @_p__void__printFormat__int(i32 %6)
  store i1 true, i1* %3, align 1
  store [2 x i8*] [i8* getelementptr inbounds ([6 x i8], [6 x i8]* @0, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @1, i32 0, i32 0)], [2 x i8*]* %anonymous.0, align 8
  %7 = load [2 x i8*], [2 x i8*]* %anonymous.0, align 8
  call void @_p__void__printFormat__stringarray([2 x i8*] %7)
  store i1 true, i1* %4, align 1
  %8 = load i32, i32* %result, align 4
  ret i32 %8
}

declare void @_p__void__printFormat__double(double)

declare void @_p__void__printFormat__int(i32)

declare void @_p__void__printFormat__stringarray([2 x i8*])
